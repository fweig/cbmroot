#ifndef CBMTRDPARMODDIGI_H
#define CBMTRDPARMODDIGI_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Double_t, Int_t, Bool_t, Option_t
#include <TArrayD.h>     // for TArrayD

#include "CbmTrdParMod.h"  // for CbmTrdParMod

class CbmTrdPoint;
class TVector3;

/** \brief Definition of chamber gain conversion for one TRD module **/
class CbmTrdParModDigi : public CbmTrdParMod {
public:
  CbmTrdParModDigi();
  CbmTrdParModDigi(Double_t x,
                   Double_t y,
                   Double_t z,
                   Double_t sizex,
                   Double_t sizey,
                   Double_t sizez,
                   Int_t nofSectors,
                   Int_t orientation,
                   const TArrayD& sectorSizeX,
                   const TArrayD& sectorSizeY,
                   const TArrayD& padSizeX,
                   const TArrayD& padSizeY);
  virtual ~CbmTrdParModDigi() { ; }
  Int_t GetNofColumns() const;
  Int_t GetNofRows() const;

  Int_t GetNofColumnsInSector(Int_t i) const;
  Int_t GetNofRowsInSector(Int_t i) const;
  Int_t GetOrientation() const { return fOrientation; }
  Double_t GetPadSizeX(Int_t i) const { return fPadSizeX.At(i); }
  Double_t GetPadSizeY(Int_t i) const { return fPadSizeY.At(i); }
  Double_t GetSectorBeginX(Int_t i) const { return fSectorBeginX.At(i); }
  Double_t GetSectorBeginY(Int_t i) const { return fSectorBeginY.At(i); }
  Double_t GetSectorSizeX(Int_t i) const { return fSectorSizeX.At(i); }
  Double_t GetSectorSizeY(Int_t i) const { return fSectorSizeY.At(i); }

  Double_t GetAnodeWireToPadPlaneDistance() const {
    return fAnodeWireToPadPlaneDistance;
  }
  Double_t GetAnodeWireOffset() const { return fAnodeWireOffset; }
  Double_t GetAnodeWireSpacing() const { return fAnodeWireSpacing; }

  Int_t GetNofSectors() const { return fNofSectors; }

  Int_t GetModuleRow(Int_t& sectorId, Int_t& rowId) const;
  /**
   * \brief Find the sector wise row given the module row. Inverse of
   *GetModuleRow()
   * \param[in] growId Module wise row id.
   * \param[out] srowId On return sector wise row id.
   * \return Sector id.
   **/
  Int_t GetSectorRow(Int_t growId, Int_t& srowId) const;

  Bool_t GetPadInfo(const Double_t* local_point,
                    Int_t& sectorId,
                    Int_t& columnId,
                    Int_t& rowId) const;

  void GetPadInfo(const CbmTrdPoint* trdPoint,
                  Int_t& sectorId,
                  Int_t& columnId,
                  Int_t& rowId) const;

  // function for the pad position with the new address format for rectangular
  // modules
  void GetPadPosition(const Int_t sector,
                      const Int_t col,
                      const Int_t row,
                      TVector3& padPos,
                      TVector3& padPosErr) const;

  // get pad position from channel number in CbmTrdDigi implementation of the
  // pad position
  void GetPadPosition(const Int_t padAddress,
                      bool isCbmTrdDigiAddress,
                      TVector3& padPos,
                      TVector3& padPosErr) const;

  // standard implementation of the pad position
  void GetPadPosition(const Int_t padAddress,
                      TVector3& padPos,
                      TVector3& padPosErr) const;

  void GetPosition(
    // Int_t moduleAddress,
    Int_t sectorId,
    Int_t columnId,
    Int_t rowId,
    TVector3& padPos,
    TVector3& padSize) const;

  Int_t GetPadColumn(
    const Int_t channelNumber) const;  // calculate the pad column based on
                                       // the channeNumber as defined in the
                                       // CbmTrdDigi
  Int_t
  GetPadRow(const Int_t channelNumber) const;  // calculate the pad row based on
    // the channeNumber as defined in the
    // CbmTrdDigi

  Int_t GetSector(Int_t npady, Int_t& rowId) const;
  Double_t GetSizeX() const { return fSizeX; }
  Double_t GetSizeY() const { return fSizeY; }
  Double_t GetSizeZ() const { return fSizeZ; }
  Double_t GetX() const { return fX; }
  Double_t GetY() const { return fY; }
  Double_t GetZ() const { return fZ; }
  void Print(Option_t* opt = "") const;

  void ProjectPositionToNextAnodeWire(Double_t* local_point) const;
  void SetAnodeWireToPadPlaneDistance(Double_t d) {
    fAnodeWireToPadPlaneDistance = d;
  }
  void SetAnodeWireOffset(Double_t off) { fAnodeWireOffset = off; }
  void SetAnodeWireSpacing(Double_t dw) { fAnodeWireSpacing = dw; }
  void TransformHitError(TVector3& hitErr) const;

  void TransformToLocalPad(const Double_t* local_point,
                           Double_t& posX,
                           Double_t& posY) const;

private:
  CbmTrdParModDigi(const CbmTrdParModDigi& ref);
  const CbmTrdParModDigi& operator=(const CbmTrdParModDigi& ref);

  void GetModuleInformation(
    /*Int_t moduleAddress, */ const Double_t* local_point,
    Int_t& sectorId,
    Int_t& columnId,
    Int_t& rowId) const;

  void TransformToLocalCorner(const Double_t* local_point,
                              Double_t& posX,
                              Double_t& posY) const;

  void TransformToLocalSector(const Double_t* local_point,
                              Double_t& posX,
                              Double_t& posY) const;

  Int_t GetSector(const Double_t* local_point) const;

  Int_t fNofSectors;  ///< number sectors for this module
  Int_t
    fOrientation;  ///< angle between long pad axis and y-axis in steps of 90
                   ///deg [0..3]
  Double_t fAnodeWireOffset;   ///< Anode Wire Offset [cm]
  Double_t fAnodeWireSpacing;  ///< anode wire pitch [cm]
  Double_t
    fAnodeWireToPadPlaneDistance;  ///< Anode Wire to PadPlane Distance [cm]

  Double_t fX;            ///< center of module in global c.s. [cm]
  Double_t fY;            ///< center of module in global c.s. [cm]
  Double_t fZ;            ///< center of module in global c.s. [cm]
  Double_t fSizeX;        ///< module half size in x [cm]
  Double_t fSizeY;        ///< module half size in y [cm]
  Double_t fSizeZ;        ///< module half size in z [cm]
  TArrayD fSectorX;       ///< center of sectors local c.s. [cm]
  TArrayD fSectorY;       ///< center of sectors local c.s. [cm]
  TArrayD fSectorZ;       ///< center of sectors local c.s. [cm]
  TArrayD fSectorBeginX;  ///< begin of sector [cm]
  TArrayD fSectorBeginY;  ///< begin of sector [cm]
  TArrayD fSectorEndX;    ///< end of sector [cm]
  TArrayD fSectorEndY;    ///< end of sector [cm]
  TArrayD fSectorSizeX;   ///< sector size in x [cm]
  TArrayD fSectorSizeY;   ///< sector size in y [cm]

  TArrayD fPadSizeX;  ///< size of the readout pad in x [cm]
  TArrayD fPadSizeY;  ///< size of the readout pad in y [cm]

  ClassDef(CbmTrdParModDigi,
           1)  // Definition of read-out parameters for one TRD module
};

#endif
