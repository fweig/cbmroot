/** @file CbmTrdRawMessageSpadic.h
  * @copyright Copyright (C) 2020-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Dennis Spicker, Florian Uhlig [committer] **/

/**
 * @file CbmTrdRawMessageSpadic.h
 * @author Dennis Spicker
 * @date 2020-01-21
 **/

#ifndef CbmTrdRawMessageSpadic_H
#define CbmTrdRawMessageSpadic_H

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Double_t

#include <boost/serialization/access.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/vector.hpp>

#include <cstdint>  // for int16_t, uint8_t, uint16_t, uint64_t
#include <vector>   // for vector

namespace Spadic
{
  /** Spadic Message Types that can occur inside a Microslice.
	 *
	 *  A message is a 64bit word, its most significant bits (MSB) define the type.
	**/
  enum class MsMessageType : uint8_t
  {
    kEPO = 0x40,  ///< Epoch Marker.              MSB: 01.. ....   uTS-MSB (bits 61-32) in clockcycles.
    kSOM =
      0x20,  ///< Start of Message.          MSB: 001. ....   timestamp (bits 50-35) in clockcycles. Currently (Feb 2020) only 11 LSB in use.
    kRDA =
      0x80,  ///< Raw Data. Preceded by SOM. MSB: 1... ....   always 64bits, bits after last transmitted sample are filled with 0.
    kINF =
      0x10,  ///< Info  Message.             MSB: 0001 ....   The 20 LSBs of the 64bit word contain the 20 LSBs of a raw spadic InfoType frame e.g. BOM.
    kNUL = 0x00,  ///< Microslice End.        0x0000000000000000   Last Word in a Microslice is 64 zeros.
    kUNK = 0x01   ///< Unkown Word.
  };

  /** Hit Type of CbmTrdRawMessageSpadic, explains why a message was generated. */
  enum class TriggerType : uint8_t
  {
    kGlobal = 0,  ///< global trigger.
    kSelf   = 1,  ///< Self trigger.
    kNeigh  = 2,  ///< Neighbor trigger.
    kSandN  = 3   ///< Self and neighbor trigger at the same time.
  };

  /**
	 * Sub-Types of MsMessageType::kINF
	 *
	 * Bitmap of an info message: 0001 eeee ee.. .... .... .... .... .... .... .... .... iiii iiii iiii iiii iiii
	 *   e: E-Link ID, i: Info Message (see below)
	 **/
  enum class MsInfoType : uint8_t
  {
    kBOM,  ///< Buffer overflow count.  11nn nnnn nnnn nnnn cccc
    kMSB,  ///< Message build error.    010. .... .... .... cccc
    kBUF,  ///< Buffer full.            011b b... .... .... cccc
    kUNU,  ///< Unused request.         100. .... .... .... cccc
    kMIS   ///< Missing request.        101. .... .... .... ....
  };
}  // namespace Spadic


/**
 * @class CbmTrdRawMessageSpadic
 * @brief Base class for storing raw information which comes from the Spadic v2.2
 *        trough flib or from a tsa file.
 **/
class CbmTrdRawMessageSpadic {
private:
  friend class boost::serialization::access;

  std::uint8_t fChannelID;
  std::uint8_t fElinkID;
  std::uint8_t fCrobId;
  std::uint16_t fCriId;
  std::uint8_t fHitType;
  std::uint8_t fNrSamples;
  bool fMultiHit;
  std::uint64_t fFullTime;            /**< Fulltime in units of Clockcycles. */
  std::vector<std::int16_t> fSamples; /**< Holds up to 32 Samples from a Spadic Message. Valid values [-256,255] */

public:
  /** Default Constructor */
  CbmTrdRawMessageSpadic();

  /** Constructor **/
  CbmTrdRawMessageSpadic(std::uint8_t channelId, std::uint8_t elinkId, std::uint8_t crobId, std::uint16_t criId,
                         std::uint8_t hitType, std::uint8_t nrSamples, bool multiHi, std::uint64_t fullTime,
                         std::vector<std::int16_t> samples);

  /** Copy Constructor **/
  CbmTrdRawMessageSpadic(const CbmTrdRawMessageSpadic&);

  /** Destructor **/
  virtual ~CbmTrdRawMessageSpadic();

  /** Assignment Operator **/
  // CbmTrdRawMessageSpadic operator=(const CbmTrdRawMessageSpadic&);
  CbmTrdRawMessageSpadic& operator=(const CbmTrdRawMessageSpadic&) = default;

  // -----------------  Getters   -----------------------------

  std::uint8_t GetChannelId() const { return fChannelID; }
  std::uint8_t GetElinkId() const { return fElinkID; }
  std::uint8_t GetCrobId() const { return fCrobId; }
  std::uint16_t GetCriId() const { return fCriId; }
  std::uint8_t GetHitType() const { return fHitType; }
  std::uint8_t GetNrSamples() const { return fNrSamples; }
  bool GetMultiHit() const { return fMultiHit; }
  std::uint64_t GetFullTime() const { return fFullTime; }
  const std::vector<std::int16_t> GetSamples() const { return fSamples; }

  /** Returns the full time in nanoseconds */
  Double_t GetTime() const { return fFullTime * 62.5; }

  /** Set the full time in nanoseconds */
  void SetTime(Double_t setvalue) { fFullTime = (std::uint64_t)(setvalue / 62.5); }

  /** Returns the value of the sample with the highest value. */
  int16_t GetMaxAdc();

  // -----------------  Setters   -----------------------------

  /**
	 * Set the value of a specific Sample in fSamples vector.
	 * \param value Integer in the range [-256:255].
	 * \param pos Integer in the range [0:31].
	 **/
  void SetSample(std::int16_t value, std::uint8_t pos);

  /// Boost serialization function.
  template<class Archive>
  void serialize(Archive& ar, const unsigned int /*version*/)
  {
    ar& fElinkID;
    ar& fChannelID;
    ar& fCrobId;
    ar& fCriId;
    ar& fHitType;
    ar& fNrSamples;
    ar& fMultiHit;
    ar& fFullTime;
    ar& fSamples;
  }

  // Root Class Def Macro
  ClassDef(CbmTrdRawMessageSpadic, 2);
};

#endif
