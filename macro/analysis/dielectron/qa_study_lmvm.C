/**
 * \file qa_study_lmvm.C
 * \brief Macro for generation of summary reports for LMVM analysis.
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2012
 */

void qa_study_lmvm()
{
   gROOT->LoadMacro("$VMCWORKDIR/gconfig/basiclibs.C");
   basiclibs();
   gROOT->LoadMacro("$VMCWORKDIR/macro/rich/cbmlibs.C");
   cbmlibs();
   gSystem->Load("libAnalysis");

   std::vector<std::string> results, names;
   std::string outputDir;

   std::string dir = "/Users/slebedev/Development/cbm/data/lmvm/";
   results.push_back(dir + "apr16/apr16_trdtofclustering/8gev/geosetup_v1512_8gev/lmvm_results_all/draw_all_hist.root");
   results.push_back(dir + "apr16/apr16_trdclustering/8gev/geosetup_v1512_8gev//lmvm_results_all/draw_all_hist.root");
   results.push_back(dir + "apr16/apr16_tofclustering/8gev/geosetup_v1512_8gev//lmvm_results_all/draw_all_hist.root");
   results.push_back(dir + "apr16/apr16_trdtofsmearing/8gev/geosetup_v1512_8gev//lmvm_results_all/draw_all_hist.root");

   names.push_back("TRD&TOF clustering");
   names.push_back("TRD clustering");
   names.push_back("TOF clustering");
   names.push_back("TRD&TOF smearing");

   outputDir = "/Users/slebedev/Development/cbm/data/lmvm/apr16_lmvm_study_report_8gev/";

   CbmAnaDielectronReports reports;
   reports.CreateStudyReport("LMVM", results, names, outputDir);
}
