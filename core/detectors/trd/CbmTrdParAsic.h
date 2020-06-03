#ifndef CBMTRDPARASIC_H
#define CBMTRDPARASIC_H

#include <Rtypes.h>        // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>    // for Int_t, Double_t, Option_t

#include "CbmTrdParMod.h"  // for CbmTrdParMod

#include <stdint.h>        // for uint64_t
#include <vector>          // for vector

class FairParamList;

/** \brief Definition of ASIC parameters **/
class CbmTrdParAsic : public CbmTrdParMod
{
public:
  CbmTrdParAsic(Int_t address=0, Int_t FebGrouping=-1, Double_t x=0, Double_t y=0, Double_t z=0, std::uint64_t compId = 0);
  virtual ~CbmTrdParAsic() {;}
  
  /** \brief Enum for decodation of spadic componentId (Hardware to software mapping)
   * Since the length of the CriIds is currently unknown, this is defined as ULong to not loose the needed precision **/
  enum ECbmTrdComponentIdDecoding : ULong_t
  {
    kElinkIdPosition    =      1
    , kCrobNrPosition   =    100
    , kCrobIdPosition   =   1000
    , kCriIdPosition    = 100000
  };

    /** Accessors **/
  virtual Double_t  GetSizeX() const = 0;
  virtual Double_t  GetSizeY() const = 0;
  virtual Double_t  GetX() const                          { return fX; }
  virtual Double_t  GetY() const                          { return fY; }
  virtual Double_t  GetZ() const                          { return fZ; }

  virtual Int_t         GetAddress() const                    { return fAddress; }
  virtual std::uint64_t GetComponentId() const                { return fComponentId; }
  virtual Int_t         GetNchannels() const = 0;
  virtual Int_t         GetFebGrouping() const                { return fFebGrouping; }
  virtual Int_t         GetChannelAddress(Int_t ich) const    { return ((ich<0||ich>=GetNchannels())?0:fChannelAddresses[ich]); }
  virtual std::vector<Int_t> GetChannelAddresses() const  { return fChannelAddresses; }
  virtual void          LoadParams(FairParamList*) {;}
  virtual void          Print(Option_t *opt="") const;
  /** \brief Query ASIC for specific pad address
   * \param[in] ch pad address within module as provided by CbmTrdModuleAbstract::GetPadAddress()
   * \return channel index within ASIC
   */
  virtual Int_t     QueryChannel(Int_t ch) const;
  virtual void      SetChannelAddress( Int_t address );
  virtual void      SetChannelAddresses( std::vector<Int_t> addresses );
  virtual void      SetFebGrouping(Int_t feb)                             { fFebGrouping=feb;}
  virtual void      SetPosition(Double_t x=0, Double_t y=0, Double_t z=0) {fX=x; fY=y; fZ=z;}
  virtual void      SetComponentId(std::uint64_t id)                      { fComponentId = id; }

protected:
  Int_t     fAddress;                   ///< unique ASIC ID
  Double_t  fX;                         ///< center of asic in global c.s. [cm]
  Double_t  fY;                         ///< center of asic in global c.s. [cm]
  Double_t  fZ;                         ///< center of asic in global c.s. [cm]
  Int_t     fFebGrouping;               ///< no of ASIC in ROB
  std::uint64_t fComponentId;           ///< For the digit decoding see ECbmTrdComponentIdDecoding. nTh cRob on the module counted from top to bottom a long the sensitive side. This Id is needed to connect the microslice to a given channel, has to be set "by hand", i.e. is not given in the geometry macros. ComponentIdMaps for the Spadic are stored in CbmTrdHardwareSetupR. A macro to write those Ids to the parameter files can be found at https://git.cbm.gsi.de/trd/macros/mcbm2020/blob/master/writeSpadicHwAddresses.C
  std::vector<Int_t> fChannelAddresses; ///< addresses of individual output channels

  ClassDef(CbmTrdParAsic, 1)  // Definition of common ASIC parameters
};

#endif
