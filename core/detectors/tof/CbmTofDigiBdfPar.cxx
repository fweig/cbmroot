/** @file CbmTofDigiBdfPar.cxx
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @date 01.08.2013
 **/
#include "CbmTofDigiBdfPar.h"

#include "CbmTofAddress.h"      // for CbmTofAddress

#include <FairParamList.h>      // for FairParamList
#include <FairParGenericSet.h>  // for FairParGenericSet
#include <FairLogger.h>         // for LOG, Logger

#include <TArrayD.h>            // for TArrayD
#include <TArrayI.h>            // for TArrayI
#include <TFitResultPtr.h>      // for TFitResultPtr
#include <TDirectory.h>         // for TDirectory, gDirectory
#include <TFile.h>              // for TFile
#include <TFitResult.h>         // for TFitResult
#include <TH1.h>                // for TH1
#include <TH2.h>                // for TH2D
#include <TMath.h>              // for Power, Sqrt
#include <TROOT.h>              // for TROOT, gROOT

ClassImp(CbmTofDigiBdfPar)

CbmTofDigiBdfPar::CbmTofDigiBdfPar( const char* name,
                                      const char* title,
                                      const char* context)
: FairParGenericSet(name,title,context),
  fbUseExpDigi(kTRUE),
  fbUseOnlyPrim(kFALSE),
  fbOneGapTrack(kTRUE),
  fiClusterModel(0),
  fdFeeGainSigma(0.0),
  fdFeeTotThr(0.0),
  fdTimeResElec(0.0),
  fdTimeResStart(0.0),
  fdDeadtime(5.0),
  fdSignalPropSpeed(0.0),
  fiNbSmTypes(0),
  fiNbSm(),
  fiNbRpc(),
  fiNbGaps(),
  fdGapSize(),
  fdSigVel(),
  fiNbCh(),
  fiChType(),
  fiChOrientation(),
  fiDetUId(),
  fMapDetInd(),
  fsBeamInputFile(""),
  fsBeamCalibFile(""),
  fiClusterRadiusModel(-1),
  fiSmTypeInpMapp(),
  fdEfficiency(),
  fdGapsEfficiency(),
  fdTimeResolution(),
  fh1ClusterSize(),
  fh1ClusterTot(),
  fdLandauMpv(),
  fdLandauSigma(),
  fbMulUseTrackId(kTRUE),
  fdMaxTimeDistClust(0.5),
  fdMaxSpaceDistClust(2.5)
{
   detName="Tof";
}

CbmTofDigiBdfPar::~CbmTofDigiBdfPar(void)
{
   LOG(DEBUG4)<<"Enter CbmTofDigiBdfPar destructor";
   clear();
   LOG(DEBUG4)<<"Leave CbmTofDigiBdfPar destructor";
}

void CbmTofDigiBdfPar::clear(void)
{
   status = kFALSE;
   resetInputVersions();
   ClearHistos();
}
void CbmTofDigiBdfPar::ClearHistos()
{
   for( UInt_t uSmType = 0; uSmType < fh1ClusterSize.size(); uSmType ++)
      delete fh1ClusterSize[uSmType];
   for( UInt_t uSmType = 0; uSmType < fh1ClusterTot.size(); uSmType ++)
      delete fh1ClusterTot[uSmType];
   fh1ClusterSize.clear();
   fh1ClusterTot.clear();
}

void CbmTofDigiBdfPar::putParams(FairParamList* l)
{
   if (!l) { return; }

   l->add("UseExpDigi",         (Int_t)fbUseExpDigi);
   l->add("UseOnlyPrim",        (Int_t)fbUseOnlyPrim);
   l->add("ClusterModel",       fiClusterModel);
   l->add("FeeGainSigma",       fdFeeGainSigma);
   l->add("FeeTotThr",             fdFeeTotThr);
   l->add("TimeResElec",         fdTimeResElec);
   l->add("TimeResStart",       fdTimeResStart);
   l->add("Deadtime",                fdDeadtime);
   l->add("SignalPropSpeed",    fdSignalPropSpeed);
   l->add("NbSmTypes",          fiNbSmTypes);
   l->add("NbSm",               fiNbSm);
   l->add("NbRpc",              fiNbRpc);
//   l->add("NbCh",               fiNbCh);
//   l->add("ChType",             fiChType);
//   l->add("ChOrientation",      fiChOrientation);
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
      l->add(Form("NbGaps%03d", iSmType), fiNbGaps[iSmType]);
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
      l->add(Form("GapSize%03d", iSmType), fdGapSize[iSmType]);
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
      l->add(Form("SigVel%03d", iSmType), fdSigVel[iSmType]);
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
      l->add(Form("NbCh%03d", iSmType), fiNbCh[iSmType]);
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
      l->add(Form("ChType%03d", iSmType), fiChType[iSmType]);
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
      l->add(Form("ChOrientation%03d", iSmType), fiChOrientation[iSmType]);
   l->add("BeamInputFile",      fsBeamInputFile);
   l->add("BeamCalibFile",      fsBeamCalibFile);
   l->add("ClusterRadiusModel", fiClusterRadiusModel);
   l->add("SmTypeInpMapp",      fiSmTypeInpMapp);
   if( 1 == fiClusterRadiusModel )
   {
      l->add("RadiusLandauMpv", fdLandauMpv);
      l->add("RadLandauSigma",  fdLandauSigma);
   } // if( 1 == fiClusterRadiusModel )
   l->add("MulUseTrkId",        (Int_t)fbUseOnlyPrim);
   l->add("MaxTimeDistClust",   fdMaxTimeDistClust);
   l->add("MaxSpaceDistClust",  fdMaxSpaceDistClust);
}

Bool_t CbmTofDigiBdfPar::getParams(FairParamList* l)
{
   if (!l) { return kFALSE; }

   LOG(debug2)<<"Get the tof digitization parameters.";

   Int_t iTemp;
   if ( ! l->fill("UseExpDigi",         &iTemp) ) return kFALSE;
   fbUseExpDigi = ( 1 == iTemp? kTRUE : kFALSE);

   if ( ! l->fill("UseOnlyPrim",        &iTemp) ) return kFALSE;
   fbUseOnlyPrim = ( 1 == iTemp? kTRUE : kFALSE);

   if ( ! l->fill("UseOneGapPerTrk",    &iTemp) ) return kFALSE;
   fbOneGapTrack = ( 1 == iTemp? kTRUE : kFALSE);

   if ( ! l->fill("ClusterModel",       &fiClusterModel) ) return kFALSE;
   if ( ! l->fill("FeeGainSigma",       &fdFeeGainSigma) ) return kFALSE;
   if ( ! l->fill("FeeTotThr",            &fdFeeTotThr) ) return kFALSE;
   if ( ! l->fill("TimeResElec",        &fdTimeResElec) ) return kFALSE;
   if ( ! l->fill("TimeResStart",       &fdTimeResStart) ) return kFALSE;
   if ( ! l->fill("Deadtime",               &fdDeadtime) ) 
     LOG(debug) <<  "Use default FEE deadtime of "<<fdDeadtime<<" ns ";
   if ( ! l->fill("SignalPropSpeed",    &fdSignalPropSpeed) ) return kFALSE;
   if ( ! l->fill("NbSmTypes",          &fiNbSmTypes) ) return kFALSE;

   LOG(debug2)<<"CbmTofDigiBdfPar => There are "<< fiNbSmTypes<<" types of SM to be initialized.";


   fiNbSm.Set(fiNbSmTypes);
   if ( ! l->fill("NbSm",               &fiNbSm) ) return kFALSE;

   fiNbRpc.Set(fiNbSmTypes);
   if ( ! l->fill("NbRpc",              &fiNbRpc) ) return kFALSE;

   fiNbGaps.resize(fiNbSmTypes);
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
   {
      fiNbGaps[iSmType].Set( fiNbRpc[iSmType] );
      if ( ! l->fill( Form("NbGaps%03d", iSmType), &(fiNbGaps[iSmType]) ) ) return kFALSE;
   } // for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)

   fdGapSize.resize(fiNbSmTypes);
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
   {
      fdGapSize[iSmType].Set( fiNbRpc[iSmType] );
      if ( ! l->fill( Form("GapSize%03d", iSmType), &(fdGapSize[iSmType]) ) ) return kFALSE;
   } // for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)

   fdSigVel.resize(fiNbSmTypes);
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
   {
      fdSigVel[iSmType].Set( fiNbSm[iSmType]*fiNbRpc[iSmType] );
      if ( ! l->fill( Form("SigVel%03d", iSmType), &(fdSigVel[iSmType]) ) )
      {
         LOG(WARNING)<<"CbmTofDigiBdfPar::getParams => parameter "
                     <<Form("SigVel%03d", iSmType)
                     <<" not found in the text file. "
                     <<"This is normal for geometries < 14a but may indicate file corruption "
                     <<" for newer geometries. Values are set to default 0.0 cm/ps."
                     ;
         for( Int_t iRpc = 0; iRpc < fiNbRpc[iSmType]; iRpc++)
            fdSigVel[iSmType].SetAt( 0.0, iRpc);
         //return kFALSE;
      } // if ( ! l->fill( Form("SigVel%03d", iSmType), &(fdSigVel[iSmType]) ) )

      if( (fdSigVel[iSmType]).GetSize() < fiNbSm[iSmType]*fiNbRpc[iSmType]){
         if( (fdSigVel[iSmType]).GetSize() == fiNbRpc[iSmType] )
         {
            LOG(info)<<Form("SigVel%03d Array size: %d < request %d, copy values to all modules ",iSmType,
                            (Int_t)(fdSigVel[iSmType]).GetSize(),fiNbSm[iSmType]*fiNbRpc[iSmType])
                     ;

            TArrayD temp ((fdSigVel[iSmType]).GetSize());  // temporary copy of data
            for(Int_t i=0; i<(fdSigVel[iSmType]).GetSize(); i++) temp[i]=fdSigVel[iSmType][i];

            fdSigVel[iSmType].Set( fiNbSm[iSmType]*fiNbRpc[iSmType] );
            for(Int_t iSm=0; iSm<fiNbSm[iSmType]; iSm++) { // fill elements for all SMs
               for(Int_t iRpc=0; iRpc<fiNbRpc[iSmType]; iRpc++)
                  fdSigVel[iSmType][iSm*fiNbRpc[iSmType]+iRpc]=temp[iRpc];
            }
         } // if( (fdSigVel[iSmType]).GetSize() == fiNbRpc[iSmType] )
         else
         {
            LOG(error) << "CbmTofDigiBdfPar::getParams => parameter "
                     << Form("SigVel%03d", iSmType)
                     << " has a not matching number of fields: "
                     << (fdSigVel[iSmType]).GetSize()
                     << " instead of "
                     << fiNbRpc[iSmType] << " or "
                     << fiNbSm[iSmType]*fiNbRpc[iSmType]
                     ;
            return kFALSE;
         } // else of if( (fdSigVel[iSmType]).GetSize() == fiNbRpc[iSmType] )
      }

   } // for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)

   fiNbCh.resize(fiNbSmTypes);
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
   {
      fiNbCh[iSmType].Set( fiNbRpc[iSmType] );
      if ( ! l->fill( Form("NbCh%03d", iSmType), &(fiNbCh[iSmType]) ) ) return kFALSE;
   } // for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)

   fiChType.resize(fiNbSmTypes);
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
   {
      fiChType[iSmType].Set( fiNbRpc[iSmType] );
      if ( ! l->fill( Form("ChType%03d", iSmType), &(fiChType[iSmType]) ) ) return kFALSE;
   } // for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)

   fiChOrientation.resize(fiNbSmTypes);
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
   {
      fiChOrientation[iSmType].Set( fiNbRpc[iSmType] );
      if ( ! l->fill( Form("ChOrientation%03d", iSmType), &(fiChOrientation[iSmType]) ) ) return kFALSE;
   } // for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)


   Int_t nDet=0;
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
   {
     nDet+=GetNbSm(iSmType)*GetNbRpc(iSmType);
   }

   fiDetUId.Set( nDet);    // dimension unique identifier array
   fMapDetInd.clear();  // reset

   Int_t iDet=0;
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
   {
    for(Int_t iSm=0; iSm<GetNbSm(iSmType); iSm++)
    {
      for(Int_t iRpc=0; iRpc<GetNbRpc(iSmType); iRpc++)
      {
	Int_t iAddr = CbmTofAddress::GetUniqueAddress(iSm,iRpc,0,0,iSmType);
	fMapDetInd[iAddr]=iDet;
	fiDetUId[iDet++]=iAddr;
      }
    }
   } // for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
/*
   fiChType.Set(fiNbSmTypes);
   if ( ! l->fill("",          &) ) return kFALSE;

   .Set(fiNbSmTypes);
   if ( ! l->fill("",   &) ) return kFALSE;
*/
   // Use Text_t as FairParamList does not seem to support TStrings yet

   
   Int_t iMaxSizeFilename = 500;
   Text_t *sTempText;
   sTempText = new Text_t[iMaxSizeFilename];
   if ( ! l->fill("BeamInputFile",   sTempText, iMaxSizeFilename ) ) return kFALSE;
   fsBeamInputFile = sTempText;
   if ( l->fill("BeamCalibFile",   sTempText, iMaxSizeFilename ) ) 
   fsBeamCalibFile = sTempText;
   
   if ( ! l->fill("ClusterRadiusModel", &fiClusterRadiusModel) ) return kFALSE;

   fiSmTypeInpMapp.Set(fiNbSmTypes);
   if ( ! l->fill("SmTypeInpMapp",      &fiSmTypeInpMapp) ) return kFALSE;

   if( 1 <= fiClusterRadiusModel )
   {
      fdLandauMpv.Set(fiNbSmTypes);
      if ( ! l->fill("RadiusLandauMpv", &fdLandauMpv) ) return kFALSE;
      fdLandauSigma.Set(fiNbSmTypes);
      if ( ! l->fill("RadLandauSigma",  &fdLandauSigma) ) return kFALSE;
   } // if( 1 <= fiClusterRadiusModel )

   if ( ! l->fill("MulUseTrkId",        &iTemp) ) return kFALSE;
   fbMulUseTrackId = ( 1 == iTemp? kTRUE : kFALSE);

   if ( ! l->fill("MaxTimeDistClust",  &fdMaxTimeDistClust) ) return kFALSE;
   if ( ! l->fill("MaxSpaceDistClust", &fdMaxSpaceDistClust) ) return kFALSE;

   // Extract beamtime parameters from file
   fdEfficiency.Set(fiNbSmTypes);
   fdTimeResolution.Set(fiNbSmTypes);
      // So that the histos are not created in more than 1 exemplar in case of a double call to this method
   ClearHistos();
   fh1ClusterSize.resize(fiNbSmTypes);
   fh1ClusterTot.resize(fiNbSmTypes);

   return kTRUE;
}

Bool_t CbmTofDigiBdfPar::LoadBeamtimeHistos()
{
   TDirectory * oldir = gDirectory;

   TFile * fBeamtimeInput = new TFile( fsBeamInputFile, "READ");
   if( kFALSE == fBeamtimeInput->IsOpen() )
   {
      LOG(error)<<"CbmTofDigiBdfPar => Could not open the beamtime data file.";
      return kFALSE;
   } // if( kFALSE == fBeamtimeInput->IsOpen() )
   TArrayD * pInputEff;
   TArrayD * pInputRes;
   fBeamtimeInput->GetObject("Efficiency", pInputEff);
   fBeamtimeInput->GetObject("TimeResol", pInputRes);
   if( 0 == pInputEff)
   {
      LOG(error)<<"CbmTofDigiBdfPar => Could not recover the Efficiency array from the beamtime data file.";
      return kFALSE;
   } // if( 0 == pInputEff)
   if( 0 == pInputRes)
   {
      LOG(error)<<"CbmTofDigiBdfPar => Could not recover the Time Resolution array from the beamtime data file.";
      gDirectory->cd( oldir->GetPath() );
      fBeamtimeInput->Close();
      return kFALSE;
   } // if( 0 == pInputEff)
   if( 0 == pInputEff->GetSize() || 0 == pInputRes->GetSize() ||
       pInputEff->GetSize() != pInputRes->GetSize() )
   {
      LOG(error)<<"CbmTofDigiBdfPar => Efficiency or Time Resolution array from the beamtime data file have wrong size.";
      gDirectory->cd( oldir->GetPath() );
      fBeamtimeInput->Close();
      return kFALSE;
   } // if wrong array size

   TH1* pH1Temp;
   gROOT->cd();
   fdGapsEfficiency.resize(fiNbSmTypes);
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
      if( fiSmTypeInpMapp[iSmType] < pInputEff->GetSize() )
      {
         fdEfficiency[iSmType]     = pInputEff->GetAt( fiSmTypeInpMapp[iSmType] );

         // For now use a simple gap treatment:
         // - a single fired gap fires the channel
         // - all gaps have exactly equivalent efficiency/firing probability
         // - independent gap firing (no charge accumulation or such)
         // The probability for a hit not to fire at all is then
         //    (1-p)^NGaps with p = gap efficiency and Ngaps the number of gaps in this RPC
         // This results in the relation: p = 1 - (1 - P)^(1/Ngaps)
         //         with P = RPC efficiency from beamtime
         fdGapsEfficiency[iSmType].Set( fiNbRpc[iSmType] );
         fdTimeResolution[iSmType] = pInputRes->GetAt( fiSmTypeInpMapp[iSmType] );

         for( Int_t iRpc = 0; iRpc < fiNbRpc[iSmType]; iRpc ++)
         {
            LOG(debug1) << iSmType << " Eff: " << iRpc << " " << fdEfficiency[iSmType] << " "
                        << GetNbGaps( iSmType, iRpc) << " " << (Double_t)GetNbGaps( iSmType, iRpc)
                        << " TRes " << fdTimeResolution[iSmType] ;
           fdGapsEfficiency[iSmType][iRpc] = 1 - TMath::Power( 1.0 - fdEfficiency[iSmType],
                                                       1.0/(Double_t)GetNbGaps( iSmType, iRpc) );
         }


         // Cluster Size histograms
         fBeamtimeInput->GetObject( Form("h1ClusterSizeType%03d", fiSmTypeInpMapp[iSmType]),
                                    pH1Temp );
         if( 0 == pH1Temp )
         {
            LOG(error)<<"CbmTofDigiBdfPar => Could not recover the Cluster Size histogram for Sm Type "
                      <<iSmType<<", mapped to input type "<<fiSmTypeInpMapp[iSmType];
            gDirectory->cd( oldir->GetPath() );
            fBeamtimeInput->Close();
            return kFALSE;
         } // if( 0 == pH1Temp )
         fh1ClusterSize[iSmType] = (TH1*)( pH1Temp->Clone( Form("ClusterSizeType%03d", iSmType )) );

         // Cluster Total ToT histograms
         fBeamtimeInput->GetObject( Form("h1ClusterTot%03d", fiSmTypeInpMapp[iSmType]),
                                    pH1Temp );
         if( 0 == pH1Temp )
         {
            LOG(error)<<"CbmTofDigiBdfPar => Could not recover the Cluster Size histogram for Sm Type "
                      <<iSmType<<", mapped to input type "<<fiSmTypeInpMapp[iSmType];
            gDirectory->cd( oldir->GetPath() );
            fBeamtimeInput->Close();
            return kFALSE;
         } // if( 0 == pH1Temp )
         fh1ClusterTot[iSmType] = (TH1*)( pH1Temp->Clone( Form("ClusterTot%03d", iSmType )) );
      } // if( fiSmTypeInpMapp[iSmType] < pInputEff->GetSize() )
         else
         {
            LOG(error)<<"CbmTofDigiBdfPar => Wrong mapping index for Sm Type "
                      <<iSmType<<": Out of input boundaries";
            gDirectory->cd( oldir->GetPath() );
            fBeamtimeInput->Close();
            return kFALSE;
         }

   if( 2 == fiClusterRadiusModel )
   {
      GetLandauParFromBeamDataFit();
   } // if( 2 == fiClusterRadiusModel )
   gDirectory->cd( oldir->GetPath() );
   fBeamtimeInput->Close();

   return kTRUE;
}
/************************************************************************************/
Bool_t CbmTofDigiBdfPar::GetLandauParFromBeamDataFit()
{
   TDirectory * oldir = gDirectory;

   TFile * fSimInput = new TFile( "RadToClustDist_0000_1000_0010_00025_05025_00025.root", "READ");
   if( kFALSE == fSimInput->IsOpen() )
   {
      LOG(error)<<"CbmTofDigiBdfPar => Could not open the Simulated Landau distribution data file.";
      return kFALSE;
   } // if( kFALSE == fSimInput->IsOpen() )

   // Get the input histograms from the Simulated Landau distributions file
   // R0 value of the Landau distribution used to generate a simple cluster size distribution as function of
   // the MPV and sigma values of a Landau fit on said Cluster size distribution
   TH2D * hFitR0All;
   fSimInput->GetObject("ClustSizeFitR0All", hFitR0All);
   // Input Sigma value of the Landau distribution used to generate a simple cluster size distribution as function of
   // the MPV and sigma values of a Landau fit on said Cluster size distribution
   TH2D * hFitSigInAll;
   fSimInput->GetObject("ClustSizeFitSigInAll", hFitSigInAll);
   // Number of entries for each bin of the R0 of a Landau Input distribution as function of
   // the MPV and sigma values of a Landau fit on the corresponding Cluster size distribution
   // The (R0, Input Sigma) pair can be used only if it is one, otherwise it means that more than 1 Cluster Size
   // distribution gave these values after being fitted with a Landau
   TH2D *    hFitR0CntAll;
   fSimInput->GetObject("hClustSizeFitR0CntAll", hFitR0CntAll);
   // Number of entries for each bin of the Sigma of a Landau Input distribution as function of
   // the MPV and sigma values of a Landau fit on the corresponding Cluster size distribution
   // The (R0, Input Sigma) pair can be used only if it is one, otherwise it means that more than 1 Cluster Size
   // distribution gave these values after being fitted with a Landau
   TH2D *    hFitSigInCntAll;
   fSimInput->GetObject("hClustSizeFitSigInCntAll", hFitSigInCntAll);
   if( 0 == hFitR0All || 0 == hFitSigInAll || 0 == hFitR0CntAll || 0 == hFitSigInCntAll )
   {
      LOG(error)<<"CbmTofDigiBdfPar::GetLandauParFromBeamDataFit => Failed to load Simulated Landau distribution values "
                <<" => Use default values from ASCII parameter file! Pointers: "
                <<hFitR0All<<" "<<hFitSigInAll<<" "<<hFitR0CntAll<<" "<<hFitSigInCntAll<<" ";
      return kFALSE;
   } // if( 0 == hFitR0All || 0 == hFitSigInAll || 0 == hFitR0CntAll || 0 == hFitSigInCntAll )

   // Prepare the TArrayD
   fdLandauMpv.Set(fiNbSmTypes);
   fdLandauSigma.Set(fiNbSmTypes);
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
   {
      TFitResultPtr pResult = fh1ClusterSize[iSmType]->Fit("landau","SN");
      if( kTRUE == pResult->IsValid() )
      {
         // Fit succedeed
         Double_t dMpv   = pResult->Parameter(1);
         Double_t dSigma = pResult->Parameter(2);

         if( 1 == hFitR0CntAll->GetBinContent( hFitR0CntAll->FindBin(dMpv, dSigma ) ) &&
              1 == hFitSigInCntAll->GetBinContent( hFitSigInCntAll->FindBin(dMpv, dSigma ) )  )
         {
            // Unique Cluster size distribution giving these values
            fdLandauMpv[iSmType]   = hFitR0All->GetBinContent(    hFitR0All->FindBin(   dMpv, dSigma ) );
            fdLandauSigma[iSmType] = hFitSigInAll->GetBinContent( hFitSigInAll->FindBin(dMpv, dSigma ) );
         } // if unique pair
            else
            {
               // Pair is not unique
               // => Do nothing, as default parameters should have been previously loaded
               LOG(WARNING)<<"CbmTofDigiBdfPar::GetLandauParFromBeamDataFit => Fit values matching a non-unique param pair for Sm Type "
                           <<iSmType<<": "<<hFitR0CntAll->GetBinContent( hFitR0CntAll->FindBin(dMpv, dSigma ) );
               LOG(WARNING)<<" => Use default values from ASCII parameter file";
            } // Nor unique pair
      } // if( kTRUE == pResult->IsValid() )
         else
         {
            // Fit failed
            // => Do nothing, as default parameters should have been previously loaded
            LOG(WARNING)<<"CbmTofDigiBdfPar::GetLandauParFromBeamDataFit => Fit failed for Sm Type "
                        <<iSmType<<" => Use default values from ASCII parameter file";
         } // else of if( kTRUE == pResult->IsValid() )
   } // for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)

   gDirectory->cd( oldir->GetPath() );

   return kTRUE;
}
/************************************************************************************/
// Geometry variables
Int_t    CbmTofDigiBdfPar::GetNbSm( Int_t iSmType)  const
{
   if( iSmType < fiNbSmTypes )
      return fiNbSm[iSmType];
      else return 0;
}
Int_t    CbmTofDigiBdfPar::GetNbRpc( Int_t iSmType) const
{
   if( iSmType < fiNbSmTypes )
      return fiNbRpc[iSmType];
      else return 0;
}
Int_t    CbmTofDigiBdfPar::GetNbGaps( Int_t iSmType, Int_t iRpc) const
{
   if( iSmType < fiNbSmTypes )
   {
      if( iRpc < fiNbRpc[iSmType] )
         return fiNbGaps[iSmType][iRpc];
         else return 0;
   } // if( iSmType < fiNbSmTypes )
      else return 0;
}
Double_t CbmTofDigiBdfPar::GetGapSize( Int_t iSmType, Int_t iRpc) const
{
   if( iSmType < fiNbSmTypes )
   {
      if( iRpc < fiNbRpc[iSmType] )
         return fdGapSize[iSmType][iRpc];
         else return 0.0;
   } // if( iSmType < fiNbSmTypes )
      else return 0.0;
}
Double_t CbmTofDigiBdfPar::GetSigVel( Int_t iSmType, Int_t iSm, Int_t iRpc) const
{
   if( iSmType < fiNbSmTypes )
   {
      if( iSm < fiNbSm[iSmType] && iRpc < fiNbRpc[iSmType] )
	return fdSigVel[iSmType][iSm*fiNbRpc[iSmType]+iRpc];
         else return 0.0;
   } // if( iSmType < fiNbSmTypes )
      else return 0.0;
}
void CbmTofDigiBdfPar::SetSigVel( Int_t iSmType, Int_t iSm, Int_t iRpc, Double_t vel)
{
  if(iSmType>-1 && iSmType < fiNbSmTypes){
    if( iSm < fiNbSm[iSmType] && iRpc < fiNbRpc[iSmType] )
       fdSigVel[iSmType][iSm*fiNbRpc[iSmType]+iRpc]=vel;
    else{
      LOG(error)<<"Invalid iSm "<<iSm<<", iRpc "<<iRpc;
    }
  }else{
    LOG(error)<<"Invalid SMType "<<iSmType;
  }
}
Int_t    CbmTofDigiBdfPar::GetNbChan( Int_t iSmType, Int_t iRpc) const
{
   if( iSmType < fiNbSmTypes )
   {
      if( iRpc < fiNbRpc[iSmType] )
         return fiNbCh[iSmType][iRpc];
         else return 0;
   } // if( iSmType < fiNbSmTypes )
      else return 0;
}
Int_t    CbmTofDigiBdfPar::GetChanType( Int_t iSmType, Int_t iRpc) const
{
   if( iSmType < fiNbSmTypes )
   {
      if( iRpc < fiNbRpc[iSmType] )
         return fiChType[iSmType][iRpc];
         else return 0;
   } // if( iSmType < fiNbSmTypes )
      else return 0;
}
Int_t    CbmTofDigiBdfPar::GetChanOrient( Int_t iSmType, Int_t iRpc) const
{
   if( iSmType < fiNbSmTypes )
   {
      if( iRpc < fiNbRpc[iSmType] )
         return fiChOrientation[iSmType][iRpc];
         else return 0;
   } // if( iSmType < fiNbSmTypes )
      else return 0;
}
// Beamtime variables
Int_t    CbmTofDigiBdfPar::GetTypeInputMap( Int_t iSmType ) const
{
   if( iSmType < fiNbSmTypes )
      return fiSmTypeInpMapp[iSmType];
      else return 0;
}
Double_t CbmTofDigiBdfPar::GetEfficiency( Int_t iSmType ) const
{
   if( iSmType < fiNbSmTypes )
      return fdEfficiency[iSmType];
      else return 0;
}
Double_t CbmTofDigiBdfPar::GetGapEfficiency( Int_t iSmType, Int_t iRpc ) const
{
   if( iSmType < fiNbSmTypes )
   {
      if( iRpc < fiNbRpc[iSmType] )
         return fdGapsEfficiency[iSmType][iRpc];
         else return 0;
   } // if( iSmType < fiNbSmTypes )
      else return 0;
}
Double_t CbmTofDigiBdfPar::GetResolution( Int_t iSmType ) const
{
   if( iSmType < fiNbSmTypes )
      return fdTimeResolution[iSmType];
      else return 0;
}
Double_t CbmTofDigiBdfPar::GetSystemResolution( Int_t iSmType ) const
{
   if( iSmType < fiNbSmTypes )
      return TMath::Sqrt( fdTimeResElec  * fdTimeResElec  +
                           fdTimeResStart * fdTimeResStart +
                           fdTimeResolution[iSmType] * fdTimeResolution[iSmType] );
      else return 0;
}
TH1*     CbmTofDigiBdfPar::GetClustSizeHist( Int_t iSmType ) const
{
   if( iSmType < fiNbSmTypes )
      return fh1ClusterSize[iSmType];
      else return 0;
}
TH1*     CbmTofDigiBdfPar::GetClustTotHist( Int_t iSmType ) const
{
   if( iSmType < fiNbSmTypes )
      return fh1ClusterTot[iSmType];
      else return 0;
}
Double_t CbmTofDigiBdfPar::GetLandauMpv( Int_t iSmType ) const
{
   if( iSmType < fiNbSmTypes )
      return fdLandauMpv[iSmType];
      else return 0;
}
Double_t CbmTofDigiBdfPar::GetLandauSigma( Int_t iSmType ) const
{
   if( iSmType < fiNbSmTypes )
      return fdLandauSigma[iSmType];
      else return 0;
}
Double_t CbmTofDigiBdfPar::GetMaxTimeDist( ) const
{
   if( -1 <  fdMaxTimeDistClust )
      return fdMaxTimeDistClust;
      else return 5*0.08;
}
void CbmTofDigiBdfPar::printParams()
{
//   LOG(info)<<"  _   _   _   _   _   _   _   _   _   _  ";
//   LOG(info)<<" / \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \ ";
//   LOG(info)<<" \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ \_/ ";
   LOG(info)<<"  _   _   _   _   _   _   _   _   _   _  ";
   LOG(info)<<" |_|+|_|+|_|+|_|+|_|+|_|+|_|+|_|+|_|+|_| ";
   LOG(info)<<"Parameter values in CbmTofDigiBdfPar: ";
   LOG(info)<<"=> MC data usage: ";
   if( kTRUE == fbUseOnlyPrim )
      LOG(info)<<"  Tracks used:                        Only Primary!";
      else LOG(info)<<"  Tracks used:                        All (Primary + Secondary)";
   // Fee properties
   LOG(info)<<"=> FEE properties: ";
   LOG(info)<<"  FEE gain standard deviation:        "<<100.0*fdFeeGainSigma<<" [%]";
   LOG(info)<<"  FEE Threshold on ToT:               "<<fdFeeTotThr<<"[ns]";
   LOG(info)<<"  FEE channel time resolution:        "<<fdTimeResElec<<"[ns]";
   LOG(info)<<"  Start channel time resolution:      "<<fdTimeResStart<<"[ns]";
   LOG(info)<<"  FEE deadtime:      "<<fdDeadtime<<"[ns]";

   // Geometry variables
   LOG(info)<<"=> Geometry variables: ";
   LOG(info)<<"  Signal propa. speed on readout el.: "<<fdSignalPropSpeed<<" [cm/ns]";
   LOG(info)<<"  Number of Super Modules (SM) Types: "<<fiNbSmTypes;

   TString sIndex = "  Sm Type                     |-- ";
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
      sIndex += Form("%3d ", iSmType);
   LOG(info)<<sIndex;

   TString sSmNb =  "  Nb SM for each SM type:     |-> ";
   TString sRpcNb = "  Nb Rpc for each SM type:    |-> ";
   sIndex =         "  Rpc index                   |--  ";
   Int_t iMaxRpcNb = 0;

   TString* sGapsNb = new TString[fiNbSmTypes];
   TString* sGapsSz = new TString[fiNbSmTypes];
   TString* sSigVel = new TString[fiNbSmTypes];
   TString* sChNb = new TString[fiNbSmTypes];
   TString* sChType = new TString[fiNbSmTypes];
   TString* sChOrient = new TString[fiNbSmTypes];

   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
   {
      sSmNb  += Form( "%3d ", GetNbSm(iSmType) );
      sRpcNb += Form( "%3d ", GetNbRpc(iSmType) );
      sGapsNb[iSmType] =   Form("  Nb of Gaps in SM type    %3d:|->  ", iSmType);
      sGapsSz[iSmType] =   Form("  Gap Size(mm) in SM type  %3d:|-> ", iSmType);
      sSigVel[iSmType] =   Form("  SigVel(cm/ps) in SM type %3d:|-> ", iSmType);
      sChNb[iSmType] =     Form("  Nb of Chan in SM type    %3d:|->  ", iSmType);
      sChType[iSmType] =   Form("  Chan Type in SM type     %3d:|->  ", iSmType);
      sChOrient[iSmType] = Form("  Chan orient. in SM type  %3d:|->  ", iSmType);
      if( iMaxRpcNb < fiNbRpc[iSmType])
         iMaxRpcNb = fiNbRpc[iSmType];


      for( Int_t iRpc = 0; iRpc < fiNbRpc[iSmType]; iRpc ++)
      {
         sGapsNb[iSmType] += Form( "%3d  ", GetNbGaps( iSmType, iRpc) );
         sGapsSz[iSmType] += Form( "%3.2f ", GetGapSize( iSmType, iRpc) );
         for(Int_t iSm=0; iSm<fiNbSm[iSmType]; iSm++)
            sSigVel[iSmType] += Form( "%4.3f ", GetSigVel( iSmType, iSm, iRpc) );
         sChNb[iSmType] += Form( "%3d  ", GetNbChan( iSmType, iRpc) );
         if( 1 == GetChanType( iSmType, iRpc) )
            sChType[iSmType] += "pad  ";
            else sChType[iSmType] += "str  ";
         if( 1 == GetChanOrient( iSmType, iRpc) )
            sChOrient[iSmType] += "hor  ";
            else sChOrient[iSmType] += "ver  ";
      }
   } // for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
   for( Int_t iRpc = 0; iRpc < iMaxRpcNb; iRpc ++)
      sIndex += Form("%3d  ", iRpc);

   LOG(info)<<sSmNb;
   LOG(info)<<sRpcNb;
   LOG(info)<<sIndex;
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
   {
      LOG(info)<<sGapsNb[iSmType];
      LOG(info)<<sGapsSz[iSmType];
      LOG(debug)<<sSigVel[iSmType];
      LOG(info)<<sChNb[iSmType];
      LOG(info)<<sChType[iSmType];
      LOG(info)<<sChOrient[iSmType];
   } // for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)

   // Beamtime variables
   LOG(info)<<"=> Beamtime variables: ";
   LOG(info)<<"  Beamtime data input file name:      "<<fsBeamInputFile;
   LOG(info)<<"  Beamtime data calib file name:      "<<fsBeamCalibFile;
   switch( fiClusterRadiusModel )
   {
      case -1:
         LOG(info)<<"  Cluster radius model:               Single value at 0.0002 cm";
         break;
      case 0:
         LOG(info)<<"  Cluster radius model:               Single value using beamtime mean cluster size";
         break;
      case 1:
         LOG(info)<<"  Cluster radius model:               Landau Distrib. using values from parameter file";
         break;
      case 2:
         LOG(info)<<"  Cluster radius model:               Landau Distrib. using values from fit on beam data";
         break;
      default:
         LOG(info)<<"  Cluster radius model:               None, this should not be!!";
         break;
   } // switch( fiClusterRadiusModel )
   sIndex    = "  Sm Type          |--     ";
   TString sTypeMap  = "  SM type in file: |->     ";
   TString sEffType  = "  Efficiency:      |->    ";
   TString sResType  = "  Resolution:      |->   ";
   TString sSysResTy = "  => Sys. Res.:    |->   ";
   TString sLandMpv  = "  Landau MPV:      |->   ";
   TString sLandSig  = "  Landau Sigma:    |->   ";
   for( Int_t iSmType = 0; iSmType < fiNbSmTypes; iSmType ++)
   {
      sIndex    += Form("%3d   ", iSmType);
      sTypeMap  += Form("%3d   ", GetTypeInputMap(iSmType) );
      sEffType  += Form("%1.2f  ", fdEfficiency[iSmType]);
      sResType  += Form("%2.3f ", fdTimeResolution[iSmType]);
      sSysResTy += Form("%2.3f ", GetSystemResolution(iSmType));
      switch( fiClusterRadiusModel )
      {
         case 1:
         case 2:
            sLandMpv += Form("%2.3f ", fdLandauMpv[iSmType]);
            sLandSig += Form("%2.3f ", fdLandauSigma[iSmType]);
            break;
         default:
            break;
      } // switch( fiClusterRadiusModel )
   }
   LOG(info)<<sIndex;
   LOG(info)<<sTypeMap;
   LOG(info)<<sEffType;
   LOG(info)<<sResType;
   LOG(info)<<sSysResTy;
   switch( fiClusterRadiusModel )
   {
      case 1:
      case 2:
         LOG(info)<<sLandMpv;
         LOG(info)<<sLandSig;
         break;
      default:
         break;
   } // switch( fiClusterRadiusModel )

   LOG(info)<<"=> Digitization variables: ";
   // Digi type to use
   if( kTRUE == fbUseExpDigi )
      LOG(info)<<"  Output ToF digi type:               Expanded (Double values)";
      else LOG(info)<<"  Output ToF digi type:               Re-digitized (Encoding in 64 bits)";
   if( kTRUE == fbOneGapTrack )
      LOG(info)<<"  Avoid multi gaps digi for same trk: ON";
      else LOG(info)<<"  Avoid multi gaps digi for same trk: OFF";

   // Cluster Model to use
   switch( fiClusterModel )
   {
      case 0:
         LOG(info)<<"  Cluster model:                      Use directly cluster size (no cluster radius)";
         break;
      case 1:
         LOG(info)<<"  Cluster model:                      Flat disc as charge distribution + geometrical overlap";
         break;
      case 2:
         LOG(info)<<"  Cluster model:                      2D gaussian as charge distribution + integral";
         break;
      default:
         LOG(info)<<"  Cluster model:                      None, this should not be!!";
         break;
   } // switch( fiClusterModel )
   LOG(info)<<"=> Simple clusterizer parameters: ";
   if( kTRUE == fbMulUseTrackId )
      LOG(info)<<"  Variable used for multiplicity cnt: Track ID";
      else LOG(info)<<"  Variable used for multiplicity cnt: TofPoint ID";
   if( -1 <  fdMaxTimeDistClust)
      LOG(info)<<"  Maximal time dist. to last chan.:   "<< GetMaxTimeDist() <<" [ns]";
      else LOG(info)<<"  Maximal time dist. to last chan.:   Use 5*Nom. Syst. Res. (80 ps) = "<< GetMaxTimeDist() <<" [ns]";

   LOG(info)<<"  Maximal dist. along ch to last one: "<<fdMaxSpaceDistClust<<" [cm]";


   delete [] sGapsNb;
   delete [] sGapsSz;
   delete [] sChNb;
   delete [] sChType;
   delete [] sChOrient;

}

Int_t CbmTofDigiBdfPar::GetNbDet() const
{
  return fiDetUId.GetSize();
}

Int_t CbmTofDigiBdfPar::GetDetInd(Int_t iAddr)
{

  const Int_t DetMask = 4194303;
  /*
  Int_t iInd=0;
  while (fiDetUId[iInd] != (iAddr & DetMask) && iInd<fiDetUId.GetSize()) iInd++; //FIXME, inefficient and using a numerical constant!
  if(iInd == fiDetUId.GetSize()){
    LOG(error)<<Form(" detector id 0x%08x unknown ",iAddr) ;
    iInd=0;
  }
  return iInd;
  */
  return fMapDetInd[(iAddr&DetMask)];
}

Int_t CbmTofDigiBdfPar::GetDetUId(Int_t iInd)
{
  return fiDetUId[iInd];
}
