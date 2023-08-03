/*
 * NicaMCEventInterface.h
 *
 *  Created on: 3 sie 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAMCEVENTINTERFACE_H_
#define NICAMCEVENTINTERFACE_H_

#include "NicaEventInterface.h"

class NicaMCEventInterface : public NicaEventInterface {
 public:
  NicaMCEventInterface();
  /** GETTER SETTER SECTION **/
  /**
   * set impact parameter
   * @param b
   */
  virtual void SetB(Double_t /*b*/){};
  /**
   *
   * @return impact parameter
   */
  virtual Double_t GetB() const { return 0.; };
  virtual ~NicaMCEventInterface();
};

#endif /* NICAMCEVENTINTERFACE_H_ */
