/** CbmMuchSectorRectangular.h
 *@author  E.Kryshen <e.kryshen@gsi.de>
 *@since   02.08.12
 *@version 2.0
 **
 ** This class describes the digitization scheme for a sector of the MuCh.
 ** The sector is a rectangle of size fLx and fLy.
 **
 **/
#ifndef CBMMUCHSECTORRECTANGULAR_H
#define CBMMUCHSECTORRECTANGULAR_H 1

#include "CbmMuchSector.h"  // for CbmMuchSector

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Bool_t, UInt_t, kFALSE
#include <TPave.h>       // for TPave
#include <TVector3.h>    // for TVector3

#include <vector>  // for vector

class CbmMuchPadRectangular;

class CbmMuchSectorRectangular : public CbmMuchSector, public TPave {
public:
  CbmMuchSectorRectangular();
  CbmMuchSectorRectangular(UInt_t modAddress, UInt_t index, TVector3 pos, TVector3 size, Int_t padNx, Int_t padNy);
  virtual ~CbmMuchSectorRectangular() {};
  //  virtual void GetPadVertices(Int_t iChannel, Double_t* xPad, Double_t* yPad);
  TVector3 GetPosition() const { return fPosition; }
  TVector3 GetSize() const { return fSize; }
  Int_t GetPadNx() const { return fPadNx; }
  Int_t GetPadNy() const { return fPadNy; }
  Double_t GetPadDx() const { return fPadDx; }
  Double_t GetPadDy() const { return fPadDy; }
  Double_t GetSigmaX() const { return fPadDx / 12; }
  Double_t GetSigmaY() const { return fPadDy / 12; }
  Double_t GetXmin() const { return fPosition[0] - fSize[0] / 2; }
  Double_t GetYmin() const { return fPosition[1] - fSize[1] / 2; }

  //  TArrayI  GetNeighbours() const { return fNeighbours; }
  void SetNeighbours(std::vector<CbmMuchSectorRectangular*> neighbours) { fNeighbours = neighbours; }
  Bool_t Inside(Double_t x, Double_t y) { return x > fX1 && x < fX2 && y > fY1 && y < fY2; }
  Bool_t IsIncomplete() { return kFALSE; }

  std::vector<CbmMuchSectorRectangular*> GetNeighbours() { return fNeighbours; }
  CbmMuchPadRectangular* GetPad(Double_t x, Double_t y);
  void AddPads();
  void DrawPads();

protected:
  TVector3 fPosition;
  TVector3 fSize;
  Int_t fPadNx;
  Int_t fPadNy;
  Double_t fPadDx;
  Double_t fPadDy;
  std::vector<CbmMuchSectorRectangular*> fNeighbours;  //!

  ClassDef(CbmMuchSectorRectangular, 1);
};

#endif
