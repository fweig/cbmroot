/*
 * NicaGeneratorReadUnigen.h
 *
 *  Created on: 13 sie 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef _NICAGENERATORREADUNIGEN_H_
#define _NICAGENERATORREADUNIGEN_H_

#include "NicaGeneratorReadTree.h"
#include <TTree.h>
#include <TBranch.h>

#include "UEvent.h"
#include "UParticle.h"


/**
 * basic class for reading unigen files
 */
class NicaGeneratorReadUnigen: public NicaGeneratorReadTree {
private:
	UEvent *fInEvent;
	Int_t fStatus_Id, fParent_Id, fParentDecay_Id, fMate_Id,fDecay_Id, fChild1_Id,fChild2_Id;
	Int_t  fX_Id, fY_Id, fZ_Id, fT_Id,fWeight_Id;
	Int_t fENes_Id, fEStepNr_Id,fEStepT_Id;
protected:
	virtual Int_t ReadEvent();
	virtual Int_t Reopen();
	virtual void CheckParameters();
public:
	/**
	 * default constructor
	 * @param name
	 * @param multi_mode
	 */
	NicaGeneratorReadUnigen(TString name, Bool_t multi_mode=kFALSE);
	virtual ~NicaGeneratorReadUnigen();
	ClassDef(NicaGeneratorReadUnigen,1)
};

#endif /* _NICAGENERATORREADUNIGEN_H_ */
