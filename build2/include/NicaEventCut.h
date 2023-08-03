/*
 * NicaEventCut.h
 *
 *  Created on: 20-06-2013
 *      Author: Daniel Wielanek
 */

#ifndef NICAEVENTCUT_H_
#define NICAEVENTCUT_H_

#include "NicaCut.h"

class NicaEvent;

/** abstract class for event cuts **/
class NicaEventCut : public NicaCut {
 public:
  /** default constructor **/
  NicaEventCut(const Int_t i = 1);
  /** create package **/
  virtual NicaPackage* Report() const;
  /** return true if event pass cut, false otherwise, should be overwittern in
   * subclass
   * *@param event - event to check**/
  virtual Bool_t Pass(NicaEvent* event) = 0;
  /** default destructor **/
  virtual ~NicaEventCut();
  ClassDef(NicaEventCut, 1)
};

#endif /* NICAEVENTCUT_H_ */
