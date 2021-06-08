/* Copyright (C) 2018-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Alexandru Bercuci */

#ifndef CBMTRDMODULEABSTRACT_H
#define CBMTRDMODULEABSTRACT_H

#include "CbmTrdParModDigi.h"  // for CbmTrdParModDigi
#include "CbmTrdParModGeo.h"   // for CbmTrdParModGeo
#include "CbmTrdParSetAsic.h"  // for CbmTrdParSetAsic

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t, Char_t, UChar_t, USho...
#include <TNamed.h>      // for TNamed

class CbmTrdParModGain;
class CbmTrdParModGas;

/**
  * \brief Abstract class for TRD module
  **/
class CbmTrdModuleAbstract : public TNamed {
public:
  /** \brief Default constructor.*/
  CbmTrdModuleAbstract();
  /** \brief Constructor with placement */
  CbmTrdModuleAbstract(Int_t mod, Int_t ly, Int_t rot);

  virtual ~CbmTrdModuleAbstract();

  /** \brief Shortcut getter size x/2 [cm] */
  virtual Double_t GetDx() const { return fGeoPar ? fGeoPar->GetDX() : 0.; }
  /** \brief Shortcut getter size y/2 [cm] */
  virtual Double_t GetDy() const { return fGeoPar ? fGeoPar->GetDY() : 0.; }
  /** \brief Shortcut getter size z/2 [cm] */
  virtual Double_t GetDz() const { return fGeoPar ? fGeoPar->GetDZ() : 0.; }
  /** \brief Shortcut getter ASICs number module wise */
  virtual Int_t GetNasics() const { return fAsicPar ? fAsicPar->GetNofAsics() : 0; }
  /** \brief Shortcut getter column size */
  virtual Int_t GetNcols() const { return fDigiPar ? fDigiPar->GetNofColumns() : 0; }
  /** \brief Shortcut getter row wise */
  virtual Int_t GetNrows() const { return fDigiPar ? fDigiPar->GetNofRows() : 0; }
  /** \brief Addressing ASIC on module based on id
   * \param[in] id module wise ASIC identifier
   * \return ASIC address within experiment
   */
  virtual Int_t GetAsicAddress(Int_t id) const { return fModAddress * 1000 + id; }
  /** \brief Addressing read-out pads on module based on (row,col)
   * \param[in] r global row id (not sector wise)
   * \param[in] c global column id
   * \return pad address within module
   */
  virtual Int_t GetPadAddress(Int_t r, Int_t c) const { return r * GetNcols() + c; }
  /** \brief Addressing read-out pads based on module address
   * \param[in] address pad address within module
   * \param[in] c on return global column id
   * \return global row address within module
   */
  virtual inline Int_t GetPadRowCol(Int_t address, Int_t& c) const;
  virtual const Char_t* GetPath() const { return fGeoPar ? fGeoPar->GetTitle() : ""; }
  virtual void LocalToMaster(Double_t in[3], Double_t out[3]);
  virtual void SetAsicPar(CbmTrdParSetAsic* p = nullptr) { fAsicPar = p; }
  virtual void SetChmbPar(const CbmTrdParModGas* p) { fChmbPar = p; }
  virtual void SetDigiPar(const CbmTrdParModDigi* p) { fDigiPar = p; }
  virtual void SetGainPar(const CbmTrdParModGain* p) { fGainPar = p; }
  virtual void SetGeoPar(const CbmTrdParModGeo* p) { fGeoPar = p; }

protected:
  // geometrical definitions imported from CbmTrdGeoHandler
  UShort_t fModAddress;  ///< unique identifier for current module
  Char_t fLayerId;       ///< layer identifier
  UChar_t fRotation;     ///< rotation angle for current module

  // calibration objects
  const CbmTrdParModDigi* fDigiPar;  ///< read-out description of module
  const CbmTrdParModGas* fChmbPar;   ///< detection description (HV, drift) of module
  CbmTrdParSetAsic* fAsicPar;        ///< the set of ASIC operating on the module (owned)
  const CbmTrdParModGain* fGainPar;  ///< Analog to digital conversion for module
  const CbmTrdParModGeo* fGeoPar;    ///< link to gGeometry for module

private:
  CbmTrdModuleAbstract(const CbmTrdModuleAbstract& ref);
  const CbmTrdModuleAbstract& operator=(const CbmTrdModuleAbstract& ref);

  ClassDef(CbmTrdModuleAbstract, 1)
};

//_______________________________________________________________________________
Int_t CbmTrdModuleAbstract::GetPadRowCol(Int_t address, Int_t& c) const
{
  if (!fDigiPar) {
    c = -1;
    return -1;
  }
  c = address % GetNcols();
  return address / GetNcols();
}

#endif
