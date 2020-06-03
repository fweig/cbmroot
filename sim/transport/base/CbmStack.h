// -------------------------------------------------------------------------
// -----                       CbmStack header file                    -----
// -----           Created 10/08/04  by D. Bertini / V. Friese         -----
// -------------------------------------------------------------------------


/** CbmStack.h
 *@author D.Bertini <d.bertini@gsi.de>
 *@author V.Friese <v.friese@gsi.de>
 **
 ** Version 14/06/07 by V. Friese
 **
 ** This class handles the particle stack for the transport simulation.
 ** For the stack FILO functunality, it uses the STL stack. To store
 ** the tracks during transport, a TParticle arry is used. 
 ** At the end of the event, tracks satisfying the filter criteria
 ** are copied to a CbmMCTrack array, which is stored in the output.
 **
 ** The filtering criteria for the output tracks are:
 ** - primary tracks are stored in any case.
 ** - secondary tracks are stored if they have a minimal number of
 **   points (sum of all detectors) and a minimal energy, or are the
 ** 
 ** The storage of secondaries can be switched off.
 ** The storage of all mothers can be switched off. 
 ** By default, the minimal number of points is 1 and the energy cut is 0.
 **/


#ifndef CBMSTACK_H
#define CBMSTACK_H


#include "CbmDefs.h"
#include "FairGenericStack.h"
#include "CbmStackFilter.h"

#include <map>
#include <memory>
#include <stack>

class TClonesArray;


class CbmStack : public FairGenericStack
{

 public:

  /** Default constructor
   *param size  Estimated track number
   **/
  CbmStack(Int_t size = 100);


  /** Destructor  **/
  virtual ~CbmStack();


  /** @brief Add a track to the stack (TVirtualMCStack)
   ** @param toBeDone  Flag for putting the track on the internal stack
   ** @param parentId  Index of parent track
   ** @param pdgCode   PDG particle code
   ** @param px        Momentum x component [GeV]
   ** @param py        Momentum y component [GeV]
   ** @param pz        Momentum z component [GeV]
   ** @param e         Energy [GeV]
   ** @param vx        Start vertex x coordinate [cm]
   ** @param vy        Start vertex y coordinate [cm]
   ** @param vz        Start vertex z coordinate [cm]
   ** @param polx      Polarisation in x
   ** @param poly      Polarisation in y
   ** @param polz      Polarisation in z
   ** @param process   Generation process
   ** @param weight    Weight factor
   ** @param status    "Generation status code" - no idea what that means
   ** @param[out] ntr  Track index
   **
   ** This method is pure virtual in TVirtualMCStack. It is called by the
   ** by the transport engine for tracks generated during the transport.
   **/
  virtual void PushTrack(Int_t toBeDone, Int_t parentId, Int_t pdgCode,
                         Double_t px, Double_t py, Double_t pz,
                         Double_t e, Double_t vx, Double_t vy,
                         Double_t vz, Double_t time, Double_t polx,
                         Double_t poly, Double_t polz, TMCProcess process,
                         Int_t& ntr, Double_t weight, Int_t status);


  /** @brief Add a track to the stack (FairGenericStack)
   ** @param toBeDone  Flag for putting the track on the internal stack
   ** @param parentId  Index of parent track
   ** @param pdgCode   PDG particle code
   ** @param px        Momentum x component [GeV]
   ** @param py        Momentum y component [GeV]
   ** @param pz        Momentum z component [GeV]
   ** @param e         Energy [GeV]
   ** @param vx        Start vertex x coordinate [cm]
   ** @param vy        Start vertex y coordinate [cm]
   ** @param vz        Start vertex z coordinate [cm]
   ** @param polx      Polarisation in x
   ** @param poly      Polarisation in y
   ** @param polz      Polarisation in z
   ** @param process   Generation process
   ** @param weight    Weight factor
   ** @param status    "Generation status code" - no idea what that means
   ** @param generatorParentId  Index of the mother track on generator level
   ** @param[out] ntr  Track index
   **
   ** This method is pure virtual in FairGenericStack. It is called by
   ** FairPrimaryGenerator::AddTrack for primary tracks.
   **
   ** The meaning of generatorParentId is that a mother-daughter relationship
   ** may be present already at generator level - usually, when a particle
   ** decay is done in the generator itself. An example is the CbmPlutoGenerator.
   ** This parental relationship is preserved for the particles during transport.
   **/
  virtual void PushTrack(Int_t toBeDone, Int_t parentId, Int_t pdgCode,
                         Double_t px, Double_t py, Double_t pz,
                         Double_t e, Double_t vx, Double_t vy,
                         Double_t vz, Double_t time, Double_t polx,
                         Double_t poly, Double_t polz, TMCProcess proc,
                         Int_t& ntr, Double_t weight, Int_t is,
                         Int_t generatorParentID);


  /** Get next particle for tracking from the stack.
   ** Declared in TVirtualMCStack
   ** @param  iTrack  index of popped track (return)
   ** @return Pointer to the TParticle of the track
   **
   ** This method is called when using TGeant3.
   **/
  virtual TParticle* PopNextTrack(Int_t& iTrack);


  /** Get primary particle by index for tracking from stack
   ** Declared in TVirtualMCStack
   ** @param  iPrim   index of primary particle
   ** @return Pointer to the TParticle of the track
   **
   ** This method is called when using TGeant4.
   **/
  virtual TParticle* PopPrimaryForTracking(Int_t iPrim); 


  /** Set the current track number 
   ** Declared in TVirtualMCStack
   *@param iTrack  track number
   **/
  virtual void SetCurrentTrack(Int_t iTrack)   { fCurrentTrack     = iTrack; }


  /** Get total number of tracks 
   ** Declared in TVirtualMCStack
   **/
  virtual Int_t GetNtrack() const { return fNParticles; }


  /** Get number of primary tracks
   ** Declared in TVirtualMCStack
   **/
  virtual Int_t GetNprimary() const { return fNPrimaries; }


  /** Get the current track's particle
   ** Declared in TVirtualMCStack
   **/
  virtual TParticle* GetCurrentTrack() const;


  /** Get the number of the current track
   ** Declared in TVirtualMCStack
   **/
  virtual Int_t GetCurrentTrackNumber() const { return fCurrentTrack; }


  /** Get the track number of the parent of the current track
   ** Declared in TVirtualMCStack
   **/
  virtual Int_t GetCurrentParentTrackNumber() const;


  /** Add a TParticle to the fParticles array **/
  virtual void AddParticle(TParticle* part);


  /** Fill the MCTrack output array, applying filter criteria **/
  virtual void FillTrackArray();


  /** Update the track index in the MCTracks and MCPoints **/
  virtual void UpdateTrackIndex(TRefArray* detArray);


  /** Resets arrays and stack and deletes particles and tracks **/
  virtual void Reset();


  /** Register the MCTrack array to the Root Manager  **/
  virtual void Register();


  /** Output to screen 
   **@param iVerbose: 0=events summary, 1=track info
   **/
  virtual void Print(Option_t* = "") const;


  /** Modifiers  **/
  void StoreSecondaries(Bool_t choice = kTRUE) { fStoreSecondaries = choice; }
  void SetMinPoints(Int_t min)                 { fMinPoints        = min;    }
  void SetEnergyCut(Double_t eMin)             { fEnergyCut        = eMin;   }
  void StoreMothers(Bool_t choice = kTRUE)     { fStoreMothers     = choice; }


  /** @brief Set the stack filter class **/
  void SetFilter(std::unique_ptr<CbmStackFilter>& filter) {
    fFilter.reset();
    fFilter = std::move(filter);
  }


  /** Increment number of points for the current track in a given detector
   *@param iDet  Detector unique identifier
   **/
  void AddPoint(ECbmModuleId iDet);


  /** Increment number of points for an arbitrary track in a given detector
   *@param iDet    Detector unique identifier
   *@param iTrack  Track number
   **/
  void AddPoint(ECbmModuleId iDet, Int_t iTrack);


  /** Accessors **/
  TParticle* GetParticle(Int_t trackId) const;
  TClonesArray* GetListOfParticles() { return fParticles; }

  /** Swich on/off the tracking of a particle*/
  void DoTracking(Bool_t doTracking = kTRUE) { fdoTracking = doTracking; }


 private:


  /** STL stack (FILO) used to handle the TParticles for tracking **/
  std::stack<TParticle*>  fStack;           //!


  std::unique_ptr<CbmStackFilter> fFilter;  //! Stack filter class


  /** Array of TParticles (contains all TParticles put into or created
   ** by the transport 
   **/
  TClonesArray* fParticles;            //!


  /** Array of CbmMCTracks containg the tracks written to the output **/
  TClonesArray* fTracks;


  /** STL map from particle index to track index  **/
  std::map<Int_t, Int_t>            fIndexMap;        //!
  std::map<Int_t, Int_t>::iterator  fIndexIter;       //!


  /** STL map from track index and detector ID to number of MCPoints **/
  std::map<std::pair<Int_t, ECbmModuleId>, Int_t> fPointsMap;     //!


  /** Some indizes and counters **/
  Int_t fCurrentTrack;  //! Index of current track
  Int_t fNPrimaries;    //! Number of primary particles
  Int_t fNParticles;    //! Number of entries in fParticles
  Int_t fNTracks;       //! Number of entries in fTracks
  Int_t fIndex;         //! Used for merging


  /** Variables defining the criteria for output selection **/
  Bool_t     fStoreSecondaries;
  Int_t      fMinPoints;
  Double32_t fEnergyCut;
  Bool_t     fStoreMothers;

  /** go to tracking  */
  Bool_t fdoTracking; //! global switch for geant propagation

  CbmStack(const CbmStack&);
  CbmStack& operator=(const CbmStack&);

  ClassDef(CbmStack,3)


};






#endif 
