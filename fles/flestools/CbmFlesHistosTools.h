/** @file CbmFlesHistosTools.h
  * @copyright Copyright (C) 2020 Facility for Antiproton and Ion Research in Europe, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Pierre-Alain Loizeau [committer], Florian Uhlig **/

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmFlesHistosTools                           -----
// -----               Created 09.04.2020 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmFlesHistosTools_H
#define CbmFlesHistosTools_H

#include <cstdint>
#include <vector>

/**********************************************************************/
std::vector<double> GenerateLogBinArray(uint32_t uNbDecadesLog, uint32_t uNbStepsDecade, uint32_t uNbSubStepsInStep,
                                        uint32_t& uNbBinsLog);
/**********************************************************************/

#endif  // CbmFlesHistosTools_H
