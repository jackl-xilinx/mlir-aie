<!-- Autogenerated by mlir-tblgen; don't manually edit -->
### `-aie-assign-runtime-sequence-bd-ids`

_Assign IDs to Buffer Descriptors Configured in the Runtime Sequence_

### `-aie-create-cores`

_Create CoreOp, MemOp, and FlowOp of AIE dialect_

Lower toplevel 'call' operations with implicit data movement into
aie.core, aie.mem, and aie.flow operations with explicit data
movement.  The tile tile associated with the operations is inferred
based on the aie.x and aie.y attributes of the call operation. e.g.:
```
func.func @aie_task(%arg0: memref<256xi32>, %arg1: i32) -> () {
  ...
  return
}
%a = constant 0 : i32
func.call @aie_task(%buf, %a) { aie.x = 1, aie.y = 1 } : (memref<256xi32>, i32) -> ()

```
will result in:
```
%0 = AIE.tile(1, 1)
%1 = AIE.buffer(%0) : memref<256xi32>
%2 = AIE.buffer(%0) : memref<1xi32>
%c0_i32 = constant 0 : i32
%5 = AIE.core(%0) {
  ...
}
```
In this case, each argument to aie_task results in an explicit buffer accessed by the core.

### `-aie-create-locks`

_Generate physical lock ops from logical lock (token) ops_

Tokens represent high-level buffer synchronization through a sequence of
pipeline stages.  This pass lowers token operations into physical aie.lock
operations.
### `-aie-dma-tasks-to-npu`

_Lower configured DMA tasks to NPU instructions_

### `-aie-dma-to-npu`
### `-aie-herd-routing`

_Lowering herds with place and route ops to AIE cores, mems, and switchboxes_

An experimental pass which elaborates herd operations (e.g. aie.herd, aie.iter, aie.select)
into an explicit representation (e.g. aie.core, aie.mem, etc.).
### `-aie-lower-broadcast-packet`

_Replace combination of broadcast and packet-switch by packet-flow_

Replace combination of broadcast and packet-switch by packet-flow
### `-aie-lower-memcpy`

_Lower aie.memcpy operations to Flows and DMA programs_

aie.memcpy operations are an experimental high-level abstraction which
move data from one buffer to another.
This pass lowers them into appropriate aie.flow and aie.mem operations.
### `-aie-lower-multicast`

_Lower AIE.multicast operation to AIE.flow operations_

This pass replaces AIE.multicast operation with the equivalent number of AIE.flow
operations. The lowered AIE.flow operations have the same source port but different
destinations.
### `-aie-materialize-bd-chains`

_Concretize aie.bd_chain ops at aiex.start_task use sites_

Inlines pre-defined `aie.bd_chains` at `aiex.start_task` use sites to become `aiex.dma_configure_task` and `aiex.dma_start_task` ops.
### `-aie-substitute-shim-dma-allocations`

_Replace symbolic references to `aie.shim_dma_allocation` ops with their `(tile, direction, channel)` triple_

### `-aiex-standard-lowering`

_Lower AIEX operations_

AIEX Npu Ops are removed.
