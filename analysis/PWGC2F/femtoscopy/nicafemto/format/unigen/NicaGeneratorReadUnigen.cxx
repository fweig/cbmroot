/*
 * NicaGeneratorReadUnigen.cxx
 *
 *  Created on: 13 sie 2015
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#include "NicaGeneratorReadUnigen.h"


Int_t NicaGeneratorReadUnigen::ReadEvent() {
	Int_t status = NicaGeneratorReadTree::ReadEvent();
	if(status!=EventOk()) return status;
	fEventParameters->SetInt(fInEvent->GetEventNr(),fEventNr_Id);
	fEventParameters->SetInt(fInEvent->GetNes(),fENes_Id);
	fEventParameters->SetInt(fInEvent->GetStepNr(),fEStepNr_Id);
	fEventParameters->SetDouble(fInEvent->GetPhi(),fPhi_Id);
	fEventParameters->SetDouble(fInEvent->GetB(),fB_Id);
	WriteEventHeader();
	for(int i =0;i<fInEvent->GetNpa();i++){
		UParticle *part = fInEvent->GetParticle(i);
		fTrackParameters->SetInt(part->GetIndex(),fIndex_Id);
		fTrackParameters->SetInt(part->GetStatus(),fStatus_Id);
		fTrackParameters->SetInt(part->GetParent(),fParent_Id);
		fTrackParameters->SetInt(part->GetParentDecay(),fParentDecay_Id);
		fTrackParameters->SetInt(part->GetMate(),fMate_Id);
		fTrackParameters->SetInt(part->GetDecay(),fDecay_Id);
		fTrackParameters->SetInt(part->GetFirstChild(),fChild1_Id);
		fTrackParameters->SetInt(part->GetLastChild(),fChild2_Id);
		fTrackParameters->SetDouble(part->Px(),fPx_Id);
		fTrackParameters->SetDouble(part->Py(),fPy_Id);
		fTrackParameters->SetDouble(part->Pz(),fPz_Id);
		fTrackParameters->SetDouble(part->E(),fE_Id);
		fTrackParameters->SetDouble(part->X(),fX_Id);
		fTrackParameters->SetDouble(part->Y(),fY_Id);
		fTrackParameters->SetDouble(part->Z(),fZ_Id);
		fTrackParameters->SetDouble(part->T(),fT_Id);
		fTrackParameters->SetInt(part->GetPdg(),fPdg_Id);
		WriteParticle();
	}
	return EventOk();
}

Int_t NicaGeneratorReadUnigen::Reopen() {
	Int_t stat = NicaGeneratorReadTree::Reopen();
	if(stat != FileOk()){
		return stat;
	}
	if(fInEvent==NULL)
	fInEvent = new UEvent();
	fBranch->SetAddress(&fInEvent);
	fInTree->GetEntry(0);
	return FileOk();
}

NicaGeneratorReadUnigen::NicaGeneratorReadUnigen(TString name, Bool_t multi_mode):
		NicaGeneratorReadTree(name,"events","event",multi_mode),
		fInEvent(NULL),
		fStatus_Id(0), fParent_Id(0), fParentDecay_Id(0), fMate_Id(0),fDecay_Id(0), fChild1_Id(0),fChild2_Id(0),
		fX_Id(0), fY_Id(0), fZ_Id(0), fT_Id(0),fWeight_Id(0),
		fENes_Id(0), fEStepNr_Id(0),fEStepT_Id(0){
			fInEvent = new UEvent();
}

void NicaGeneratorReadUnigen::CheckParameters() {
	NicaGeneratorRead::CheckParameters();
	fStatus_Id = CheckParameter("status");
	fParent_Id = CheckParameter("parent");
	fParentDecay_Id = CheckParameter("parent_decay");
	fMate_Id = CheckParameter("mate");
	fDecay_Id = CheckParameter("decay_id");
	fChild1_Id = CheckParameter("first_child");
	fChild2_Id =  CheckParameter("last_child");
	fX_Id = CheckParameter("frx");
	fY_Id = CheckParameter("fry");
	fZ_Id = CheckParameter("frz");
	fT_Id = CheckParameter("frt");
	fWeight_Id= CheckParameter("weight");;
	fENes_Id = CheckParameter("Nes");
	fEStepNr_Id = CheckParameter("Step_nr");
	fEStepT_Id = CheckParameter("Step_t");
}

NicaGeneratorReadUnigen::~NicaGeneratorReadUnigen() {
	// TODO Auto-generated destructor stub
}
