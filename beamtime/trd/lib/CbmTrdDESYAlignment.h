#ifndef CbmTrdDESYAlignment_H
#define CbmTrdDESYAlignment_H

#include "CbmTrdQABase.h"

/** @Class CbmTrdDESYAlignment
 ** @author Philipp Munkes <p.munkes@uni-muenster.de>
 ** @date 10th April 2017
 ** @brief Example of a derived class for CBM TRD Testbeam analysis.
 **
 ** This class is intended for the Alignment of the TRD-Modules at DESY 2017.
 **/

class CbmTrdDESYAlignment : public CbmTrdQABase
{
protected:
  /** Constructor with parameters (Optional) **/
  CbmTrdDESYAlignment (TString ClassName);
public:

  virtual void
  CreateHistograms ();
  virtual void
  Exec (Option_t*);
  std::vector<CbmSpadicRawMessage*>::iterator
  FindNextHit (std::vector<CbmSpadicRawMessage*>::iterator,
	       std::vector<CbmSpadicRawMessage*>::iterator,
	       std::vector<CbmSpadicRawMessage*>, Long64_t);
  /** Default constructor **/
  CbmTrdDESYAlignment () :
      CbmTrdDESYAlignment ("CbmTrdDESYAlignment")
  {
  }
  ;
  TH2* fTemporalAlign,*fSpatialAlign;

  CbmTrdDESYAlignment(const CbmTrdDESYAlignment&) = delete;
  CbmTrdDESYAlignment& operator=(const CbmTrdDESYAlignment&) = delete;

ClassDef(CbmTrdDESYAlignment,1)
  ;
};

#endif
