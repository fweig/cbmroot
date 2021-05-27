/*
 * CbmDetectorID.h
 *
 *  Created on: 3 gru 2018
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef CBMROOT_NICA_CBM_HELPERS_CBMDETECTORID_H_
#define CBMROOT_NICA_CBM_HELPERS_CBMDETECTORID_H_

#include "NicaDataFormat.h"

namespace NicaCbmDetectorID
{
  const UInt_t kTOF  = NicaDetectorID::kTOF;
  const UInt_t kSTS  = NicaDetectorID::kSTS;
  const UInt_t kMVD  = 29641;
  const UInt_t kRICH = 1283489;
  const UInt_t kTRD  = 38569;
  const UInt_t kECAL = 669117;
  const UInt_t kPSD  = 33421;
  const UInt_t kMUCH = 1065761;
};  // namespace NicaCbmDetectorID

enum class NicaCbmSetup
{
  kSis100Hadron   = 0,
  kSis100Electron = 1,
  kSis100Muon     = 2,
  kSis100Mini     = 3
};


#endif /* CBMROOT_NICA_CBM_HELPERS_CBMDETECTORID_H_ */
