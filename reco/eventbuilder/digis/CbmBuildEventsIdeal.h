/** @file CbmBuildEventsIdeal.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 17.09.2016
 **/
#ifndef CBMBUILDEVENTSIDEAL_H_
#define CBMBUILDEVENTSIDEAL_H 1


#include <vector>
#include <FairTask.h>
#include "CbmDefs.h"

class TClonesArray;
class CbmDigiManager;

/** @class CbmStsBuildEventsIdeal
 ** @brief Task class for associating digis to events
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 17.09.2016
 ** @version 1.0
 **
 ** The event association uses MC truth (CbmMatch of CbmDigi).
 ** It operates within one time slice; splitting of events between
 ** time slice is not treated.
 **/
class CbmBuildEventsIdeal: public FairTask {

	public:

		/** Constructor **/
		CbmBuildEventsIdeal();

		/** Destructor **/
		virtual ~CbmBuildEventsIdeal();

		/** Task execution **/
		virtual void Exec(Option_t* opt);


	private:

		CbmDigiManager* fDigiMan = nullptr;  //!
		std::vector<ECbmModuleId> fSystems { };
		TClonesArray* fEvents = nullptr;     //! Output array (class CbmEvent)
		Int_t fNofEntries = 0;               //  Number of processed time slices

		/** Task initialisation **/
		virtual InitStatus Init();

		CbmBuildEventsIdeal(const CbmBuildEventsIdeal&) = delete;
		CbmBuildEventsIdeal& operator=(const CbmBuildEventsIdeal&) = delete;

		ClassDef(CbmBuildEventsIdeal, 3);

};

#endif /* CBMBUILDEVENTSIDEAL_H */
