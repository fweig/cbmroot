/*
 * NicaCbmFirstTask.h
 *
 *  Created on: 3 wrz 2019
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICACBMFIRSTTASK_H_
#define NICACBMFIRSTTASK_H_
#include "FairTask.h"
#include "CbmHelix.h"


class NicaCbmFirstTask :public  FairTask{
public:
	NicaCbmFirstTask();
	InitStatus Init();
	void Exec(Option_t */*opt*/){};
	virtual ~NicaCbmFirstTask();
	ClassDef(NicaCbmFirstTask,1)
};

#endif /* CBM_ANALYSIS_NICA_CBM_HELPERS_NICACBMFIRSTTASK_H_ */
