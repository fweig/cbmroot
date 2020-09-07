// -------------------------------------------------------------------------
// -----                   CbmStsTrackFinder header file               -----
// -----                  Created 02/02/05  by V. Friese               -----
// -------------------------------------------------------------------------


/** CbmStsTrackFinder
 *@author V.Friese <v.friese@gsi.de>
 **
 ** Abstract base class for concrete STS track finding algorithm.
 ** Each derived class must implement the method DoFind. This has
 ** to operate on the two TClonesArrays of pixel and strip hits
 ** and to fill the CbmStsTrackArray.
 **/

#ifndef CBMSTSTRACKFINDER
#define CBMSTSTRACKFINDER 1


#include "TNamed.h"
#include "TObject.h"

class TClonesArray;
class CbmStsDigiScheme;
class FairField;
class CbmEvent;
class CbmStsTrack;


class CbmStsTrackFinder : public TNamed {

public:
  /** Default constructor **/
  CbmStsTrackFinder();


  /** Destructor **/
  virtual ~CbmStsTrackFinder() {};


  /** Virtual method Init. If needed, to be implemented in the
   ** concrete class. Else no action.
   **/
  virtual void Init() {};


  /** Abstract method DoFind. To be implemented in the concrete class.
   ** Task: Read the hit array and fill the track array,
   ** pointers to which are private members and set by the task
   **
   *@value Number of tracks created
   **/
  virtual Int_t DoFind() = 0;


  /** Virtual method Finish. If needed, to be implemented in the concrete
   ** class. Executed at the end of the run.
   **/
  virtual void Finish() {};


  /** Track finding in one event (abstract)
   ** @param event    Pointer to event object
   ** @param nTracks  Number of StsTrack objects created
   **/
  virtual Int_t FindTracks(CbmEvent* event) = 0;


  /** Modifiers **/
  void SetDigiScheme(CbmStsDigiScheme* scheme) { fDigiScheme = scheme; }
  void SetField(FairField* field) { fField = field; }
  void SetMvdHitArray(TClonesArray* hits) { fMvdHits = hits; }
  void SetStsHitArray(TClonesArray* hits) { fStsHits = hits; }
  void SetTrackArray(TClonesArray* tracks) { fTracks = tracks; }
  void SetVerbose(Int_t verbose) { fVerbose = verbose; };


protected:
  CbmStsDigiScheme* fDigiScheme;  // STS digitisation scheme
  FairField* fField;              // Magnetic field
  TClonesArray* fMvdHits;         // MvdHit array
  TClonesArray* fStsHits;         // StsHit array
  TClonesArray* fTracks;          // StsTrack array
  TClonesArray* fStsClusters;     // StsCluster array
  Int_t fVerbose;                 // Verbosity level


  /** Median energy loss calculation for the tracks in event/timeslice
   ** Ported from CbmKFParticleFinderPID
   ** Description of the method given at 30th CBM CM
   ** https://indico.gsi.de/event/4760/session/4/contribution/80/material/slides/0.pdf
   **/
  double CalculateEloss(CbmStsTrack* cbmStsTrack);
  void FillEloss();

private:
  constexpr static int MaxAdcVal() { return 31; }

  /** Calculate median value of a vector
   **/
  double VecMedian(std::vector<double>& vec);


  CbmStsTrackFinder(const CbmStsTrackFinder&);
  CbmStsTrackFinder& operator=(const CbmStsTrackFinder&);

  ClassDef(CbmStsTrackFinder, 1);
};

#endif
