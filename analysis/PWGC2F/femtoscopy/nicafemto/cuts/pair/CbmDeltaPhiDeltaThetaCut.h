/*
 * CbmDeltaPhiDeltaThetaCut.h
 *
 *  Created on: 22 cze 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMDELTAPHIDELTATHETACUT_H_
#define CBMDELTAPHIDELTATHETACUT_H_
#include "NicaCbmFormatTypes.h"
#include "NicaTwoTrackCut.h"

class CbmDeltaPhiDeltaThetaStarCut : public NicaTwoTrackCut {
  Double_t fR;
  NicaCbmFormats::ECbmFormatType fDataType;

public:
  CbmDeltaPhiDeltaThetaStarCut();
  CbmDeltaPhiDeltaThetaStarCut& operator=(const CbmDeltaPhiDeltaThetaStarCut& other);
  void SetR(Double_t r) { fR = r; }
  virtual Bool_t Init(Int_t task_id = 0);
  Bool_t Pass(NicaTwoTrack* pair);
  virtual NicaPackage* Report() const;
  virtual ~CbmDeltaPhiDeltaThetaStarCut();
  ClassDef(CbmDeltaPhiDeltaThetaStarCut, 1)
};

class CbmDeltaPhiDeltaThetaCut : public NicaTwoTrackCut {
public:
  CbmDeltaPhiDeltaThetaCut();
  virtual Bool_t Init(Int_t task_id = 0);
  Bool_t Pass(NicaTwoTrack* pair);
  virtual ~CbmDeltaPhiDeltaThetaCut();
  ClassDef(CbmDeltaPhiDeltaThetaCut, 1)
};

#endif /* CBMDELTAPHIDELTATHETACUT_H_ */
