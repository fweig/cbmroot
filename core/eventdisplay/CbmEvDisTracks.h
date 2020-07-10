// -------------------------------------------------------------------------
// -----                      FairMCTracks header file                 -----
// -----                Created 10/12/07  by M. Al-Turany              -----
// -------------------------------------------------------------------------


/** from FairMCTracks
 * @author M. Al-Turany
 * @since 10.12.07
 *   MVD event display object
 **
 **/
#define TOFDisplay                                                             \
  1  // =1 means active, other: without Label and not relying on TEvePointSet

#ifndef CBMEVDISTRACKS_H
#define CBMEVDISTRACKS_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Bool_t, Int_t, Double_t, kFALSE, kTRUE, Opti...
#include <TEveTrackPropagator.h>  // IWYU pragma: keep needed by cling
#include <TString.h>              // for TString

#include <FairEventManager.h>  // IWYU pragma: keep needed by cling
#include <FairTask.h>          // for FairTask, InitStatus

class TClonesArray;
class TEveElementList;
class TEveTrackList;
class TObjArray;

class CbmEvDisTracks : public FairTask {

public:
  /** Default constructor **/
  CbmEvDisTracks();


  /** Standard constructor
    *@param name        Name of task
    *@param iVerbose    Verbosity level
    **/
  CbmEvDisTracks(const char* name,
                 Int_t iVerbose = 1,
                 Bool_t renderP = kFALSE,
                 Bool_t renderT = kTRUE);

  /** Destructor **/
  virtual ~CbmEvDisTracks();

  inline static CbmEvDisTracks* Instance() { return fInstance; }

  /** Set verbosity level. For this task and all of the subtasks. **/
  void SetVerbose(Int_t iVerbose) { fVerbose = iVerbose; }
  void SetRenderP(Bool_t render) { fRenderP = render; }
  void SetRenderT(Bool_t render) { fRenderT = render; }
  /** Executed task **/
  virtual void Exec(Option_t* option);
  virtual InitStatus Init();
  virtual void SetParContainers();

  /** Action after each event**/
  virtual void Finish();
  void Reset();
  TEveTrackList* GetTrGroup(Int_t ihmul, Int_t iOpt);
#if TOFDisplay == 1  //List for TEvePointSets
  TEveElementList* GetPSGroup(Int_t ihuml, Int_t iOpt);
#endif

protected:
  TClonesArray* fTrackList;  //!
  TEveTrackPropagator* fTrPr;
  FairEventManager* fEventManager;  //!
  TObjArray* fEveTrList;
  TString fEvent;          //!
  TEveTrackList* fTrList;  //!
  TObjArray* fEvePSList;
  TEveElementList* fPSList;
  //TEveElementList *fTrackCont;

  Bool_t fRenderP;
  Bool_t fRenderT;
  Double_t MinEnergyLimit;
  Double_t MaxEnergyLimit;
  Double_t PEnergy;

private:
  static CbmEvDisTracks* fInstance;
  CbmEvDisTracks(const CbmEvDisTracks&);
  CbmEvDisTracks& operator=(const CbmEvDisTracks&);

  ClassDef(CbmEvDisTracks, 1);
};


#endif
