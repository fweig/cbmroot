/** @file CbmAuxDigi.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 12.02.2014
 **/

#ifndef CBMAUXDIGI_H
#define CBMAUXDIGI_H

#include <Rtypes.h>
#include <RtypesCore.h>

#include "CbmDefs.h"

/** @class CbmAuxDigi
 ** @brief Data class for data from AUX channels in ROCs
 ** @author V.Friese <v.friese@gsi.de>
 ** @version 1.0
 **/
class CbmAuxDigi {

	public:

		/** Default constructor **/
		CbmAuxDigi();


		/** Standard constructor **/
		CbmAuxDigi(UShort_t rocId, UShort_t channel, ULong64_t time);


		/** Destructor **/
		~CbmAuxDigi() { };


		/** Get Address (dummy)
		 ** Abstract method from CbmDigi. Must be implemented in order to
		 ** treat AUX like digis, but does not have any meaning here.
		 **/
		Int_t GetAddress() const { return -1; }


		/** Channel number **/
		UShort_t GetChannel() const { return fChannel; }


		/** ROC number **/
		UShort_t GetRocId() const { return fRocId; }


		/** System ID
		 ** Hardcoded to 999. Did not want to introduce a new item
		 ** in enum DetectorId.
		 **/
		static ECbmModuleId GetSystem() { return static_cast<ECbmModuleId>(999);}


		/** Time [ns]  **/
		Double_t GetTime() const { return Double_t(fTime); }

                std::string ToString() const { return std::string{""}; }
	private:

		UShort_t  fRocId;     ///< ROC number
		UShort_t  fChannel;   ///< AUX channel number (0 - 3)
		ULong64_t fTime;      ///< Time [ns]

		ClassDefNV(CbmAuxDigi, 2);
};


#endif /* CBMAUXDIGI_H */
