#include "CbmDefs.h"

#include <stdexcept>         // for out_of_range
#include <string>            // for to_string

// operator ++ for ECbmModuleId for convenient usage in loops
// This operator is tuned for ECbmModuleID. It takes into account non
// continuous values for the enum. Since the detectorID which is stored
// in the generated output has only 4 bit the maximum number of detectors
// can be 16 (0-15). To avoid that the enum class has to be changed again
// the values 11-15 are reserved for future detectors.
// The ids of the passive modules are only relevant at run time so they can
// be shifted easily
// The opeartor takes care about the non continuous values for the enum
// When it reaches the last detector it automatically continuous with the
// first passive module
ECbmModuleId& operator ++ (ECbmModuleId& e)
{
    if (e == ECbmModuleId::kLastModule) {
        throw std::out_of_range("for ECbmModuleId& operator ++ (ECbmModuleId&)");
    }
    else if (e == ECbmModuleId::kT0) {
      e = ECbmModuleId::kMagnet;
    } else {
      e = ECbmModuleId(static_cast<std::underlying_type<ECbmModuleId>::type>(e) + 1);
    }
    return e;
}

// operator << for convenient output to std::ostream.
// Converts the enum value to a string which is put in the stream
std::ostream& operator<<(std::ostream& strm, const ECbmModuleId& modId) {
  strm << std::to_string(ToIntegralType(modId));
  return strm;
}

// operator << for convenient output to std::ostream.
// Converts the enum value to a string which is put in the stream
std::ostream& operator<<(std::ostream& strm, const ECbmDataType& dataType) {
  strm << std::to_string(ToIntegralType(dataType));
  return strm;
}
