/**
 *    file CbmKresConversionMain.cxx
 *
 *    author Ievgenii Kres
 *    date 04.04.2017
 *    modified 30.01.2020
 *
 *    Main class which is executed in the macro. In this class other subclasses can be defined for different parts of analysis: CbmKresConversionGeneral, CbmKresConversionPhotons, etc.
 *    In this class are defined opening angle and invariant mass cut for pi^0 reconstruction.
 *    At the end all executed tasks will draw obtained histograms.
 *
 **/

#include "CbmKresConversionMain.h"

#include "CbmGlobalTrack.h"
#include "CbmMCTrack.h"
#include "CbmMvdHit.h"
#include "CbmRichHit.h"
#include "CbmRichPoint.h"
#include "CbmRichRing.h"
#include "CbmStsHit.h"
#include "CbmStsTrack.h"
#include "CbmTrackMatchNew.h"

#include "FairRootManager.h"

#include "TDirectory.h"

#include <boost/assign/list_of.hpp>

#include <iostream>
#include <string>

using namespace std;
using boost::assign::list_of;

///// constructor with definition of variables for tasks. If variable == 1 task will be executed, 0 - not
CbmKresConversionMain::CbmKresConversionMain()
  : FairTask("CbmKresConversionMain")
  , DoKresGeneral(0)
  , DoKresReconstruction(0)
  , DoKresKF(0)
  , DoKresManual(0)
  , DoKresManualmbias(0)
  , DoKresTemperature(0)
  , DoKresPhotons(0)
  , DoKresCorrectedPhotons(0)
  , DoKresEtaMCAnalysis(0)
  , DoKresEta(0)
  , fKresGeneral(nullptr)
  , fKresReco(nullptr)
  , fKresKF(nullptr)
  , fKresManual(nullptr)
  , fKFparticle(nullptr)
  , fKFparticleFinderQA(nullptr)
  , fKresManualmbiasPart1(nullptr)
  , fKresManualmbiasPart2(nullptr)
  , fKresManualmbiasPart3(nullptr)
  , fKresManualmbiasPart4(nullptr)
  , fKresTemperature(nullptr)
  , fKresPhotons(nullptr)
  , fKresCorrectedPhotons(nullptr)
  , fKresEtaMCAnalysis(nullptr)
  , fKresEta(nullptr)
  , fEventNum(0)
  , OpeningAngleCut(0.)
  , GammaInvMassCut(0.)
  , fRealPID(0)
{
  OpeningAngleCut = 2.;    //  Opening angle cut for photon reconstruction
  GammaInvMassCut = 0.02;  //  Invariant mass for photon reconstruction
  fRealPID        = 1;     //  Particle identifiaction from detectors.    1 is Real !!!; 0 is MC.

  DoKresGeneral          = 0;
  DoKresReconstruction   = 0;  //  estimation for eta and pi^0 reconstruction
  DoKresManual           = 0;  //  central mode
  DoKresPhotons          = 0;  //  photons  mode
  DoKresCorrectedPhotons = 0;  //  photons  mode with apllying of acceptance (rough) correction

  DoKresKF          = 0;
  DoKresManualmbias = 1;  //  mbias  mode

  DoKresTemperature = 0;  //  temperature  mode

  DoKresEtaMCAnalysis = 0;  //  Monte Carlo analysis for eta
  DoKresEta           = 0;  //  eta  mode
}


CbmKresConversionMain::~CbmKresConversionMain() {}


///// initialization of task to be performed. The description of each is given in their files
InitStatus CbmKresConversionMain::Init()
{

  cout << "CbmKresConversionMain::Init" << endl;

  // if (DoKresManual == 1 && DoKresManualmbias == 1) { Fatal("", " !!!! Central and mbias modes are activated together  !!!! "); }


  cout << endl;
  if (DoKresGeneral) {
    fKresGeneral = new CbmKresConversionGeneral();
    fKresGeneral->Init();
    cout << endl;
    cout << "\t *** CbmKresConversionGeneral                          ==> "
            "analysis is activated"
         << endl;
  }

  if (DoKresReconstruction) {
    fKresReco = new CbmKresConversionReconstruction();
    fKresReco->Init();
    cout << "\t *** CbmKresConversionReconstruction                   ==> "
            "analysis is activated"
         << endl;
  }
  if (DoKresKF) {
    fKresKF = new CbmKresConversionKF();
    fKresKF->SetKF(fKFparticle, fKFparticleFinderQA);
    fKresKF->Init();
    cout << "\t *** CbmKresConversionKF                               ==> "
            "analysis is activated"
         << endl;
  }
  if (DoKresManual) {
    fKresManual = new CbmKresConversionManual();
    fKresManual->Init();
    cout << "\t *** CbmKresConversionManual                           ==> "
            "analysis is activated"
         << endl;
  }
  if (DoKresManualmbias) {
    fKresManualmbiasPart1 = new CbmKresConversionManualmbias1();
    fKresManualmbiasPart1->Init();
    cout << "\t *** CbmKresConversionManualmbias (b <= 2 fm)          ==> "
            "analysis is activated"
         << endl;

    fKresManualmbiasPart2 = new CbmKresConversionManualmbias2();
    fKresManualmbiasPart2->Init();
    cout << "\t *** CbmKresConversionManualmbias (2 < b <= 6 fm)      ==> "
            "analysis is activated"
         << endl;

    fKresManualmbiasPart3 = new CbmKresConversionManualmbias3();
    fKresManualmbiasPart3->Init();
    cout << "\t *** CbmKresConversionManualmbias (6 < b <= 10 fm)     ==> "
            "analysis is activated"
         << endl;

    fKresManualmbiasPart4 = new CbmKresConversionManualmbias4();
    fKresManualmbiasPart4->Init();
    cout << "\t *** CbmKresConversionManualmbias (b > 10 fm)          ==> "
            "analysis is activated"
         << endl;
  }

  if (DoKresTemperature) {
    fKresTemperature = new CbmKresTemperature();
    fKresTemperature->Init();
    cout << "\t *** CbmKresTemperature                                ==> "
            "analysis is activated"
         << endl;
  }

  if (DoKresPhotons) {
    fKresPhotons = new CbmKresConversionPhotons();
    fKresPhotons->Init();
    cout << "\t *** CbmKresConversionPhotons                           ==> "
            "analysis is activated"
         << endl;
  }

  if (DoKresCorrectedPhotons) {
    fKresCorrectedPhotons = new CbmKresConversionCorrectedPhotons();
    fKresCorrectedPhotons->Init(OpeningAngleCut, GammaInvMassCut * 1000);
    cout << "\t *** CbmKresConversionCorrectedPhotons                  ==> "
            "analysis is activated"
         << endl;
  }

  if (DoKresEtaMCAnalysis) {
    fKresEtaMCAnalysis = new CbmKresEtaMCAnalysis();
    fKresEtaMCAnalysis->Init();
    cout << "\t *** CbmKresEtaMCAnalysis                               ==> "
            "analysis is activated"
         << endl;
  }

  if (DoKresEta) {
    fKresEta = new CbmKresEta();
    fKresEta->Init();
    cout << "\t *** CbmKresEta                                         ==> "
            "analysis is activated"
         << endl;
  }


  cout << "\t CUTS : " << endl;
  cout << "\t        Gamma Invariant Mass cut = " << GammaInvMassCut << endl;
  cout << "\t        Gamma Opening Angle cut  = " << OpeningAngleCut << endl;
  if (fRealPID == 1) { cout << "\t        PID mode is             << RealPID >> " << endl; }
  else {
    cout << "\t        PID mode is             << MCPID >> " << endl;
  }
  cout << endl;


  InitHistograms();

  return kSUCCESS;
}

///// place where all histograms are created. Called only once during initialization. I did not need it here
void CbmKresConversionMain::InitHistograms() {}

/////  main body of the class, where one performs the physics analysis.
void CbmKresConversionMain::Exec(Option_t* /*option*/)
{
  fEventNum++;
  // cout << "CbmKresConversionMain, event No. " <<  fEventNum << endl;


  if (DoKresGeneral) { fKresGeneral->Exec(fEventNum); }

  if (DoKresReconstruction) { fKresReco->Exec(fEventNum); }

  if (DoKresKF) { fKresKF->Exec(fEventNum, OpeningAngleCut, GammaInvMassCut, fRealPID); }

  if (DoKresManual) { fKresManual->Exec(fEventNum, OpeningAngleCut, GammaInvMassCut, fRealPID); }

  if (DoKresManualmbias) {
    fKresManualmbiasPart1->Exec(fEventNum, OpeningAngleCut, GammaInvMassCut, fRealPID);
    fKresManualmbiasPart2->Exec(fEventNum, OpeningAngleCut, GammaInvMassCut, fRealPID);
    fKresManualmbiasPart3->Exec(fEventNum, OpeningAngleCut, GammaInvMassCut, fRealPID);
    fKresManualmbiasPart4->Exec(fEventNum, OpeningAngleCut, GammaInvMassCut, fRealPID);
  }

  if (DoKresTemperature) { fKresTemperature->Exec(fEventNum); }

  if (DoKresPhotons) { fKresPhotons->Exec(fEventNum, OpeningAngleCut, GammaInvMassCut, fRealPID); }

  if (DoKresCorrectedPhotons) { fKresCorrectedPhotons->Exec(fEventNum, OpeningAngleCut, GammaInvMassCut, fRealPID); }

  if (DoKresEtaMCAnalysis) { fKresEtaMCAnalysis->Exec(fEventNum, OpeningAngleCut, GammaInvMassCut); }

  if (DoKresEta) { fKresEta->Exec(fEventNum, OpeningAngleCut, GammaInvMassCut, fRealPID); }
}

///// one needs to set KFparticle if one wants to use its features.
void CbmKresConversionMain::SetKF(CbmKFParticleFinder* kfparticle, CbmKFParticleFinderQA* kfparticleQA)
{
  fKFparticle         = kfparticle;
  fKFparticleFinderQA = kfparticleQA;
  if (fKFparticle) { cout << "kf works" << endl; }
  else {
    cout << "kf works not" << endl;
  }
}

///// this part is executed at the very end. Decicated for histogram saving into the file
void CbmKresConversionMain::Finish()
{
  gDirectory->mkdir("conversionKres");
  gDirectory->cd("conversionKres");

  if (DoKresGeneral) { fKresGeneral->Finish(); }
  if (DoKresReconstruction) { fKresReco->Finish(); }
  if (DoKresKF) { fKresKF->Finish(); }
  if (DoKresManual) { fKresManual->Finish(); }
  if (DoKresManualmbias) {
    gDirectory->mkdir("Manual_mbias");
    gDirectory->cd("Manual_mbias");

    gDirectory->mkdir("b <= 2 (central) ");
    gDirectory->cd("b <= 2 (central) ");
    fKresManualmbiasPart1->Finish();
    gDirectory->cd("..");

    gDirectory->mkdir("2 < b <= 6 ");
    gDirectory->cd("2 < b <= 6 ");
    fKresManualmbiasPart2->Finish();
    gDirectory->cd("..");

    gDirectory->mkdir("6 < b <= 10 ");
    gDirectory->cd("6 < b <= 10 ");
    fKresManualmbiasPart3->Finish();
    gDirectory->cd("..");

    gDirectory->mkdir("b > 10 (miss) ");
    gDirectory->cd("b > 10 (miss) ");
    fKresManualmbiasPart4->Finish();
    gDirectory->cd("..");

    gDirectory->cd("..");
  }
  if (DoKresTemperature) { fKresTemperature->Finish(); }
  if (DoKresPhotons) { fKresPhotons->Finish(); }
  if (DoKresCorrectedPhotons) { fKresCorrectedPhotons->Finish(); }
  if (DoKresEtaMCAnalysis) { fKresEtaMCAnalysis->Finish(); }
  if (DoKresEta) { fKresEta->Finish(); }

  gDirectory->cd("..");

  // remove unnecessary folders, which are created by default
  gDirectory->rmdir("cbmout");
  gDirectory->rmdir("BranchList");
  gDirectory->rmdir("TimeBasedBranchList");
  gDirectory->rmdir("FileHeader");


  cout << endl;
  cout << "END... we will continue soon... " << fEventNum << endl;
}

ClassImp(CbmKresConversionMain)
