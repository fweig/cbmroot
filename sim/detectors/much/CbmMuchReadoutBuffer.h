/*
 * CbmMuchReadoutBuffer.h
 *
 *  Created on: 11.05.2016
 *      Author: vikas@vecc.gov.in
 */

#ifndef CBMMUCHREADOUTBUFFER_H
#define CBMMUCHREADOUTBUFFER_H 1

#include "CbmReadoutBuffer.h"
#include "CbmMuchSignal.h"


// The MUCH Signal Buffer class which is used for intermidiate Buffer for the CbmMuchSignal, for the Readout the Much Signal.
// methods 
// Double_t GetTimeStart(),
// Double_t GetTimeStop(), 
// void SetTimeStop(Double_t),
// copy constructor (default one is fine).
// This class inherted from CbmReadoutBuffer.h abstract class.

class CbmMuchReadoutBuffer : public CbmReadoutBuffer<CbmMuchSignal> {

	public:
		// Default constructor
		CbmMuchReadoutBuffer() : fCounter{0} {};
	  // ---------------------------------------------------------------------



	  // ---------------------------------------------------------------------
		/** Constructor with branch and folder name
		 ** @param branchName  Name of branch in output tree
		 ** @param folderName  No idea what this is good for
		 ** @param persistence  If kTRUE (default), the branch will appear in the output file.
		 **
		 ** If this constructor is used, a branch with a TClonesArray will be created. Data
		 ** from the buffer will be copied to this array. Note that in this case,
		 ** ReadOutData will deliver no data (for the same readout time), since the
		 ** buffered data are deleted after being copied to the array.
		 **/
		//Vikas Todo:- Create similar  for the CbmMuchReadoutBuffe
		CbmMuchReadoutBuffer(TString branchName, TString folderName, Bool_t persistence = kTRUE) 
				:CbmReadoutBuffer(branchName,folderName,persistence), fCounter{0} { }

                CbmMuchReadoutBuffer(const CbmMuchReadoutBuffer&) = delete;
                CbmMuchReadoutBuffer& operator=(const CbmMuchReadoutBuffer&) = delete;
	  // ---------------------------------------------------------------------
	    	/**   Access to singleton instance
     		** @return pointer to instance
    		**/

		static CbmMuchReadoutBuffer* Instance();

	  // ---------------------------------------------------------------------
		virtual Int_t	Merge(CbmMuchSignal*, CbmMuchSignal*, std::vector<CbmMuchSignal*>&);

	//For getting Event File name etc.	
//		void GetEventInfo(Int_t&, Int_t&, Double_t&);


	  // ---------------------------------------------------------------------
		/** Destructor
		 **
		 ** Cleans up remaining data in the buffer, if present, which should not
		 ** be the case if used properly.
		 **/
		~CbmMuchReadoutBuffer(){};
	  // ---------------------------------------------------------------------

	private:
	    /** Pointer to singleton instance **/
 		static CbmMuchReadoutBuffer* fgInstance;

                Int_t fCounter;



	ClassDef(CbmMuchReadoutBuffer, 1);
};

#endif /* CBMMUCHREADOUTBUFFER_H_ */
