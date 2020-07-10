//TODO runid, eventid, vertex, fitter!, match in STS, constants

#include "DataTreeCbmInterface.h"
#include <fstream>
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::ifstream;
using std::vector;

#include "TDirectory.h"
#include "TROOT.h"

#include "CbmDigiManager.h"
#include "CbmGlobalTrack.h"
#include "CbmMCTrack.h"
#include "CbmPsdDigi.h"
#include "CbmPsdHit.h"
#include "CbmPsdPoint.h"
#include "CbmStsTrack.h"
#include "CbmTofHit.h"
#include "CbmTrackMatchNew.h"
// #include "CbmTrackMatch.h"

#include "FairMCEventHeader.h"

//L1Algo tools
// #include "CbmL1.h"
// #include "L1Algo.h"
// #include "CbmL1Track.h"
// #include "L1TrackPar.h"
// #include "L1Station.h"
// #include "L1Extrapolation.h"
// #include "L1AddMaterial.h"
// #include "L1Filtration.h"
// #include "L1MaterialInfo.h"
#include "CbmKFTrack.h"
#include "CbmL1PFFitter.h"

#include "CbmKFVertex.h"
#include "KFParticleTopoReconstructor.h"
#include "L1Field.h"

#include "DataTreeConstants.h"
#include "DataTreeEvent.h"
#include "DataTreeTrack.h"

#include <TGeoBBox.h>

//=================================================================> MAIN <===============================================================
DataTreeCbmInterface::DataTreeCbmInterface()
  : FairTask("DataTreeCbmInterface", 1) {
  fDTEvent = new DataTreeEvent();
}
DataTreeCbmInterface::~DataTreeCbmInterface() {}


//=================================================================> INIT <===============================================================
//----------------------------------
InitStatus DataTreeCbmInterface::Init() {
  InitInput();
  InitOutput();
  return kSUCCESS;
}


void DataTreeCbmInterface::LoadGeo(const TString& geoFile) {
  const char* fairGeom         = "FAIRGeom";
  const char* moduleNamePrefix = "module";

  TGeoManager* geoMan = TGeoManager::Import(geoFile.Data(), fairGeom);
  TGeoNode* caveNode  = geoMan->GetTopNode();
  TGeoNode* psdNode   = nullptr;
  TString nodeName;

  for (int i = 0; i < caveNode->GetNdaughters(); i++) {
    psdNode  = caveNode->GetDaughter(i);
    nodeName = psdNode->GetName();
    if (nodeName.Contains("psd")) break;
  }
  std::cout << "-I- " << psdNode->GetName() << std::endl;

  auto psdGeoMatrix = psdNode->GetMatrix();
  auto psdBox       = (TGeoBBox*) psdNode->GetVolume()->GetShape();
  TVector3 frontFaceLocal(0, 0, -psdBox->GetDZ());

  TVector3 frontFaceGlobal;
  psdGeoMatrix->LocalToMaster(&frontFaceLocal[0], &frontFaceGlobal[0]);

  fPsdPosition = frontFaceGlobal;

  fPsdModules = 0;
  fPsdModulePositions.clear();
  for (int i_d = 0; i_d < psdNode->GetNdaughters(); ++i_d) {
    auto* daughter = psdNode->GetDaughter(i_d);
    TString daughterName(daughter->GetName());
    if (daughterName.BeginsWith(moduleNamePrefix)) {

      auto geoMatrix = daughter->GetMatrix();
      TVector3 translation(geoMatrix->GetTranslation());

      int modID = daughter->GetNumber();
      double x  = translation.X();
      double y  = translation.Y();

      std::cout << "mod" << modID << " : " << Form("(%.3f, %3f)", x, y)
                << std::endl;
      fPsdModulePositions.insert({modID, translation});
      fPsdModules++;
    }
  }
  std::cout << "-I- Number of PSD modules: " << fPsdModules << std::endl;
  // Fix to avoid crash
  if (gROOT->GetVersionInt() >= 60602) {
    geoMan->GetListOfVolumes()->Delete();
    geoMan->GetListOfShapes()->Delete();
  }
  delete geoMan;
}


//----------------------------------
void DataTreeCbmInterface::InitInput() {
  FairRootManager* ioman = FairRootManager::Instance();
  fPrimVtx               = (CbmVertex*) ioman->GetObject("PrimaryVertex.");
  if (!fPrimVtx) fPrimVtx = (CbmVertex*) ioman->GetObject("PrimaryVertex.");
  fHeader            = (FairMCEventHeader*) ioman->GetObject("MCEventHeader.");
  flistPSDhit        = (TClonesArray*) ioman->GetObject("PsdHit");
  flistMCtrack       = (TClonesArray*) ioman->GetObject("MCTrack");
  flistSTSRECOtrack  = (TClonesArray*) ioman->GetObject("StsTrack");
  flistSTStrackMATCH = (TClonesArray*) ioman->GetObject("StsTrackMatch");
  fGlobalTrackArray  = (TClonesArray*) ioman->GetObject("GlobalTrack");
  fTofHitArray       = (TClonesArray*) ioman->GetObject("TofHit");
  fTofHitMatchArray  = (TClonesArray*) ioman->GetObject("TofHitMatch");
  fPsdPointArray     = (TClonesArray*) ioman->GetObject("PsdPoint");
  fDigiMan           = CbmDigiManager::Instance();
  fDigiMan->Init();
}

//----------------------------------
void DataTreeCbmInterface::InitOutput() { InitOutputTree(); }

//--------------------------------------------------------------------------------------------------
void DataTreeCbmInterface::InitDataTreeEvent() {
  for (int i = 0; i < fPsdModules; ++i)
    fDTEvent->AddPSDModule(10);
}

//--------------------------------------------------------------------------------------------------
void DataTreeCbmInterface::InitOutputTree() {
  fTreeFile = new TFile(fOutputFileName, "RECREATE");
  fTreeFile->cd();
  fDataTree = new TTree("DataTree", "DataTree");
  fDataTree->Branch("DTEvent", &fDTEvent);
}

//=================================================================> EXEC <===============================================================
void DataTreeCbmInterface::Exec(Option_t*) {
  ClearEvent();

  InitDataTreeEvent();
  ReadEvent();
  ReadPSD();
  ReadMC();
  ReadTracks();
  //    LinkSTS();
  ReadV0(0);
  ReadV0(1);
  ReadTOF();
  if (fPsdModules == 0) ReadPsdPrimaryParticles();

  fDataTree->Fill();
}
//--------------------------------------------------------------------------------------------------
void DataTreeCbmInterface::ClearEvent() {
  fMCTrackIDs.clear();
  fTrackIDs.clear();
  fDTEvent->ClearEvent();
}

//--------------------------------------------------------------------------------------------------
void DataTreeCbmInterface::ReadEvent() {
  if (!fHeader) {
    cout << "No fHeader!" << endl;
    return;
  }

  fDTEvent->SetRPAngle(fHeader->GetRotZ());
  fDTEvent->SetImpactParameter(fHeader->GetB());
  fDTEvent->SetRunId(fHeader->GetRunID());
  fDTEvent->SetEventId(fHeader->GetEventID());
  fDTEvent->SetMCVertexPosition(
    fHeader->GetX(), fHeader->GetY(), fHeader->GetZ());
  if (!fPrimVtx) {
    if (fPsdModules != 0) cout << "No fPrimVtx!" << endl;
    return;
  } else {
    fDTEvent->SetVertexPosition(
      fPrimVtx->GetX(), fPrimVtx->GetY(), fPrimVtx->GetZ());
    fDTEvent->SetVertexQuality(fPrimVtx->GetChi2() / fPrimVtx->GetNDF(), 0);
  }

  //  KFParticle KFParticle_vtx_TOF = fFinderTOF->GetTopoReconstructor()->GetPrimVertex(0);
  //  fDTEvent -> SetVertexPosition (KFParticle_vtx_TOF.X(), KFParticle_vtx_TOF.Y(), KFParticle_vtx_TOF.Z(), 0);
  //  fDTEvent -> SetVertexQuality (KFParticle_vtx_TOF.Chi2() / KFParticle_vtx_TOF.NDF(), 0);
  //  KFParticle KFParticle_vtx_MC = fFinderMC->GetTopoReconstructor()->GetPrimVertex(0);
  //  fDTEvent -> SetVertexPosition (KFParticle_vtx_MC.X(), KFParticle_vtx_MC.Y(), KFParticle_vtx_MC.Z(), 1);
  //  fDTEvent -> SetVertexQuality (KFParticle_vtx_MC.Chi2() / KFParticle_vtx_MC.NDF(), 1);
}

//--------------------------------------------------------------------------------------------------
void DataTreeCbmInterface::ReadPSD() {
  if (!flistPSDhit) return;
  const int nPSDhits = flistPSDhit->GetEntriesFast();

  for (int i = 0; i < fPsdModules; ++i) {
    fDTEvent->GetPSDModule(i)->SetPosition(fPsdModulePositions[i].X(),
                                           fPsdModulePositions[i].Y(),
                                           fPsdModulePositions[i].Z());
  }

  CbmPsdHit* hit {nullptr};
  Float_t PsdEnergy {0.};

  for (int i = 0; i < nPSDhits; ++i) {
    hit = (CbmPsdHit*) flistPSDhit->At(i);
    if (hit == nullptr) continue;
    fDTEvent->GetPSDModule(hit->GetModuleID() - 1)->SetEnergy(hit->GetEdep());
    PsdEnergy += hit->GetEdep();
  }

  fDTEvent->SetPsdEnergy(PsdEnergy);
  fDTEvent->SetPsdPosition(
    fPsdPosition.X(), fPsdPosition.Y(), fPsdPosition.Z());

  const int nPSDdigits = fDigiMan->GetNofDigis(ECbmModuleId::kPsd);
  const CbmPsdDigi* digit {nullptr};
  for (int i = 0; i < nPSDdigits; ++i) {
    digit = fDigiMan->Get<CbmPsdDigi>(i);
    if (digit == nullptr) continue;
    fDTEvent->GetPSDModule(digit->GetModuleID() - 1)
      ->GetSection(digit->GetSectionID() - 1)
      ->AddEnergy(digit->GetEdep());
  }
}
//--------------------------------------------------------------------------------------------------
void DataTreeCbmInterface::ReadPsdPrimaryParticles() {
  Int_t nPoints = fPsdPointArray->GetEntriesFast();
  for (Int_t iPoint = 0; iPoint < nPoints; iPoint++) {
    CbmPsdPoint* point = (CbmPsdPoint*) fPsdPointArray->At(iPoint);
    if (!point) {
      cout << "No PSD point number " << iPoint << endl;
      continue;
    }
    CbmMCTrack* mctrack = (CbmMCTrack*) flistMCtrack->At(point->GetTrackID());
    if (!mctrack)
      cout << "No MC track number " << point->GetTrackID()
           << "matched to PSD point number " << iPoint << endl;
    float mass;
    const long int type = mctrack->GetPdgCode();
    if (type < 10000)
      mass = mctrack->GetMass();
    else
      mass = (type / 10 % 1000) * 0.931;
    TLorentzVector momentum;
    momentum.SetXYZM(point->GetPx(), point->GetPy(), point->GetPz(), mass);
    TVector3 position(point->GetX(), point->GetY(), point->GetZ());

    DataTreePSDPrimaryParticle* psdParticle = fDTEvent->AddPSDPrimaryParticle();
    psdParticle->SetMomentum(momentum);
    psdParticle->SetPosition(position);
    psdParticle->SetPdgID(mctrack->GetPdgCode());
    psdParticle->SetType(mctrack->GetMotherId());
  }
}

//--------------------------------------------------------------------------------------------------
int DataTreeCbmInterface::GetMCTrackMatch(const int idx) {
  for (int i = 0; i < fDTEvent->GetNTracks(); ++i)
    if (fDTEvent->GetTrack(i)->GetMCTrackId() == idx) return i;
  return EnumGlobalConst::kUndefinedValue;
}

//--------------------------------------------------------------------------------------------------
void DataTreeCbmInterface::ReadMC() {
  if (!flistMCtrack) return;
  CbmMCTrack* mctrack {nullptr};
  Float_t mass {0.};
  Int_t charge {0};
  TLorentzVector mom;

  const int nTracks = flistMCtrack->GetEntries();

  for (int i = 0; i < nTracks; ++i) {
    mctrack            = (CbmMCTrack*) flistMCtrack->At(i);
    const int motherid = mctrack->GetMotherId();
    //    if (motherid != -1) continue;

    const long int type = mctrack->GetPdgCode();
    if (type < 1000000000) {
      charge = mctrack->GetCharge() / 3;
      mass   = mctrack->GetMass();
    } else {
      //pdg = 1000000000 + 10*1000*z + 10*a + i;
      charge = type / 10000 % 1000;
      mass   = (type / 10 % 1000) * 0.931;
    }

    mom.SetXYZM(mctrack->GetPx(), mctrack->GetPy(), mctrack->GetPz(), mass);

    fMCTrackIDs.push_back(i);
    fDTEvent->AddMCTrack();
    DataTreeMCTrack* DTMCTrack = fDTEvent->GetLastMCTrack();

    DTMCTrack->SetMomentum(mom);
    DTMCTrack->SetCharge(charge);

    DTMCTrack->SetPdgId(type);
    DTMCTrack->SetMotherId(motherid);

    //  std::cout << GetMCTrackMatch(i) << std::endl;
    //  DTMCTrack->SetRecoTrackId( {GetMCTrackMatch(i)} );
  }
}

//--------------------------------------------------------------------------------------------------
void DataTreeCbmInterface::ReadTracks() {
  if (!flistSTSRECOtrack) return;
  std::cout << "ReadTracks" << std::endl;

  const Float_t mass = 0.14;  // pion mass assumption to write TLorentzVector

  const Int_t nSTStracks = flistSTSRECOtrack->GetEntries();

  int nMCtracks = fDTEvent->GetNMCTracks();

  for (Int_t i = 0; i < nSTStracks; ++i) {
    CbmStsTrack* track {nullptr};
    CbmTrackMatchNew* match {nullptr};
    Int_t mcTrackID {-999};
    //    CbmMCTrack* mctrack{nullptr}; (FU) unused
    DataTreeTrack* DTTrack {nullptr};
    DataTreeTrack* DTVertexTrack {nullptr};
    const FairTrackParam* trackParam {nullptr};
    DataTreeTrackParams Params;
    TVector3 momRec;
    TLorentzVector mom;
    //         std::cout << "i = " << i << std::endl;
    track = (CbmStsTrack*) flistSTSRECOtrack->At(i);

    if (track == nullptr) {
      cout << "ERROR: empty track!";
      continue;
    }

    fDTEvent->AddTrack();

    trackParam = track->GetParamFirst();
    trackParam->Momentum(momRec);
    const Int_t q = trackParam->GetQp() > 0 ? 1 : -1;

    mom.SetXYZM(momRec.X(), momRec.Y(), momRec.Z(), mass);

    DTTrack = fDTEvent->GetLastTrack();

    DTTrack->SetMomentum(mom);
    DTTrack->SetId(i);
    DTTrack->SetNumberOfHits(track->GetNofHits(), 0);
    DTTrack->SetFlag(track->GetFlag());
    DTTrack->SetChi2(track->GetChiSq());
    DTTrack->SetNDF(track->GetNDF());
    DTTrack->SetCharge(q);

    Params.SetMagFieldFit(0., 0., 0., 0., 0., 0., 0., 0., 0., 0.);

    std::vector<double> trackParametersValuesT = {trackParam->GetX(),
                                                  trackParam->GetY(),
                                                  trackParam->GetZ(),
                                                  trackParam->GetTx(),
                                                  trackParam->GetTy(),
                                                  trackParam->GetQp()};

    std::vector<double> covMatrixValuesT(25, 0.);

    //         if (i==0)
    //            std::cout << "V: before " << momRec.X() << std::endl;

    Params.SetParameters(trackParametersValuesT);
    Params.SetCovMatrix(covMatrixValuesT);
    Params.SetPosition(
      trackParam->GetX(), trackParam->GetY(), trackParam->GetZ());
    DTTrack->SetParams(Params, EnumParamsPoint::kVertex);

    fTrackIDs.push_back(i);


    // // // // // // // // // // // // // // // // // // // // // // //
    //                Vertex tracks
    // // // // // // // // // // // // // // // // // // // // // // //

    vector<CbmStsTrack> vRTracks(1);
    vRTracks[0] = *track;
    CbmL1PFFitter fitter;
    vector<float> vChiToPrimVtx;

    if (!fPrimVtx) continue;

    //        CbmKFTrack kftrack(vRTracks[0]);
    //       kftrack.Extrapolate(fPrimVtx->GetZ());

    vector<L1FieldRegion> vField;
    CbmKFVertex kfVertex = CbmKFVertex(*fPrimVtx);
    std::vector<int> pdg = {211};
    fitter.Fit(vRTracks, pdg);
    fitter.GetChiToVertex(vRTracks, vField, vChiToPrimVtx, kfVertex, 100000.);

    //         if (i==0)
    {
      //             std::cout << vField.at(0).cx0 << " " << vField.at(0).cx1 << " " << vField.at(0).cx2 << std::endl;
      //             std::cout << vField.at(0).cy0 << " " << vField.at(0).cy1 << " " << vField.at(0).cy2 << std::endl;
      //             std::cout << vField.at(0).cz0 << " " << vField.at(0).cz1 << " " << vField.at(0).cz2 << std::endl;
      //             std::cout << vField.at(0).z0 << std::endl;
    }

    fDTEvent->AddVertexTrack();
    //
    //BEGIN vertex point

    track = &(vRTracks[0]);

    trackParam = track->GetParamFirst();
    trackParam->Momentum(momRec);

    //const float xyz[] = {0.,0.,0.};
    //CbmKFParticle tmpPart(kftrack);
    //tmpPart.TransportToPoint(xyz);


    //         if (i==0)
    //        std::cout << "V: after 1  " << momRec.X() << std::endl;
    //std::cout << "V: after 2  " << kftrack.GetTrack()[2] << std::endl;

    mom.SetXYZM(momRec.X(), momRec.Y(), momRec.Z(), mass);

    DTVertexTrack = fDTEvent->GetLastVertexTrack();
    DTVertexTrack->SetId(i);
    DTVertexTrack->SetMomentum(mom);
    DTVertexTrack->SetFlag(track->GetFlag());
    DTVertexTrack->SetChi2(track->GetChiSq());
    DTVertexTrack->SetNDF(track->GetNDF());
    DTVertexTrack->SetNumberOfHits(track->GetNofHits(), 0);
    DTVertexTrack->SetCharge(q);
    DTVertexTrack->SetDCA(trackParam->GetX() - fPrimVtx->GetX(),
                          trackParam->GetY() - fPrimVtx->GetY(),
                          trackParam->GetZ() - fPrimVtx->GetZ());

    Params.SetMagFieldFit(float(vField.at(0).cx0[0]),
                          float(vField.at(0).cx1[0]),
                          float(vField.at(0).cx2[0]),
                          float(vField.at(0).cy0[0]),
                          float(vField.at(0).cy1[0]),
                          float(vField.at(0).cy2[0]),
                          float(vField.at(0).cz0[0]),
                          float(vField.at(0).cz1[0]),
                          float(vField.at(0).cz2[0]),
                          float(vField.at(0).z0[0]));

    std::vector<double> trackParametersValues = {trackParam->GetX(),
                                                 trackParam->GetY(),
                                                 trackParam->GetZ(),
                                                 trackParam->GetTx(),
                                                 trackParam->GetTy(),
                                                 trackParam->GetQp()};

    DTVertexTrack->SetVtxChi2(vChiToPrimVtx[0]);

    std::vector<double> covMatrixValues(25, 0.);

    Params.SetParameters(trackParametersValues);
    Params.SetCovMatrix(covMatrixValues);
    Params.SetPosition(
      trackParam->GetX(), trackParam->GetY(), trackParam->GetZ());

    DTVertexTrack->SetParams(Params, EnumParamsPoint::kVertex);
    match = (CbmTrackMatchNew*) flistSTStrackMATCH->At(i);
    if (match->GetNofLinks() > 0) {
      mcTrackID = match->GetMatchedLink().GetIndex();
      if (mcTrackID >= 0 && mcTrackID < nMCtracks) {
        fDTEvent->AddTrackMatch(DTVertexTrack, fDTEvent->GetMCTrack(mcTrackID));
        DTVertexTrack->SetMCTrackId(mcTrackID);
      }
      //            else
      //            {
      //                cout << "Reco id = " << i;
      //                cout << "\tMC id = " << mcTrackID;
      //                cout << "\tnMCtracks = " << nMCtracks << endl;
      //            }
    }
  }
}

//--------------------------------------------------------------------------------------------------
void DataTreeCbmInterface::LinkSTS() {
  bool found = false;
  //     std::cout << "==================== MC" << std::endl;

  for (int i = 0; i < fDTEvent->GetNTracks(); ++i) {
    found = false;
    int j {0};
    for (; j < fDTEvent->GetNMCTracks() && !found; j++) {
      // 	    std::cout<<j<<" "<<fMCTrackIDs.at(j)<<" " <<fDTEvent->GetTrack(i)->GetMCTrackId()<<" "<<fDTEvent->GetMCTrack(j)->GetId() <<std::endl;
      if (fMCTrackIDs.at(j) == fDTEvent->GetTrack(i)->GetMCTrackId()) {
        // 		std::cout<<"track id: "<<i<<"; before: " << fDTEvent->GetTrack(i)->GetMCTrackId()<<"; after: "<<j << std::endl;
        found = true;
        fDTEvent->GetTrack(i)->SetMCTrackId(j);
        break;
      }
    }
    if (found) {
      fDTEvent->AddTrackMatch(fDTEvent->GetTrack(i), fDTEvent->GetMCTrack(j));
    } else {
      fDTEvent->GetTrack(i)->SetMCTrackId(EnumGlobalConst::kUndefinedValue);
    }
  }
}

//--------------------------------------------------------------------------------------------------
void DataTreeCbmInterface::ReadTOF() {
  if (!fGlobalTrackArray) return;
  std::cout << "ReadTOF" << std::endl;

  for (Int_t igt = 0; igt < fGlobalTrackArray->GetEntries(); igt++) {
    const CbmGlobalTrack* globalTrack =
      static_cast<const CbmGlobalTrack*>(fGlobalTrackArray->At(igt));

    const Int_t stsTrackIndex = globalTrack->GetStsTrackIndex();
    if (stsTrackIndex < 0) continue;

    CbmStsTrack* cbmStsTrack =
      (CbmStsTrack*) flistSTSRECOtrack->At(stsTrackIndex);
    const Int_t tofHitIndex = globalTrack->GetTofHitIndex();

    if (tofHitIndex < 0) continue;

    const CbmTofHit* tofHit =
      static_cast<const CbmTofHit*>(fTofHitArray->At(tofHitIndex));
    if (!tofHit) continue;

    const FairTrackParam* globalPar = globalTrack->GetParamLast();
    TVector3 mom;
    cbmStsTrack->GetParamFirst()->Momentum(mom);

    const Float_t p = mom.Mag();
    const Int_t q   = globalPar->GetQp() > 0 ? 1 : -1;
    const Float_t l =
      globalTrack->GetLength();  // l is calculated by global tracking

    const Float_t time = tofHit->GetTime();
    const Float_t m2 =
      p * p
      * (1. / ((l / time / SpeedOfLight) * (l / time / SpeedOfLight)) - 1.);
    const Float_t hitX = tofHit->GetX();
    const Float_t hitY = tofHit->GetY();
    const Float_t hitZ = tofHit->GetZ();

    const Float_t Tx     = globalPar->GetTx();
    const Float_t Ty     = globalPar->GetTy();
    const Float_t trackZ = globalPar->GetZ();
    const Float_t dz     = hitZ - trackZ;

    const Float_t trackX =
      globalPar->GetX() + Tx * dz;  //extrapolation to TOF hit
    const Float_t trackY = globalPar->GetY() + Ty * dz;

    DataTreeTOFHit* DTTOFHit = fDTEvent->AddTOFHit();
    DTTOFHit->SetId(tofHitIndex);
    DTTOFHit->SetPosition(hitX, hitY, hitZ);
    DTTOFHit->SetTime(time);
    DTTOFHit->SetPathLength(l);
    DTTOFHit->SetCharge(q);
    DTTOFHit->SetSquaredMass(m2);


    Int_t iTrk = 0;
    for (; iTrk < fDTEvent->GetNTracks(); iTrk++) {
      if (stsTrackIndex >= 0
          && fDTEvent->GetTrack(iTrk)->GetId() == UInt_t(stsTrackIndex))
        break;
    }
    if (iTrk == fDTEvent->GetNTracks()) continue;

    DTTOFHit->AddRecoTrackId(iTrk);

    DataTreeTrackParams par;
    par.SetPosition(trackX, trackY, hitZ);  //extrapolated to hit

    //         std::cout << "X : " << hitX << " " <<  globalPar->GetX() << " " << trackX << std::endl;
    //         std::cout << "Z : " << hitZ << " " <<  trackZ << " " << Tx * dz << std::endl;

    DataTreeTrack* track = fDTEvent->GetTrack(iTrk);
    track->SetLength(l);
    track->SetTOFHitId(fDTEvent->GetNTOFHits() - 1);
    track->SetParams(par, EnumParamsPoint::kTof);

    DataTreeTrack* vtrack = fDTEvent->GetVertexTrack(iTrk);
    vtrack->SetLength(l);
    vtrack->SetTOFHitId(fDTEvent->GetNTOFHits() - 1);
    vtrack->SetParams(par, EnumParamsPoint::kTof);
  }
}

//--------------------------------------------------------------------------------------------------
void DataTreeCbmInterface::ReadV0(const int UseMCpid) {
  const KFParticleTopoReconstructor* topo_rec;
  if (UseMCpid) topo_rec = fFinderMC->GetTopoReconstructor();
  if (!UseMCpid) topo_rec = fFinderTOF->GetTopoReconstructor();

  if (!topo_rec) {
    cout
      << "DataTreeCbmInterface::ReadV0: ERROR: no KFParticleTopoReconstructor!"
      << endl;
    return;
  }
  TLorentzVector mom;

  //    cout << "DataTreeCbmInterface::ReadV0    1" << endl;

  const int ConstNV0Types = fDTEvent->GetNV0Types();

  int V0Mult[ConstNV0Types];
  for (int i = 0; i < ConstNV0Types; ++i) {
    V0Mult[i] = 0;
  }

  for (unsigned int iP = 0; iP < topo_rec->GetParticles().size(); iP++) {
    bool accept_V0 = false;
    for (int i = 0; i < ConstNV0Types; ++i) {
      if (topo_rec->GetParticles()[iP].GetPDG() == fDTEvent->GetNV0Pdg(i)) {
        accept_V0 = true;
        V0Mult[i]++;
      }
    }
    if (!accept_V0) continue;

    const KFParticle& V0 = topo_rec->GetParticles()[iP];
    DataTreeV0Candidate* V0Candidate;
    if (!UseMCpid) { V0Candidate = fDTEvent->AddV0CandidateTOFpid(); }
    if (UseMCpid) { V0Candidate = fDTEvent->AddV0CandidateMCpid(); }
    if (!V0Candidate)
      cout << "DataTreeCbmInterface::ReadV0_TOF: ERROR: no V0Candidate!"
           << endl;

    mom.SetXYZM(V0.GetPx(), V0.GetPy(), V0.GetPz(), V0.GetMass());

    V0Candidate->SetMomentum(mom);
    V0Candidate->SetPdgId(V0.GetPDG());
    V0Candidate->SetChi2(V0.GetChi2());
    V0Candidate->SetCharge((int) V0.GetQ());

    if (V0.NDaughters() != 2) {
      printf("Number of daughters not %d (%d)!\n", 2, V0.NDaughters());
      continue;
    }

    for (int iDaughter = 0; iDaughter < V0.NDaughters(); iDaughter++) {
      const int daugherIndex     = V0.DaughterIds()[iDaughter];
      const KFParticle& daughter = topo_rec->GetParticles()[daugherIndex];

      V0Candidate->AddDaughter();
      DataTreeV0Candidate* Daughter = V0Candidate->GetDaughter(iDaughter);

      mom.SetXYZM(daughter.GetPx(),
                  daughter.GetPy(),
                  daughter.GetPz(),
                  daughter.GetMass());

      Daughter->SetMomentum(mom);
      Daughter->SetPdgId(daughter.GetPDG());
      Daughter->SetChi2(daughter.GetChi2());
      if (daughter.NDaughters() == 1) {
        Daughter->SetTrackId(daughter.DaughterIds()[0]);
      }
    }
  }
  int V0Mult_All = 0;
  for (int i = 0; i < ConstNV0Types; ++i) {
    if (!UseMCpid) { fDTEvent->SetNV0SpecificCandidatesTOFpid(i, V0Mult[i]); }
    if (UseMCpid) { fDTEvent->SetNV0SpecificCandidatesMCpid(i, V0Mult[i]); }
    V0Mult_All += V0Mult[i];
  }

  if (!UseMCpid) { fDTEvent->SetNV0CandidatesTOFpid(V0Mult_All); }
  if (UseMCpid) { fDTEvent->SetNV0CandidatesMCpid(V0Mult_All); }
}

//================================================================> FINISH <==============================================================
void DataTreeCbmInterface::Finish() {
  cout << "DataTreeCbmInterface::Finish" << endl;
  fDataTree->Write();
  fTreeFile->Write();
  fTreeFile->Close();
}

ClassImp(DataTreeCbmInterface)
