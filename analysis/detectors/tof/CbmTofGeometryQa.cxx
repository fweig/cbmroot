/** @file CbmTofGeometryQa.cxx
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @date 27/08/2015
 **/

#include "CbmTofGeometryQa.h"

// TOF Classes and includes
#include "CbmTofAddress.h"          // in cbmdata/tof
#include "CbmTofCell.h"             // in tof/TofData
#include "CbmTofDetectorId_v12b.h"  // in cbmdata/tof
#include "CbmTofDetectorId_v14a.h"  // in cbmdata/tof
#include "CbmTofGeoHandler.h"       // in tof/TofTools
#include "CbmTofPoint.h"            // in cbmdata/tof

// CBMroot classes and includes
#include "CbmMCTrack.h"
#include "CbmMatch.h"

// FAIR classes and includes
#include "FairMCEventHeader.h"
#include "FairRootManager.h"
#include "FairRunAna.h"
#include "FairRuntimeDb.h"
#include <Logger.h>

// ROOT Classes and includes
#include "TClonesArray.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TMath.h"
#include "TProfile2D.h"
#include "TROOT.h"
#include "TRandom.h"
#include "TString.h"

#include "Riostream.h"

using std::cout;
using std::endl;

//___________________________________________________________________
// Constants definitions: Particles list
const Int_t kiNbPart              = 13;
const TString ksPartTag[kiNbPart] = {"others", "ep", "em", "pip", "pim", "kp", "km", "p", "pbar", "d", "t", "he", "a"};
const Int_t kiPartPdgCode[kiNbPart] = {0,    11,    -11,        211,        -211,       321,       -321,
                                       2212, -2212, 1000010020, 1000010030, 1000020030, 1000020040};
const TString ksPartName[kiNbPart]  = {"any other part.", "e+", "e-", "#pi+", "#pi-",  "k+", "k-", "p",
                                      "anti-p",          "d",  "t",  "he",   "#alpha"};
const Int_t kiMinNbStsPntAcc        = 3;   // Number of STS Pnt for Trk to be reconstructable
const Int_t kiMinNbMuchPntAcc       = 10;  // Number of MUCH Pnt for Trk to be reconstructable
//___________________________________________________________________


//___________________________________________________________________
//
// CbmTofGeometryQa
//
// Task for QA of TOF geometry
//
// ------------------------------------------------------------------
CbmTofGeometryQa::CbmTofGeometryQa()
  : FairTask("CbmTofGeometryQa")
  , fEvents(0)
  , fGeoHandler(new CbmTofGeoHandler())
  , fTofId(NULL)
  , fChannelInfo(NULL)
  , iNbSmTot(0)
  , fvTypeSmOffs()
  , iNbRpcTot(0)
  , fvSmRpcOffs()
  , fiNbChTot(0)
  , fvRpcChOffs()
  , fMCEventHeader(NULL)
  , fTofPointsColl(NULL)
  , fMcTracksColl(NULL)
  , fRealTofPointsColl(NULL)
  , fRealTofMatchColl(NULL)
  , fbRealPointAvail(kFALSE)
  , fsHistoOutFilename("./tofQa.hst.root")
  , fdWallPosZ(1000)
  , fbCentDepOn(kFALSE)
  , fvhTrackAllStartZCent()
  ,  // Beam pipe check
  fvhTrackSecStartZCent()
  ,  // Beam pipe check
  fvhTrackAllStartXZCent()
  ,  // Beam pipe check
  fvhTofPntAllAngCent()
  ,  // Beam pipe check
  fvhTrackAllStartXZ()
  ,  // Beam pipe check
  fvhTrackAllStartYZ()
  ,  // Beam pipe check
  fhTrackMapXY(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapXZ(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapYZ(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapAng(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapSph(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapAngPrimAll(NULL)
  ,  // For angular acceptance study
  fhTrackMapAngPrimSts(NULL)
  ,  // For angular acceptance study
  fhTrackMapAngPrimRich(NULL)
  ,  // For angular acceptance study
  fhTrackMapAngPrimMuch(NULL)
  ,  // For angular acceptance study
  fhTrackMapAngPrimTrd(NULL)
  ,  // For angular acceptance study
  fhTrackMapAngPrimTof(NULL)
  ,  // For angular acceptance study
  fhPointMapXY(NULL)
  , fhPointMapXZ(NULL)
  , fhPointMapYZ(NULL)
  , fhPointMapAng(NULL)
  , fhPointMapSph(NULL)
  , fhRealPointMapXY(NULL)
  , fhRealPointMapXZ(NULL)
  , fhRealPointMapYZ(NULL)
  , fhRealPointMapAng(NULL)
  , fhRealPointMapSph(NULL)
  , fhPointMapAngWithSts(NULL)
  , fhPointMapAngWithRich(NULL)
  , fhPointMapAngWithMuch(NULL)
  , fhPointMapAngWithTrd(NULL)
  , fbSphAppOn(kFALSE)
  , fhPointSphAprRadiusErrMapXY(NULL)
  , fhPointSphAprRadiusErrMapXZ(NULL)
  , fhPointSphAprRadiusErrMapYZ(NULL)
  , fhPointSphAprRadiusErrMapAng(NULL)
  , fhPointSphAprRadiusErrMapSph(NULL)
  , fhPointSphAprZposErrMapXY(NULL)
  , fhPointSphAprZposErrMapXZ(NULL)
  , fhPointSphAprZposErrMapYZ(NULL)
  , fhPointSphAprZposErrMapAng(NULL)
  , fhPointSphAprZposErrMapSph(NULL)
  , fvhPtmRapGenTrk()
  , fvhPtmRapStsPnt()
  , fvhPtmRapTofPnt()
  , fvhPtmRapSecGenTrk()
  , fvhPtmRapSecStsPnt()
  , fvhPtmRapSecTofPnt()
  , fvhPlabGenTrk()
  , fvhPlabStsPnt()
  , fvhPlabTofPnt()
  , fvhPlabSecGenTrk()
  , fvhPlabSecStsPnt()
  , fvhPlabSecTofPnt()
  , fvhPtmRapGenTrkTofPnt()
  , fvhPlabGenTrkTofPnt()
  , fvhPlabStsTrkTofPnt()
  , fvhPtmRapSecGenTrkTofPnt()
  , fvhPlabSecGenTrkTofPnt()
  , fvhPlabSecStsTrkTofPnt()
{
  cout << "CbmTofGeometryQa: Task started " << endl;
}
// ------------------------------------------------------------------

// ------------------------------------------------------------------
CbmTofGeometryQa::CbmTofGeometryQa(const char* name, Int_t verbose)
  : FairTask(name, verbose)
  , fEvents(0)
  , fGeoHandler(new CbmTofGeoHandler())
  , fTofId(NULL)
  , fChannelInfo(NULL)
  , iNbSmTot(0)
  , fvTypeSmOffs()
  , iNbRpcTot(0)
  , fvSmRpcOffs()
  , fiNbChTot(0)
  , fvRpcChOffs()
  , fMCEventHeader(NULL)
  , fTofPointsColl(NULL)
  , fMcTracksColl(NULL)
  , fRealTofPointsColl(NULL)
  , fRealTofMatchColl(NULL)
  , fbRealPointAvail(kFALSE)
  , fsHistoOutFilename("./tofQa.hst.root")
  , fdWallPosZ(1000)
  , fbCentDepOn(kFALSE)
  , fvhTrackAllStartZCent()
  ,  // Beam pipe check
  fvhTrackSecStartZCent()
  ,  // Beam pipe check
  fvhTrackAllStartXZCent()
  ,  // Beam pipe check
  fvhTofPntAllAngCent()
  ,  // Beam pipe check
  fvhTrackAllStartXZ()
  ,  // Beam pipe check
  fvhTrackAllStartYZ()
  ,  // Beam pipe check
  fhTrackMapXY(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapXZ(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapYZ(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapAng(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapSph(NULL)
  ,  // Only when creating normalization histos
  fhTrackMapAngPrimAll(NULL)
  ,  // For angular acceptance study
  fhTrackMapAngPrimSts(NULL)
  ,  // For angular acceptance study
  fhTrackMapAngPrimRich(NULL)
  ,  // For angular acceptance study
  fhTrackMapAngPrimMuch(NULL)
  ,  // For angular acceptance study
  fhTrackMapAngPrimTrd(NULL)
  ,  // For angular acceptance study
  fhTrackMapAngPrimTof(NULL)
  ,  // For angular acceptance study
  fhPointMapXY(NULL)
  , fhPointMapXZ(NULL)
  , fhPointMapYZ(NULL)
  , fhPointMapAng(NULL)
  , fhPointMapSph(NULL)
  , fhRealPointMapXY(NULL)
  , fhRealPointMapXZ(NULL)
  , fhRealPointMapYZ(NULL)
  , fhRealPointMapAng(NULL)
  , fhRealPointMapSph(NULL)
  , fhPointMapAngWithSts(NULL)
  , fhPointMapAngWithRich(NULL)
  , fhPointMapAngWithMuch(NULL)
  , fhPointMapAngWithTrd(NULL)
  , fbSphAppOn(kFALSE)
  , fhPointSphAprRadiusErrMapXY(NULL)
  , fhPointSphAprRadiusErrMapXZ(NULL)
  , fhPointSphAprRadiusErrMapYZ(NULL)
  , fhPointSphAprRadiusErrMapAng(NULL)
  , fhPointSphAprRadiusErrMapSph(NULL)
  , fhPointSphAprZposErrMapXY(NULL)
  , fhPointSphAprZposErrMapXZ(NULL)
  , fhPointSphAprZposErrMapYZ(NULL)
  , fhPointSphAprZposErrMapAng(NULL)
  , fhPointSphAprZposErrMapSph(NULL)
  , fvhPtmRapGenTrk()
  , fvhPtmRapStsPnt()
  , fvhPtmRapTofPnt()
  , fvhPtmRapSecGenTrk()
  , fvhPtmRapSecStsPnt()
  , fvhPtmRapSecTofPnt()
  , fvhPlabGenTrk()
  , fvhPlabStsPnt()
  , fvhPlabTofPnt()
  , fvhPlabSecGenTrk()
  , fvhPlabSecStsPnt()
  , fvhPlabSecTofPnt()
  , fvhPtmRapGenTrkTofPnt()
  , fvhPlabGenTrkTofPnt()
  , fvhPlabStsTrkTofPnt()
  , fvhPtmRapSecGenTrkTofPnt()
  , fvhPlabSecGenTrkTofPnt()
  , fvhPlabSecStsTrkTofPnt()
{
}
// ------------------------------------------------------------------

// ------------------------------------------------------------------
CbmTofGeometryQa::~CbmTofGeometryQa()
{
  // Destructor
}
// ------------------------------------------------------------------
/************************************************************************************/
// FairTasks inherited functions
InitStatus CbmTofGeometryQa::Init()
{
  if (kFALSE == RegisterInputs()) return kFATAL;

  // Initialize the TOF GeoHandler
  Bool_t isSimulation = kFALSE;
  Int_t iGeoVersion   = fGeoHandler->Init(isSimulation);
  LOG(info) << "CbmTofGeometryQa::Init with GeoVersion " << iGeoVersion;

  if (k12b > iGeoVersion) {
    LOG(error) << "CbmTofGeometryQa::Init => Only compatible with geometries "
                  "after v12b !!!";
    return kFATAL;
  }  // if( k12b > iGeoVersion )

  if (NULL != fTofId) LOG(info) << "CbmTofGeometryQa::Init with GeoVersion " << fGeoHandler->GetGeoVersion();
  else {
    switch (iGeoVersion) {
      case k12b: fTofId = new CbmTofDetectorId_v12b(); break;
      case k14a: fTofId = new CbmTofDetectorId_v14a(); break;
      default: LOG(error) << "CbmTofGeometryQa::Init => Invalid geometry!!!" << iGeoVersion; return kFATAL;
    }  // switch(iGeoVersion)
  }    // else of if(NULL != fTofId)

  if (kFALSE == LoadGeometry()) return kFATAL;

  if (kFALSE == CreateHistos()) return kFATAL;

  return kSUCCESS;
}

void CbmTofGeometryQa::SetParContainers()
{
  LOG(info) << " CbmTofGeometryQa => Get the digi parameters for tof";

  // Get Base Container
  /*
   FairRunAna* ana = FairRunAna::Instance();
   FairRuntimeDb* rtdb=ana->GetRuntimeDb();
*/
}

void CbmTofGeometryQa::Exec(Option_t* /*option*/)
{
  // Task execution

  LOG(debug) << " CbmTofGeometryQa => New event";

  FillHistos();

  if (0 == (fEvents % 1000) && 0 < fEvents) {
    LOG(info) << "CbmTofGeometryQa::Exec : "
              << "event " << fEvents << " processed." << endl;
  }
  fEvents += 1;
}

void CbmTofGeometryQa::Finish()
{
  // Normalisations
  cout << "CbmTofGeometryQa::Finish up with " << fEvents << " analyzed events " << endl;

  WriteHistos();
  // Prevent them from being sucked in by the CbmHadronAnalysis WriteHistograms method
  DeleteHistos();
}

/************************************************************************************/
// Functions common for all clusters approximations
Bool_t CbmTofGeometryQa::RegisterInputs()
{
  FairRootManager* fManager = FairRootManager::Instance();

  fMCEventHeader = (FairMCEventHeader*) fManager->GetObject("MCEventHeader.");
  if (NULL == fMCEventHeader) {
    LOG(error) << "CbmTofGeometryQa::RegisterInputs => Could not get the "
                  "MCEventHeader object!!!";
    return kFALSE;
  }

  fTofPointsColl = (TClonesArray*) fManager->GetObject("TofPoint");
  if (NULL == fTofPointsColl) {
    LOG(error) << "CbmTofGeometryQa::RegisterInputs => Could not get the "
                  "TofPoint TClonesArray!!!";
    return kFALSE;
  }  // if( NULL == fTofPointsColl)

  fMcTracksColl = (TClonesArray*) fManager->GetObject("MCTrack");
  if (NULL == fMcTracksColl) {
    LOG(error) << "CbmTofGeometryQa::RegisterInputs => Could not get the "
                  "MCTrack TClonesArray!!!";
    return kFALSE;
  }  // if( NULL == fMcTracksColl)

  fRealTofPointsColl = (TClonesArray*) fManager->GetObject("RealisticTofPoint");
  fRealTofMatchColl  = (TClonesArray*) fManager->GetObject("TofRealPntMatch");
  if (NULL != fRealTofPointsColl && NULL != fRealTofMatchColl) {
    fbRealPointAvail = kTRUE;
    LOG(info) << "CbmTofGeometryQa::RegisterInputs => Both fRealTofPointsColl & "
                 "fRealTofMatchColl there, realistic mean TOF MC point used for QA";
  }  // if( NULL != fRealTofPointsColl && NULL != fRealTofMatchColl )

  return kTRUE;
}
/************************************************************************************/
Bool_t CbmTofGeometryQa::LoadGeometry()
{
  /*
     Type 0: 5 RPC/SM,  24 SM, 32 ch/RPC =>  3840 ch          , 120 RPC         ,
     Type 1: 5 RPC/SM, 142 SM, 32 ch/RPC => 22720 ch => 26560 , 710 RPC =>  830 , => 166
     Type 3: 3 RPC/SM,  50 SM, 56 ch/RPC =>  8400 ch => 34960 , 150 RPC =>  980 , => 216
     Type 4: 5 RPC/SM,   8 SM, 96 ch/RPC =>  3840 ch => 38800 ,  40 RPC => 1020 , => 224
     Type 5: 5 RPC/SM,   8 SM, 96 ch/RPC =>  3840 ch => 42640 ,  40 RPC => 1060 , => 232
     Type 6: 2 RPC/SM,  10 SM, 96 ch/RPC =>  1920 ch => 44560 ,  20 RPC => 1080 , => 242
   */
  /*
   // Count the total number of channels and
   // generate an array with the global channel index of the first channe in each RPC
   Int_t iNbSmTypes = fDigiBdfPar->GetNbSmTypes();
   fvTypeSmOffs.resize( iNbSmTypes );
   fvSmRpcOffs.resize( iNbSmTypes );
   fvRpcChOffs.resize( iNbSmTypes );
   iNbSmTot  = 0;
   iNbRpcTot = 0;
   fiNbChTot  = 0;
   for( Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++ )
   {
      Int_t iNbSm  = fDigiBdfPar->GetNbSm(  iSmType);
      Int_t iNbRpc = fDigiBdfPar->GetNbRpc( iSmType);

      fvTypeSmOffs[iSmType] = iNbSmTot;
      iNbSmTot += iNbSm;

      fvSmRpcOffs[iSmType].resize( iNbSm );
      fvRpcChOffs[iSmType].resize( iNbSm );

      for( Int_t iSm = 0; iSm < iNbSm; iSm++ )
      {
         fvSmRpcOffs[iSmType][iSm] = iNbRpcTot;
         iNbRpcTot += iNbRpc;

         fvRpcChOffs[iSmType][iSm].resize( iNbRpc );
         for( Int_t iRpc = 0; iRpc < iNbRpc; iRpc++ )
         {
            fvRpcChOffs[iSmType][iSm][iRpc] = fiNbChTot;
            fiNbChTot += fDigiBdfPar->GetNbChan( iSmType, iRpc );
         } // for( Int_t iRpc = 0; iRpc < iNbRpc; iRpc++ )
      } // for( Int_t iSm = 0; iSm < iNbSm; iSm++ )
   } // for( Int_t iSmType = 0; iSmType < iNbSmTypes; iSmType++ )
*/
  return kTRUE;
}
/************************************************************************************/
// ------------------------------------------------------------------
Bool_t CbmTofGeometryQa::SetWallPosZ(Double_t dWallPosCm)
{
  fdWallPosZ = dWallPosCm;
  LOG(info) << "CbmTofGeometryQa::SetWallPosZ => Change histograms center on Z axis to " << dWallPosCm << " cm";
  return kTRUE;
}
Bool_t CbmTofGeometryQa::CreateHistos()
{
  // Create histogramms

  TDirectory* oldir = gDirectory;  // <= To prevent histos from being sucked in by the param file of the TRootManager!
  gROOT->cd();                     // <= To prevent histos from being sucked in by the param file of the TRootManager !
                                   /*
   Double_t ymin=-1.;
   Double_t ymax=4.;
   Double_t ptmmax=2.5;
   Int_t    ptm_nbx=30;
   Int_t    ptm_nby=30;

   Double_t v1_nbx=20.;
   Double_t v1_nby=20.;
   Double_t yvmax=1.3;
*/
  // xy - hit densities and rates
  Int_t nbinx     = 1500;
  Int_t nbiny     = 1000;
  Int_t nbinz     = 1500;
  Double_t xrange = 750.;
  Double_t yrange = 500.;
  Double_t zmin   = fdWallPosZ - 50.;
  Double_t zmax   = fdWallPosZ + 200.;

  // angular densities for overlap check
  Int_t iNbBinThetaX  = 1200;
  Double_t dThetaXMin = -60.0;
  Double_t dThetaXMax = 60.0;
  Int_t iNbBinThetaY  = 900;
  Double_t dThetaYMin = -45.0;
  Double_t dThetaYMax = 45.0;

  Int_t iNbBinTheta  = 180;
  Double_t dThetaMin = 0;
  Double_t dThetaMax = TMath::Pi() * 90 / 180;
  Int_t iNbBinPhi    = 180;
  Double_t dPhiMin   = -TMath::Pi();
  Double_t dPhiMax   = TMath::Pi();

  // Mapping
  // Dependence of Track origin on centrality
  Int_t iNbBinsStartZ  = 1250;
  Double_t dMinStartZ  = -50.0;
  Double_t dMaxStartZ  = 1200.0;
  Int_t iNbBinsStartXY = 1200;
  Double_t dMinStartXY = -600.0;
  Double_t dMaxStartXY = 600.0;
  Int_t iNbBinsCentr   = 16;
  Double_t dNbMinCentr = 0.0;
  Double_t dNbMaxCentr = 16.0;

  if (kTRUE == fbCentDepOn) {
    fvhTrackAllStartZCent.resize(kiNbPart);
    fvhTrackSecStartZCent.resize(kiNbPart);
    fvhTrackAllStartXZCent.resize(kiNbPart);
    fvhTofPntAllAngCent.resize(kiNbPart);
  }  // if( kTRUE == fbCentDepOn )
  fvhTrackAllStartXZ.resize(kiNbPart);
  fvhTrackAllStartYZ.resize(kiNbPart);
  for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
    // Track origin for tracks reaching TOF
    if (kTRUE == fbCentDepOn) {
      fvhTrackAllStartZCent[iPartIdx] =
        new TH2D(Form("TofTests_TrackAllStartZCent_%s", ksPartTag[iPartIdx].Data()),
                 Form("Centrality vs Start Z distribution for MC tracks w/ TOF Pnt, %s, "
                      "all tracks; Start Z [cm]; B [fm]; # []",
                      ksPartName[iPartIdx].Data()),
                 iNbBinsStartZ, dMinStartZ, dMaxStartZ, iNbBinsCentr, dNbMinCentr, dNbMaxCentr);
      fvhTrackSecStartZCent[iPartIdx] =
        new TH2D(Form("TofTests_TrackSecStartZCent_%s", ksPartTag[iPartIdx].Data()),
                 Form("Centrality vs Start Z distribution for MC tracks w/ TOF Pnt, %s, "
                      "secondary tracks; Start Z [cm]; B [fm]; # []",
                      ksPartName[iPartIdx].Data()),
                 iNbBinsStartZ, dMinStartZ, dMaxStartZ, iNbBinsCentr, dNbMinCentr, dNbMaxCentr);
      if (2 == iPartIdx)  // 3D plot only for e-
        fvhTrackAllStartXZCent[iPartIdx] = new TH3D(Form("TofTests_TrackAllStartXZCent_%s", ksPartTag[iPartIdx].Data()),
                                                    Form("Centrality vs Start Z distribution for MC tracks w/ TOF Pnt, "
                                                         "%s, all tracks; Start X [cm]; Start Z [cm]; B [fm];",
                                                         ksPartName[iPartIdx].Data()),
                                                    iNbBinsStartXY / 2, dMinStartXY, dMaxStartXY, iNbBinsStartZ / 2,
                                                    dMinStartZ, dMaxStartZ, iNbBinsCentr, dNbMinCentr, dNbMaxCentr);
      fvhTofPntAllAngCent[iPartIdx] = new TH3D(Form("TofTests_TofPntAllAngCent_%s", ksPartTag[iPartIdx].Data()),
                                               Form("Centrality vs Angular position of TOF Pnt, %s, all tracks; "
                                                    "#theta_{x}[Deg.]; #theta_{y}[Deg.]; B [fm];",
                                                    ksPartName[iPartIdx].Data()),
                                               iNbBinThetaX / 2, dThetaXMin, dThetaXMax, iNbBinThetaY / 2, dThetaYMin,
                                               dThetaYMax, iNbBinsCentr, dNbMinCentr, dNbMaxCentr);
    }  // if( kTRUE == fbCentDepOn )
    fvhTrackAllStartXZ[iPartIdx] =
      new TH2D(Form("TofTests_TrackAllStartXZ_%s", ksPartTag[iPartIdx].Data()),
               Form("Start X vs Z distribution for MC tracks w/ TOF Pnt, %s, "
                    "all tracks; Start Z [cm]; Start X [cm]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsStartZ / 2, dMinStartZ, dMaxStartZ, iNbBinsStartXY, dMinStartXY, dMaxStartXY);
    fvhTrackAllStartYZ[iPartIdx] =
      new TH2D(Form("TofTests_TrackAllStartYZ_%s", ksPartTag[iPartIdx].Data()),
               Form("Start Y vs Z distribution for MC tracks w/ TOF Pnt, %s, "
                    "all tracks; Start Z [cm]; Start Y [cm]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsStartZ / 2, dMinStartZ, dMaxStartZ, iNbBinsStartXY, dMinStartXY, dMaxStartXY);
  }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)

  // tracks
  fhTrackMapXY =
    new TH2D("TofTests_TracksMapXY", "Position of the MC Tracks assuming along Z axis; X[cm]; Y[cm]; # [Tracks]", nbinx,
             -xrange, xrange, nbiny, -yrange, yrange);
  fhTrackMapXZ =
    new TH2D("TofTests_TracksMapXZ", "Position of the MC Tracks assuming along Z axis; X[cm]; Z[cm]; # [Tracks]", nbinx,
             -xrange, xrange, nbinz, zmin, zmax);
  fhTrackMapYZ =
    new TH2D("TofTests_TracksMapYZ", "Position of the MC Tracks assuming along Z axis; Y[cm]; Z[cm]; # [Tracks]", nbiny,
             -yrange, yrange, nbinz, zmin, zmax);
  fhTrackMapAng = new TH2D("TofTests_TracksMapAng",
                           "Position of the MC Tracks assuming from origin; "
                           "#theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Tracks]",
                           iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);
  fhTrackMapSph = new TH2D("TofTests_TracksMapSph",
                           "Position of the MC Tracks assuming from origin; "
                           "#theta[rad.]; #phi[rad.]; # [Tracks]",
                           iNbBinTheta, dThetaMin, dThetaMax, iNbBinPhi, dPhiMin, dPhiMax);
  // Tracks in angular coordinates with detectors points, For angular acceptance study
  fhTrackMapAngPrimAll  = new TH2D("TofTests_TracksMapAngPrimAll",
                                  "Position of the MC Tracks assuming from origin, primary only; "
                                  "#theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Tracks]",
                                  iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);
  fhTrackMapAngPrimSts  = new TH2D("TofTests_TracksMapAngPrimSts",
                                  "Position of the MC Tracks w/ STS Pnts assuming from origin, "
                                  "primary only; #theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Tracks]",
                                  iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);
  fhTrackMapAngPrimRich = new TH2D("TofTests_TracksMapAngPrimRich",
                                   "Position of the MC Tracks w/ RICH Pnts assuming from origin, "
                                   "primary only; #theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Tracks]",
                                   iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);
  fhTrackMapAngPrimMuch = new TH2D("TofTests_TracksMapAngPrimMuch",
                                   "Position of the MC Tracks w/ MUCH Pnts assuming from origin, "
                                   "primary only; #theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Tracks]",
                                   iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);
  fhTrackMapAngPrimTrd  = new TH2D("TofTests_TracksMapAngPrimTrd",
                                  "Position of the MC Tracks w/ TRD Pnts  assuming from origin, "
                                  "primary only; #theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Tracks]",
                                  iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);
  fhTrackMapAngPrimTof  = new TH2D("TofTests_TracksMapAngPrimTof",
                                  "Position of the MC Tracks w/ TOF Pnts assuming from origin, "
                                  "primary only; #theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Tracks]",
                                  iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);
  // points
  fhPointMapXY  = new TH2D("TofTests_PointsMapXY", "Position of the Tof Points; X[cm]; Y[cm]; # [Points]", nbinx,
                          -xrange, xrange, nbiny, -yrange, yrange);
  fhPointMapXZ  = new TH2D("TofTests_PointsMapXZ", "Position of the Tof Points; X[cm]; Z[cm]; # [Points]", nbinx,
                          -xrange, xrange, nbinz, zmin, zmax);
  fhPointMapYZ  = new TH2D("TofTests_PointsMapYZ", "Position of the Tof Points; Y[cm]; Z[cm]; # [Points]", nbiny,
                          -yrange, yrange, nbinz, zmin, zmax);
  fhPointMapAng = new TH2D("TofTests_PointsMapAng",
                           "Position of the Tof Points; #theta_{x}[Deg.]; "
                           "#theta_{y}[Deg.]; # [Points]",
                           iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);
  fhPointMapSph = new TH2D("TofTests_PointsMapSph", "Position of the Tof Points; #theta[rad.]; #phi[rad.]; # [Points]",
                           iNbBinTheta, dThetaMin, dThetaMax, iNbBinPhi, dPhiMin, dPhiMax);
  // real (mean over all gaps) points
  fhRealPointMapXY =
    new TH2D("TofTests_RealPointsMapXY", "Position of the Tof Points (mean o/ gaps); X[cm]; Y[cm]; # [Points]", nbinx,
             -xrange, xrange, nbiny, -yrange, yrange);
  fhRealPointMapXZ =
    new TH2D("TofTests_RealPointsMapXZ", "Position of the Tof Points (mean o/ gaps); X[cm]; Z[cm]; # [Points]", nbinx,
             -xrange, xrange, nbinz, zmin, zmax);
  fhRealPointMapYZ =
    new TH2D("TofTests_RealPointsMapYZ", "Position of the Tof Points (mean o/ gaps); Y[cm]; Z[cm]; # [Points]", nbiny,
             -yrange, yrange, nbinz, zmin, zmax);
  fhRealPointMapAng = new TH2D("TofTests_RealPointsMapAng",
                               "Position of the Tof Points (mean o/ gaps); "
                               "#theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Points]",
                               iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);
  fhRealPointMapSph = new TH2D("TofTests_RealPointsMapSph",
                               "Position of the Tof Points (mean o/ gaps); "
                               "#theta[rad.]; #phi[rad.]; # [Points]",
                               iNbBinTheta, dThetaMin, dThetaMax, iNbBinPhi, dPhiMin, dPhiMax);
  // Pints in angular coordinates with other detectors points in track, For angular acceptance study
  fhPointMapAngWithSts  = new TH2D("TofTests_PointsMapAngWithSts",
                                  "Position of the Tof Points with enough STS points in Track; "
                                  "#theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Points]",
                                  iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);
  fhPointMapAngWithRich = new TH2D("TofTests_PointsMapAngWithRich",
                                   "Position of the Tof Points with RICH points in Track; "
                                   "#theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Points]",
                                   iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);
  fhPointMapAngWithMuch = new TH2D("TofTests_PointsMapAngWithMuch",
                                   "Position of the Tof Points with enough MUCH points in Track; "
                                   "#theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Points]",
                                   iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);
  fhPointMapAngWithTrd  = new TH2D("TofTests_PointsMapAngWithTrd",
                                  "Position of the Tof Points with TRD points in Track; "
                                  "#theta_{x}[Deg.]; #theta_{y}[Deg.]; # [Points]",
                                  iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);

  // Errors relative to spherical approx
  if (kTRUE == fbSphAppOn) {
    // Radius error (distance from target)
    fhPointSphAprRadiusErrMapXY = new TProfile2D("TofTests_PointSphAprRadiusErrMapXY",
                                                 "Radius error (distance from target) in spherical approx. "
                                                 "(mean o/ gaps); X[cm]; Y[cm]; Rp - Rwall [cm]",
                                                 nbinx, -xrange, xrange, nbiny, -yrange, yrange);
    fhPointSphAprRadiusErrMapXZ = new TProfile2D("TofTests_PointSphAprRadiusErrMapXZ",
                                                 "Radius error (distance from target) in spherical approx. "
                                                 "(mean o/ gaps); X[cm]; Z[cm]; Rp - Rwall [cm]",
                                                 nbinx, -xrange, xrange, nbinz, zmin, zmax);
    fhPointSphAprRadiusErrMapYZ = new TProfile2D("TofTests_PointSphAprRadiusErrMapYZ",
                                                 "Radius error (distance from target) in spherical approx. "
                                                 "(mean o/ gaps); Y[cm]; Z[cm]; Rp - Rwall [cm]",
                                                 nbiny, -yrange, yrange, nbinz, zmin, zmax);
    fhPointSphAprRadiusErrMapAng =
      new TProfile2D("TofTests_PointSphAprRadiusErrMapAng",
                     "Radius error (distance from target) in spherical approx. (mean o/ "
                     "gaps); #theta_{x}[Deg.]; #theta_{y}[Deg.]; Rp - Rwall [cm]",
                     iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);
    fhPointSphAprRadiusErrMapSph = new TProfile2D("TofTests_PointSphAprRadiusErrMapSph",
                                                  "Radius error (distance from target) in spherical approx. (mean o/ "
                                                  "gaps); #theta[rad.]; #phi[rad.]; Rp - Rwall [cm]",
                                                  iNbBinTheta, dThetaMin, dThetaMax, iNbBinPhi, dPhiMin, dPhiMax);
    // Z position error
    fhPointSphAprZposErrMapXY = new TProfile2D("TofTests_PointSphAprZposErrMapXY",
                                               "Z Position error of the Tof Points in spherical approx. "
                                               "(mean o/ gaps); X[cm]; Y[cm]; Zp - Zsph(X,Y) [cm]",
                                               nbinx, -xrange, xrange, nbiny, -yrange, yrange);
    fhPointSphAprZposErrMapXZ = new TProfile2D("TofTests_PointSphAprZposErrMapXZ",
                                               "Position of the Tof Points in spherical approx. (mean o/ "
                                               "gaps); X[cm]; Z[cm]; Zp - Zsph(X,Y) [cm]",
                                               nbinx, -xrange, xrange, nbinz, zmin, zmax);
    fhPointSphAprZposErrMapYZ = new TProfile2D("TofTests_PointSphAprZposErrMapYZ",
                                               "Position of the Tof Points in spherical approx. (mean o/ "
                                               "gaps); Y[cm]; Z[cm]; Zp - Zsph(X,Y) [cm]",
                                               nbiny, -yrange, yrange, nbinz, zmin, zmax);
    fhPointSphAprZposErrMapAng =
      new TProfile2D("TofTests_PointSphAprZposErrMapAng",
                     "Position of the Tof Points in spherical approx. (mean o/ gaps); "
                     "#theta_{x}[Deg.]; #theta_{y}[Deg.]; Zp - Zsph(X,Y) [cm]",
                     iNbBinThetaX, dThetaXMin, dThetaXMax, iNbBinThetaY, dThetaYMin, dThetaYMax);
    fhPointSphAprZposErrMapSph = new TProfile2D("TofTests_PointSphAprZposErrMapSph",
                                                "Position of the Tof Points in spherical approx. (mean o/ "
                                                "gaps); #theta[rad.]; #phi[rad.]; Zp - Zsph(X,Y) [cm]",
                                                iNbBinTheta, dThetaMin, dThetaMax, iNbBinPhi, dPhiMin, dPhiMax);
  }  // if( kTRUE == fbSphAppOn )

  // Physics coord mapping, 1 per particle type
  // Phase space
  Int_t iNbBinsY    = 30;
  Double_t dMinY    = -1.;
  Double_t dMaxY    = 4.;
  Int_t iNbBNinsPtm = 30;
  Double_t dMinPtm  = 0.0;
  Double_t dMaxPtm  = 2.5;
  fvhPtmRapGenTrk.resize(kiNbPart);
  fvhPtmRapStsPnt.resize(kiNbPart);
  fvhPtmRapTofPnt.resize(kiNbPart);
  fvhPtmRapSecGenTrk.resize(kiNbPart);
  fvhPtmRapSecStsPnt.resize(kiNbPart);
  fvhPtmRapSecTofPnt.resize(kiNbPart);
  // PLab
  Int_t iNbBinsPlab = 100;
  Double_t dMinPlab = 0.0;
  Double_t dMaxPlab = 10.0;
  fvhPlabGenTrk.resize(kiNbPart);
  fvhPlabStsPnt.resize(kiNbPart);
  fvhPlabTofPnt.resize(kiNbPart);
  fvhPlabSecGenTrk.resize(kiNbPart);
  fvhPlabSecStsPnt.resize(kiNbPart);
  fvhPlabSecTofPnt.resize(kiNbPart);
  // MC Tracks losses
  fvhPtmRapGenTrkTofPnt.resize(kiNbPart);
  fvhPlabGenTrkTofPnt.resize(kiNbPart);
  fvhPlabStsTrkTofPnt.resize(kiNbPart);
  fvhPtmRapSecGenTrkTofPnt.resize(kiNbPart);
  fvhPlabSecGenTrkTofPnt.resize(kiNbPart);
  fvhPlabSecStsTrkTofPnt.resize(kiNbPart);
  for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
    // Phase space
    fvhPtmRapGenTrk[iPartIdx] = new TH2D(Form("TofTests_PtmRapGenTrk_%s", ksPartTag[iPartIdx].Data()),
                                         Form("P_{t}/M vs y distribution for MC tracks, %s, primary "
                                              "tracks; y; P_{t}/M; # []",
                                              ksPartName[iPartIdx].Data()),
                                         iNbBinsY, dMinY, dMaxY, iNbBNinsPtm, dMinPtm, dMaxPtm);
    fvhPtmRapStsPnt[iPartIdx] = new TH2D(Form("TofTests_PtmRapStsPnt_%s", ksPartTag[iPartIdx].Data()),
                                         Form("P_{t}/M vs y distribution from MC Track with STS points, "
                                              "%s, primary tracks; y; P_{t}/M; # []",
                                              ksPartName[iPartIdx].Data()),
                                         iNbBinsY, dMinY, dMaxY, iNbBNinsPtm, dMinPtm, dMaxPtm);
    fvhPtmRapTofPnt[iPartIdx] = new TH2D(Form("TofTests_PtmRapTofPnt_%s", ksPartTag[iPartIdx].Data()),
                                         Form("P_{t}/M vs y distribution from MC Track for TOF points, "
                                              "%s, primary tracks; y; P_{t}/M; # []",
                                              ksPartName[iPartIdx].Data()),
                                         iNbBinsY, dMinY, dMaxY, iNbBNinsPtm, dMinPtm, dMaxPtm);
    // PLab
    fvhPlabGenTrk[iPartIdx] = new TH1D(Form("TofTests_PlabGenTrk_%s", ksPartTag[iPartIdx].Data()),
                                       Form("P_{lab} distribution for MC tracks, %s, primary tracks; "
                                            "P_{lab} [GeV/c]; # []",
                                            ksPartName[iPartIdx].Data()),
                                       iNbBinsPlab, dMinPlab, dMaxPlab);
    fvhPlabStsPnt[iPartIdx] = new TH1D(Form("TofTests_PlabStsPnt_%s", ksPartTag[iPartIdx].Data()),
                                       Form("P_{lab} distribution from MC Track with STS points, %s, "
                                            "primary tracks; P_{lab} [GeV/c]; # []",
                                            ksPartName[iPartIdx].Data()),
                                       iNbBinsPlab, dMinPlab, dMaxPlab);
    fvhPlabTofPnt[iPartIdx] = new TH1D(Form("TofTests_PlabTofPnt_%s", ksPartTag[iPartIdx].Data()),
                                       Form("P_{lab} distribution from MC Track for TOF points, %s, "
                                            "primary tracks; P_{lab} [GeV/c]; # []",
                                            ksPartName[iPartIdx].Data()),
                                       iNbBinsPlab, dMinPlab, dMaxPlab);


    // MC Tracks losses
    fvhPtmRapGenTrkTofPnt[iPartIdx] = new TH2D(Form("TofTests_PtmRapGenTrkTofPnt_%s", ksPartTag[iPartIdx].Data()),
                                               Form("P_{t}/M vs y distribution for MC tracks with TOF Point(s), %s, "
                                                    "primary tracks; y; P_{t}/M; # []",
                                                    ksPartName[iPartIdx].Data()),
                                               iNbBinsY, dMinY, dMaxY, iNbBNinsPtm, dMinPtm, dMaxPtm);

    fvhPlabGenTrkTofPnt[iPartIdx] = new TH1D(Form("TofTests_PlabGenTrkTofPnt_%s", ksPartTag[iPartIdx].Data()),
                                             Form("P_{lab} distribution for MC tracks with TOF Point(s), %s, "
                                                  "primary tracks; P_{lab} [GeV/c]; # []",
                                                  ksPartName[iPartIdx].Data()),
                                             iNbBinsPlab, dMinPlab, dMaxPlab);
    fvhPlabStsTrkTofPnt[iPartIdx] = new TH1D(Form("TofTests_PlabStsTrkTofPnt_%s", ksPartTag[iPartIdx].Data()),
                                             Form("P_{lab} distribution for MC tracks with STS and TOF "
                                                  "Point(s), %s, primary tracks; P_{lab} [GeV/c]; # []",
                                                  ksPartName[iPartIdx].Data()),
                                             iNbBinsPlab, dMinPlab, dMaxPlab);

    // Secondary tracks
    // Phase space
    fvhPtmRapSecGenTrk[iPartIdx] = new TH2D(Form("TofTests_PtmRapSecGenTrk_%s", ksPartTag[iPartIdx].Data()),
                                            Form("P_{t}/M vs y distribution for MC tracks, %s, secondary "
                                                 "tracks; y; P_{t}/M; # []",
                                                 ksPartName[iPartIdx].Data()),
                                            iNbBinsY, dMinY, dMaxY, iNbBNinsPtm, dMinPtm, dMaxPtm);
    fvhPtmRapSecStsPnt[iPartIdx] = new TH2D(Form("TofTests_PtmRapSecStsPnt_%s", ksPartTag[iPartIdx].Data()),
                                            Form("P_{t}/M vs y distribution from MC Track with STS points, "
                                                 "%s, secondary tracks; y; P_{t}/M; # []",
                                                 ksPartName[iPartIdx].Data()),
                                            iNbBinsY, dMinY, dMaxY, iNbBNinsPtm, dMinPtm, dMaxPtm);
    fvhPtmRapSecTofPnt[iPartIdx] = new TH2D(Form("TofTests_PtmRapSecTofPnt_%s", ksPartTag[iPartIdx].Data()),
                                            Form("P_{t}/M vs y distribution from MC Track for TOF points, "
                                                 "%s, secondary tracks; y; P_{t}/M; # []",
                                                 ksPartName[iPartIdx].Data()),
                                            iNbBinsY, dMinY, dMaxY, iNbBNinsPtm, dMinPtm, dMaxPtm);
    // PLab
    fvhPlabSecGenTrk[iPartIdx] = new TH1D(Form("TofTests_PlabSecGenTrk_%s", ksPartTag[iPartIdx].Data()),
                                          Form("P_{lab} distribution for MC tracks, %s, secondary tracks; "
                                               "P_{lab} [GeV/c]; # []",
                                               ksPartName[iPartIdx].Data()),
                                          iNbBinsPlab, dMinPlab, dMaxPlab);
    fvhPlabSecStsPnt[iPartIdx] = new TH1D(Form("TofTests_PlabSecStsPnt_%s", ksPartTag[iPartIdx].Data()),
                                          Form("P_{lab} distribution from MC Track with STS points, %s, "
                                               "secondary tracks; P_{lab} [GeV/c]; # []",
                                               ksPartName[iPartIdx].Data()),
                                          iNbBinsPlab, dMinPlab, dMaxPlab);
    fvhPlabSecTofPnt[iPartIdx] = new TH1D(Form("TofTests_PlabSecTofPnt_%s", ksPartTag[iPartIdx].Data()),
                                          Form("P_{lab} distribution from MC Track for TOF points, %s, "
                                               "secondary tracks; P_{lab} [GeV/c]; # []",
                                               ksPartName[iPartIdx].Data()),
                                          iNbBinsPlab, dMinPlab, dMaxPlab);


    // MC Tracks losses
    fvhPtmRapSecGenTrkTofPnt[iPartIdx] = new TH2D(Form("TofTests_PtmRapSecGenTrkTofPnt_%s", ksPartTag[iPartIdx].Data()),
                                                  Form("P_{t}/M vs y distribution for MC tracks with TOF Point(s), %s, "
                                                       "secondary tracks; y; P_{t}/M; # []",
                                                       ksPartName[iPartIdx].Data()),
                                                  iNbBinsY, dMinY, dMaxY, iNbBNinsPtm, dMinPtm, dMaxPtm);

    fvhPlabSecGenTrkTofPnt[iPartIdx] = new TH1D(Form("TofTests_PlabSecGenTrkTofPnt_%s", ksPartTag[iPartIdx].Data()),
                                                Form("P_{lab} distribution for MC tracks with TOF Point(s), %s, "
                                                     "secondary tracks; P_{lab} [GeV/c]; # []",
                                                     ksPartName[iPartIdx].Data()),
                                                iNbBinsPlab, dMinPlab, dMaxPlab);

    fvhPlabSecStsTrkTofPnt[iPartIdx] =
      new TH1D(Form("TofTests_PlabSecStsTrkTofPnt_%s", ksPartTag[iPartIdx].Data()),
               Form("P_{lab} distribution for MC tracks with STS and TOF Point(s), %s, "
                    "secondary tracks; P_{lab} [GeV/c]; # []",
                    ksPartName[iPartIdx].Data()),
               iNbBinsPlab, dMinPlab, dMaxPlab);
  }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)

  gDirectory->cd(oldir->GetPath());  // <= To prevent histos from being sucked in by the param file of the TRootManager!

  return kTRUE;
}

// ------------------------------------------------------------------
Bool_t CbmTofGeometryQa::FillHistos()
{
  // Declare variables outside the loop
  CbmMCTrack* pMcTrk;
  CbmTofPoint* pTofPoint;

  Int_t iNbTracks, iNbTofPts, iNbTofRealPts;

  iNbTracks = fMcTracksColl->GetEntriesFast();
  iNbTofPts = fTofPointsColl->GetEntriesFast();
  if (kTRUE == fbRealPointAvail) iNbTofRealPts = fRealTofPointsColl->GetEntriesFast();
  else
    iNbTofRealPts = 0;

  // Tracks Info
  Int_t iNbTofTracks     = 0;
  Int_t iNbTofTracksPrim = 0;
  std::vector<Bool_t> vbTrackHasHit(iNbTracks, kFALSE);
  // Are MC tracks reconstructable in STS?
  //   std::vector< Bool_t > vbTrackStsRecOk( iNbTracks kFALSE);
  for (Int_t iTrkInd = 0; iTrkInd < iNbTracks; iTrkInd++) {
    pMcTrk = (CbmMCTrack*) fMcTracksColl->At(iTrkInd);

    // Is track reconstructable in STS
    /*
      UInt_t uNbStsPnts = pMcTrk->GetNPoints(ECbmModuleId::kSts);

         // True criterium is whether enough STS stations are crossed
         // but if already less STS points, can escape looping
      if( kiMinNbStsPntAcc <= uNbStsPnts )
      {
         std::vector< UInt_t > vStsStationsId();
         for( UInt_t uStsPntIdx = 0; uStsPntIdx < uNbStsPnts; uStsPntIdx++)
         {
            UInt_t uStation = CbmStsAddress::GetElementId(
               (dynamic_cast<CbmStsPoint*>fStsPointsColl->At( uStsPntIdx ))->GetDetectorID(), 1);
            if( kiMinNbStsPntAcc <= vStsStationsId.size() )
            {
               vbTrackStsRecOk[iTrkInd] = kTRUE;
               break;
            }
         } // for( UInt_t uStsPntIdx = 0; uStsPntIdx < uNbStsPnts; uStsPntIdx++)
      } // if( kiMinNbStsPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kSts) )
       */

    if (0 < pMcTrk->GetNPoints(ECbmModuleId::kTof)) {
      iNbTofTracks++;
      // Keep track of MC tracks with at least one TOF Point

      if (-1 == pMcTrk->GetMotherId()) { iNbTofTracksPrim++; }  // if( -1 == pMcTrk->GetMotherId() )
    }                                                           // if( 0 < pMcTrk->GetNPoints(ECbmModuleId::kTof) )

    // tracks mapping: Only when creating normalization histos
    // Assume only TOF in setup, no field (only straight tracks)
    // and all tracks reach TOF (protons)
    // XYZ mapping: assume tracks along Z axis
    if (pMcTrk->GetPz() == pMcTrk->GetP()) {
      fhTrackMapXY->Fill(pMcTrk->GetStartX(), pMcTrk->GetStartY());
      //            fhTrackMapXZ->Fill( pMcTrk->GetStartX(), fdWallPosZ ); // Not sure how to get Z here
      //            fhTrackMapYZ->Fill( pMcTrk->GetStartY(), fdWallPosZ ); // Not sure how to get Z here
    }  // if( pMcTrk->GetPz() == pMcTrk->GetP() )

    // Angular mapping: assume tracks all coming from origin and not necess. along Z axis
    // track with non zero Z momentum + non spectator (at least deflected in X or Y)
    if (0 != pMcTrk->GetPz() && ((0 != pMcTrk->GetPx()) || (0 != pMcTrk->GetPy()))) {
      Double_t dThetaX = TMath::ATan2(pMcTrk->GetPx(), pMcTrk->GetPz()) * 180.0 / TMath::Pi();
      Double_t dThetaY = TMath::ATan2(pMcTrk->GetPy(), pMcTrk->GetPz()) * 180.0 / TMath::Pi();

      fhTrackMapAng->Fill(dThetaX, dThetaY);

      // Primary tracks
      if (-1 == pMcTrk->GetMotherId()) {
        fhTrackMapAngPrimAll->Fill(dThetaX, dThetaY);

        if (kiMinNbStsPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kSts)) fhTrackMapAngPrimSts->Fill(dThetaX, dThetaY);

        if (0 < pMcTrk->GetNPoints(ECbmModuleId::kRich)) fhTrackMapAngPrimRich->Fill(dThetaX, dThetaY);

        if (kiMinNbMuchPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kMuch)) fhTrackMapAngPrimMuch->Fill(dThetaX, dThetaY);

        if (0 < pMcTrk->GetNPoints(ECbmModuleId::kTrd)) fhTrackMapAngPrimTrd->Fill(dThetaX, dThetaY);

        if (0 < pMcTrk->GetNPoints(ECbmModuleId::kTof)) fhTrackMapAngPrimTof->Fill(dThetaX, dThetaY);
      }  // if( -1 == pMcTrk->GetMotherId() )
    }    // if( 0 != pMcTrk->GetPz() && ( (0 != pMcTrk->GetPx() ) || (0 != pMcTrk->GetPy() ) ))

    // Spherical mapping: assume tracks all coming from origin and not necess. along Z axis
    if (0 != pMcTrk->GetPz() && 0 != pMcTrk->GetPx())
      fhTrackMapSph->Fill(TMath::ATan2(pMcTrk->GetPt(), pMcTrk->GetPz()),
                          TMath::ATan2(pMcTrk->GetPy(), pMcTrk->GetPx()));

    // Physics coord mapping, 1 per particle type
    Int_t iPdgCode = pMcTrk->GetPdgCode();
    Int_t iPartIdx = -1;
    for (Int_t iPart = 0; iPart < kiNbPart; iPart++)
      if (kiPartPdgCode[iPart] == iPdgCode) {
        iPartIdx = iPart;
        break;
      }  // if( kiPartPdgCode[iPart] == iPdgCode )
    if (-1 == iPartIdx) iPartIdx = 0;

    // Dependence of Track origin on centrality or position
    if (0 < pMcTrk->GetNPoints(ECbmModuleId::kTof)) {
      if (kTRUE == fbCentDepOn) {
        fvhTrackAllStartZCent[iPartIdx]->Fill(pMcTrk->GetStartZ(), fMCEventHeader->GetB());
        if (2 == iPartIdx)  // 3D plot only for e-
          fvhTrackAllStartXZCent[iPartIdx]->Fill(pMcTrk->GetStartX(), pMcTrk->GetStartZ(), fMCEventHeader->GetB());
      }  // if( kTRUE == fbCentDepOn )
      fvhTrackAllStartXZ[iPartIdx]->Fill(pMcTrk->GetStartZ(), pMcTrk->GetStartX());
      fvhTrackAllStartYZ[iPartIdx]->Fill(pMcTrk->GetStartZ(), pMcTrk->GetStartY());
    }

    if (-1 == pMcTrk->GetMotherId()) {
      // primary track
      // Phase space
      fvhPtmRapGenTrk[iPartIdx]->Fill(pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
      // PLab
      fvhPlabGenTrk[iPartIdx]->Fill(pMcTrk->GetP());
      // Do the same for tracks within STS acceptance
      if (kiMinNbStsPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kSts)) {
        fvhPtmRapStsPnt[iPartIdx]->Fill(pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
        fvhPlabStsPnt[iPartIdx]->Fill(pMcTrk->GetP());
      }  // if( 0 < pMcTrk->GetNPoints(ECbmModuleId::kSts) )
      // Do the same for tracks within STS acceptance
      if (0 < pMcTrk->GetNPoints(ECbmModuleId::kTof)) {
        fvhPtmRapGenTrkTofPnt[iPartIdx]->Fill(pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
        fvhPlabGenTrkTofPnt[iPartIdx]->Fill(pMcTrk->GetP());

        if (kiMinNbStsPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kSts))
          fvhPlabStsTrkTofPnt[iPartIdx]->Fill(pMcTrk->GetP());
      }  // if( 0 < pMcTrk->GetNPoints(ECbmModuleId::kTof) )
    }    // if( -1 == pMcTrk->GetMotherId() )
    else {
      // secondary track
      // Dependence of Track origin on centrality
      if (0 < pMcTrk->GetNPoints(ECbmModuleId::kTof) && kTRUE == fbCentDepOn)
        fvhTrackSecStartZCent[iPartIdx]->Fill(pMcTrk->GetStartZ(), fMCEventHeader->GetB());

      // Phase space
      fvhPtmRapSecGenTrk[iPartIdx]->Fill(pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
      // PLab
      fvhPlabSecGenTrk[iPartIdx]->Fill(pMcTrk->GetP());
      // Do the same for tracks within STS acceptance
      if (kiMinNbStsPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kSts)) {
        fvhPtmRapSecStsPnt[iPartIdx]->Fill(pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
        fvhPlabSecStsPnt[iPartIdx]->Fill(pMcTrk->GetP());
      }  // if( 0 < pMcTrk->GetNPoints(ECbmModuleId::kSts) )
      // Do the same for tracks within STS acceptance
      if (0 < pMcTrk->GetNPoints(ECbmModuleId::kTof)) {
        fvhPtmRapSecGenTrkTofPnt[iPartIdx]->Fill(pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
        fvhPlabSecGenTrkTofPnt[iPartIdx]->Fill(pMcTrk->GetP());

        if (kiMinNbStsPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kSts))
          fvhPlabSecStsTrkTofPnt[iPartIdx]->Fill(pMcTrk->GetP());
      }  // if( 0 < pMcTrk->GetNPoints(ECbmModuleId::kTof) )
    }    // else of if( -1 == pMcTrk->GetMotherId() )
  }      // for(Int_t iTrkInd = 0; iTrkInd < nMcTracks; iTrkInd++)

  // Loop over Points and map them?
  for (Int_t iPntInd = 0; iPntInd < iNbTofPts; iPntInd++) {
    // Get a pointer to the TOF point
    pTofPoint = (CbmTofPoint*) fTofPointsColl->At(iPntInd);
    // Get a pointer to the corresponding MC Track
    pMcTrk = (CbmMCTrack*) fMcTracksColl->At(pTofPoint->GetTrackID());

    // Obtain position
    TVector3 vPntPos;
    pTofPoint->Position(vPntPos);

    Double_t dX = vPntPos.X();
    Double_t dY = vPntPos.Y();
    Double_t dZ = vPntPos.Z();

    // tracks mapping: Only when creating normalization histos
    // Assume only TOF in setup, no field (only straight tracks)
    // and all tracks reach TOF (protons)
    // XYZ mapping: assume tracks along Z axis
    if (pMcTrk->GetPz() == pMcTrk->GetP() && pMcTrk->GetStartX() == dX && pMcTrk->GetStartY() == dY) {
      fhTrackMapXZ->Fill(dX, dZ);  // only way to get Z here?
      fhTrackMapYZ->Fill(dY, dZ);  // only way to get Z here?
    }                              // if( pMcTrk->GetPz() == pMcTrk->GetP() )

    fhPointMapXY->Fill(dX, dY);
    fhPointMapXZ->Fill(dX, dZ);
    fhPointMapYZ->Fill(dY, dZ);

    Double_t dThetaX = TMath::ATan2(dX, dZ) * 180.0 / TMath::Pi();
    Double_t dThetaY = TMath::ATan2(dY, dZ) * 180.0 / TMath::Pi();
    fhPointMapAng->Fill(dThetaX, dThetaY);

    if (-1 == pMcTrk->GetMotherId()) {
      // primary track
      if (kiMinNbStsPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kSts)) fhPointMapAngWithSts->Fill(dThetaX, dThetaY);

      if (0 < pMcTrk->GetNPoints(ECbmModuleId::kRich)) fhPointMapAngWithRich->Fill(dThetaX, dThetaY);

      if (kiMinNbMuchPntAcc <= pMcTrk->GetNPoints(ECbmModuleId::kMuch)) fhPointMapAngWithMuch->Fill(dThetaX, dThetaY);

      if (0 < pMcTrk->GetNPoints(ECbmModuleId::kTrd)) fhPointMapAngWithTrd->Fill(dThetaX, dThetaY);
    }  // if( -1 == pMcTrk->GetMotherId() )

    Double_t dTheta = TMath::ATan2(TMath::Sqrt(dX * dX + dY * dY), dZ);  // *180.0/TMath::Pi();
    Double_t dPhi   = TMath::ATan2(dY, dX);                              // *180.0/TMath::Pi();
    fhPointMapSph->Fill(dTheta, dPhi);

    // Errors relative to spherical approx
    if (kTRUE == fbSphAppOn) {
      // Radius error (distance from target)
      Double_t dSphereRadius = fdWallPosZ + 115;  // Make constant a user parameter!!!
      Double_t dRadiusError  = TMath::Sqrt(dX * dX + dY * dY + dZ * dZ) - dSphereRadius;
      fhPointSphAprRadiusErrMapXY->Fill(dX, dY, dRadiusError);
      fhPointSphAprRadiusErrMapXZ->Fill(dX, dZ, dRadiusError);
      fhPointSphAprRadiusErrMapYZ->Fill(dY, dZ, dRadiusError);
      fhPointSphAprRadiusErrMapAng->Fill(dThetaX, dThetaY, dRadiusError);
      fhPointSphAprRadiusErrMapSph->Fill(dTheta, dPhi, dRadiusError);
      // Z position error
      Double_t dZposErr =
        dZ - TMath::Sqrt(dX * dX + dY * dY) / TMath::Tan(TMath::ASin(TMath::Sqrt(dX * dX + dY * dY) / dSphereRadius));
      fhPointSphAprZposErrMapXY->Fill(dX, dY, dZposErr);
      fhPointSphAprZposErrMapXZ->Fill(dX, dZ, dZposErr);
      fhPointSphAprZposErrMapYZ->Fill(dY, dZ, dZposErr);
      fhPointSphAprZposErrMapAng->Fill(dThetaX, dThetaY, dZposErr);
      fhPointSphAprZposErrMapSph->Fill(dTheta, dPhi, dZposErr);
    }  // if( kTRUE == fbSphAppOn )

    // Physics coord mapping, 1 per particle type
    Int_t iPdgCode = pMcTrk->GetPdgCode();
    Int_t iPartIdx = -1;
    for (Int_t iPart = 0; iPart < kiNbPart; iPart++)
      if (kiPartPdgCode[iPart] == iPdgCode) {
        iPartIdx = iPart;
        break;
      }  // if( kiPartPdgCode[iPart] == iPdgCode )
    if (-1 == iPartIdx) iPartIdx = 0;

    // Beam pipe check
    if (kTRUE == fbCentDepOn) fvhTofPntAllAngCent[iPartIdx]->Fill(dThetaX, dThetaY, fMCEventHeader->GetB());

    if (-1 == pMcTrk->GetMotherId()) {
      // primary track
      // Phase space
      fvhPtmRapTofPnt[iPartIdx]->Fill(pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
      // PLab
      fvhPlabTofPnt[iPartIdx]->Fill(pMcTrk->GetP());
    }  // if( -1 == pMcTrk->GetMotherId() )
    else {
      // secondary track
      // Phase space
      fvhPtmRapSecTofPnt[iPartIdx]->Fill(pMcTrk->GetRapidity(), pMcTrk->GetPt() / pMcTrk->GetMass());
      // PLab
      fvhPlabSecTofPnt[iPartIdx]->Fill(pMcTrk->GetP());
    }  // else of if( -1 == pMcTrk->GetMotherId() )
  }    // for (Int_t iPntInd = 0; iPntInd < nTofPoint; iPntInd++ )

  // Loop over Real Points and map them, only in case of protons
  if (kTRUE == fbRealPointAvail)
    for (Int_t iPntInd = 0; iPntInd < iNbTofRealPts; iPntInd++) {
      // Get a pointer to the TOF point
      pTofPoint = (CbmTofPoint*) fRealTofPointsColl->At(iPntInd);
      // Get a pointer to the corresponding MC Track
      pMcTrk = (CbmMCTrack*) fMcTracksColl->At(pTofPoint->GetTrackID());

      // Physics coord mapping, 1 per particle type
      //	   Int_t iPdgCode = pMcTrk->GetPdgCode();
      //      if( 2212 == iPdgCode ) // Protons cut, comment to get all
      {
        // Obtain position
        TVector3 vPntPos;
        pTofPoint->Position(vPntPos);

        Double_t dX = vPntPos.X();
        Double_t dY = vPntPos.Y();
        Double_t dZ = vPntPos.Z();

        fhRealPointMapXY->Fill(dX, dY);
        fhRealPointMapXZ->Fill(dX, dZ);
        fhRealPointMapYZ->Fill(dY, dZ);

        Double_t dThetaX = TMath::ATan2(dX, dZ) * 180.0 / TMath::Pi();
        Double_t dThetaY = TMath::ATan2(dY, dZ) * 180.0 / TMath::Pi();
        fhRealPointMapAng->Fill(dThetaX, dThetaY);

        Double_t dTheta = TMath::ATan2(TMath::Sqrt(dX * dX + dY * dY),
                                       dZ);      // *180.0/TMath::Pi();
        Double_t dPhi   = TMath::ATan2(dY, dX);  // *180.0/TMath::Pi();
        fhRealPointMapSph->Fill(dTheta, dPhi);
      }  // if( 2212 == iPdgCode )
    }    // for (Int_t iPntInd = 0; iPntInd < iNbTofRealPts; iPntInd++ )

  return kTRUE;
}
// ------------------------------------------------------------------

Bool_t CbmTofGeometryQa::SetHistoFileName(TString sFilenameIn)
{
  fsHistoOutFilename = sFilenameIn;
  return kTRUE;
}
Bool_t CbmTofGeometryQa::WriteHistos()
{
  // TODO: add sub-folders ?

  /// Save old global file and folder pointer to avoid messing with FairRoot
  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  // Write histogramms to the file
  TFile* fHist = new TFile(fsHistoOutFilename, "RECREATE");
  fHist->cd();

  // Mapping
  TDirectory* cdGeomMap = fHist->mkdir("GeomMap");
  cdGeomMap->cd();  // make the "GeomMap" directory the current directory
  fhTrackMapXY->Write();
  fhTrackMapXZ->Write();
  fhTrackMapYZ->Write();
  fhTrackMapAng->Write();
  fhTrackMapSph->Write();

  fhTrackMapAngPrimAll->Write();
  fhTrackMapAngPrimSts->Write();
  fhTrackMapAngPrimRich->Write();
  fhTrackMapAngPrimMuch->Write();
  fhTrackMapAngPrimTrd->Write();
  fhTrackMapAngPrimTof->Write();

  fhPointMapXY->Write();
  fhPointMapXZ->Write();
  fhPointMapYZ->Write();
  fhPointMapAng->Write();
  fhPointMapSph->Write();
  fhRealPointMapXY->Write();
  fhRealPointMapXZ->Write();
  fhRealPointMapYZ->Write();
  fhRealPointMapAng->Write();
  fhRealPointMapSph->Write();

  fhPointMapAngWithSts->Write();
  fhPointMapAngWithRich->Write();
  fhPointMapAngWithMuch->Write();
  fhPointMapAngWithTrd->Write();

  // Errors relative to spherical approx
  if (kTRUE == fbSphAppOn) {
    TDirectory* cdSphApprox = fHist->mkdir("SphApprox");
    cdSphApprox->cd();  // make the "SphApprox" directory the current directory
    fhPointSphAprRadiusErrMapXY->Write();
    fhPointSphAprRadiusErrMapXZ->Write();
    fhPointSphAprRadiusErrMapYZ->Write();
    fhPointSphAprRadiusErrMapAng->Write();
    fhPointSphAprRadiusErrMapSph->Write();
    fhPointSphAprZposErrMapXY->Write();
    fhPointSphAprZposErrMapXZ->Write();
    fhPointSphAprZposErrMapYZ->Write();
    fhPointSphAprZposErrMapAng->Write();
    fhPointSphAprZposErrMapSph->Write();

    fHist->cd();  // make the file root the current directory
  }               // if( kTRUE == fbSphAppOn )

  TDirectory* cdStartZ = fHist->mkdir("StartZ");
  cdStartZ->cd();  // make the "MixMap" directory the current directory
                   // Dependence of Track origin on centrality
  for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
    if (kTRUE == fbCentDepOn) {
      fvhTrackAllStartZCent[iPartIdx]->Write();
      fvhTrackSecStartZCent[iPartIdx]->Write();
      if (2 == iPartIdx)  // 3D plot only for e-
        fvhTrackAllStartXZCent[iPartIdx]->Write();
      fvhTofPntAllAngCent[iPartIdx]->Write();
    }  // if( kTRUE == fbCentDepOn )
    fvhTrackAllStartXZ[iPartIdx]->Write();
    fvhTrackAllStartYZ[iPartIdx]->Write();
  }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)

  TDirectory* cdPhysMap = fHist->mkdir("PhysMap");
  cdPhysMap->cd();  // make the "PhysMap" directory the current directory
                    // Physics coord mapping, 1 per particle type
  for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
    // Phase space
    fvhPtmRapGenTrk[iPartIdx]->Write();
    fvhPtmRapStsPnt[iPartIdx]->Write();
    fvhPtmRapTofPnt[iPartIdx]->Write();
    // PLab
    fvhPlabGenTrk[iPartIdx]->Write();
    fvhPlabStsPnt[iPartIdx]->Write();
    fvhPlabTofPnt[iPartIdx]->Write();
    // MC Tracks losses
    fvhPtmRapGenTrkTofPnt[iPartIdx]->Write();
    fvhPlabGenTrkTofPnt[iPartIdx]->Write();
    fvhPlabStsTrkTofPnt[iPartIdx]->Write();
  }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)

  TDirectory* cdPhysMapSec = fHist->mkdir("PhysMapSec");
  cdPhysMapSec->cd();  // make the "PhysMap" directory the current directory
                       // Physics coord mapping, 1 per particle type
  for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
    // Phase space
    fvhPtmRapSecGenTrk[iPartIdx]->Write();
    fvhPtmRapSecStsPnt[iPartIdx]->Write();
    fvhPtmRapSecTofPnt[iPartIdx]->Write();
    // PLab
    fvhPlabSecGenTrk[iPartIdx]->Write();
    fvhPlabSecStsPnt[iPartIdx]->Write();
    fvhPlabSecTofPnt[iPartIdx]->Write();
    // MC Tracks losses
    fvhPtmRapSecGenTrkTofPnt[iPartIdx]->Write();
    fvhPlabSecGenTrkTofPnt[iPartIdx]->Write();
    fvhPlabSecStsTrkTofPnt[iPartIdx]->Write();
  }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)

  fHist->cd();  // make the file root the current directory

  /// Restore old global file and folder pointer to avoid messing with FairRoot
  gFile      = oldFile;
  gDirectory = oldDir;

  fHist->Close();

  return kTRUE;
}
Bool_t CbmTofGeometryQa::DeleteHistos()
{
  // Mapping
  // Physics coord mapping, 1 per particle type
  for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
    if (kTRUE == fbCentDepOn) {
      delete fvhTrackAllStartZCent[iPartIdx];
      delete fvhTrackSecStartZCent[iPartIdx];
      if (2 == iPartIdx)  // 3D plot only for e-
        delete fvhTrackAllStartXZCent[iPartIdx];
      delete fvhTofPntAllAngCent[iPartIdx];
    }  // if( kTRUE == fbCentDepOn )
    delete fvhTrackAllStartXZ[iPartIdx];
    delete fvhTrackAllStartYZ[iPartIdx];
  }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
  if (kTRUE == fbCentDepOn) {
    fvhTrackAllStartZCent.clear();
    fvhTrackSecStartZCent.clear();
    fvhTrackAllStartXZCent.clear();
    fvhTofPntAllAngCent.clear();
  }  // if( kTRUE == fbCentDepOn )
  fvhTrackAllStartXZ.clear();
  fvhTrackAllStartYZ.clear();

  delete fhTrackMapXY;
  delete fhTrackMapXZ;
  delete fhTrackMapYZ;
  delete fhTrackMapAng;
  delete fhTrackMapSph;

  delete fhTrackMapAngPrimAll;
  delete fhTrackMapAngPrimSts;
  delete fhTrackMapAngPrimRich;
  delete fhTrackMapAngPrimMuch;
  delete fhTrackMapAngPrimTrd;
  delete fhTrackMapAngPrimTof;

  delete fhPointMapXY;
  delete fhPointMapXZ;
  delete fhPointMapYZ;
  delete fhPointMapAng;
  delete fhPointMapSph;
  delete fhRealPointMapXY;
  delete fhRealPointMapXZ;
  delete fhRealPointMapYZ;
  delete fhRealPointMapAng;
  delete fhRealPointMapSph;

  delete fhPointMapAngWithSts;
  delete fhPointMapAngWithRich;
  delete fhPointMapAngWithMuch;
  delete fhPointMapAngWithTrd;

  if (kTRUE == fbSphAppOn) {
    delete fhPointSphAprRadiusErrMapXY;
    delete fhPointSphAprRadiusErrMapXZ;
    delete fhPointSphAprRadiusErrMapYZ;
    delete fhPointSphAprRadiusErrMapAng;
    delete fhPointSphAprRadiusErrMapSph;
    delete fhPointSphAprZposErrMapXY;
    delete fhPointSphAprZposErrMapXZ;
    delete fhPointSphAprZposErrMapYZ;
    delete fhPointSphAprZposErrMapAng;
    delete fhPointSphAprZposErrMapSph;
  }  // if( kTRUE == fbSphAppOn )

  // Physics coord mapping, 1 per particle type
  for (Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++) {
    // Phase space
    delete fvhPtmRapGenTrk[iPartIdx];
    delete fvhPtmRapStsPnt[iPartIdx];
    delete fvhPtmRapTofPnt[iPartIdx];
    // PLab
    delete fvhPlabGenTrk[iPartIdx];
    delete fvhPlabStsPnt[iPartIdx];
    delete fvhPlabTofPnt[iPartIdx];
    // MC Tracks losses
    delete fvhPtmRapGenTrkTofPnt[iPartIdx];
    delete fvhPlabGenTrkTofPnt[iPartIdx];

    // Secondary tracks
    // Phase space
    delete fvhPtmRapSecGenTrk[iPartIdx];
    delete fvhPtmRapSecStsPnt[iPartIdx];
    delete fvhPtmRapSecTofPnt[iPartIdx];
    // PLab
    delete fvhPlabSecGenTrk[iPartIdx];
    delete fvhPlabSecStsPnt[iPartIdx];
    delete fvhPlabSecTofPnt[iPartIdx];
    // MC Tracks losses
    delete fvhPtmRapSecGenTrkTofPnt[iPartIdx];
    delete fvhPlabSecGenTrkTofPnt[iPartIdx];
    delete fvhPlabSecStsTrkTofPnt[iPartIdx];
  }  // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
     // Phase space
  fvhPtmRapGenTrk.clear();
  fvhPtmRapStsPnt.clear();
  fvhPtmRapTofPnt.clear();
  // PLab
  fvhPlabGenTrk.clear();
  fvhPlabStsPnt.clear();
  fvhPlabTofPnt.clear();
  // MC Tracks losses
  fvhPtmRapGenTrkTofPnt.clear();
  fvhPlabGenTrkTofPnt.clear();
  fvhPlabStsTrkTofPnt.clear();

  // Secondary tracks
  // Phase space
  fvhPtmRapSecGenTrk.clear();
  fvhPtmRapSecStsPnt.clear();
  fvhPtmRapSecTofPnt.clear();
  // PLab
  fvhPlabSecGenTrk.clear();
  fvhPlabSecStsPnt.clear();
  fvhPlabSecTofPnt.clear();
  // MC Tracks losses
  fvhPtmRapSecGenTrkTofPnt.clear();
  fvhPlabSecGenTrkTofPnt.clear();
  fvhPlabSecStsTrkTofPnt.clear();

  return kTRUE;
}


ClassImp(CbmTofGeometryQa);
