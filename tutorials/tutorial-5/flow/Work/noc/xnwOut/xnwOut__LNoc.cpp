// (c) Copyright 1995-2018 Xilinx, Inc. All rights reserved.

#include "xnwOut/xnwOut__LNoc.h"
static xsc::common_cpp::properties PS_AIE_Config_Master_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X0Y5" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S00_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X10Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S01_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X15Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S02_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X1Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S03_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X3Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S04_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X2Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S05_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X9Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S06_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X8Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S07_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X14Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S08_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X7Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S09_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X12Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S10_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X4Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S11_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X11Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S12_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X16Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S13_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X13Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S14_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X6Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties S15_AXI_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NMU128_X5Y10" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties PS_AIE_Config_Slave_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NSU128_X5Y6" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties PS_PL_RTP_Slave_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "NOC_NSU512_X0Y0" );
	props.addBitString( "REG_DWIDTH", "10000" );
	return props;
}();

static xsc::common_cpp::properties MC_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PHYS_NAME", "DDRMC_X1Y0" );
	props.addBool     ( "EXT_DC_CLOCK", "true" );
	return props;
}();

static xsc::common_cpp::properties SNoc_Params = []()
-> xsc::common_cpp::properties {
	auto props = xsc::common_cpp::properties();
	props.addString   ( "PARAM_FILE", "/group/xrlabs/jackl/repo/ghe/jackl/AIE_examples/adf_full_gmio/Work/noc/xnwOut/xnwOut__snoc__registers__out.txt" );
	props.addString   ( "DEVICE_FILE"  , "/proj/xbuilds/2022.2_daily_latest/installs/lin64/Vivado/2022.2/data/parts/xilinx/devint/vault/versal/data/device_misc/xcvc1902/xcvc1902.nocme.v10.model" );
	props.addString   ( "ADDRMAP_FILE" , "/proj/xbuilds/2022.2_daily_latest/installs/lin64/Vivado/2022.2/data/parts/xilinx/common/addressing/Versal_Address_Map.xml" );
	return props;
}();

LNoc::LNoc( sc_core::sc_module_name nm, const xsc::common_cpp::properties& props)
	: sc_core::sc_module (nm) 
	, noc_clk ("noc_clk") 
	, reset_n ("reset_n") 
	, PS_AIE_Config_Master_axi_clk ("PS_AIE_Config_Master_axi_clk")
	, S00_AXI_axi_clk ("S00_AXI_axi_clk")
	, S01_AXI_axi_clk ("S01_AXI_axi_clk")
	, S02_AXI_axi_clk ("S02_AXI_axi_clk")
	, S03_AXI_axi_clk ("S03_AXI_axi_clk")
	, S04_AXI_axi_clk ("S04_AXI_axi_clk")
	, S05_AXI_axi_clk ("S05_AXI_axi_clk")
	, S06_AXI_axi_clk ("S06_AXI_axi_clk")
	, S07_AXI_axi_clk ("S07_AXI_axi_clk")
	, S08_AXI_axi_clk ("S08_AXI_axi_clk")
	, S09_AXI_axi_clk ("S09_AXI_axi_clk")
	, S10_AXI_axi_clk ("S10_AXI_axi_clk")
	, S11_AXI_axi_clk ("S11_AXI_axi_clk")
	, S12_AXI_axi_clk ("S12_AXI_axi_clk")
	, S13_AXI_axi_clk ("S13_AXI_axi_clk")
	, S14_AXI_axi_clk ("S14_AXI_axi_clk")
	, S15_AXI_axi_clk ("S15_AXI_axi_clk")
	, PS_AIE_Config_Slave_axi_clk ("PS_AIE_Config_Slave_axi_clk")
	, PS_PL_RTP_Slave_axi_clk ("PS_PL_RTP_Slave_axi_clk")
	, MC_mc_clk ("MC_mc_clk")
	, PS_AIE_Config_Master_axi_rd ("PS_AIE_Config_Master_axi_rd", 512 )
	, S00_AXI_axi_rd ("S00_AXI_axi_rd", 512 )
	, S01_AXI_axi_rd ("S01_AXI_axi_rd", 512 )
	, S02_AXI_axi_rd ("S02_AXI_axi_rd", 512 )
	, S03_AXI_axi_rd ("S03_AXI_axi_rd", 512 )
	, S04_AXI_axi_rd ("S04_AXI_axi_rd", 512 )
	, S05_AXI_axi_rd ("S05_AXI_axi_rd", 512 )
	, S06_AXI_axi_rd ("S06_AXI_axi_rd", 512 )
	, S07_AXI_axi_rd ("S07_AXI_axi_rd", 512 )
	, S08_AXI_axi_rd ("S08_AXI_axi_rd", 512 )
	, S09_AXI_axi_rd ("S09_AXI_axi_rd", 512 )
	, S10_AXI_axi_rd ("S10_AXI_axi_rd", 512 )
	, S11_AXI_axi_rd ("S11_AXI_axi_rd", 512 )
	, S12_AXI_axi_rd ("S12_AXI_axi_rd", 512 )
	, S13_AXI_axi_rd ("S13_AXI_axi_rd", 512 )
	, S14_AXI_axi_rd ("S14_AXI_axi_rd", 512 )
	, S15_AXI_axi_rd ("S15_AXI_axi_rd", 512 )
	, PS_AIE_Config_Master_axi_wr ("PS_AIE_Config_Master_axi_wr", 512 )
	, S00_AXI_axi_wr ("S00_AXI_axi_wr", 512 )
	, S01_AXI_axi_wr ("S01_AXI_axi_wr", 512 )
	, S02_AXI_axi_wr ("S02_AXI_axi_wr", 512 )
	, S03_AXI_axi_wr ("S03_AXI_axi_wr", 512 )
	, S04_AXI_axi_wr ("S04_AXI_axi_wr", 512 )
	, S05_AXI_axi_wr ("S05_AXI_axi_wr", 512 )
	, S06_AXI_axi_wr ("S06_AXI_axi_wr", 512 )
	, S07_AXI_axi_wr ("S07_AXI_axi_wr", 512 )
	, S08_AXI_axi_wr ("S08_AXI_axi_wr", 512 )
	, S09_AXI_axi_wr ("S09_AXI_axi_wr", 512 )
	, S10_AXI_axi_wr ("S10_AXI_axi_wr", 512 )
	, S11_AXI_axi_wr ("S11_AXI_axi_wr", 512 )
	, S12_AXI_axi_wr ("S12_AXI_axi_wr", 512 )
	, S13_AXI_axi_wr ("S13_AXI_axi_wr", 512 )
	, S14_AXI_axi_wr ("S14_AXI_axi_wr", 512 )
	, S15_AXI_axi_wr ("S15_AXI_axi_wr", 512 )
	, PS_AIE_Config_Slave_axi_rd ("PS_AIE_Config_Slave_axi_rd", 512 )
	, PS_PL_RTP_Slave_axi_rd ("PS_PL_RTP_Slave_axi_rd", 512 )
	, PS_AIE_Config_Slave_axi_wr ("PS_AIE_Config_Slave_axi_wr", 512 )
	, PS_PL_RTP_Slave_axi_wr ("PS_PL_RTP_Slave_axi_wr", 512 )
	, PS_AIE_Config_Master ("PS_AIE_Config_Master", PS_AIE_Config_Master_Params) 
	, S00_AXI ("S00_AXI", S00_AXI_Params) 
	, S01_AXI ("S01_AXI", S01_AXI_Params) 
	, S02_AXI ("S02_AXI", S02_AXI_Params) 
	, S03_AXI ("S03_AXI", S03_AXI_Params) 
	, S04_AXI ("S04_AXI", S04_AXI_Params) 
	, S05_AXI ("S05_AXI", S05_AXI_Params) 
	, S06_AXI ("S06_AXI", S06_AXI_Params) 
	, S07_AXI ("S07_AXI", S07_AXI_Params) 
	, S08_AXI ("S08_AXI", S08_AXI_Params) 
	, S09_AXI ("S09_AXI", S09_AXI_Params) 
	, S10_AXI ("S10_AXI", S10_AXI_Params) 
	, S11_AXI ("S11_AXI", S11_AXI_Params) 
	, S12_AXI ("S12_AXI", S12_AXI_Params) 
	, S13_AXI ("S13_AXI", S13_AXI_Params) 
	, S14_AXI ("S14_AXI", S14_AXI_Params) 
	, S15_AXI ("S15_AXI", S15_AXI_Params) 
	, MC ("MC", MC_Params) 
	, PS_AIE_Config_Slave ("PS_AIE_Config_Slave", PS_AIE_Config_Slave_Params) 
	, PS_PL_RTP_Slave ("PS_PL_RTP_Slave", PS_PL_RTP_Slave_Params) 
	, sNoc( "sNoc", SNoc_Params )
{
	PS_AIE_Config_Master.aclk ( PS_AIE_Config_Master_axi_clk);
	S00_AXI.aclk ( S00_AXI_axi_clk);
	S01_AXI.aclk ( S01_AXI_axi_clk);
	S02_AXI.aclk ( S02_AXI_axi_clk);
	S03_AXI.aclk ( S03_AXI_axi_clk);
	S04_AXI.aclk ( S04_AXI_axi_clk);
	S05_AXI.aclk ( S05_AXI_axi_clk);
	S06_AXI.aclk ( S06_AXI_axi_clk);
	S07_AXI.aclk ( S07_AXI_axi_clk);
	S08_AXI.aclk ( S08_AXI_axi_clk);
	S09_AXI.aclk ( S09_AXI_axi_clk);
	S10_AXI.aclk ( S10_AXI_axi_clk);
	S11_AXI.aclk ( S11_AXI_axi_clk);
	S12_AXI.aclk ( S12_AXI_axi_clk);
	S13_AXI.aclk ( S13_AXI_axi_clk);
	S14_AXI.aclk ( S14_AXI_axi_clk);
	S15_AXI.aclk ( S15_AXI_axi_clk);
	PS_AIE_Config_Master.tbclk (reset_n);
	S00_AXI.tbclk (reset_n);
	S01_AXI.tbclk (reset_n);
	S02_AXI.tbclk (reset_n);
	S03_AXI.tbclk (reset_n);
	S04_AXI.tbclk (reset_n);
	S05_AXI.tbclk (reset_n);
	S06_AXI.tbclk (reset_n);
	S07_AXI.tbclk (reset_n);
	S08_AXI.tbclk (reset_n);
	S09_AXI.tbclk (reset_n);
	S10_AXI.tbclk (reset_n);
	S11_AXI.tbclk (reset_n);
	S12_AXI.tbclk (reset_n);
	S13_AXI.tbclk (reset_n);
	S14_AXI.tbclk (reset_n);
	S15_AXI.tbclk (reset_n);
	PS_AIE_Config_Master.aresetn (reset_n);
	S00_AXI.aresetn (reset_n);
	S01_AXI.aresetn (reset_n);
	S02_AXI.aresetn (reset_n);
	S03_AXI.aresetn (reset_n);
	S04_AXI.aresetn (reset_n);
	S05_AXI.aresetn (reset_n);
	S06_AXI.aresetn (reset_n);
	S07_AXI.aresetn (reset_n);
	S08_AXI.aresetn (reset_n);
	S09_AXI.aresetn (reset_n);
	S10_AXI.aresetn (reset_n);
	S11_AXI.aresetn (reset_n);
	S12_AXI.aresetn (reset_n);
	S13_AXI.aresetn (reset_n);
	S14_AXI.aresetn (reset_n);
	S15_AXI.aresetn (reset_n);
	PS_AIE_Config_Slave.aclk ( PS_AIE_Config_Slave_axi_clk);
	PS_PL_RTP_Slave.aclk ( PS_PL_RTP_Slave_axi_clk);
	PS_AIE_Config_Slave.aresetn (reset_n);
	PS_PL_RTP_Slave.aresetn (reset_n);
	MC.mc_clk( MC_mc_clk);
	MC.mc_rst_n(reset_n);
	PS_AIE_Config_Master_axi_rd.bind(PS_AIE_Config_Master.arr_socket);
	S00_AXI_axi_rd.bind(S00_AXI.arr_socket);
	S01_AXI_axi_rd.bind(S01_AXI.arr_socket);
	S02_AXI_axi_rd.bind(S02_AXI.arr_socket);
	S03_AXI_axi_rd.bind(S03_AXI.arr_socket);
	S04_AXI_axi_rd.bind(S04_AXI.arr_socket);
	S05_AXI_axi_rd.bind(S05_AXI.arr_socket);
	S06_AXI_axi_rd.bind(S06_AXI.arr_socket);
	S07_AXI_axi_rd.bind(S07_AXI.arr_socket);
	S08_AXI_axi_rd.bind(S08_AXI.arr_socket);
	S09_AXI_axi_rd.bind(S09_AXI.arr_socket);
	S10_AXI_axi_rd.bind(S10_AXI.arr_socket);
	S11_AXI_axi_rd.bind(S11_AXI.arr_socket);
	S12_AXI_axi_rd.bind(S12_AXI.arr_socket);
	S13_AXI_axi_rd.bind(S13_AXI.arr_socket);
	S14_AXI_axi_rd.bind(S14_AXI.arr_socket);
	S15_AXI_axi_rd.bind(S15_AXI.arr_socket);
	PS_AIE_Config_Master_axi_wr.bind(PS_AIE_Config_Master.awb_socket);
	S00_AXI_axi_wr.bind(S00_AXI.awb_socket);
	S01_AXI_axi_wr.bind(S01_AXI.awb_socket);
	S02_AXI_axi_wr.bind(S02_AXI.awb_socket);
	S03_AXI_axi_wr.bind(S03_AXI.awb_socket);
	S04_AXI_axi_wr.bind(S04_AXI.awb_socket);
	S05_AXI_axi_wr.bind(S05_AXI.awb_socket);
	S06_AXI_axi_wr.bind(S06_AXI.awb_socket);
	S07_AXI_axi_wr.bind(S07_AXI.awb_socket);
	S08_AXI_axi_wr.bind(S08_AXI.awb_socket);
	S09_AXI_axi_wr.bind(S09_AXI.awb_socket);
	S10_AXI_axi_wr.bind(S10_AXI.awb_socket);
	S11_AXI_axi_wr.bind(S11_AXI.awb_socket);
	S12_AXI_axi_wr.bind(S12_AXI.awb_socket);
	S13_AXI_axi_wr.bind(S13_AXI.awb_socket);
	S14_AXI_axi_wr.bind(S14_AXI.awb_socket);
	S15_AXI_axi_wr.bind(S15_AXI.awb_socket);
	PS_AIE_Config_Slave.arr_socket.bind(PS_AIE_Config_Slave_axi_rd);
	PS_PL_RTP_Slave.arr_socket.bind(PS_PL_RTP_Slave_axi_rd);
	PS_AIE_Config_Slave.awb_socket.bind(PS_AIE_Config_Slave_axi_wr);
	PS_PL_RTP_Slave.awb_socket.bind(PS_PL_RTP_Slave_axi_wr);
	sNoc.noc_clk  ( noc_clk );
	sNoc.noc_rst_n( reset_n );
}

std::unique_ptr<LNoc> createLNoc(sc_core::sc_module_name nm)
	{return std::unique_ptr<LNoc>(new LNoc(nm, xsc::common_cpp::properties()));}
