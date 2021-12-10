/* Copyright (C) 2004-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig, Denis Bertini [committer], Semen Lebedev */

#ifndef CBM_RICH_RING_H
#define CBM_RICH_RING_H

#include <Rtypes.h>      // for ClassDef
#include <TObject.h>     // for TObject

#include <cstdint>
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
  CbmRichRing(float x, float y, float r);

  /** Destructor **/
  virtual ~CbmRichRing();

  /** to attach the rich hit to the ring */
  void AddHit(uint32_t pHit) { fHitCollection.push_back(pHit); }
  /** remove hit from ring
	 * hitId is the index in the RICH hit array
     * return false if hit is not found in ring*/
  bool RemoveHit(uint32_t hitId);
  /** to obtain the number of hits associated to the ring */
  int32_t GetNofHits() const { return fHitCollection.size(); }
  /** to obtain the rich hit at a particular index */
  uint32_t GetHit(int32_t i) const { return fHitCollection[i]; }
  /** to print ring parameters **/
  virtual void Print(Option_t* opt = "") const;


  /** Modifiers**/
  void SetAPar(double a) { fAPar = a; }
  void SetBPar(double b) { fBPar = b; }
  void SetCPar(double c) { fCPar = c; }
  void SetDPar(double d) { fDPar = d; }
  void SetEPar(double e) { fEPar = e; }
  void SetFPar(double f) { fFPar = f; }

  void SetCenterX(float x) { fCenterX = x; }
  void SetCenterY(float y) { fCenterY = y; }
  void SetRadius(float r) { fRadius = r; }
  void SetAaxis(double a) { fAaxis = a; }
  void SetBaxis(double b) { fBaxis = b; }
  void SetAaxisCor(double a) { fAaxisCor = a; }
  void SetBaxisCor(double b) { fBaxisCor = b; }
  void SetXYABPhi(double x, double y, double a, double b, double phi);
  void SetPhi(double phi) { fPhi = phi; }
  void SetChi2(double chi2) { fChi2 = chi2; }
  void SetRecFlag(int32_t recflag) { fRecFlag = recflag; }
  void SetAngle(double angle) { fAngle = angle; }
  void SetNofHitsOnRing(int32_t onring) { fNofHitsOnRing = onring; }
  /** number between -1 and 1: -1 = fake ring, 1 = good ring (selection by neural net)*/
  void SetSelectionNN(double selectionNN) { fSelectionNN = selectionNN; }
  void SetTime(double time) { fTime = time; }

  /** Accessors **/
  double GetAPar() const { return fAPar; }
  double GetBPar() const { return fBPar; }
  double GetCPar() const { return fCPar; }
  double GetDPar() const { return fDPar; }
  double GetEPar() const { return fEPar; }
  double GetFPar() const { return fFPar; }

  float GetCenterX() const { return fCenterX; }
  float GetCenterY() const { return fCenterY; }
  float GetRadius() const { return fRadius; }
  double GetAaxis() const { return fAaxis; }
  double GetBaxis() const { return fBaxis; }
  double GetAaxisCor() const { return fAaxisCor; }
  double GetBaxisCor() const { return fBaxisCor; }
  double GetPhi() const { return fPhi; }
  double GetXF1() const;
  double GetYF1() const;
  double GetXF2() const;
  double GetYF2() const;


  double GetSelectionNN() const { return fSelectionNN; }
  double GetChi2() const { return fChi2; }
  double GetNDF() const { return GetNofHits() - 5; }
  float GetRadialPosition() const;
  double GetAngle() const { return fAngle; }
  int32_t GetNofHitsOnRing() const { return fNofHitsOnRing; }
  double GetRadialAngle() const;
  int32_t GetRecFlag() const { return fRecFlag; }
  double GetTime() const { return fTime; }

private:
  std::vector<uint32_t> fHitCollection; /** STL container to hold the hits */

protected:
  double fAPar;  // Axx+Bxy+Cyy+Dx+Ey+F
  double fBPar;
  double fCPar;
  double fDPar;
  double fEPar;
  double fFPar;

  float fCenterX;
  float fCenterY;
  float fRadius;

  double fAaxis;     // major axis of ellipse
  double fBaxis;     // minor axes of the ellipse
  double fAaxisCor;  // major axis of ellipse after correction
  double fBaxisCor;  // minor axes of the ellipse after correction

  double fPhi;  // rotation angle

  double fChi2;
  double fAngle;
  int32_t fNofHitsOnRing;

  double fSelectionNN;  // value for selection high quality rings

  int32_t fRecFlag;

  double fTime;

  ClassDef(CbmRichRing, 2)
};

#endif  // CBM_RICH_RING_H
