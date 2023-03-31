/* Copyright (C) 20236 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Christina Dritsa [committer], Philipp Sitzmann, Florian Uhlig */

#ifndef CBMMVDPIXELCHARGE_H
#define CBMMVDPIXELCHARGE_H 1

#include "CbmLink.h"

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Float_t, Int_t, Short_t, Bool_t, Option_t
#include <TObject.h>     // for TObject

class TBuffer;
class TClass;
class TMemberInspector;


class CbmMvdPixelCharge : public TObject {

public:
  /** Default constructor **/
  CbmMvdPixelCharge() = default;

  /** Constructor with all variables **/

  CbmMvdPixelCharge(Float_t charge, Int_t channelNrX, Int_t channelNrY, Int_t hitId, Int_t trackId, Float_t pointX,
                    Float_t pointY, Float_t time = 0.0, Int_t frame = 0);

  virtual ~CbmMvdPixelCharge() = default;

  Bool_t TestXY(Int_t channelNrX, Int_t channelNrY);

  void DigestCharge(Float_t pointX, Float_t pointY, Double_t time, Int_t PointId,
                    Int_t trackId);  // TODO: add time here
  void AddCharge(Float_t charge) { fTrackCharge = fTrackCharge + charge; };


  Float_t GetCharge() { return fCharge; };
  Int_t GetX() { return fChannelNrX; };
  Int_t GetY() { return fChannelNrY; };
  Float_t GetMaxChargeContribution() { return fMaxChargeContribution; };
  Short_t GetNContributors() { return fContributors; };
  Int_t GetDominatorIndex() { return fDominatorIndex; }
  std::vector<Int_t>& GetTrackID() { return fTrackId; }
  std::vector<Int_t>& GetPointID() { return fPointId; }
  std::vector<Float_t>& GetPointX() { return fPointX; }
  std::vector<Float_t>& GetPointY() { return fPointY; }
  std::vector<Float_t>& GetPointWeight() { return fPointWeight; }
  std::vector<Double_t>& GetTime() { return fTime; }
  Float_t GetPixelTime() { return fPixelTime; }
  Int_t GetFrame() { return fFrame; }

  virtual void Clear(Option_t* = "") {};


private:
  Int_t fFrame = {-1};

  Float_t fCharge                   = {-1.};
  Float_t fMaxChargeContribution    = {0.};
  Float_t fDominatingPointX         = {-1.};
  Float_t fDominatingPointY         = {-1.};
  Short_t fContributors             = {0};
  Int_t fChannelNrX                 = {0};
  Int_t fChannelNrY                 = {0};
  Float_t fTrackCharge              = {0.};
  Int_t fDominatorTrackId           = {-1};
  Int_t fDominatorPointId           = {-1};
  Short_t fDominatorIndex           = {0};
  Float_t fPixelTime                = {-1.};
  std::vector<Int_t> fTrackId       = {};
  std::vector<Int_t> fPointId       = {};
  std::vector<Float_t> fPointWeight = {};
  std::vector<Float_t> fPointX      = {};
  std::vector<Float_t> fPointY      = {};
  std::vector<Double_t> fTime       = {};
  std::vector<CbmLink> fLink        = {};

  ClassDef(CbmMvdPixelCharge, 1);
};

#endif
