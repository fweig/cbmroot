/* Copyright (C) 2009-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

#ifndef CBMTRDDIGI_H
#define CBMTRDDIGI_H

#include "CbmDefs.h"  // for kTrd

#ifndef NO_ROOT
#include <Rtypes.h>  // for CLRBIT, SETBIT, TESTBIT, ClassDef
#endif

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>

#include <cstdint>
#include <string>  // for string

class CbmTrdDigi {
public:
  enum class eCbmTrdAsicType : size_t
  {
    kSPADIC = 0,
    kFASP,
    kNTypes
  };
  enum class eTriggerType : int32_t
  {
    kBeginTriggerTypes = 0,
    kSelf              = kBeginTriggerTypes,
    kNeighbor,
    kMulti,
    kTrg2,
    kNTrg
  };
  enum CbmTrdDigiDef
  {
    kFlag1 = 0  //<
      ,
    kFlag2  //< in case of FASP simulations define pileup
      ,
    kFlag3  //< in case of FASP simulations define masked
      ,
    kNflags
  };
  /**
   * \brief Default constructor.
   */
  CbmTrdDigi();
  /**
   * \brief Constructor for the FASP type.
   * \param[in] padChNr Unique channel address in the module.
   * \param[in] chargeT Charge for tilt pad parring.
   * \param[in] chargeR Charge for rectangle pad parring.
   * \param[in] time   Absolute time [ASIC clocks].
   */
  CbmTrdDigi(int32_t padChNr, float chargeT, float chargeR, uint64_t time);
  /**
   * \brief Constructor for the SPADIC type.
   * \param[in] padChNr Unique channel address in the module.
   * \param[in] uniqueModuleId Unique Id of the module.
   * \param[in] charge Charge.
   * \param[in] time   Absolute time [ns].
   * \param[in] triggerType SPADIC trigger type see CbmTrdTriggerType.
   * \param[in] errClass SPADIC signal error parametrization based on message type.
   */
  CbmTrdDigi(int32_t padChNr, int32_t uniqueModuleId, float charge, uint64_t time, eTriggerType triggerType,
             int32_t errClass /*nrSamples*/);

  /**
   * \brief Constructor for backward compatibillity.
   * Does not do anything.
   */
  CbmTrdDigi(int32_t /*address*/, double /*fullTime*/, int32_t /*triggerType*/, int32_t /*infoType*/,
             int32_t /*stopType*/, int32_t /*nrSamples*/, float* /*samples*/)
    : fInfo(0)
    , fCharge(0)
    , fTime(0)
  {
    ;
  }

  /**
   * @brief Copy Construct a new Cbm Trd Digi
   *
   */
  CbmTrdDigi(const CbmTrdDigi&);

  /**
   * @brief Assignment operator
   *
   * @return CbmTrdDigi&
  */
  CbmTrdDigi& operator=(const CbmTrdDigi&) = default;


  /** \brief Charge addition in case of pile-up (FASP simulation only)
   * \param[in] sd previous digi absorbed by current
   * \param[in] f scaling factor
   */
  void AddCharge(CbmTrdDigi* sd, double f);
  /** \brief Charge addition in case of pile-up (SPADIC simulation only)
   * \param[in] c extra charge to be added
   * \param[in] f scaling factor
   */
  void AddCharge(double c, double f = 1);
  /** \brief DAQ clock accessor for each ASIC*/
  static float Clk(eCbmTrdAsicType ty) { return fgClk[static_cast<size_t>(ty)]; }
  /** \brief Address getter for module in the format defined by CbmTrdDigi (format of CbmTrdAddress can be accessed via CbmTrdParModDigi)
   */
  int32_t GetAddress() const { return (fInfo >> fgkRoOffset) & 0x7fffff; }
  /** \brief Getter read-out id.
   * \return index of row-column read-out unit in the module
   */
  int32_t GetAddressChannel() const;
  /** \brief Getter module address in the experiment.
   * \return index of module in the experimental set-up. Should be dropped if data get organized module-wise
   */
  int32_t GetAddressModule() const;
  /** \brief Common purpose charge getter
   * \return normal charge for SPADIC and rect paired charge for FASP */
  double GetCharge() const;
  /** \brief Charge getter for FASP
   * \param[out] tilt on returns contain the charge measured on tilted pads
   * \param[out] dt on returns contain the time difference wrt tilted pads [ASIC clocks]
   * \return charge measured on rectangular coupled pads
   */
  double GetCharge(double& tilt, int32_t& dt) const;
  /** \brief Charge error parametrisation. SPADIC specific see GetErrorClass()*/
  double GetChargeError() const;
  /** \brief Channel status. SPADIC specific see LUT*/
  int32_t GetErrorClass() const { return (fInfo >> fgkErrOffset) & 0x7; }

  /** System ID (static)
  ** @return System identifier (ECbmModuleId)
  **/
  static ECbmModuleId GetSystem() { return ECbmModuleId::kTrd; }


  /** @brief Class name (static)
   ** @return CbmTrdDigi
   **/
  static const char* GetClassName() { return "CbmTrdDigi"; }

  /** @brief Get the desired name of the branch for this obj in the cbm output tree  (static)
   ** @return TrdDigi
   **/
  static const char* GetBranchName() { return "TrdDigi"; }


  /** \brief Getter for physical time [ns]. Accounts for clock representation of each ASIC. In SPADIC case physical time is already stored in fTime. */
  double GetTime() const
  {
    return (GetType() == eCbmTrdAsicType::kFASP) ? fTime * fgClk[static_cast<size_t>(GetType())] : fTime;
  }
  /** \brief Getter for global DAQ time [clk]. Differs for each ASIC. In FASP case DAQ time is already stored in fTime.*/
  uint64_t GetTimeDAQ() const
  {
    return (GetType() == eCbmTrdAsicType::kFASP) ? fTime : fTime / fgClk[static_cast<size_t>(GetType())];
  }
  /** \brief Channel trigger type. SPADIC specific see CbmTrdTriggerType*/
  int32_t GetTriggerType() const { return (fInfo >> fgkTrgOffset) & 0x3; }

  /**
   * @brief Get the trigger combination, i.e. St or Nt and is multihit or not (true/false)
   * @param[in] int32_t trigger value of a digi as stored in fInfo
   * @return std::pair<eTriggerType, bool> <St or Nt, multihit or not>
  */
  static std::pair<eTriggerType, bool> GetTriggerPair(const int32_t triggerValue);

  /** \brief Channel FEE SPADIC/FASP according to CbmTrdAsicType*/
  eCbmTrdAsicType GetType() const
  {
    return ((fInfo >> fgkTypOffset) & 0x1) ? eCbmTrdAsicType::kFASP : eCbmTrdAsicType::kSPADIC;
  }

  /** \brief Query digi mask (FASP only)*/
  bool IsMasked() const { return (GetType() == eCbmTrdAsicType::kFASP) && IsFlagged(kFlag3); }
  /** \brief Query digi pile-up (FASP only)*/
  bool IsPileUp() const { return (GetType() == eCbmTrdAsicType::kFASP) && IsFlagged(kFlag2); }
  /** \brief Query flag status (generic)*/
  bool IsFlagged(const int32_t iflag) const;
  int32_t Layer() const { return (fInfo >> fgkLyOffset) & 0xf; }
  int32_t Module() const { return (fInfo >> fgkModOffset) & 0x7f; }

  /** \brief Module address setter for digi
   * \param[in] a module address as it is defined in CbmTrdAddress
   */
  void SetAddress(const int32_t a);
  /** \brief Alias for SetAddress() */
  void SetAddressModule(const int32_t a) { SetAddress(a); }

  void SetAsic(eCbmTrdAsicType ty = eCbmTrdAsicType::kSPADIC);
  /** \brief Charge setter for SPADIC ASIC
   * \param[in] c charge on read-out pad
   */
  void SetCharge(float c);
  /** \brief Charge setter for FASP ASIC
   * \param[in] cT charge on tilt paired
   * \param[in] cR charge on rectangular paired
   * \param[in] dt time difference between T and R channel
   */
  void SetCharge(float cT, float cR, int32_t dt = 0);
  /** \brief Generic flag status setter*/
  void SetFlag(const int32_t iflag, bool set = true);
  /** \brief Set digi mask (FASP only)*/
  void SetMasked(bool set = true)
  {
    if (GetType() == eCbmTrdAsicType::kFASP) SetFlag(kFlag3, set);
  }
  /** \brief Set digi pile-up (FASP only)*/
  void SetPileUp(bool set = true)
  {
    if (GetType() == eCbmTrdAsicType::kFASP) SetFlag(kFlag2, set);
  }
  /** \brief Set global digi time (ns)*/
  void SetTime(double t);
  /** \brief Set global digi time (clk)*/
  void SetTimeDAQ(uint64_t t) { fTime = t; }
  /** \brief Set time offset of rectangular to tilt pads for FASP (clk)*/
  void SetTimeOffset(int8_t t);
  /** \brief Set digi trigger type */
  void SetTriggerType(const eTriggerType triggerType);
  /** \brief Set digi trigger type */
  void SetTriggerType(const int32_t triggerValue);
  /** \brief Set digi error class (SPADIC only)*/
  void SetErrorClass(const int32_t n)
  {
    fInfo &= ~(0x7 << fgkErrOffset);
    fInfo |= ((n & 0x7) << fgkErrOffset);
  }
  /** \brief String representation of a TRD digi. Account for digi type and specific information.*/
  std::string ToString() const;

  // TEMPORARY ADDED WITHOUT PROVIDING FUNCTIONALITY ! TODO should be removed
  // keep backward compatible compilation of CbmTrdSPADIC
  void SetStopType(int32_t /*stopType*/) { ; }
  int32_t GetStopType() { return 0; }
  void SetPulseShape(float /*pulse*/[45]) { ; }
  // keep backward compatible compilation of fles/reader/tasks [/tools]
  double GetChargeTR() const { return 0.; }
  void SetInfoType(int32_t /*infoType*/) { ; }
  int32_t GetNrSamples() { return 0; }
  float* GetSamples() { return nullptr; }

protected:
  void SetChannel(const int32_t a)
  {
    fInfo &= ~(0xfff << fgkRoOffset);
    fInfo |= (a & 0xfff) << fgkRoOffset;
  }
  void SetLayer(const int32_t a)
  {
    fInfo &= ~(0xf << fgkLyOffset);
    fInfo |= ((a & 0xf) << fgkLyOffset);
  }
  void SetModule(const int32_t a)
  {
    fInfo &= ~(0x7f << fgkModOffset);
    fInfo |= ((a & 0x7f) << fgkModOffset);
  }

  uint32_t fInfo   = 0;  //< pad address and extra information
  uint32_t fCharge = 0;  //< measured charge. For SPADIC is int32_t(charge*1eN) where N is the precission while
  //< for FASP it contains the R and T charges each on 12bits and the time difference between R and T pads in CLK (8 bits).
  uint64_t fTime = 0;  //< global time of the digi: For SPADIC in ns, for FASP in ASIC clock

  /**
   * @brief clock length in ns for acquisition
   *
   */
  static const double fgClk[static_cast<size_t>(eCbmTrdAsicType::kNTypes) + 1];

  /**
  * @brief Nr. of digits stored for ASIC
  *
  */
  static const float fgPrecission[static_cast<size_t>(eCbmTrdAsicType::kNTypes) + 1];


private:
  static const int32_t fgkRoOffset  = 0;
  static const int32_t fgkModOffset = 12;
  static const int32_t fgkLyOffset  = 19;
  static const int32_t fgkErrOffset = 23;
  static const int32_t fgkFlgOffset = 26;
  static const int32_t fgkTrgOffset = 29;
  static const int32_t fgkTypOffset = 31;

  /// BOOST serialization interface
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fInfo;
    ar& fCharge;
    ar& fTime;
  }

#ifndef NO_ROOT
  ClassDefNV(CbmTrdDigi, 4);  // Production ready TRD digit
#endif
};

#endif
