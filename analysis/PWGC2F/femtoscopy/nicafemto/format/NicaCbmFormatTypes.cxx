/** @file NicaCbmFormatTypes.cxx
  * @copyright Copyright (C) 2020-2021 Faculty of Physics, Warsaw University of Technology, Warsaw
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Daniel Wielanek [committer] **/

/*
 * NicaCbmFormatTypes.cxx
 *
 *  Created on: 21 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */

#include "NicaCbmFormatTypes.h"

#include "NicaDataFormatManager.h"
#include "NicaEvent.h"
#include "NicaStd.h"


namespace NicaCbmFormats
{

  ECbmFormatType GetFormatType(Int_t task_id, ENicaFormatDepth depth)
  {
    const NicaEvent* ev = NicaDataFormatManager::Instance()->GetFormat(task_id, depth);
    if (ev->InheritsFrom("NicaCbmEvent")) { return ECbmFormatType::kAnaTree; }
    else if (ev->InheritsFrom("CbmHbtEvent")) {
      return ECbmFormatType::kHbt;
    }
    return ECbmFormatType::kUnknown;
  }
}  // namespace NicaCbmFormats
