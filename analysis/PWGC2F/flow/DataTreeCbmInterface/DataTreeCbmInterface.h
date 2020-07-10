#ifndef DataTreeCbmInterface_H
#define DataTreeCbmInterface_H 1

#include "CbmKFPartEfficiencies.h"
#include "CbmKFParticleFinder.h"

#include "CbmVertex.h"
#include "FairTask.h"
#include "TLorentzVector.h"
#include <cstring>
#include <map>
#include <vector>

#include "FairMCEventHeader.h"
#include "TCanvas.h"
#include "TClonesArray.h"
#include "TFile.h"
#include "TGraphErrors.h"
#include "TH2F.h"
#include "TProfile.h"
#include <TGeoManager.h>
#include <iostream>

#include "CbmKFVertex.h"
#include "CbmTrackMatch.h"

class DataTreeEvent;
class TClonesArray;
class CbmVertex;
class TDirectory;
class TH1F;
class TProfile;
class TH2F;
class CbmDigiManager;


class DataTreeCbmInterface : public FairTask {

public:
  DataTreeCbmInterface();
  ~DataTreeCbmInterface();

  virtual InitStatus Init();
  virtual void Exec(Option_t* opt);
  virtual void Finish();

  void SetOutputFile(const TString filename) { fOutputFileName = filename; }

  void LoadGeo(const TString& geoFile);

  void SetKFParticleFinderTOF(const CbmKFParticleFinder* finder) {
    fFinderTOF = finder;
  }
  void SetKFParticleFinderMC(const CbmKFParticleFinder* finder) {
    fFinderMC = finder;
  }

private:
  void InitInput();
  void InitOutput();
  void InitOutputTree();
  void InitDataTreeEvent();
  void ClearEvent();
  void ReadEvent();
  void ReadPSD();
  void ReadTracks();
  void LinkSTS();
  void ReadTOF();
  void ReadMC();
  int GetMCTrackMatch(const int idx);
  void ReadV0(const int UseMCpid = 0);
  void ReadPsdPrimaryParticles();

  TString fOutputFileName {""};
  TFile* fTreeFile {nullptr};
  TTree* fDataTree {nullptr};

  CbmVertex* fPrimVtx {nullptr};
  FairMCEventHeader* fHeader {nullptr};
  CbmDigiManager* fDigiMan {nullptr};
  TClonesArray* flistPSDhit {nullptr};
  TClonesArray* flistMCtrack {nullptr};
  TClonesArray* flistSTSRECOtrack {nullptr};
  TClonesArray* flistSTStrackMATCH {nullptr};
  TClonesArray* fGlobalTrackArray {nullptr};
  TClonesArray* fTofHitArray {nullptr};
  TClonesArray* fTofHitMatchArray {nullptr};
  TClonesArray* fPsdPointArray {nullptr};

  DataTreeEvent* fDTEvent {nullptr};

  int fPsdModules {0};
  TVector3 fPsdPosition;
  std::map<int, TVector3> fPsdModulePositions;


  std::vector<int> fMCTrackIDs;
  std::vector<int> fTrackIDs;

  const CbmKFParticleFinder* fFinderTOF {nullptr};
  const CbmKFParticleFinder* fFinderMC {nullptr};

  ClassDef(DataTreeCbmInterface, 1)
};

#endif
