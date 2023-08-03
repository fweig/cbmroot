/*
 * NicaMiniEventTask.h
 *
 *  Created on: 19 lut 2017
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAMINIEVENTTASK_H_
#define NICAMINIEVENTTASK_H_

#include "NicaTrackAna.h"

/**
 * creates "minievents from given format and thus - makes possible reduction of
 * memory requirements removes unusued tracks - however don;t modify oryginal
 * traks
 */
class NicaMiniEventTask : public NicaTrackAna {
  Int_t fArraySize;
  Int_t *fArray;  //[fArray_Size]
  Int_t *fMap;    //[fArray_Size]
  void Sum();

 protected:
  virtual void Check();
  virtual InitStatus Init();

 public:
  NicaMiniEventTask();
  virtual void Exec(Option_t *opt);
  virtual ~NicaMiniEventTask();
  ClassDef(NicaMiniEventTask, 1)
};

#endif /* NICAMINIEVENTTASK_H_ */
