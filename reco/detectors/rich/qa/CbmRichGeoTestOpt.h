/** @file CbmRichGeoTestOpt.h
  * @copyright Copyright (C) 2019 UGiessen, JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Semen Lebedev [committer] **/

/**
 * \file CbmRichGeoTestOpt.h
 * \brief Creates comparison plots for RICH geometry testing.
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2019
 */
#ifndef CBM_RICH_GEO_TEST_OPT
#define CBM_RICH_GEO_TEST_OPT

#include "CbmHistManager.h"

#include "TFile.h"
#include "TObject.h"

using namespace std;

enum CbmRichGeoTestOptFileEnum
{
  kGeoTestBoxFile,
  kGeoTestOmega3File,
  kGeoTestOmega8File,
  kUrqmdTestFile,
  kRecoQaBoxFile,
  kRecoQaUrqmdFile
};

enum CbmRichGeoTestOptHistEnum
{
  kH1MeanHist,
  kH1RmsHist,
  kH2MeanHist,
  kH2RmsHist,
  kHEntriesHist
};

class CbmRichGeoTestOpt : public TObject {
public:
  /**
     * \brief Constructor.
     */
  CbmRichGeoTestOpt();

  /**
     * \brief Destructor.
     */
  virtual ~CbmRichGeoTestOpt();

  void SetFilePathes(vector<string> geoTestPathes, vector<string> geoTestOmega3Pathes,
                     vector<string> geoTestOmega8Pathes, vector<string> urqmdTestPathes, vector<string> recoQaBoxPathes,
                     vector<string> recoQaUrqmdPathes);

  void Draw(Option_t* option = "");

  string GetFilePath(CbmRichGeoTestOptFileEnum fileType, int iFile);
  int GetNofFiles();
  string GetFileEnumName(CbmRichGeoTestOptFileEnum fileEnum);

  pair<double, double> H1MeanRms(CbmRichGeoTestOptFileEnum fileType, int iFile, const string& histName);
  pair<double, double> H2ProjYMeanRms(CbmRichGeoTestOptFileEnum fileType, int iFile, const string& histName);
  double HEntries(CbmRichGeoTestOptFileEnum fileEnum, int iFile, const string& histName);

  void DrawMeanRms(CbmRichGeoTestOptFileEnum fileEnum, const string& histName, CbmRichGeoTestOptHistEnum histEnum,
                   const string& titleY, double minY, double maxY, int nofParts, int nofFilesPart);
  void DrawMeanEff(CbmRichGeoTestOptFileEnum fileEnum, const string& histName1, const string& histName2,
                   const string& titleY, double minY, double maxY, int nofParts, int nofFilesPart, double effCoeff);
  void DrawLines(bool drawCamTilt, bool drawCamY, double minY, double maxY);
  void DrawManyH1(const vector<TH1*>& hist, const vector<string>& legend, double minY, double maxY);

  void DrawMeanRms2D(CbmRichGeoTestOptFileEnum fileEnum, const string& histName, CbmRichGeoTestOptHistEnum histEnum,
                     const string& titleZ, double minZ, double maxZ, int precision);

  void DrawMeanEff2D(CbmRichGeoTestOptFileEnum fileEnum, const string& histName1, const string& histName2,
                     const string& titleZ, double minZ, double maxZ, double effCoeff, int precision);

  void DrawTextLabelsH2(TH2* h, int precision);

  void DrawReferenceLineH1(double value);

  void DrawReferenceBoxH2(double centerX, double centerY, double widthX, double widthY);

  void SetOutputDir(const string& outDir) { fOutputDir = outDir; }

  void SetReferenceInd(int refInd) { fReferenceInd = refInd; }

  void SetDrawReference(bool drawRef) { fDrawReference = drawRef; }

private:
  vector<string> fGeoTestBoxPathes;
  vector<string> fGeoTestOmega3Pathes;
  vector<string> fGeoTestOmega8Pathes;
  vector<string> fUrqmdTestPathes;
  vector<string> fRecoQaBoxPathes;
  vector<string> fRecoQaUrqmdPathes;

  int fReferenceInd;
  bool fDrawReference;

  CbmHistManager* fHM;

  string fOutputDir;

  ClassDef(CbmRichGeoTestOpt, 1)
};

#endif
