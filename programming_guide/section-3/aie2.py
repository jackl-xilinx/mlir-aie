
#
# This file is licensed under the Apache License v2.0 with LLVM Exceptions.
# See https://llvm.org/LICENSE.txt for license information.
# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
#
# (c) Copyright 2023 AMD Inc.

from aie.dialects.aie import *                     # primary mlir-aie dialect definitions
from aie.extras.context import mlir_mod_ctx        # mlir ctx wrapper 

from aie.dialects.aiex import *                    # extended mlir-aie dialect definitions
from aie.dialects.scf import *                     # scf (strcutred control flow) dialect
from aie.extras.dialects.ext import memref, arith  # memref and arithmatic dialects

# AI Engine structural design function
def my_first_aie_program():
    # ctx wrapper - to convert python to mlir
    with mlir_mod_ctx() as ctx:

        # Dvice declaration - aie2 device IPU (aka Ryzen AI)
        @device(AIEDevice.ipu)
        def device_body():
            # Memref types
            memRef_8_ty = T.memref(8, T.i32())
            memRef_16_ty = T.memref(16, T.i32())
            memRef_32_ty = T.memref(32, T.i32())
            memRef_64_ty = T.memref(64, T.i32())

            # Tile declarations
            ComputeTile = tile(0, 2)
            ShimTile = tile(0, 0)

            # AIE-array data movement with object fifos
            # Input
            of_in0 = object_fifo("in0", ShimTile, ComputeTile, 2, memRef_8_ty)

            # Output
            of_out0 = object_fifo("out0", ComputeTile, ShimTile, 2, memRef_8_ty)

            # Compute tile
            @core(ComputeTile)
            def core_body():
                for _ in for_(8):
                    elem_in = of_in0.acquire(ObjectFifoPort.Consume, 1)
                    elem_out = of_out0.acquire(ObjectFifoPort.Produce, 1)
                    for i in for_(8):
                        v0 = memref.load(elem_in, [i])
                        v1 = arith.addi(v0, arith.constant(1, T.i32()))
                        memref.store(v1, elem_out, [i])
                        yield_([])
                    of_in0.release(ObjectFifoPort.Consume, 1)
                    of_out0.release(ObjectFifoPort.Produce, 1)
                    yield_([])

            # To/from AIE-array data movement
            @FuncOp.from_py_func(memRef_64_ty, memRef_32_ty, memRef_64_ty)
            def sequence(inTensor, notUsed, outTensor):
                ipu_dma_memcpy_nd(
                    metadata="out0", bd_id=0, mem=outTensor, sizes=[1, 1, 1, 64]
                )
                ipu_dma_memcpy_nd(
                    metadata="in0", bd_id=1, mem=inTensor, sizes=[1, 1, 1, 64]
                )
                ipu_sync(column=0, row=0, direction=0, channel=0)

    print(ctx.module)

my_first_aie_program()