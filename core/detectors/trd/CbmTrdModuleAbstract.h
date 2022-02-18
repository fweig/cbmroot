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
  enum eCbmTrdModuleDef
  {
    kTrd2d = 0  ///< toggle pad-plane type of the chamber
      ,
    kFasp = 1  ///< toggle FEE type for the module
  };
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
  /** \brief Inquire the FEE read-out type of the module
   * \return false for SPADIC and true for FASP
   */
  bool HasFaspFEE() const { return TESTBIT(fModConfig, eCbmTrdModuleDef::kFasp); }
  /** \brief Inquire the pad-plane type of the chamber
   * \return false for TRD-1D and true for TRD-2D
   */
  bool Has2dPadPlane() const { return TESTBIT(fModConfig, eCbmTrdModuleDef::kTrd2d); }

  virtual void LocalToMaster(Double_t in[3], Double_t out[3]);

  virtual void SetAsicPar(CbmTrdParSetAsic* p = nullptr) { fAsicPar = p; }
  virtual void SetChmbPar(const CbmTrdParModGas* p) { fChmbPar = p; }
  virtual void SetDigiPar(const CbmTrdParModDigi* p) { fDigiPar = p; }
  virtual void SetGainPar(const CbmTrdParModGain* p) { fGainPar = p; }
  virtual void SetGeoPar(const CbmTrdParModGeo* p) { fGeoPar = p; }

  /** \brief Define the read-out FEE type of the module
   * \param[in] set true for FASP and false [default] for SPADIC
   */
  void SetFEE(bool set = true)
  {
    set ? SETBIT(fModConfig, eCbmTrdModuleDef::kFasp) : CLRBIT(fModConfig, eCbmTrdModuleDef::kFasp);
  }
  /** \brief Define the pad-plane type of the chamber
   * \param[in] set true for TRD-2D and false [default] for TRD-1D
   */
  void SetPadPlane(bool set = true)
  {
    set ? SETBIT(fModConfig, eCbmTrdModuleDef::kTrd2d) : CLRBIT(fModConfig, eCbmTrdModuleDef::kTrd2d);
  }

protected:
  /** 8 bits bit map for module configuration
   * [0] - chamber's pad-plane type; 0 rectangular pads, 1 triangular pads, \see SetRO
   * [1] - chamber's FEE type; 0 SPADIC, 1 FASP, \see SetFEE
   * [2] -  
   * [3] -  
   * [4] -  
   * [5] -  
   * [6] -  
   * [7] -  
   */
  uint8_t fModConfig;
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

  ClassDef(CbmTrdModuleAbstract, 2)
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
