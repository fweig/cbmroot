/*
 * NicaCbmMatchedTrack.h
 *
 *  Created on: 12 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMMATCHEDTRACK_H_
#define NICACBMMATCHEDTRACK_H_

#include "NicaCbmGlobalEventInterface.h"
#include "NicaCbmTrack.h"

class NicaCbmMatchedTrack : public NicaCbmTrack {
  Int_t fStsMCIndex;
  Int_t fTrdMCIndex;
  Int_t fTofMCIndex;
  Int_t fRichMCIndex;
  Int_t fMuchMCIndex;

public:
  NicaCbmMatchedTrack();
  virtual void CopyData(NicaTrack* track);
  virtual void Update(CbmGlobalTrack* track,
                      NicaCbmGlobalEventInterface* interface);
  Int_t GetStsMcIndex() const { return fStsMCIndex; }
  Int_t GetTofMcIndex() const { return fTofMCIndex; }
  Int_t GetTrdMcIndex() const { return fTrdMCIndex; }
  Int_t GetRichMcIndex() const { return fRichMCIndex; }
  Int_t GetMuchMcIndex() const { return fMuchMCIndex; }
  void SetStsMcIndex(Int_t stsMcIndex) { fStsMCIndex = stsMcIndex; }
  void SetTofMcIndex(Int_t tofMcIndex) { fTofMCIndex = tofMcIndex; }
  void SetTrdMcIndex(Int_t trdMcIndex) { fTrdMCIndex = trdMcIndex; }
  void SetRichMcIndex(Int_t richMcIndex) { fTrdMCIndex = richMcIndex; }
  void SetMuchMcIndex(Int_t muchIndex) { fMuchMCIndex = muchIndex; };
  virtual ~NicaCbmMatchedTrack();
  ClassDef(NicaCbmMatchedTrack, 1)
};

#endif /* NICA_CBM_FORMAT_MATCHED_NICACBMMATCHEDTRACK_H_ */
