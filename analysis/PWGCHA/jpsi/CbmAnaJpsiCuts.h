
#ifndef CBM_ANA_JPSI_CUTS_H
#define CBM_ANA_JPSI_CUTS_H

#include "TObject.h"

#include <iostream>


class CbmAnaJpsiCuts {
public:
	CbmAnaJpsiCuts():
		fChiPrimCut(0.),
		fPtCut(0.)
	{
		SetDefaultCuts();
	}

	/*
	 * Set default electron ID and analysis cuts.
	 */
	void SetDefaultCuts()
	{
		//electron ID cuts, we use CbmLitGlobalElectronId

		// analysis cuts
		fPtCut = 1.;//1.0;//1.5;
		fChiPrimCut = 3;//3.;
	}

	/*
	 * Print out cuts.
	 */
	void Print()
	{
		std::cout << "Used cuts:" << std::endl <<
				"fChiPrimCut = " << fChiPrimCut << std::endl <<
				"fPtCut = " << fPtCut << std::endl;
	}

public:
	 // ID cuts, we use CbmLitGlobalElectronId

	// Analysis cuts
	Double_t fChiPrimCut;
	Double_t fPtCut;
};

#endif
