/*
 * NicaSplitedTrackToStatusTask.h
 *
 *  Created on: 27 lis 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICASPLITEDTRACKTOSTATUSTASK_H_
#define NICASPLITEDTRACKTOSTATUSTASK_H_

#include "NicaArray.h"
#include "NicaEventAna.h"

/**
 * set track status based on reconstruction
 * -1 - MC track not registered, reconstructed track not matched with MC
 * 0 - track reconstructed one
 * 1 - track reconstructed more than once (or MC track assigned to other track)
 */

class NicaSplitedTrackToStatusTask : public NicaEventAna {
  NicaArray_1<Int_t> fArrayReco;
  NicaArray_1<Int_t> fArraySim;

protected:
  virtual InitStatus Init();
  virtual void ProcessEvent();

public:
  NicaSplitedTrackToStatusTask();
  virtual ~NicaSplitedTrackToStatusTask();
  NicaSplitedTrackToStatusTask(const NicaSplitedTrackToStatusTask& other);
  NicaSplitedTrackToStatusTask& operator=(const NicaSplitedTrackToStatusTask& other);
  ClassDef(NicaSplitedTrackToStatusTask, 1)
};


#endif /* NICASPLITEDTRACKTOSTATUSTASK_H_ */
