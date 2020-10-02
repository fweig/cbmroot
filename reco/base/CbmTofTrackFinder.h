// -------------------------------------------------------------------------
// -----                   CbmTofTrackFinder header file               -----
// -----                  Created 25/04/15  by N. Herrmann             -----
// -----                according to the CbmStsTrackFinder             -----
// -------------------------------------------------------------------------


/** CbmTofTrackFinder
 *@author D.Kresan <D.Kresan@gsi.de>
 **
 ** Abstract base class for concrete TOF track finding algorithm.
 ** Each derived class must implement the method DoFind. This has
 ** to operate on the TClonesArrays of TOF hits and to fill
 ** the CbmTofTrackArray.
 **/

#ifndef CBMTOFTRACKFINDER
#define CBMTOFTRACKFINDER 1

#include "TObject.h"

class TClonesArray;
class CbmTofHit;
class CbmTofTrack;
class CbmTofTrackFitter;

class CbmTofTrackFinder : public TObject {

public:
  /** Default constructor **/
  CbmTofTrackFinder() {};


  /** Destructor **/
  virtual ~CbmTofTrackFinder() {};


  /** Virtual method Init. If needed, to be implemented in the
   ** concrete class. Else no action.
   **/
  virtual void Init() {};

  virtual inline void SetFitter(CbmTofTrackFitter* /*Fitter*/) {};
  virtual inline void SetMaxTofTimeDifference(Double_t /*val*/) {};
  virtual inline void SetTxLIM(Double_t /*val*/) {};
  virtual inline void SetTyLIM(Double_t /*val*/) {};
  virtual inline void SetTxMean(Double_t /*val*/) {};
  virtual inline void SetTyMean(Double_t /*val*/) {};
  virtual inline void SetSIGLIM(Double_t /*val*/) {};
  virtual inline void SetSIGLIMMOD(Double_t /*val*/) {};
  virtual inline void SetChiMaxAccept(Double_t /*val*/) {};
  virtual inline void SetSIGT(Double_t /*val*/) {};
  virtual inline void SetSIGX(Double_t /*val*/) {};
  virtual inline void SetSIGY(Double_t /*val*/) {};

  /** Abstract method DoFind. To be implemented in the concrete class.
   ** Task: Read the hit array and fill the track array,
   ** pointers to which are given as arguments
   **
   *@param hitArray   Array of TOF hits
   *@param trackArray  Array of CbmTofTrack
   **
   *@value Number of tracks created
   **/
  //  virtual Int_t DoFind( vector<CbmTofHit*> &hits, vector<CbmTofTrack*> &tracks)=0;

  virtual Int_t DoFind(TClonesArray* hits, TClonesArray* tracks) = 0;

  ClassDef(CbmTofTrackFinder, 1);
};

#endif
