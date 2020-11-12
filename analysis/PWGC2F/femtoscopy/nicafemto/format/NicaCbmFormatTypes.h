/*
 * NicaCbmFormatTypes.h
 *
 *  Created on: 21 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMFORMATTYPES_H_
#define NICACBMFORMATTYPES_H_

#include "NicaDataFormatManager.h"

namespace NicaCbmFormats {

  enum class ECbmFormatType { kCbmRoot, kAnaTree, kHbt, kUnknown };
  ECbmFormatType GetFormatType(Int_t task_id,
                               ENicaFormatDepth depht = ENicaFormatDepth::kAll);

}  // namespace NicaCbmFormats

#endif /* NICACBMFORMATTYPES_H_ */
