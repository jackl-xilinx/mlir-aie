<!---//===- README.md --------------------------*- Markdown -*-===//
//
// This file is licensed under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
// Copyright (C) 2022, Advanced Micro Devices, Inc.
// 
//===----------------------------------------------------------------------===//-->

# <ins>Tutorial 1 - modules, tile, buffer, core</ins>
In the MLIR-based AI Engine representation, every physical component of the AI Engine array including connections are declared within the top level block called a `module`. All parameters and customizations of these components are then elaborated within the `module`. We generally write the MLIR code in a file with the .mlir file extension as it integrates well with the lit based auto-test of LLVM, such as those found in `test` sub-folder. A module declaration is shown below:

```
module @module_name {
    ... 
    AI Engine array components and connections 
    ...
}
```

## <ins>Tile Components</ins>
AI Engine tiles are the basic building blocks of AIE designs and can be declared as `AIE.tile(col,row)`. Examples incude:
```
%tile13 = AIE.tile(1,3)
%tile23 = AIE.tile(2,3)
%tile33 = AIE.tile(3,3)
```
The two major components of an AI Engine tile is 

* VLIW processor core declared as `AIE.core(tileName) { body }`
* Local memory buffer declared as `AIE.buffer(tileName) : memref<depthxdata_type> { body }`. 

Examples declarations include:
```
AIE.core(%tile13) {
    ... 
    core body 
    ...
}

%buff0 = AIE.buffer(%tile13) : memref<256xi32>
%buff1 = AIE.buffer(%tile13) : memref<256xi32>
```
The association between these declarations and the physical AI Engine tile components can be seen here. For more details on mlir-aie dialect syntax, you can refer to the onine reference document [here](https://xilinx.github.io/mlir-aie/AIEDialect.html).
<img src="../images/diagram1.jpg?raw=true" width="800">

### <ins>Tile</ins>

For the tile, we simply declare its coordinates by column and row 
>**Note:** index values start at 0, with row 0 belonging to the shim which is not a regular row. The first regular row for first generation AI engines is row index 1. 
Tile declaration is mainly needed so other sub components can be associated to the tile by name. Some higher level logical components may also automatically declare tiles so they are enabled (e.g. logical flows require the intermediate tiles along the path to be enabled to support stream switch routing).

The type of tiles and orientation of its associated local memory is architecture dependent. In the case of the first generation of AI Engines, each tile has an associated local memory physically to its left or right depending on the row.
* odd row - memory on left
* even row - memory on right

<p><img src="../images/diagram3.jpg?raw=true" width="500"><p>

### <ins>Buffer</ins>

When delcaring a buffer, we pass in associated AIE tile and declare the buffer parameters. Those parameters are the depth and data type width (though the local memory itself is not physically organized in this way). 
> One important note about buffers is that one buffer is not strictly mapped to the entire local memory. You can declare multiple buffers that are associated with the local memory of a tile and they would, by default, be allocated sequentially in that tile's local memory.

### <ins>Core</ins>

The AIE core functionality is defined within the core body. More details on this is in tutorial 2.

We will be introducing more components and the ways these components are customized in subsequent tutorials. Additional syntax for these MLIR-based AI Engine components can be found in the github<area>.io docs [here](https://xilinx.github.io/mlir-aie/AIEDialect.html).

## <ins>Tutorial 1 Lab</ins>
1. Take a close look at the source file [aie.mlir](aie.mlir) and read through the code comments which walks through the syntax of this simple design. What is the variable name of the module? <img src="../images/answer1.jpg" title="tutorial_1" height=25>

2. After building and installing `mlir-aie`, run make to compile the first tutorial.
    ```
    > make
    ```
Under the hood, `make` calls `aiecc.py` which itself calls a number of utilities that are built as part of the `mlir-aie` project (`aie-translate`, `aie-opt`). The MLIR operations inside the core are then converted to an LLVM representation which the AMD internal compiler (currently xchesscc) takes and builds the executable that will run on each individaul AIE tile. 
   
3. In [aie.mlir](aie.mlir), what is the variable name for tile(1,4)? <img src="../images/answer1.jpg" title="%tile14" height=25> 

    What about the variable name and size of the buffer that is associated with the local memory of tile(1,4)? <img src="../images/answer1.jpg" title="%buf, 256 x int32" height=25>

In first generation AI Engines, each tile has 32 kB of local memory assigned to it. In addition, it can access the local memory in the other 3 cardinal directions giving a total accessible memory of 128 kB.

4. What percentage of the local memory (32 kB) does this design currently use?
<img src="../images/answer1.jpg" title="3%, 256 / 8192 int32" height=25>

5. Change the size of the buffer to the size of our local memory (8192 x i32) and run `make` again. What do you expect to happen and what happens instead? <img src="../images/answer1.jpg" title="Allocated buffers exceed local memory." height=25>

While we have a separate 16 kB of program memory which stores the AIE program code, the 32 kB of data memory is also used for the program stack. By default, the tool reserves 4096 bytes for the stack so all buffers are then allocated immediately after that. 

6. Declare an horizontally adjacent tile (pay attention to which row we're in) so that tile (1,4) can access the neighbor tile's local memory. Declare a buffer in this tile that uses the entire local memory (8192 x i32) and replace the reference %buf in line 34 with the new buffer and run `make ` again. <img src="../images/answer1.jpg" title="You can see answer by calling 'make tutorial-1_q6.exe'" height=25>

To create complete AI Engine designs that can run on a Versal device, we also need code that is used to configure and program the AIE array. `aiecc.py` is used to translate the physical MLIR description of our design into elf files that run on each tile (e.g. core_1_4.elf). It also generates configuration and initialization functions under `acdc_project/aie_inc.cpp`. These functions can be invoked as part of a host code testbench [test.cpp](test.cpp) which can be run on a target platform such as the the vck190 development board. While running our `mlir-aie` tools on an x86 machine, we can cross-compile the final executable (`tutorial-1.exe`) to run on the ARM processor on the Versal device.

7. Take a look at  [test.cpp](test.cpp). There are a number of configuration functions for initializing and configuring the AIE array. These functions are defined in the test library [test_library.h](../../runtime_lib/test_library.h) and the generated `acdc_project/aie_inc.cpp` file. Read through this testbench to see the explanations of what each helper function does and how we can check the results after confiuring and enabling the AIE cores. What is the expected value at buf[3] that we're checking for after our design is run? <img src="../images/answer1.jpg" title="14" height=25>

8. **PLACEHOLDER** Run simuation

9. Copy the generated executables (tutorial-1.exe, core_1_4.elf) to the vck190 board and run the test bench executable to see that the compiled program works on the board.
    ```
    sudo ./tutorial-1.exe
    ```

## 
