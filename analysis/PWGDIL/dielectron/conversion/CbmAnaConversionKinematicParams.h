/**
 * @author S. Reinecke <reinecke@uni-wuppertal.de>
 * @since 2015
 * @based on CbmLmvmKinematicParams.h by E. Lebedeva
 **/

#ifndef CBM_ANA_CONVERSION_KINEMATIC_PARAMS
#define CBM_ANA_CONVERSION_KINEMATIC_PARAMS

#include "TMath.h"
#include "CbmMCTrack.h"
#include "TLorentzVector.h"

#define M2E 2.6112004954086e-7

class CbmAnaConversionKinematicParams{
public:
   Double_t fMomentumMag = 0;	// Absolute value of momentum
   Double_t fPt = 0;			// Transverse momentum
   Double_t fRapidity = 0;		// Rapidity
   Double_t fMinv = 0;			// Invariant mass
   Double_t fAngle = 0;			// Opening angle
   Double_t fRap2 = 0;
   Double_t fPseudoRap2 = 0;

   /*
    * Calculate kinematic parameters for MC tracks.
    */
	static CbmAnaConversionKinematicParams KinematicParams_4particles_MC(const CbmMCTrack* mctrack1, const CbmMCTrack* mctrack2, const CbmMCTrack* mctrack3, const CbmMCTrack* mctrack4)
	{
		CbmAnaConversionKinematicParams params;

		TLorentzVector lorVec1;
		mctrack1->Get4Momentum(lorVec1);
		TVector3 part1 = lorVec1.Vect();
		Double_t energy1 = lorVec1.Energy();
    
		TLorentzVector lorVec2;
		mctrack2->Get4Momentum(lorVec2);
		TVector3 part2 = lorVec2.Vect();
		Double_t energy2 = lorVec2.Energy();
    
		TLorentzVector lorVec3;
		mctrack3->Get4Momentum(lorVec3);
		TVector3 part3 = lorVec3.Vect();
		Double_t energy3 = lorVec3.Energy();
    
		TLorentzVector lorVec4;
		mctrack4->Get4Momentum(lorVec4);
		TVector3 part4 = lorVec4.Vect();
		Double_t energy4 = lorVec4.Energy();
    
		TLorentzVector sum;
		sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;    

		TVector3 momPair = part1 + part2 + part3 + part4;
		Double_t energyPair = energy1 + energy2 + energy3 + energy4;
		Double_t pzPair = momPair.Pz();
		Double_t yPair = 0.5*TMath::Log((energyPair+pzPair)/(energyPair-pzPair));
   
		Double_t invmass = sum.Mag();
		
		Double_t perp = sum.Perp();
		//Double_t pt = TMath::Sqrt(sum.X() * sum.X() + sum.Y() * sum.Y() );
		


		params.fMomentumMag = momPair.Mag();
		params.fPt = perp;
		params.fRapidity = yPair;
		params.fMinv = invmass;
		params.fRap2 = sum.Rapidity();
		params.fPseudoRap2 = sum.PseudoRapidity();
		return params;
	}

    /*
    * Calculate kinematic parameters for reconstructed momenta
    */
	static CbmAnaConversionKinematicParams KinematicParams_4particles_Reco(const TVector3 part1, const TVector3 part2, const TVector3 part3, const TVector3 part4)
	{
		CbmAnaConversionKinematicParams params;
		
		Double_t energy1 = TMath::Sqrt(part1.Mag2() + M2E);
		TLorentzVector lorVec1(part1, energy1);

		Double_t energy2 = TMath::Sqrt(part2.Mag2() + M2E);
		TLorentzVector lorVec2(part2, energy2);

		Double_t energy3 = TMath::Sqrt(part3.Mag2() + M2E);
		TLorentzVector lorVec3(part3, energy3);

		Double_t energy4 = TMath::Sqrt(part4.Mag2() + M2E);
		TLorentzVector lorVec4(part4, energy4);
    
		TLorentzVector sum;
		sum = lorVec1 + lorVec2 + lorVec3 + lorVec4;    

		TVector3 momPair = part1 + part2 + part3 + part4;
		Double_t energyPair = energy1 + energy2 + energy3 + energy4;
		Double_t pzPair = momPair.Pz();
		Double_t yPair = 0.5*TMath::Log((energyPair+pzPair)/(energyPair-pzPair));
   
		Double_t invmass = sum.Mag();
		
		Double_t perp = sum.Perp();
		//Double_t pt = TMath::Sqrt(sum.X() * sum.X() + sum.Y() * sum.Y() );
		


		params.fMomentumMag = momPair.Mag();
		params.fPt = perp;
		params.fRapidity = yPair;
		params.fMinv = invmass;
		params.fRap2 = sum.Rapidity();
		params.fPseudoRap2 = sum.PseudoRapidity();
		return params;
	}
	
	
	
	static CbmAnaConversionKinematicParams KinematicParams_2particles_Reco(const TVector3 electron1, const TVector3 electron2)
	{
	CbmAnaConversionKinematicParams params;

    Double_t energyP = TMath::Sqrt(electron1.Mag2() + M2E);
    TLorentzVector lorVecP(electron1, energyP);

    Double_t energyM = TMath::Sqrt(electron2.Mag2() + M2E);
    TLorentzVector lorVecM(electron2, energyM);

    TVector3 momPair = electron1 + electron2;
    Double_t energyPair = energyP + energyM;
    Double_t ptPair = momPair.Perp();
    Double_t pzPair = momPair.Pz();
    Double_t yPair = 0.5*TMath::Log((energyPair+pzPair)/(energyPair-pzPair));
    Double_t anglePair = lorVecM.Angle(lorVecP.Vect());
    Double_t theta = 180.*anglePair/TMath::Pi();
    Double_t minv = 2.*TMath::Sin(anglePair / 2.)*TMath::Sqrt(electron1.Mag()*electron2.Mag());

    params.fMomentumMag = momPair.Mag();
    params.fPt = ptPair;
    params.fRapidity = yPair;
    params.fMinv = minv;
    params.fAngle = theta;
    return params;
	}
	
	


};

#endif
