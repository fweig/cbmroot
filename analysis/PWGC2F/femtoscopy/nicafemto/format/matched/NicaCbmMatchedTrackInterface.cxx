/*
 * NicaCbmMatchedTrackInterface.cxx
 *
 *  Created on: 12 lip 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaCbmMatchedTrackInterface.h"

NicaCbmMatchedTrackInterface::NicaCbmMatchedTrackInterface()
  : fStsLink(NULL)
  , fTofLink(NULL)
  , fTrdLink(NULL)
  , fRichLink(NULL)
  , fMuchLink(NULL) {}

NicaCbmMatchedTrackInterface::~NicaCbmMatchedTrackInterface() {
  // TODO Auto-generated destructor stub
}
