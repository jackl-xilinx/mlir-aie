#
# This file is licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
# (c) Copyright 2023 AMD Inc.

import aie.dialects.aie  as AIE
import aie.dialects.aiex as AIEX 
import aie.dialects.scf  as SCF 
from aie.extras.context import mlir_mod_ctx


def my_matmul():
    M = 256
    K = 256
    N = 256
    m = 64
    k = 32
    n = 64
    r = 4
    s = 8
    t = 4
    word_size_in = 2
    word_size_out = 4

    vectorized = True
    enable_tracing = True
    trace_size = 4096

    A_sz_in_i32s = M * K * word_size_in // 4
    B_sz_in_i32s = K * N * word_size_in // 4
    C_sz_in_bytes = M * N * word_size_out
    C_sz_in_i32s = C_sz_in_bytes // 4

    M_div_m = M // m
    K_div_k = K // k
    N_div_n = N // n
    tiles = M_div_m * N_div_n

    # Matrix A: MxK, submatrices a: mxk
    k_in_i32s = k * word_size_in // 4
    K_in_i32s = K * word_size_in // 4

    # Matrix B: KxN, submatrices b: kxn
    n_in_i32s = n * word_size_in // 4
    N_in_i32s = N * word_size_in // 4
    k_x_N_in_i32s = k * N * word_size_in // 4

    # Output Matrix C: MxN
    n_in_i32s_out = n * word_size_out // 4
    N_in_i32s_out = N * word_size_out // 4
    m_x_N_in_i32s_out = m * N * word_size_out // 4


    with mlir_mod_ctx() as ctx:

        @AIE.device(AIE.AIEDevice.ipu)
        def device_body():
            memref_a_ty = AIE.T.memref(m, k, AIE.T.bf16())
            memref_b_ty = AIE.T.memref(k, n, AIE.T.bf16())
            memref_c_ty = AIE.T.memref(m, n, AIE.T.f32())

            ofifo_memref_a_ty = SCF.TypeAttr.get(AIE.ObjectFifoType.get(memref_a_ty))
            ofifo_memref_b_ty = SCF.TypeAttr.get(AIE.ObjectFifoType.get(memref_b_ty))
            ofifo_memref_c_ty = SCF.TypeAttr.get(AIE.ObjectFifoType.get(memref_c_ty))

            # AIE Core Function declarations
            zero_scalar = AIE.external_func("zero_scalar_f32", inputs=[memref_c_ty])
            zero = AIE.external_func("zero_f32", inputs=[memref_c_ty])
            matmul_scalar = AIE.external_func(
                "matmul_scalar_bf16_f32",
                inputs=[memref_a_ty, memref_b_ty, memref_c_ty],
            )
            matmul = AIE.external_func(
                "matmul_bf16_f32", inputs=[memref_a_ty, memref_b_ty, memref_c_ty]
            )

            # Tile declarations
            shim_tile = AIE.tile(0, 0)
            mem_tile  = AIE.tile(0, 1)
            compute_tile2_col, compute_tile2_row = 0, 2
            compute_tile2 = AIE.tile(compute_tile2_col, compute_tile2_row)

            # AIE-array data movement with object fifos
            # Input A
            AIE.objectfifo("inA", shim_tile, [mem_tile], 2, ofifo_memref_a_ty, [], [])
            AIE.objectfifo(
                "memA",
                mem_tile,
                [compute_tile2],
                2,
                ofifo_memref_a_ty,
                [
                    (m // r, r * k * word_size_in // 4),
                    (k // s, s * word_size_in // 4),
                    (r, k * word_size_in // 4),
                    (s * word_size_in // 4, 1),
                ],
                [],
            )
            AIE.objectfifo_link(["inA"], ["memA"])

            # Input B
            AIE.objectfifo("inB", shim_tile, [mem_tile], 2, ofifo_memref_b_ty, [], [])
            AIE.objectfifo(
                "memB",
                mem_tile,
                [compute_tile2],
                2,
                ofifo_memref_b_ty,
                [
                    (k // s, s * n * word_size_in // 4),
                    (n // t, t * word_size_in // 4),
                    (s, n * word_size_in // 4),
                    (t * word_size_in // 4, 1),
                ],
                [],
            )
            AIE.objectfifo_link(["inB"], ["memB"])

            # Output C
            AIE.objectfifo("memC", compute_tile2, [mem_tile], 2, ofifo_memref_c_ty, [], [])
            AIE.objectfifo(
                "outC",
                mem_tile,
                [shim_tile],
                2,
                ofifo_memref_c_ty,
                [
                    (m // r, r * n * word_size_out // 4),
                    (r, t * word_size_out // 4),
                    (n // t, r * t * word_size_out // 4),
                    (t * word_size_out // 4, 1),
                ],
                [],
            )
            AIE.objectfifo_link(["memC"], ["outC"])

            # Set up a circuit-switched flow from core to shim for tracing information
            if enable_tracing:
                AIE.flow(compute_tile2, AIE.WireBundle.Trace, 0, shim_tile, AIE.WireBundle.DMA, 1)

            # Set up compute tiles

            # Compute tile 2
            @AIE.core(compute_tile2, "mm.o")
            def core_body():
                for _ in SCF.for_(0xFFFFFFFF):
                    for _ in SCF.for_(tiles):
                        elem_out = AIE.acquire(
                            AIE.ObjectFifoPort.Produce, "memC", 1, memref_c_ty
                        ).acquired_elem()
                        if vectorized:
                            AIE.Call(zero, [elem_out])
                        else:
                            AIE.Call(zero_scalar, [elem_out])

                        for _ in SCF.for_(K_div_k):
                            elem_in_a = AIE.acquire(
                                AIE.ObjectFifoPort.Consume, "memA", 1, memref_a_ty
                            ).acquired_elem()
                            elem_in_b = AIE.acquire(
                                AIE.ObjectFifoPort.Consume, "memB", 1, memref_b_ty
                            ).acquired_elem()
                            if vectorized:
                                AIE.Call(matmul, [elem_in_a, elem_in_b, elem_out])
                            else:
                                AIE.Call(matmul_scalar, [elem_in_a, elem_in_b, elem_out])
                            AIE.objectfifo_release(AIE.ObjectFifoPort.Consume, "memA", 1)
                            AIE.objectfifo_release(AIE.ObjectFifoPort.Consume, "memB", 1)
                            SCF.yield_([])

                        AIE.objectfifo_release(AIE.ObjectFifoPort.Produce, "memC", 1)
                        SCF.yield_([])
                    SCF.yield_([])

            # To/from AIE-array data movement

            @AIE.FuncOp.from_py_func(
                AIE.T.memref(A_sz_in_i32s, AIE.T.i32()),
                AIE.T.memref(B_sz_in_i32s, AIE.T.i32()),
                AIE.T.memref(C_sz_in_i32s, AIE.T.i32()),
            )
            def sequence(A, B, C):

                # Configure tracing, see https://github.com/Xilinx/mlir-aie/blob/resnet/docs/Tracing.md
                if enable_tracing:
                    # 0x340D0: Trace Control 0
                    #          0xAABB---C
                    #            AA        <- Event to stop trace capture
                    #              BB      <- Event to start trace capture
                    #                   C  <- Trace mode, 00=event=time, 01=event-PC, 10=execution
                    # Configure so that "Event 1" (always true) causes tracing to start
                    AIEX._generated_ipu_write32(column=compute_tile2_col, row=compute_tile2_row, address=0x340D0, value=0x00010000)
                    # 0x340D4: Trace Control 1
                    AIEX._generated_ipu_write32(column=compute_tile2_col, row=compute_tile2_row, address=0x340D4, value=0x00000000)
                    # 0x340E0: Trace Event Group 1  (Which events to trace)
                    #          0xAABBCCDD    AA, BB, CC, DD <- four event slots
                    AIEX._generated_ipu_write32(column=compute_tile2_col, row=compute_tile2_row, address=0x340E0, value=0x4B222125)
                    # 0x340E4: Trace Event Group 2  (Which events to trace)
                    #          0xAABBCCDD    AA, BB, CC, DD <- four event slots
                    AIEX._generated_ipu_write32(column=compute_tile2_col, row=compute_tile2_row, address=0x340E4, value=0x00000000)

                    # Configure a buffer descriptor to write tracing information that has been routed into this shim tile
                    # out to host DDR memory
                    trace_bd_id = 13  # use BD 13 for writing trace output from compute tile to DDR host memory
                    output_size = C_sz_in_bytes
                    AIEX._generated_ipu_writebd_shimtile(
                            bd_id              =  trace_bd_id,
                            buffer_length      =   trace_size,
                            buffer_offset      =  output_size,
                            enable_packet      =            0,
                            out_of_order_id    =            0,
                            packet_id          =            0,
                            packet_type        =            0,
                            column             =            0,
                            column_num         =            1,
                            d0_size            =            0,
                            d0_stride          =            0,
                            d1_size            =            0,
                            d1_stride          =            0,
                            d2_stride          =            0, 
                            ddr_id             =            2,
                            iteration_current  =            0,
                            iteration_size     =            0,
                            iteration_stride   =            0,
                            lock_acq_enable    =            0,
                            lock_acq_id        =            0,
                            lock_acq_val       =            0,
                            lock_rel_id        =            0,
                            lock_rel_val       =            0,
                            next_bd            =            0,
                            use_next_bd        =            0,
                            valid_bd           =            1)
                    # Set start BD to our shim bd_Id (3)
                    AIEX._generated_ipu_write32(column=0, row=0, address=0x1D20C, value=trace_bd_id)

                # only do 5 tile rows at a time before synchronizing, so we can reuse BDs
                rows_per_block = 5
                for tile_row_block in range(
                    (M_div_m + rows_per_block - 1) // rows_per_block
                ):
                    C_row_offset_in_i32s = (
                        tile_row_block * rows_per_block * m * N * word_size_out // 4
                    )
                    num_tile_rows = min(
                        [rows_per_block, M_div_m - tile_row_block * rows_per_block]
                    )
                    AIEX.ipu_dma_memcpy_nd(
                        metadata="outC",
                        bd_id=0,
                        mem=C,
                        offsets=[0, 0, 0, C_row_offset_in_i32s],
                        sizes=[num_tile_rows, N_div_n, m, n_in_i32s_out],
                        strides=[m_x_N_in_i32s_out, n_in_i32s_out, N_in_i32s_out],
                    )
                    for tile_row in range(num_tile_rows):
                        A_row_offset_in_i32s = (
                            ((tile_row_block * rows_per_block) + tile_row)
                            * m
                            * K
                            * word_size_in
                            // 4
                        )
                        AIEX.ipu_dma_memcpy_nd(
                            metadata="inA",
                            bd_id=2 * tile_row + 1,
                            mem=A,
                            offsets=[0, 0, 0, A_row_offset_in_i32s],
                            sizes=[N_div_n, K_div_k, m, k_in_i32s],
                            strides=[0, k_in_i32s, K_in_i32s],
                        )
                        AIEX.ipu_dma_memcpy_nd(
                            metadata="inB",
                            bd_id=2 * tile_row + 2,
                            mem=B,
                            sizes=[N_div_n, K_div_k, k, n_in_i32s],
                            strides=[n_in_i32s, k_x_N_in_i32s, N_in_i32s],
                        )

                    AIEX.ipu_sync(column=0, row=0, direction=0, channel=0)

    print(ctx.module)


my_matmul()
