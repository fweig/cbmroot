// -------------------------------------------------------------------------
// -----                         CbmTof header file                    -----
// -----                  Created 28/07/04  by V. Friese               -----
// -------------------------------------------------------------------------

/**  CbmTof.h
 *@author V.Friese <v.friese@gsi.de>
 **
 ** Defines the active detector TOF. Constructs the geometry and
 ** registeres MCPoints.
 **/



#ifndef CBMTOF_H
#define CBMTOF_H


#include "FairDetector.h"

#include "TLorentzVector.h"

#include <map>
#include <tuple>
#include <set>


class CbmTofPoint;
class CbmTofGeoHandler;
class FairVolume; 
class TClonesArray;
class TVector3;
class TGeoCombiTrans;
class TGeoPhysicalNode;

class CbmTof : public FairDetector
{

 public:

  /** Default constructor **/
  CbmTof();


  /** Standard constructor.
   *@param name    detetcor name
   *@param active  sensitivity flag
   **/
  CbmTof(const char* name, Bool_t active);


  /** Destructor **/
  virtual ~CbmTof();

  virtual void PreTrack();
  virtual void FinishEvent();

  /** Virtual method ProcessHits
   **
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates CbmTofPoints and adds them to the
   ** collection.
   *@param vol  Pointer to the active volume
   **/
  virtual Bool_t  ProcessHits(FairVolume* vol = 0);


  /** Virtual method EndOfEvent
   **
   ** If verbosity level is set, print hit collection at the
   ** end of the event and resets it afterwards.
   **/
  virtual void EndOfEvent();


  /** Virtual method Register
   **
   ** Registers the hit collection in the ROOT manager.
   **/
  virtual void Register();


  /** Accessor to the hit collection **/
  virtual TClonesArray* GetCollection(Int_t iColl) const;


  /** Virtual method Print
   **
   ** Screen output of hit collection.
   **/
  virtual void Print(Option_t* ="") const;


  /** Virtual method Reset
   **
   ** Clears the hit collection
   **/
  virtual void Reset();


  /** Virtual method CopyClones
   **
   ** Copies the hit collection with a given track index offset
   *@param cl1     Origin
   *@param cl2     Target
   *@param offset  Index offset
   **/
  virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2, 
			  Int_t offset);


  /** Virtual method Construct geometry
   **
   ** Constructs the TOF geometry
   **/
  virtual void ConstructGeometry();

  /** Do all initilization for the TOF detector **/
  virtual void Initialize();

  void GenerateOnePointPerTrack( Bool_t bOnePointPerTrack = kTRUE ) { fbOnePointPerTrack = bOnePointPerTrack; }
  void SetProcessAnyTrack(Bool_t bProcess = kTRUE ) { fbProcessAnyTrack = bProcess; }
  void SetAllCountersInactive(Bool_t bInactive = kTRUE ) { fbAllCountersInactive = bInactive; }

  void SetCounterActive(Int_t iModuleType, Int_t iModuleIndex, Int_t iCounterIndex);

    /** Set a counter inactive
     ** @param iModuleType    type of inactive module
     ** @param iModuleIndex   index of inactive module
     ** @param iCounterIndex  index of inactive counter
     **
     ** If a counter is set inactive, no CbmTofPoint objects will be created for
     ** collision products traversing its Cell volumes.
     **/
  void SetCounterInactive(Int_t iModuleType, Int_t iModuleIndex, Int_t iCounterIndex);

    /** Set a counter in beam
     ** @param iModuleType    type of inactive module
     ** @param iModuleIndex   index of inactive module
     ** @param iCounterIndex  index of inactive counter
     **
     ** If a counter is set in beam, the beam line will be extrapolated to the
     ** counter coordinate system and create a CbmTofPoint object if it actually
     ** traverses the counter volume.
     **/
  void SetCounterInBeam(Int_t iModuleType, Int_t iModuleIndex, Int_t iCounterIndex);

 private:

  /** Track information to be stored until the track leaves the
      active volume. **/
  Int_t          fTrackID;           //!  track index
  Int_t          fVolumeID;          //!  volume id
  TLorentzVector fPos;               //!  position
  TLorentzVector fMom;               //!  momentum
  Double32_t     fTime;              //!  time
  Double32_t     fLength;            //!  length
  Double32_t     fELoss;             //!  energy loss

  Int_t fPosIndex;                   //!
  TClonesArray* fTofCollection;      //! Hit collection
  CbmTofGeoHandler *fGeoHandler;      //! Interface to gMC and gGeoManager

  TGeoCombiTrans*   fCombiTrans;  //! Transformation matrix for geometry positioning

  std::string fVolumeName;    //! Name of Volume to be imported

  Bool_t fbOnePointPerTrack;
  Bool_t fbIsNewTrack;

  TString        fTofNodePath;         //! Path to physical ToF node
  TString        fCurrentNodePath;     //! Path to current physical node
  Int_t          fCurrentModuleType;   //! Current module type
  Int_t          fCurrentModuleIndex;  //! Current module index
  Int_t          fCurrentCounterIndex; //! Current counter index

  std::set<std::tuple<Int_t, Int_t, Int_t>> fActiveCounters;   //! Set of   active counters
  std::set<std::tuple<Int_t, Int_t, Int_t>> fInactiveCounters; //! Set of inactive counters
  std::set<Int_t> fInactiveCounterIDs;                         //! Set of inactive counter IDs

  std::map<std::tuple<Int_t, Int_t, Int_t>, std::pair<TString, TGeoPhysicalNode*>> fCountersInBeam; //! Map of counters in beam

  Int_t fOutputTreeEntry;

  Bool_t fbProcessAnyTrack;

  Bool_t fbAllCountersInactive;

  /** Private method AddHit
   **
   ** Adds a CbmTofPoint to the HitCollection
   **/
  CbmTofPoint* AddHit(Int_t trackID, Int_t detID, TVector3 pos,
		      TVector3 mom, Double_t time, Double_t length, 
		      Double_t eLoss); 

  /** Private method ResetParameters
   **
   ** Resets the private members for the track parameters
   **/
  void ResetParameters();

  Bool_t CheckIfSensitive(std::string name);

  virtual void        ConstructRootGeometry(TGeoMatrix* shift = NULL);

  void CreateInBeamNodes();

  CbmTof(const CbmTof&);
  CbmTof& operator=(const CbmTof&);


  ClassDef(CbmTof,6)

};


inline void CbmTof::ResetParameters() {
  fTrackID = fVolumeID = 0;
  fPos.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMom.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fTime = fLength = fELoss = 0;
  fPosIndex = 0;
};


#endif

