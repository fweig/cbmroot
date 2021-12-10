/* Copyright (C) 2013-2020 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer], Florian Uhlig */

/**
 * \file CbmTrackMatchNew.h
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2013
 *
 * Base data class for storing RECO-to-MCTrack matching information.
 **/

#ifndef CBMTRACKMATCHNEW_H_
#define CBMTRACKMATCHNEW_H_

#include "CbmMatch.h"  // for CbmMatch

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef

#include <cstdint>
#include <string>  // for string

class CbmTrackMatchNew : public CbmMatch {
public:
  /**
    * \brief Default constructor.
    */
  CbmTrackMatchNew();

  /**
    * \brief Destructor.
    */
  virtual ~CbmTrackMatchNew();

  /* Accessors */
  int32_t GetNofTrueHits() const { return fNofTrueHits; }
  int32_t GetNofWrongHits() const { return fNofWrongHits; }
  int32_t GetNofHits() const { return fNofTrueHits + fNofWrongHits; }
  double GetTrueOverAllHitsRatio() const
  {
    double all = GetNofHits();
    return (all == 0) ? 0. : (fNofTrueHits / all);
  }
  double GetWrongOverAllHitsRatio() const
  {
    double all = GetNofHits();
    return (all == 0) ? 0. : (fNofWrongHits / all);
  }

  /* Modifiers */
  void SetNofTrueHits(int32_t nofTrueHits) { fNofTrueHits = nofTrueHits; }
  void SetNofWrongHits(int32_t nofWrongHits) { fNofWrongHits = nofWrongHits; }

  /**
    * \brief Return string representation of the object.
    * \return String representation of the object.
    **/
  virtual std::string ToString() const;

private:
  int32_t fNofTrueHits;   // Number of true hits in reconstructed track
  int32_t fNofWrongHits;  // Number of wrong hits in reconstructed track

  ClassDef(CbmTrackMatchNew, 1);
};

#endif /* CBMTRACKMATCHNEW_H_ */
