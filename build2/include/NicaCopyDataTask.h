/*
 * NicaCopyDataTask.h
 *
 *  Created on: 06-11-2014
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACOPYDATATASK_H_
#define NICACOPYDATATASK_H_

#include <FairTask.h>

class NicaEvent;

/**
 * task for copying data
 */
class NicaCopyDataTask : public FairTask {
  NicaEvent *fInputData;

 protected:
  /**
   * initialization
   * @return
   */
  virtual InitStatus Init();

 public:
  NicaCopyDataTask();
  /**
   * set format of data to copy
   * @param event event with type of data that should be copied
   */
  virtual void SetFormat(NicaEvent *event);
  /**
   * function called for each event
   * @param opt not used
   */
  virtual void Exec(Option_t *opt);
  virtual ~NicaCopyDataTask();
  ClassDef(NicaCopyDataTask, 1)
};

#endif /* NICACOPYDATATASK_H_ */
