/**
 * @author S. Reinecke <reinecke@uni-wuppertal.de>
 * @since 2015
 * @based on CbmLmvmKinematicParams.h by E. Lebedeva
 **/

#ifndef CBM_ANA_CONVERSION_CUT_SETTINGS
#define CBM_ANA_CONVERSION_CUT_SETTINGS

#include "TMath.h"
#include "CbmMCTrack.h"
#include "TLorentzVector.h"

#define M2E 2.6112004954086e-7

class CbmAnaConversionCutSettings{
public:

	/*
	 * Calculate cut for opening angle between electrons
	 */
	static Double_t CalcOpeningAngleCut(Double_t pt)
	{
		Double_t cut = 0;
		//cut = 1.5 - 0.5 * pt;
		cut = 1.8 - 0.6 * pt;
		return cut;
	}


	static Double_t CalcOpeningAngleCutAlt1(Double_t pt)
	{
		Double_t cut = 0;
		cut = 1.5 - 0.5 * pt;
		return cut;
	}


	static Double_t CalcOpeningAngleCutAlt2(Double_t /*pt*/)
	{
		Double_t cut = 0;
		cut = 1.8;
		return cut;
	}


	static Double_t CalcOpeningAngleCutAlt3(Double_t /*pt*/)
	{
		Double_t cut = 0;
		cut = 1.5;
		return cut;
	}


	static Double_t CalcOpeningAngleCutAlt4(Double_t pt)
	{
		Double_t cut = 0;
		if(pt <= 1) cut = 1.5 - 0.5 * pt;
		if(pt > 1) cut = 1.0;
		return cut;
	}


	static Double_t CalcOpeningAngleCutAlt5(Double_t pt)
	{
		Double_t cut = 0;
		if(pt <= 1) cut = 1.0 + 1.0 * pt;
		if(pt > 1) cut = 2.0;
		return cut;
	}


	/*
	 * Calculate cut for chi for reconstructed momenta
	 */
	static Double_t CalcChiCut(Double_t pt)
	{
		Double_t cut = 0;
		if(pt < 0.4) {
			cut = 31. - 70.*pt;
		}
		if(pt >= 0.4) {
			cut = 3;
		}
		return cut;
	}


	static Double_t CalcChiCutAlt1(Double_t /*pt*/)
	{
		Double_t cut = 0;
		cut = 3;
		return cut;
	}


	static Double_t CalcChiCutAlt2(Double_t pt)
	{
		Double_t cut = 0;
		if(pt < 0.4) {
			cut = 15. - 30.*pt;
		}
		if(pt >= 0.4) {
			cut = 3;
		}
		return cut;
	}


	static Double_t CalcChiCutAlt3(Double_t /*pt*/)
	{
		Double_t cut = 0;
		cut = 4;
		return cut;
	}

};

#endif
