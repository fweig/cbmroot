/** @file CbmMuchMatchTracks.h
  * @copyright Copyright (C) 2007-2012 St. Petersburg Polytechnic University, St. Petersburg
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Andrey Lebedev, Mikhail Ryzhinskiy [committer] **/

/** CbmMuchMatchTracks.h
 *@author A.Lebedev <Andrey.Lebedev@gsi.de>
 *
 * 2009-03-16 E. Kryshen
 * Matching of hits to clusters with several MC points
 * 2009-06-01 A.Lebedev
 * Include matching for straw hits
 * 2011-05-19 A.Lebedev
 * Correct matching in case of clustering
 **/

#ifndef CBMMUCHMATCHTRACKS_H_
#define CBMMUCHMATCHTRACKS_H_ 1

#include "FairTask.h"

#include <map>

class TClonesArray;

class CbmMuchMatchTracks : public FairTask {
public:
  CbmMuchMatchTracks();
  virtual ~CbmMuchMatchTracks();

  virtual InitStatus Init();
  virtual void Exec(Option_t* opt);
  virtual void Finish();

private:
  void ExecPixel(std::map<Int_t, Int_t>& matchMap, Int_t index);

  TClonesArray* fTracks;
  TClonesArray* fPoints;
  TClonesArray* fPixelHits;
  TClonesArray* fMatches;
  TClonesArray* fPixelDigiMatches;
  TClonesArray* fClusters;

  Int_t fNofHits;
  Int_t fNofTrueHits;
  Int_t fNofWrongHits;
  Int_t fNofFakeHits;

  Int_t fNEvents;

  CbmMuchMatchTracks(const CbmMuchMatchTracks&);
  CbmMuchMatchTracks& operator=(const CbmMuchMatchTracks&);

  ClassDef(CbmMuchMatchTracks, 1);
};

#endif
