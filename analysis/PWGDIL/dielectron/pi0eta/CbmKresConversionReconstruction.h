/** @file CbmKresConversionReconstruction.h
  * @copyright Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

#ifndef CBM_KRES_CONVERSION_RECONSTRUCTION
#define CBM_KRES_CONVERSION_RECONSTRUCTION


class TH1;
class TH1D;
class TH2D;
class TH3D;

#include "CbmMCTrack.h"

#include <TClonesArray.h>

#include <vector>

using namespace std;

class CbmKresConversionReconstruction {

public:
  //***** brief Standard constructor.
  CbmKresConversionReconstruction();
  //***** brief Standard destructor.
  virtual ~CbmKresConversionReconstruction();


  void Init();
  void InitHistograms();
  void Exec(int fEventNumRec);
  void Finish();

  void MakeRecoAnalysis(vector<TVector3> RefMom, vector<CbmMCTrack*> MC, vector<Int_t> Id, vector<TH1*> gg,
                        vector<TH1*> gee, vector<TH1*> eeee);
  Double_t CalculateOpeningAngleBetweenGammas_MC(CbmMCTrack* mctrack1, CbmMCTrack* mctrack2, CbmMCTrack* mctrack3,
                                                 CbmMCTrack* mctrack4);
  Double_t CalculateOpeningAngleBetweenGammas_Reco(TVector3 electron1, TVector3 electron2, TVector3 electron3,
                                                   TVector3 electron4);


private:
  TClonesArray* fMcTracks;
  TClonesArray* fGlobalTracks;
  TClonesArray* fStsTracks;
  TClonesArray* fStsTrackMatches;
  TClonesArray* fRichRings;
  TClonesArray* fRichRingMatches;

  vector<TVector3> STS_refmomentum;
  vector<CbmMCTrack*> STS_MCtrack;
  vector<Int_t> STS_Id;

  vector<TVector3> STS_and_RICH_refmomentum;
  vector<CbmMCTrack*> STS_and_RICH_MCtrack;
  vector<Int_t> STS_and_RICH_Id;

  // definition of histograms
  vector<TH1*> fHistoList_reco;
  TH2D* ConversionPoints2D;
  TH3D* ConversionPoints3D;

  vector<TH1*> fHistoList_reco_STS_gg;
  vector<TH1*> fHistoList_reco_STS_and_RICH_gg;
  vector<TH1*> fHistoList_reco_STS_gee;
  vector<TH1*> fHistoList_reco_STS_and_RICH_gee;
  vector<TH1*> fHistoList_reco_STS_eeee;
  vector<TH1*> fHistoList_reco_STS_and_RICH_eeee;

  ///////   pi -> e+ e- e+ e-
  TH1D* STS_InvMass_eeee_mc;
  TH1D* STS_InvMass_eeee_refitted;
  TH1D* STSRICH_InvMass_eeee_mc;
  TH1D* STSRICH_InvMass_eeee_refitted;

  //////   pi -> gamma(e+ e-) e+ e-
  TH1D* STS_InvMass_gee_mc;
  TH1D* STS_InvMass_gee_refitted;
  TH1D* STS_InvMass_realg_gee_reffited;
  TH1D* STS_InvMass_img_gee_refitted;
  TH1D* STS_OpeningAngle_realg_gee_mc;
  TH1D* STS_OpeningAngle_realg_gee_reffited;
  TH1D* STS_OpeningAngle_img_gee_mc;
  TH1D* STS_OpeningAngle_img_gee_reffited;

  TH1D* STSRICH_InvMass_gee_mc;
  TH1D* STSRICH_InvMass_gee_refitted;
  TH1D* STSRICH_InvMass_realg_gee_reffited;
  TH1D* STSRICH_InvMass_img_gee_refitted;
  TH1D* STSRICH_OpeningAngle_realg_gee_mc;
  TH1D* STSRICH_OpeningAngle_realg_gee_reffited;
  TH1D* STSRICH_OpeningAngle_img_gee_mc;
  TH1D* STSRICH_OpeningAngle_img_gee_reffited;

  TH1D* STS_InvMass_gg_mc;
  TH1D* STS_InvMass_gg_reffited;
  TH1D* STS_InvMass_realg_gg_mc;
  TH1D* STS_InvMass_realg_gg_reffited;
  TH1D* STS_OpeningAngle_realg_gg_mc;
  TH1D* STS_OpeningAngle_realg_gg_reffited;
  TH1D* STS_OpeningAngle_between_gg_mc;
  TH1D* STS_OpeningAngle_between_gg_reffited;

  TH1D* STSRICH_InvMass_gg_mc;
  TH1D* STSRICH_InvMass_gg_reffited;
  TH1D* STSRICH_InvMass_realg_gg_mc;
  TH1D* STSRICH_InvMass_realg_gg_reffited;
  TH1D* STSRICH_OpeningAngle_realg_gg_mc;
  TH1D* STSRICH_OpeningAngle_realg_gg_reffited;
  TH1D* STSRICH_OpeningAngle_between_gg_mc;
  TH1D* STSRICH_OpeningAngle_between_gg_reffited;


  //***** brief Copy constructor.
  CbmKresConversionReconstruction(const CbmKresConversionReconstruction&);

  //***** brief Assignment operator.
  CbmKresConversionReconstruction operator=(const CbmKresConversionReconstruction&);


  ClassDef(CbmKresConversionReconstruction, 1)
};

#endif
