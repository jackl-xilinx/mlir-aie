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
	sc_core::sc_in_clk PS_AIE_Config_Slave_axi_clk;
	sc_core::sc_in_clk PS_PL_RTP_Slave_axi_clk;
	sc_core::sc_in_clk MC_mc_clk;

	// AXI sockets
	xtlm::xtlm_aximm_target_socket  PS_AIE_Config_Master_axi_rd;
	xtlm::xtlm_aximm_target_socket  S00_AXI_axi_rd;
	xtlm::xtlm_aximm_target_socket  PS_AIE_Config_Master_axi_wr;
	xtlm::xtlm_aximm_target_socket  S00_AXI_axi_wr;
	xtlm::xtlm_aximm_initiator_socket PS_AIE_Config_Slave_axi_rd;
	xtlm::xtlm_aximm_initiator_socket PS_PL_RTP_Slave_axi_rd;
	xtlm::xtlm_aximm_initiator_socket PS_AIE_Config_Slave_axi_wr;
	xtlm::xtlm_aximm_initiator_socket PS_PL_RTP_Slave_axi_wr;

	// NoC components
	NOCMasterUnitXtlm PS_AIE_Config_Master;
	NOCMasterUnitXtlm S00_AXI;
	NOCSlaveUnitXtlm  PS_AIE_Config_Slave;
	NOCSlaveUnitXtlm  PS_PL_RTP_Slave;
	NOCDdrMcXtlm      MC;

	// SNOC
	NOCCoreXtlm sNoc;
};

extern "C" std::unique_ptr<LNoc> createLNoc(sc_core::sc_module_name nm);
