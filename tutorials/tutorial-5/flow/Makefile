
.PHONY: all clean

# TODO - Change this to env variable?
SYSROOT = /group/xrlabs/platforms/vck190-pynq-v2.7/sysroot

all: tutorial-5.exe

# Command line mlir-aie compile script "aiecc.py"
# Sysroot and host target used to cross compile  host executable
# Local runtime_lib needed for testbench functions
tutorial-5.exe: aie.mlir test.cpp 
	aiecc.py -j4 --sysroot=${SYSROOT} --host-target=aarch64-linux-gnu ./aie.mlir -I../../../runtime_lib ../../../runtime_lib/test_library.cpp ./test.cpp -o $@

Work/ps/c_rts/systemC/generated-objects/ps_i96.so: test.cpp aie.mlir
	make -C Work/ps/c_rts/systemC link

Work/arch/aieshim_solution.aiesol:
	make -C Work/arch aieshim_solution.aiesol

Work/reports/graph.xpe:
	make -C Work/reports graph.xpe


sim: Work/ps/c_rts/systemC/generated-objects/ps_i96.so Work/arch/aieshim_solution.aiesol Work/reports/graph.xpe
	aiesimulator --pkg-dir=./Work --dump-vcd foo

view: routes.json
	../../scripts/visualize.py $<

routes.json: aie.mlir
	aie-opt --aie-create-pathfinder-flows --aie-find-flows $< | aie-translate --aie-flows-to-json > $@

clean:
	rm -rf acdc_project *elf *exe core*
