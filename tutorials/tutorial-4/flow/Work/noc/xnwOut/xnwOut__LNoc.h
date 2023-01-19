// (c) Copyright 1995-2018 Xilinx, Inc. All rights reserved.

#include <memory>
#include "NOCMasterUnitXtlm.h"
#include "NOCSlaveUnitXtlm.h"
#include "NOCDdrMcXtlm.h"
#include "NOCCoreXtlm.h"
#include "properties.h"

class LNoc : public sc_core::sc_module
{
public:
	LNoc( sc_core::sc_module_name, const xsc::common_cpp::properties& );

	// clocks and resets
	sc_core::sc_in_clk   noc_clk; 
	sc_core::sc_in<bool> reset_n;
	sc_core::sc_in_clk PS_AIE_Config_Master_axi_clk;
	sc_core::sc_in_clk S00_AXI_axi_clk;
	sc_core::sc_in_clk S01_AXI_axi_clk;
	sc_core::sc_in_clk S02_AXI_axi_clk;
	sc_core::sc_in_clk S03_AXI_axi_clk;
	sc_core::sc_in_clk S04_AXI_axi_clk;
	sc_core::sc_in_clk S05_AXI_axi_clk;
	sc_core::sc_in_clk S06_AXI_axi_clk;
	sc_core::sc_in_clk S07_AXI_axi_clk;
	sc_core::sc_in_clk S08_AXI_axi_clk;
	sc_core::sc_in_clk S09_AXI_axi_clk;
	sc_core::sc_in_clk S10_AXI_axi_clk;
	sc_core::sc_in_clk S11_AXI_axi_clk;
	sc_core::sc_in_clk S12_AXI_axi_clk;
	sc_core::sc_in_clk S13_AXI_axi_clk;
	sc_core::sc_in_clk S14_AXI_axi_clk;
	sc_core::sc_in_clk S15_AXI_axi_clk;
	sc_core::sc_in_clk PS_AIE_Config_Slave_axi_clk;
	sc_core::sc_in_clk PS_PL_RTP_Slave_axi_clk;
	sc_core::sc_in_clk MC_mc_clk;

	// AXI sockets
	xtlm::xtlm_aximm_target_socket  PS_AIE_Config_Master_axi_rd;
	xtlm::xtlm_aximm_target_socket  S00_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S01_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S02_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S03_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S04_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S05_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S06_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S07_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S08_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S09_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S10_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S11_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S12_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S13_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S14_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  S15_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  PS_AIE_Config_Master_axi_wr;
	xtlm::xtlm_aximm_target_socket  S00_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S01_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S02_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S03_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S04_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S05_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S06_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S07_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S08_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S09_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S10_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S11_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S12_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S13_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S14_AXI_axi_wr;
	xtlm::xtlm_aximm_target_socket  S15_AXI_axi_wr;
	xtlm::xtlm_aximm_initiator_socket PS_AIE_Config_Slave_axi_rd;
	xtlm::xtlm_aximm_initiator_socket PS_PL_RTP_Slave_axi_rd;
	xtlm::xtlm_aximm_initiator_socket PS_AIE_Config_Slave_axi_wr;
	xtlm::xtlm_aximm_initiator_socket PS_PL_RTP_Slave_axi_wr;

	// NoC components
	NOCMasterUnitXtlm PS_AIE_Config_Master;
	NOCMasterUnitXtlm S00_AXI;
	NOCMasterUnitXtlm S01_AXI;
	NOCMasterUnitXtlm S02_AXI;
	NOCMasterUnitXtlm S03_AXI;
	NOCMasterUnitXtlm S04_AXI;
	NOCMasterUnitXtlm S05_AXI;
	NOCMasterUnitXtlm S06_AXI;
	NOCMasterUnitXtlm S07_AXI;
	NOCMasterUnitXtlm S08_AXI;
	NOCMasterUnitXtlm S09_AXI;
	NOCMasterUnitXtlm S10_AXI;
	NOCMasterUnitXtlm S11_AXI;
	NOCMasterUnitXtlm S12_AXI;
	NOCMasterUnitXtlm S13_AXI;
	NOCMasterUnitXtlm S14_AXI;
	NOCMasterUnitXtlm S15_AXI;
	NOCSlaveUnitXtlm  PS_AIE_Config_Slave;
	NOCSlaveUnitXtlm  PS_PL_RTP_Slave;
	NOCDdrMcXtlm      MC;

	// SNOC
	NOCCoreXtlm sNoc;
};

extern "C" std::unique_ptr<LNoc> createLNoc(sc_core::sc_module_name nm);
