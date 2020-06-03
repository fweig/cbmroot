// -------------------------------------------------------------------------
// -----                   CbmTofTrackFitter header file               -----
// -----                  Created 28/11/05  by D. Kresan               -----
// -----                according to the CbmStsTrackFitter             -----
// -------------------------------------------------------------------------


/** CbmTofTrackFitter
 *@author D.Kresan <D.Kresan@gsi.de>
 **
 ** Abstract base class for concrete TOF track fitting algorithm.
 ** Each derived class must implement the method DoFit. This has
 ** to operate on an object of type CbmTofTrack and fill the
 ** parameters fPidHypo, fParamFirst, fParamLast, fFlag and fChi2.
 **/

#ifndef CBMTOFTRACKFITTER
#define CBMTOFTRACKFITTER 1

#include "FairTrackParam.h"

#include "TObject.h"

class CbmTofTrack;
class CbmTofTracklet;


class CbmTofTrackFitter : public TObject
{

 public:

  /** Default constructor **/
  CbmTofTrackFitter() { };


  /** Destructor **/
  virtual ~CbmTofTrackFitter() { };


  /** Virtual method Init. If needed, to be implemented in the
   ** concrete class. Else no action.
   **/
  virtual void Init() { };


  /** Abstract method DoFit. To be implemented in the concrete class.
   ** Task: Make a fit to the hits attached to the track by the track
   ** finder. Fill the track parameter member variables.
   **
   *@param pTrack      Pointer to CbmTofTrack
   **/
  virtual Int_t DoFit(CbmTofTrack* pTrack)    = 0;
  virtual Int_t DoFit(CbmTofTracklet* pTrack) = 0;
  virtual void Extrapolate(const FairTrackParam* trackPar, Double_t z, FairTrackParam *e_track )=0;  
  /*
  virtual void Extrapolate(CbmTofTracklet* track, Double_t z, FairTrackParam *e_track );
  */
  ClassDef(CbmTofTrackFitter,1);

};

#endif
