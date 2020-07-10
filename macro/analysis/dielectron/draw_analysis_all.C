/** draw_analysis.C
 * @author Elena Lebedeva <e.lebedeva@gsi.de>
 * @since 2011
 * @version 3.0
 **/

void draw_analysis_all() {

  Bool_t useMvd   = false;
  std::string dir = "/Users/slebedev/Development/cbm/data/lmvm/gregor_lmvm/";
  std::string fileName = "analysis.agag.4.5gev.mbias.all.root";

  std::string script = std::string(TString(gSystem->Getenv("SCRIPT")).Data());
  if (script == "yes") {
    dir = std::string(TString(gSystem->Getenv("LMVM_MAIN_DIR")).Data());
    fileName =
      std::string(TString(gSystem->Getenv("LMVM_ANA_FILE_NAME")).Data());
  }

  std::string outputDir = dir + "lmvm_results_all/";
  std::string fnInmed   = dir + "inmed_" + fileName;
  std::string fnQgp     = "";
  std::string fnOmega   = dir + "omegaepem_" + fileName;
  std::string fnPhi     = dir + "phi_" + fileName;
  std::string fnOmegaD  = dir + "omegadalitz_" + fileName;

  CbmAnaDielectronTaskDrawAll* draw = new CbmAnaDielectronTaskDrawAll();
  draw->DrawHistosFromFile(
    fnInmed, fnQgp, fnOmega, fnPhi, fnOmegaD, outputDir, useMvd);
}
