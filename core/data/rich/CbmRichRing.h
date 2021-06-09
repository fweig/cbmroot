/* Copyright (C) 2004-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Denis Bertini [committer], Semen Lebedev */

#ifndef CBM_RICH_RING_H
#define CBM_RICH_RING_H

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double_t, UInt_t, Float_t, Int_t, Bool_t
#include <TObject.h>     // for TObject

#include <vector>  // for vector

class CbmRichRing : public TObject {

public:
  /** Default constructor **/
  CbmRichRing();

  /** Constructor with arguments
	   *@param x    x Position of ring center [cm]
	   *@param y    y Position of ring center [cm]
	   *@param r    radius of ring [cm]
	 **/
  CbmRichRing(Float_t x, Float_t y, Float_t r);

  /** Destructor **/
  virtual ~CbmRichRing();

  /** to attach the rich hit to the ring */
  void AddHit(UInt_t pHit) { fHitCollection.push_back(pHit); }
  /** remove hit from ring
	 * hitId is the index in the RICH hit array
     * return false if hit is not found in ring*/
  Bool_t RemoveHit(UInt_t hitId);
  /** to obtain the number of hits associated to the ring */
  Int_t GetNofHits() const { return fHitCollection.size(); }
  /** to obtain the rich hit at a particular index */
  UInt_t GetHit(Int_t i) const { return fHitCollection[i]; }
  /** to print ring parameters **/
  virtual void Print(Option_t* opt = "") const;


  /** Modifiers**/
  void SetAPar(Double_t a) { fAPar = a; }
  void SetBPar(Double_t b) { fBPar = b; }
  void SetCPar(Double_t c) { fCPar = c; }
  void SetDPar(Double_t d) { fDPar = d; }
  void SetEPar(Double_t e) { fEPar = e; }
  void SetFPar(Double_t f) { fFPar = f; }

  void SetCenterX(Float_t x) { fCenterX = x; }
  void SetCenterY(Float_t y) { fCenterY = y; }
  void SetRadius(Float_t r) { fRadius = r; }
  void SetAaxis(Double_t a) { fAaxis = a; }
  void SetBaxis(Double_t b) { fBaxis = b; }
  void SetAaxisCor(Double_t a) { fAaxisCor = a; }
  void SetBaxisCor(Double_t b) { fBaxisCor = b; }
  void SetXYABPhi(Double_t x, Double_t y, Double_t a, Double_t b, Double_t phi);
  void SetPhi(Double_t phi) { fPhi = phi; }
  void SetChi2(Double_t chi2) { fChi2 = chi2; }
  void SetRecFlag(Int_t recflag) { fRecFlag = recflag; }
  void SetAngle(Double_t angle) { fAngle = angle; }
  void SetNofHitsOnRing(Int_t onring) { fNofHitsOnRing = onring; }
  /** number between -1 and 1: -1 = fake ring, 1 = good ring (selection by neural net)*/
  void SetSelectionNN(Double_t selectionNN) { fSelectionNN = selectionNN; }
  void SetTime(Double_t time) { fTime = time; }

  /** Accessors **/
  Double_t GetAPar() const { return fAPar; }
  Double_t GetBPar() const { return fBPar; }
  Double_t GetCPar() const { return fCPar; }
  Double_t GetDPar() const { return fDPar; }
  Double_t GetEPar() const { return fEPar; }
  Double_t GetFPar() const { return fFPar; }

  Float_t GetCenterX() const { return fCenterX; }
  Float_t GetCenterY() const { return fCenterY; }
  Float_t GetRadius() const { return fRadius; }
  Double_t GetAaxis() const { return fAaxis; }
  Double_t GetBaxis() const { return fBaxis; }
  Double_t GetAaxisCor() const { return fAaxisCor; }
  Double_t GetBaxisCor() const { return fBaxisCor; }
  Double_t GetPhi() const { return fPhi; }
  Double_t GetXF1() const;
  Double_t GetYF1() const;
  Double_t GetXF2() const;
  Double_t GetYF2() const;


  Double_t GetSelectionNN() const { return fSelectionNN; }
  Double_t GetChi2() const { return fChi2; }
  Double_t GetNDF() const { return GetNofHits() - 5; }
  Float_t GetRadialPosition() const;
  Double_t GetAngle() const { return fAngle; }
  Int_t GetNofHitsOnRing() const { return fNofHitsOnRing; }
  Double_t GetRadialAngle() const;
  Int_t GetRecFlag() const { return fRecFlag; }
  Double_t GetTime() const { return fTime; }

private:
  std::vector<UInt_t> fHitCollection; /** STL container to hold the hits */

protected:
  Double_t fAPar;  // Axx+Bxy+Cyy+Dx+Ey+F
  Double_t fBPar;
  Double_t fCPar;
  Double_t fDPar;
  Double_t fEPar;
  Double_t fFPar;

  Float_t fCenterX;
  Float_t fCenterY;
  Float_t fRadius;

  Double_t fAaxis;     // major axis of ellipse
  Double_t fBaxis;     // minor axes of the ellipse
  Double_t fAaxisCor;  // major axis of ellipse after correction
  Double_t fBaxisCor;  // minor axes of the ellipse after correction

  Double_t fPhi;  // rotation angle

  Double_t fChi2;
  Double_t fAngle;
  Int_t fNofHitsOnRing;

  Double_t fSelectionNN;  // value for selection high quality rings

  Int_t fRecFlag;

  Double_t fTime;

  ClassDef(CbmRichRing, 2)
};

#endif  // CBM_RICH_RING_H
