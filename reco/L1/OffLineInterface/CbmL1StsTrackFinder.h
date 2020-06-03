/*
 *====================================================================
 *
 *  CBM Level 1 Reconstruction 
 *  
 *  Authors: I.Kisel,  S.Gorbunov
 *
 *  e-mail : ikisel@kip.uni-heidelberg.de 
 *
 *====================================================================
 *
 *  CbmL1StsTrackFinder header
 *
 *====================================================================
 */

#ifndef CBML1STSTRACKFINDER
#define CBML1STSTRACKFINDER 1


#include "CbmL1.h"

#include "CbmStsTrackFinder.h"

class TClonesArray;
class CbmEvent;


class CbmL1StsTrackFinder : public CbmStsTrackFinder
{

 public:

  /** Default constructor **/
  CbmL1StsTrackFinder();


  /** Destructor **/
  virtual ~CbmL1StsTrackFinder();


  /** Initialisation **/
  virtual void Init();


  /** Track finding algorithm
   **/
  virtual Int_t DoFind();

  /** Execute track finding on one event
   ** @param event  Pointer to event object
   ** @value Number of created tracks
   **/
  virtual Int_t FindTracks(CbmEvent* event);


 private:

  /** Copy the tracks from the L1-internal format and array
   ** to the output TClonesArray.
   ** @value  Number of created tracks
   **/
  Int_t CopyL1Tracks(CbmEvent* event = NULL);


  ClassDef(CbmL1StsTrackFinder,1);

};


#endif
