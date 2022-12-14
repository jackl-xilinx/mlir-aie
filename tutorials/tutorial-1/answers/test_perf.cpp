//===- test.cpp -------------------------------------------------*- C++ -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// Copyright (C) 2022, Advanced Micro Devices, Inc.
//
//===----------------------------------------------------------------------===//

#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <thread>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <xaiengine.h>
#include "test_library.h"

#define LOCK_TIMEOUT 100
#define HIGH_ADDR(addr)	((addr & 0xffffffff00000000) >> 32)
#define LOW_ADDR(addr)	(addr & 0x00000000ffffffff)
#define MLIR_STACK_OFFSET 4096

#include "aie_inc.cpp"

int
main(int argc, char *argv[])
{
    printf("Tutorial-1 test start.\n");

    int errors   = 0;
    int num_iter = 1;
    u32 pc0_times[num_iter]; // track timer values

    // Standard helper function for initializing and configuring AIE array.
    // The host is used to initialize/ configure/ program the AIE array.
    // ------------------------------------------------------------------------
    // aie_libxaie_ctx_t - AIE config struct
    // mlir_aie_init_device ##### TODO #######
    // mlir_aie_configure_cores - Reset cores and locks. Load elfs.
    // mlir_aie_configure_switchboxes - Switchboxes not used in this example.
    // mlir_aie_configure_dmas - TileDMAs not used in this example.
    // mlir_aie_initialize_locks - Locks not used in this example.
    aie_libxaie_ctx_t *_xaie = mlir_aie_init_libxaie();
    mlir_aie_init_device(_xaie);                        
    mlir_aie_configure_cores(_xaie);
    mlir_aie_configure_switchboxes(_xaie);
    mlir_aie_configure_dmas(_xaie);
    mlir_aie_initialize_locks(_xaie);

    // Helper function to clear tile data memory
    mlir_aie_clear_tile_memory(_xaie, 1, 4);

    // Check the buffer value at index 3 to ensure it is zeroed out
    // prior to running our simple kernel.
    // ------------------------------------------------------------------------
    // mlir_aie_read_buffer_a14 - helper function to read tile local
    // memory at an offset (offset=3 in this case). _a14 maps to the 
    // symbolic buffer name defined in aie.mlir.
    //
    // mlir_aie_check - helper function to compare values to expected
    // golden value and print error message to stdout and increment 
    // "errors" variable if mismatch occurs.
    mlir_aie_check("Before start cores:", mlir_aie_read_buffer_a14(_xaie, 3), 0,
                   errors);

	// Performance counters
	// Trigger off start (0x00) of an AIE program
	XAie_EventPCEnable(&(_xaie->DevInst), XAie_TileLoc(1,4), 0, 0x00);
	// Trigger off done (0x088) of an AIE program
	XAie_EventPCEnable(&(_xaie->DevInst), XAie_TileLoc(1,4), 1, 0x088);

	// Define custom EventMonitor class to track event triggers for program counter
	EventMonitor pc0(_xaie, 1, 4, 1, XAIE_EVENT_PC_0_CORE, XAIE_EVENT_PC_1_CORE,
					 XAIE_EVENT_NONE_CORE, XAIE_CORE_MOD);
	pc0.set();

    // Helper function to enable all AIE cores
    printf("Start cores\n");
    mlir_aie_start_cores(_xaie);

    // Wait time for cores to run. Number used here is much larger than needed.
    usleep(100);

    pc0_times[0] = pc0.diff(); // store program counter value (0th iteration)

    // Check buffer at index 3 again for expected value of 14
    mlir_aie_check("After start cores:", mlir_aie_read_buffer_a14(_xaie, 3), 14,
                   errors);

    // Print Pass/Fail result of our test
    int res = 0;
    if (!errors) {
      printf("PASS!\n");
      res = 0;
    } else {
      printf("Fail!\n");
      res = -1;
    }

    printf("\nProgram cycle counts:\n")
    // Output the timer values (average, standard deviation) for 1 iteration
    computeStats(pc0_times, 1);

    // Teardown and cleanup of AIE array
    mlir_aie_deinit_libxaie(_xaie);

    printf("Tutorial-1 test done.\n");
    return res;
}