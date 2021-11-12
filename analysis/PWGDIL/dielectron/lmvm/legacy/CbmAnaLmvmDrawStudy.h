/* Copyright (C) 2012-2016 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Elena Lebedeva [committer], Florian Uhlig */

#ifndef CBM_ANA_LMVM_DRAW_STUDY
#define CBM_ANA_LMVM_DRAW_STUDY

#include "TObject.h"

#include <string>
#include <vector>

#include "LmvmHist.h"

class TH1;
class TH2D;
class TH1D;
class TFile;
class TCanvas;
class CbmHistManager;
class CbmAnaPTree;

class CbmAnaLmvmDrawStudy : public TObject {

public:
  /**
   * \brief Default constructor.
   */
  CbmAnaLmvmDrawStudy() : TObject(), fCanvas(), fNofStudies(0), fStudyNames(), fHM(), fMeanFiles(), fOutputDir("") { ; }

  /**
    * \brief Destructor.
    */
  virtual ~CbmAnaLmvmDrawStudy() { ; }

  /**
   * \brief Implement functionality of drawing histograms in the macro
   * from the specified files, this function should be called from macro.
   * \param[in] fileNames Names of the file with histograms.
   * \param[in] studyNames Names of the study to be drawn in Legend.
   * \param[in] outputDir Name of the output directory.
   * \param[in] useMvd .
   **/
  void DrawFromFile(const std::vector<std::string>& fileNames, const std::vector<std::string>& fileNamesMean,
                    const std::vector<std::string>& studyNames, const std::string& outputDir = "");

private:
  std::vector<TCanvas*> fCanvas;  // store all pointers to TCanvas -> save to images
  Int_t fNofStudies;
  std::vector<std::string> fStudyNames;

  std::vector<CbmHistManager*> fHM;  // store pointers to histogram manager for different simulations

  std::vector<std::string> fMeanFiles;  // Files mean

  std::string fOutputDir;  // output directory for figures and .json file

  void SaveCanvasToImage();

  void DrawMinv();

  ClassDef(CbmAnaLmvmDrawStudy, 1);
};

#endif
