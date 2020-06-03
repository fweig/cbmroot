
/** Configuration macro for setting common cuts and processes for both G3 and G4 (M. Al-Turany 14.11.2007)
   specific cuts and processes to g3 or g4 should be set in the g3Config.C or g4Config.C
*/

void SetCuts()
{

  gMC->SetProcess("PAIR",1); /** pair production*/ //1
  gMC->SetProcess("COMP",0); /**Compton scattering*/ //1
  gMC->SetProcess("PHOT",0); /** photo electric effect */ //1
  gMC->SetProcess("PFIS",0); /**photofission*/ //0
  gMC->SetProcess("DRAY",0); /**delta-ray*/ //1
  gMC->SetProcess("ANNI",0); /**annihilation*/ //1
  gMC->SetProcess("BREM",0); /**bremsstrahlung*/ //1
  gMC->SetProcess("HADR",0); /**hadronic process*/ //1
  gMC->SetProcess("MUNU",0); /**muon nuclear interaction*/ //1
  gMC->SetProcess("DCAY",0); /**decay*/ //1
  gMC->SetProcess("LOSS",0); /**energy loss*/ //1
  gMC->SetProcess("MULS",0); /**multiple scattering*/ //1
  gMC->SetProcess("SYNC",0); /** whaterver hopefully annihilation*/ // ?
  Double_t cut1=1.0E-3; //GeV
  Double_t tofmax = 1.0; //seconds

  gMC->SetCut("CUTGAM",cut1);   /** gammas (GeV)*/
  gMC->SetCut("CUTELE",cut1);   /** electrons (GeV)*/
  gMC->SetCut("CUTNEU",cut1);   /** neutral hadrons (GeV)*/
  gMC->SetCut("CUTHAD",cut1);   /**charged hadrons (GeV)*/
  gMC->SetCut("CUTMUO",cut1);   /** muons (GeV)*/
  gMC->SetCut("BCUTE",cut1);    /**electron bremsstrahlung (GeV)*/
  gMC->SetCut("BCUTM",cut1);    /**muon and hadron bremsstrahlung(GeV)*/ 
  gMC->SetCut("DCUTE",cut1);    /**delta-rays by electrons (GeV)*/
  gMC->SetCut("DCUTM",cut1);    /**delta-rays by muons (GeV)*/
  gMC->SetCut("PPCUTM",cut1);   /**direct pair production by muons (GeV)*/
  gMC->SetCut("TOFMAX",tofmax); /**time of flight cut in seconds*/

}
