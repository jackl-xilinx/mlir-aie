//===- aie.mlir ------------------------------------------------*- MLIR -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// Copyright (C) 2022, Advanced Micro Devices, Inc.
//
//===----------------------------------------------------------------------===//

// REQUIRES: valid_xchess_license
// RUN: aiecc.py --sysroot=%VITIS_SYSROOT% %s -I%aie_runtime_lib%/ %aie_runtime_lib%/test_library.cpp %S/test.cpp -o tutorial-1.elf
// RUN: %run_on_board ./tutorial-1.elf


// Declare this MLIR module. A wrapper that can contain all 
// AIE tiles, buffers, and data movement
module @tutorial_3 {

    // 2 tiles in row 4 (col 1 and col 2)
    // even rows have local memory to its left
    %tile14 = AIE.tile(1, 4) 
    %tile24 = AIE.tile(2, 4)

    // Declare local memory of tile(2,4) which is shared with tile(1,4)
    %buf = AIE.buffer(%tile24) { sym_name = "a24" } : memref<256xi32>

    // Declare shared lock (belonging to tile(2,4), lock ID=1)
    %lock24_1 = AIE.lock(%tile24, 1) { sym_name = "lock_a24_1" }

    // Define core algorithm for tile(1,4)
    // buf[3] = 14
    %core14 = AIE.core(%tile14) {
        // Locks init value is Release 0, so this will always succeed first
        AIE.useLock(%lock24_1, "Acquire", 0)

		%val = arith.constant 14 : i32 
		%idx = arith.constant 3 : index 
		memref.store %val, %buf[%idx] : memref<256xi32> 

        AIE.useLock(%lock24_1, "Release", 1)
        AIE.end
    } 

    // Define core algorithm for tile(2,4) which reads value set by tile(1,4)
    // buf[5] = buf[3] + 100
    %core24 = AIE.core(%tile24) {
        // This acquire will stall since locks are initialized to Release, 0
        AIE.useLock(%lock24_1, "Acquire", 1)

        %idx1 = arith.constant 3 : index
        %d1   = memref.load %buf[%idx1] : memref<256xi32>
        %c1   = arith.constant 100 : i32 
        %d2   = arith.addi %d1, %c1 : i32
		%idx2 = arith.constant 5 : index
		memref.store %d2, %buf[%idx2] : memref<256xi32> 

        // This release doesn't do much in our example but mimics ping-pong
        AIE.useLock(%lock24_1, "Release", 0)
        AIE.end
    }

}