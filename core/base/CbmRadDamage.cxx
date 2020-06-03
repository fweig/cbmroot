/** @file CbmRadDamage.cxx
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 6 December 2011
 **/
#include "CbmRadDamage.h"

#include <FairLogger.h>         // for LOG    
#include <TMathBase.h>          // for Abs
#include <TMath.h>              // for Exp, Qe, K
#include <TString.h>            // for TString, operator+, operator<<

#include <stdlib.h>             // for getenv


// =====   Constructor   ==================================================
CbmRadDamage::CbmRadDamage() 
 : niel_neutron(),
   niel_proton(),
   niel_pion(),
   niel_electron(),
   fIAlpha(4.e-17),
   fEGap0(1.166),
   fEGapAlpha(4.73e-4),
   fEGapBeta(636.),
   fNeff0(9.0e11),
   fNeffC(2.5e-14),
   fNeffGc(1.5e-2),
   fEpsilon(1.04e-12)
{
  Init();
}
// ========================================================================




// =====   Destructor   ===================================================
CbmRadDamage::~CbmRadDamage() { }
// ========================================================================




// =====   Get leakage current   ==========================================
Double_t CbmRadDamage::GetLeakageCurrent(Double_t fluence,
					 Double_t volume,
					 Double_t temperature) {

  // --- Boltzmann constant in ev/K
  Double_t kB = TMath::K() / TMath::Qe();

  // --- Leakage current at room temperature (293 K)
  Double_t i20 = fIAlpha * fluence * volume;

  // --- Gap energy at given temperature
  Double_t eGap = fEGap0 
    - fEGapAlpha * temperature * temperature / ( temperature + fEGapBeta);

  // --- Leakage current at given temperature
  Double_t exponent = -1. * eGap / 2. / kB * ( 1./temperature - 1./293.);
  Double_t iLeak = i20 * temperature * temperature / 85849. 
                   * TMath::Exp(exponent);

  return iLeak;
}
// ========================================================================




// =====   Get NIEL factor   ==============================================
Double_t CbmRadDamage::GetNiel(Int_t type, Double_t energy) {

  // Convert energy to MeV like in table
  energy = energy * 1000.;

  std::map<Double_t, Double_t>* table = nullptr;
  Int_t atype = TMath::Abs(type);

  // Select table according to particle type
  switch(atype) {
  case 2112: table = &niel_neutron; break;    // neutrons
  case 2212: table = &niel_proton; break;     // protons
  case 211:  table = &niel_pion; break;       // pions
  case 11:   table = &niel_electron; break;   // electrons
  default:   table = nullptr;
  }

  // Return zero for unknown particles
  if ( ! table ) return 0.;

  // First table entry above selected energy
  std::map<Double_t, Double_t>::iterator it = table->upper_bound(energy);

  // Return zero if below lower limit
  if ( it == table->begin() ) {
    //LOG(warn) << "-I- Below table limit: " << atype << "  " << energy;
    return 0.;
  }

  // Return asymptotic value if above upper limit
  if ( it == table->end() ) {
    //LOG(warn) << "-I- Above table limit: " << atype << "  " << energy;
    switch(atype) {
    case 2112: return 0.44; break;
    case 2212: return 0.50; break;
    case 211:  return 0.38; break;
    case 11:   return 0.08; break;
    default:   return 0.00; break;
    }
  }

  // Interpolate within table values
  Double_t e2 = (*it).first;
  Double_t v2 = (*it).second;
  it--;
  Double_t e1 = (*it).first;
  Double_t v1 = (*it).second;

  Double_t v = v1 + (v2-v1) * (energy-e1) / (e2-e1);

  return v;

}
// ========================================================================




// =====   Get full depletion voltage   ===================================
Double_t CbmRadDamage::GetVfd(Double_t fluence, Double_t d) {

  // --- Calculate effective doping concentration at given fluence
  Double_t corr1 = 0.7 * fNeff0 
                   * ( 1. - TMath::Exp(-1. * fNeffC * fluence) );
  Double_t corr2 = fNeffGc * fluence;
  Double_t nEff = fNeff0 - corr1 - corr2;

  // --- Calculate full depletion voltage from doping concentration
  Double_t vfd = TMath::Qe() * nEff * d * d / 2. / fEpsilon;

  return vfd;
}
// ========================================================================




// =====   Private method Init   ==========================================
void CbmRadDamage::Init() {

  // --- Read NIEL tables
  ReadData("niel_neutrons.dat", niel_neutron);
  ReadData("niel_protons.dat", niel_proton);
  ReadData("niel_pions.dat", niel_pion);
  ReadData("niel_electrons.dat", niel_electron);


  // --- Proportionality constant of leakage current and fluence
  // --- for Silicon at room temperature
  // --- Numerical value provided by S. Chatterji
  fIAlpha = 4.e-17;    // [A/cm]


  // --- Constants for temperature dependence of leakage current
  // --- Values are for Silicon
  // --- Numerical values provided by S. Chatterji
  fEGap0     = 1.166;     // Gap energy [eV] at T = 0K
  fEGapAlpha = 4.73e-4;   // [ev/K]
  fEGapBeta  = 636.;      // [K]


  // --- Constants for effective doping concentration
  // --- Values are for Silicon
  // --- Numerical values provided by S. Chatterji
  fNeff0  = 9.0e11;  // Doping concentration without irradiation [cm^-3];
  fNeffC  = 2.5e-14; // [cm^2]
  fNeffGc = 1.5e-2;  // [1/cm]


  // --- Permittivivity of Silicon
  fEpsilon = 1.04e-12;  // [F/cm]

}
// ========================================================================




// =====   Private method ReadData   ======================================
void CbmRadDamage::ReadData(const char* file, 
			     std::map<Double_t, Double_t>& table) {

  TString wrkdir = getenv("VMCWORKDIR");
  TString fileName = wrkdir + "/input/" + TString(file);
  LOG(info) << "Reading " << fileName;

  std::ifstream* data = new std::ifstream(fileName.Data());
  if ( ! data->is_open() ) {
    LOG(fatal) << "Error when reading from file!";
  }

  Double_t e = 0.;
  Double_t v = 0.;
  Int_t nEntries = 0;

  *data >> e >> v;
  while ( ! data->eof() ) {
    table[e] = v; 
    nEntries++;
    *data >> e >> v;
  }

  data->close();
  delete data;

  std::map<Double_t, Double_t>::iterator it1 = table.begin();
  std::map<Double_t, Double_t>::iterator it2 = table.end();
  it2--;

  LOG(info) << nEntries << " values read; energy range " 
       << (*it1).first << " to " << (*it2).first << " MeV; map size " 
       << table.size();

}
// ========================================================================



ClassImp(CbmRadDamage)


