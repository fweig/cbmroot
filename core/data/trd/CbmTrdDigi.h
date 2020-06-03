#ifndef CBMTRDDIGI_H
#define CBMTRDDIGI_H

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Int_t, Float_t, Double_t, Bool_t, ULong64_t

#include <string>        // for string

#include "CbmDefs.h"     // for kTrd

class CbmTrdDigi
{
public:
  enum CbmTrdAsicType{
    kSPADIC = 0
    ,kFASP
    ,kNTypes 
  };
  enum CbmTrdTriggerType{
    kSelf = 0
    ,kNeighbor 
    ,kMulti 
    ,kTrg2 
    ,kNTrg 
  };
  enum CbmTrdDigiDef{
    kFlag1=0    //< 
    ,kFlag2     //< in case of FASP simulations define pileup
    ,kFlag3     //< in case of FASP simulations define masked
    ,kNflags
  };
  /**
   * \brief Default constructor.
   */
  CbmTrdDigi();
  /**
   * \brief Constructor for the FASP type.
   * \param[in] address  Unique channel address in the module.
   * \param[in] chargeT Charge for tilt pad parring.
   * \param[in] chargeR Charge for rectangle pad parring.
   * \param[in] time   Absolute time [ASIC clocks].
   */
  CbmTrdDigi(Int_t address, Float_t chargeT, Float_t chargeR, ULong64_t time);
  /**
   * \brief Constructor for the SPADIC type.
   * \param[in] address  Unique channel address in the module.
   * \param[in] charge Charge.
   * \param[in] time   Absolute time [ASIC clocks].
   * \param[in] triggerType SPADIC trigger type see CbmTrdTriggerType.
   * \param[in] errClass SPADIC signal error parametrization based on message type.
   */
  CbmTrdDigi(Int_t address, Float_t charge, ULong64_t time, Int_t triggerType, Int_t errClass/*nrSamples*/);
  
  /**
   * \brief Constructor for backward compatibillity.
   * Does not do anything.
   */
  CbmTrdDigi(Int_t /*address*/, Double_t /*fullTime*/, Int_t /*triggerType*/, Int_t /*infoType*/, Int_t /*stopType*/, Int_t /*nrSamples*/, Float_t* /*samples*/) 
  : fAddress(0)
  ,fCharge(0)
  ,fTime(0)
  {;}

  /** \brief Charge addition in case of pile-up (FASP simulation only)
   * \param[in] sd previous digi absorbed by current
   * \param[in] f scaling factor
   */
  void      AddCharge(CbmTrdDigi *sd, Double_t f);
  /** \brief Charge addition in case of pile-up (SPADIC simulation only)
   * \param[in] c extra charge to be added
   * \param[in] f scaling factor
   */
  void      AddCharge(Double_t c, Double_t f=1);
  /** \brief DAQ clock accessor for each ASIC*/
  static Float_t Clk(CbmTrdAsicType ty)     { return (ty==kNTypes?0:fgClk[ty]);}
  /** \brief Address getter for module in the format defined by CbmTrdAddress
   */
  Int_t     GetAddress() const              { return GetAddressModule();}
  /** \brief Getter read-out id.
   * \return index of row-column read-out unit in the module
   */
  Int_t     GetAddressChannel() const;
  /** \brief Getter module address in the experiment.
   * \return index of module in the experimental set-up. Should be dropped if data get organized module-wise
   */
  Int_t     GetAddressModule() const;
  /** \brief Charge getter for SPADIC*/
  Double_t  GetCharge()  const;
  /** \brief Charge getter for FASP
   * \param[out] tilt on returns contain the charge measured on tilted pads
   * \param[out] dt on returns contain the time difference wrt tilted pads [ASIC clocks]
   * \return charge measured on rectangular coupled pads
   */
  Double_t  GetCharge(Double_t &tilt, Int_t &dt)  const;
  /** \brief Charge error parametrisation. SPADIC specific see GetErrorClass()*/
  Double_t  GetChargeError()  const;
  /** \brief Channel status. SPADIC specific see LUT*/
  Int_t     GetErrorClass() const           { return (fAddress>>fgkErrOffset)&0x7;}

  /** System ID (static)
  ** @return System identifier (ECbmModuleId)
  **/
  static ECbmModuleId GetSystem() { return ECbmModuleId::kTrd; }


  /** @brief Class name (static)
   ** @return CbmTrdDigi
   **/
  static const char* GetClassName() { return "CbmTrdDigi"; }



  /** \brief Getter for physical time [ns]. Accounts for clock representation of each ASIC. In SPADIC case physical time is already stored in fTime. */
  Double_t GetTime() const {return (GetType()==kFASP)?fTime*fgClk[GetType()]:fTime;}
  /** \brief Getter for global DAQ time [clk]. Differs for each ASIC. In FASP case DAQ time is already stored in fTime.*/
  ULong64_t GetTimeDAQ() const { return (GetType()==kFASP)?fTime:fTime/fgClk[GetType()];}
  /** \brief Channel trigger type. SPADIC specific see CbmTrdTriggerType*/
  Int_t     GetTriggerType() const          { return (fAddress>>fgkTrgOffset)&0x3;}
  /** \brief Channel FEE SPADIC/FASP according to CbmTrdAsicType*/
  CbmTrdAsicType  GetType() const     { return ((fAddress>>fgkTypOffset)&0x1)?kFASP:kSPADIC;} 

  /** \brief Query digi mask (FASP only)*/
  Bool_t    IsMasked() const {return (GetType()==kFASP)&&IsFlagged(kFlag3);}
  /** \brief Query digi pile-up (FASP only)*/
  Bool_t    IsPileUp() const {return (GetType()==kFASP)&&IsFlagged(kFlag2);}
  /** \brief Query flag status (generic)*/
  Bool_t    IsFlagged(const Int_t iflag)  const;
  Int_t     Layer() const    { return (fAddress>>fgkLyOffset)&0xf;}
  Int_t     Module() const   { return (fAddress>>fgkModOffset)&0x7f;}

  /** \brief Module address setter for digi
   * \param[in] a module address as it is defined in CbmTrdAddress
   */
  void      SetAddress(const Int_t a);
  /** \brief Alias for SetAddress() */
  void      SetAddressModule(const Int_t a) { SetAddress(a);}
  
  void      SetAsic(CbmTrdAsicType ty=kSPADIC);
  /** \brief Charge setter for SPADIC ASIC
   * \param[in] c charge on read-out pad
   */
  void      SetCharge(Float_t c);
  /** \brief Charge setter for FASP ASIC
   * \param[in] cT charge on tilt paired
   * \param[in] cR charge on rectangular paired
   * \param[in] dt time difference between T and R channel
   */
  void      SetCharge(Float_t cT, Float_t cR, Int_t dt=0);
  /** \brief Generic flag status setter*/
  void      SetFlag(const Int_t iflag, Bool_t set=kTRUE);
  /** \brief Set digi mask (FASP only)*/
  void      SetMasked(Bool_t set=kTRUE)     { if(GetType()==kFASP) SetFlag(kFlag3, set); }
  /** \brief Set digi pile-up (FASP only)*/
  void      SetPileUp(Bool_t set=kTRUE)     { if(GetType()==kFASP) SetFlag(kFlag2, set); }
  /** \brief Set global digi time (ns)*/
  void      SetTime(Double_t t);
  /** \brief Set global digi time (clk)*/
  void      SetTimeDAQ(ULong64_t t)         { fTime=t;}
  /** \brief Set time offset of rectangular to tilt pads for FASP (clk)*/
  void      SetTimeOffset(Char_t t);         
  /** \brief Set digi trigger type (SPADIC only)*/
  void      SetTriggerType(const Int_t ttype);
  /** \brief Set digi error class (SPADIC only)*/
  void      SetErrorClass(const Int_t n)    { fAddress&= ~(0x7<<fgkErrOffset);
                                              fAddress|=((n&0x7)<<fgkErrOffset);}
  /** \brief String representation of a TRD digi. Account for digi type and specific information.*/
  std::string ToString() const;
  
  // TEMPORARY ADDED WITHOUT PROVIDING FUNCTIONALITY ! TODO should be removed 
  // keep backward compatible compilation of CbmTrdSPADIC
  void SetStopType(Int_t /*stopType*/)  {;}
  Int_t GetStopType() {return 0;}
  void SetPulseShape(Float_t /*pulse*/[45]) {;}
  // keep backward compatible compilation of fles/reader/tasks [/tools]
  Double_t GetChargeTR() const  {return 0.;}
  void SetInfoType(Int_t /*infoType*/)  {;}
  Int_t GetNrSamples() { return 0;}
  Float_t* GetSamples() {return nullptr;}

protected:
  void      SetChannel(const Int_t a){ fAddress&= ~(0xfff<<fgkRoOffset); 
                                       fAddress|=(a&0xfff)<<fgkRoOffset;}
  void      SetLayer(const Int_t a)  { fAddress&= ~(0xf<<fgkLyOffset); 
                                       fAddress|=((a&0xf)<<fgkLyOffset);}
  void      SetModule(const Int_t a) { fAddress&= ~(0x7f<<fgkModOffset); 
                                       fAddress|=((a&0x7f)<<fgkModOffset);}

  UInt_t    fAddress;     //< pad address and extra information
  UInt_t    fCharge;      //< measured charge. For SPADIC is Int_t(charge*1eN) where N is the precission while 
                          //< for FASP it contains the R and T charges each on 12bits and the time difference between R and T pads in CLK (8 bits).
  ULong64_t fTime;        //< global time of the digi: For SPADIC in ns, for FASP in ASIC clock
  
  static Double_t  fgClk[kNTypes]; //< clock length in ns for acquisition
  static Float_t  fgPrecission[kNTypes];   //< no. of digits stored for ASIC

private:    
  static const Int_t fgkRoOffset  =  0;
  static const Int_t fgkModOffset =  12;
  static const Int_t fgkLyOffset  =  19;
  static const Int_t fgkErrOffset =  23;
  static const Int_t fgkFlgOffset =  26;
  static const Int_t fgkTrgOffset =  29;
  static const Int_t fgkTypOffset =  31;
  
  ClassDefNV(CbmTrdDigi, 2);   // Production ready TRD digit
};

#endif
