/*
 * NicaTrackCombined.h
 *
 *  Created on: 4 kwi 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACOMPLEXTRACK_H_
#define NICACOMPLEXTRACK_H_

#include "NicaTrack.h"

/**
 * base class for "comlex track" - object that is build from "real" and
 * "imaginary" track
 */
class NicaComplexTrack : public NicaTrack {
  Int_t fMatchID;
  NicaTrack* fRealTrack;
  NicaTrack* fImgTrack;

public:
  NicaComplexTrack();
  NicaComplexTrack(const NicaComplexTrack& other);
  NicaComplexTrack& operator=(const NicaComplexTrack& other);
  void SetMatchID(Int_t match) { fMatchID = match; };
  void SetRealTrack(NicaTrack* tr) { fRealTrack = tr; };
  void SetImgTrack(NicaTrack* tr) { fImgTrack = tr; };
  Int_t GetMatchID() const { return fMatchID; };
  virtual void Clear(Option_t* opt = "");
  virtual Float_t GetFieldVal(Int_t fieldID) const;
  virtual void CopyData(NicaTrack* other);
  virtual void CopyAllData(NicaTrack* other);
  virtual void ResetTrack(Int_t thisId = -1, NicaEvent* event = nullptr);
  virtual TString GetFieldName(Int_t fieldID) const;
  NicaTrack* GetRealTrack() const { return fRealTrack; };
  NicaTrack* GetImgTrack() const { return fImgTrack; };
  virtual ~NicaComplexTrack();
  ClassDef(NicaComplexTrack, 1)
};

#endif /* NICACOMPLEXTRACK_H_ */
