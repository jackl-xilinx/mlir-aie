// RUN: aiecc.py --sysroot=/group/xrlabs/platforms/pynq_on_versal_vck190_hacked/vck190-sysroot %s -I%S/../../../runtime_lib/ %S/../../../runtime_lib/test_library.cpp %S/test.cpp -o test.elf


module @test6b_core_llvm1 {
  %tile13 = AIE.tile(1, 3)

  %buf13_0 = AIE.buffer(%tile13) { sym_name = "a" } : memref<256xi32>
  %buf13_1 = AIE.buffer(%tile13) { sym_name = "b" } : memref<256xi32>

  %lock13_3 = AIE.lock(%tile13, 3)
  %lock13_5 = AIE.lock(%tile13, 5)

  func private @func(%A: memref<256xi32>, %B: memref<256xi32>) -> ()

  func @func_wrap(%A: memref<256xi32>, %B: memref<256xi32>) -> () {
    call @func(%A, %B) : (memref<256xi32>, memref<256xi32>) -> ()
    return
  }

  %core13 = AIE.core(%tile13) { 
    AIE.useLock(%lock13_3, "Acquire", 1, 0) // acquire for read(e.g. input ping)
    AIE.useLock(%lock13_5, "Acquire", 0, 0) // acquire for write
//    %idx1 = constant 3 : index
//    %val1 = load %buf13_0[%idx1] : memref<256xi32>
//    %2    = addi %val1, %val1 : i32
//    %3 = addi %2, %val1 : i32
//    %4 = addi %3, %val1 : i32
//    %5 = addi %4, %val1 : i32
//    %idx2 = constant 5 : index
//    store %5, %buf13_1[%idx2] : memref<256xi32>
    %A = alloc() : memref<256xi32>
    %B = alloc() : memref<256xi32>
    call @func_wrap(%A, %B) : (memref<256xi32>, memref<256xi32>) -> ()
    AIE.useLock(%lock13_3, "Release", 0, 0) // release for write
    AIE.useLock(%lock13_5, "Release", 1, 0) // release for read
    AIE.end
  } { elf_file = "core_1_3.elf" }

}

