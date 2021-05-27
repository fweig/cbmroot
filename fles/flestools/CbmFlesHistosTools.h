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
