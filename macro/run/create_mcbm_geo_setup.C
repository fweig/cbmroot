/* Copyright (C) 2022 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer]  */

void create_mcbm_geo_setup(uint64_t ulRunId, const std::string& sOutputDir = "data/")
{
  /// Do automatic mapping
  std::string sSetupName = "";
  cbm::mcbm::ToForceLibLoad dummy;  /// Needed to trigger loading of the library as no fct dict in ROOT6 and CLING
  try {
    sSetupName = cbm::mcbm::GetSetupFromRunId(ulRunId);
  }
  catch (const std::invalid_argument& e) {
    std::cout << "Error in mapping from runID to setup name: " << e.what() << std::endl;
    return;
  }
  std::string sPath = sOutputDir + "/" + sSetupName;
  if (gSystem->AccessPathName(Form("%s.geo.root", sPath.data()))) {
    std::string sSrcDir         = gSystem->Getenv("VMCWORKDIR");  // top source directory
    std::string sTransportMacro = sSrcDir + "/macro/mcbm/mcbm_transport.C";
    gROOT->LoadMacro(sTransportMacro.data());

    std::string sTransportCall = "mcbm_transport( 1, \"" + sSetupName + "\", \"" + sPath + "\")";
    gInterpreter->ProcessLine(sTransportCall.data());
  }
  else {
    std::cout << "Geofile already existing for this run at: " << Form("%s.geo.root", sPath.data()) << std::endl;
    std::cout << " Test passed" << std::endl;
    std::cout << " All ok " << std::endl;
  }
}
