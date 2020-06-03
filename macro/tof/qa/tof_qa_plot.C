
// Constants definitions: Particles list
const Int_t   kiNbPart = 13;
const TString ksPartTag[kiNbPart] = 
   { "others",
     "ep", "em",   "pip", "pim", "kp", "km", 
     "p",  "pbar", "d",   "t",   "he",  "a" };
const Int_t   kiPartPdgCode[kiNbPart] = 
   {   0,
      21,   -21,    211,   -211,  321,  -321, 
    2212, -2212, 1000010020, 1000010030, 1000020030, 1000020040 };
const TString ksPartName[kiNbPart] = 
   { "any other part.", 
     "e-", "e+",   "#pi+", "#pi-", "k+", "k-", 
     "p",  "anti-p", "d",    "t",    "he", "#alpha"};
//___________________________________________________________________

const Int_t kiRebinFactX = 2;
const Int_t kiRebinFactY = 2;
const Int_t kiRebinFactZ = 2;

// C++ Classes and includes
#include <vector>

Bool_t tof_qa_plot( TString sDigiHistFileName = "", TString sClustHistFileName = "",
                    TString sQaHistFileName = "", TString sOutHistFilename = "",
                    Double_t fdWallPosZ = 900)
{
   
//******* Open the input and output files
   if( "" == sDigiHistFileName || "" == sClustHistFileName || 
       "" == sQaHistFileName || "" == sOutHistFilename )
      return kFALSE;
      
   TFile * fDigiHistFile  = new TFile( sDigiHistFileName,  "READ");   
   TFile * fClustHistFile = new TFile( sClustHistFileName, "READ");   
   TFile * fQaHistFile    = new TFile( sQaHistFileName,    "READ");   
   TFile * fOutHistFile   = new TFile( sOutHistFilename,   "RECREATE");   

   if( NULL == fDigiHistFile || NULL == fClustHistFile ||
       NULL == fQaHistFile   || NULL == fOutHistFile)
       return kFALSE;
       
   gROOT->cd();
   std::cout << "File opening OK!" << std::endl;
//******* Recovers and clones the input histograms
      // Geometric Mapping
/*      
   TH2 * fhTrackMapXY;  // Only when creating normalization histos, bec. B Field in normal MC
   TH2 * fhTrackMapXZ;  // Only when creating normalization histos, bec. B Field in normal MC
   TH2 * fhTrackMapYZ;  // Only when creating normalization histos, bec. B Field in normal MC
   TH2 * fhTrackMapAng; // Only when creating normalization histos, bec. B Field in normal MC
   TH2 * fhTrackMapSph; // Only when creating normalization histos, bec. B Field in normal MC
*/
   TH2 * fhPointMapXY;
   TH2 * fhPointMapXZ;
   TH2 * fhPointMapYZ;
   TH2 * fhPointMapAng;
   TH2 * fhPointMapSph;
   TH2 * fhDigiMapXY;
   TH2 * fhDigiMapXZ;
   TH2 * fhDigiMapYZ;
   TH2 * fhDigiMapAng;
   TH2 * fhDigiMapSph;
   TH2 * fhHitMapXY;
   TH2 * fhHitMapXZ;
   TH2 * fhHitMapYZ;
   TH2 * fhHitMapAng;
   TH2 * fhHitMapSph;    
      // Mapping of position for hits coming from a single MC Point
   TH2 * fhHitMapSingPntXY;
   TH2 * fhHitMapSingPntXZ;
   TH2 * fhHitMapSingPntYZ;
   TH2 * fhHitMapSingPntAng;
   TH2 * fhHitMapSingPntSph;
      // Mapping of position for hits coming from multiple MC Points
   TH2 * fhHitMapMultPntXY;
   TH2 * fhHitMapMultPntXZ;
   TH2 * fhHitMapMultPntYZ;
   TH2 * fhHitMapMultPntAng;
   TH2 * fhHitMapMultPntSph;
      // Mapping of position for hits coming from a single MC Track
   TH2 * fhHitMapSingTrkXY;
   TH2 * fhHitMapSingTrkXZ;
   TH2 * fhHitMapSingTrkYZ;
   TH2 * fhHitMapSingTrkAng;
   TH2 * fhHitMapSingTrkSph;
      // Mapping of position for hits coming from multiple MC Tracks
   TH2 * fhHitMapMultTrkXY;
   TH2 * fhHitMapMultTrkXZ;
   TH2 * fhHitMapMultTrkYZ;
   TH2 * fhHitMapMultTrkAng;
   TH2 * fhHitMapMultTrkSph;

      // Physics coord mapping, 1 per particle type
         // Phase space
   std::vector<TH2 *> fvhPtmRapGenTrk;
   std::vector<TH2 *> fvhPtmRapStsPnt;
   std::vector<TH2 *> fvhPtmRapTofPnt;
   std::vector<TH2 *> fvhPtmRapTofHit;
   std::vector<TH2 *> fvhPtmRapTofHitSinglePnt;
   std::vector<TH2 *> fvhPtmRapTofHitSingleTrk;
   fvhPtmRapGenTrk.resize(kiNbPart);
   fvhPtmRapStsPnt.resize(kiNbPart);
   fvhPtmRapTofPnt.resize(kiNbPart);
   fvhPtmRapTofHit.resize(kiNbPart);
   fvhPtmRapTofHitSinglePnt.resize(kiNbPart);
   fvhPtmRapTofHitSingleTrk.resize(kiNbPart);
         // PLab
   std::vector<TH1 *> fvhPlabGenTrk;
   std::vector<TH1 *> fvhPlabStsPnt;
   std::vector<TH1 *> fvhPlabTofPnt;
   std::vector<TH1 *> fvhPlabTofHit;
   std::vector<TH1 *> fvhPlabTofHitSinglePnt;
   std::vector<TH1 *> fvhPlabTofHitSingleTrk;
   fvhPlabGenTrk.resize(kiNbPart);
   fvhPlabStsPnt.resize(kiNbPart);
   fvhPlabTofPnt.resize(kiNbPart);
   fvhPlabTofHit.resize(kiNbPart);
   fvhPlabTofHitSinglePnt.resize(kiNbPart);
   fvhPlabTofHitSingleTrk.resize(kiNbPart);
   
      // MC Tracks losses
   std::vector<TH2 *> fvhPtmRapGenTrkTofPnt;
   std::vector<TH2 *> fvhPtmRapGenTrkTofHit;
   std::vector<TH1 *> fvhPlabGenTrkTofPnt;
   std::vector<TH1 *> fvhPlabGenTrkTofhit;
   fvhPtmRapGenTrkTofPnt.resize(kiNbPart);
   fvhPtmRapGenTrkTofHit.resize(kiNbPart);
   fvhPlabGenTrkTofPnt.resize(kiNbPart);
   fvhPlabGenTrkTofhit.resize(kiNbPart);
   
   // TEMP pointers
   TH1 * tempOneDimHist = NULL;
   TH2 * tempTwoDimHist = NULL;
   
   // Recovery and cloning
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_PointsMapXY" ) );
   if( NULL != tempTwoDimHist )
      fhPointMapXY = (TH2*)(tempTwoDimHist->Clone( "PointsMapXY_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_PointsMapXZ" ) );
   if( NULL != tempTwoDimHist )
      fhPointMapXZ = (TH2*)(tempTwoDimHist->Clone( "PointsMapXZ_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_PointsMapYZ" ) );
   if( NULL != tempTwoDimHist )
      fhPointMapYZ = (TH2*)(tempTwoDimHist->Clone( "PointsMapYZ_normed" ) );
      else return kFALSE;

   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_PointsMapAng" ) );
   if( NULL != tempTwoDimHist )
      fhPointMapAng = (TH2*)(tempTwoDimHist->Clone( "PointsMapAng_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_PointsMapSph" ) );
   if( NULL != tempTwoDimHist )
      fhPointMapSph = (TH2*)(tempTwoDimHist->Clone( "PointsMapSph_normed" ) );
      else return kFALSE;
      
//----------------------------------- 
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_DigisMapXY" ) );
   if( NULL != tempTwoDimHist )
      fhDigiMapXY = (TH2*)(tempTwoDimHist->Clone( "DigisMapXY_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_DigisMapXZ" ) );
   if( NULL != tempTwoDimHist )
      fhDigiMapXZ = (TH2*)(tempTwoDimHist->Clone( "DigisMapXZ_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_DigisMapYZ" ) );
   if( NULL != tempTwoDimHist )
      fhDigiMapYZ = (TH2*)(tempTwoDimHist->Clone( "DigisMapYZ_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_DigisMapAng" ) );
   if( NULL != tempTwoDimHist )
      fhDigiMapAng = (TH2*)(tempTwoDimHist->Clone( "DigisMapAng_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_DigisMapSph" ) );
   if( NULL != tempTwoDimHist )
      fhDigiMapSph = (TH2*)(tempTwoDimHist->Clone( "DigisMapSph_normed" ) );
      else return kFALSE;
      
//-----------------------------------
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapXY" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapXY = (TH2*)(tempTwoDimHist->Clone( "HitsMapXY_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapXZ" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapXZ = (TH2*)(tempTwoDimHist->Clone( "HitsMapXZ_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapYZ" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapYZ = (TH2*)(tempTwoDimHist->Clone( "HitsMapYZ_normed" ) );
      else return kFALSE;

   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapAng" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapAng = (TH2*)(tempTwoDimHist->Clone( "HitsMapAng_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapSph" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSph = (TH2*)(tempTwoDimHist->Clone( "HitsMapSph_normed" ) );
      else return kFALSE;
      
//-----------------------------------
      // Mapping of position for hits coming from a single MC Point
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapSingPntXY" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSingPntXY = (TH2*)(tempTwoDimHist->Clone( "HitsMapSingPntXY_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapSingPntXZ" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSingPntXZ = (TH2*)(tempTwoDimHist->Clone( "HitsMapSingPntXZ_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapSingPntYZ" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSingPntYZ = (TH2*)(tempTwoDimHist->Clone( "HitsMapSingPntYZ_normed" ) );
      else return kFALSE;

   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapSingPntAng" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSingPntAng = (TH2*)(tempTwoDimHist->Clone( "HitsMapSingPntAng_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapSingPntSph" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSingPntSph = (TH2*)(tempTwoDimHist->Clone( "HitsMapSingPntSph_normed" ) );
      else return kFALSE;
      
//-----------------------------------
      // Mapping of position for hits coming from multiple MC Points
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapMultPntXY" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapMultPntXY = (TH2*)(tempTwoDimHist->Clone( "HitsMapMultPntXY_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapMultPntXZ" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapMultPntXZ = (TH2*)(tempTwoDimHist->Clone( "HitsMapMultPntXZ_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapMultPntYZ" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapMultPntYZ = (TH2*)(tempTwoDimHist->Clone( "HitsMapMultPntYZ_normed" ) );
      else return kFALSE;

   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapMultPntAng" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapMultPntAng = (TH2*)(tempTwoDimHist->Clone( "HitsMapMultPntAng_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapMultPntSph" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapMultPntSph = (TH2*)(tempTwoDimHist->Clone( "HitsMapMultPntSph_normed" ) );
      else return kFALSE;
      
//-----------------------------------
      // Mapping of position for hits coming from a single MC Track
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapSingTrkXY" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSingTrkXY = (TH2*)(tempTwoDimHist->Clone( "HitsMapSingTrkXY_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapSingTrkXZ" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSingTrkXZ = (TH2*)(tempTwoDimHist->Clone( "HitsMapSingTrkXZ_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapSingTrkYZ" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSingTrkYZ = (TH2*)(tempTwoDimHist->Clone( "HitsMapSingTrkYZ_normed" ) );
      else return kFALSE;

   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapSingTrkAng" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSingTrkAng = (TH2*)(tempTwoDimHist->Clone( "HitsMapSingTrkAng_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapSingTrkSph" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapSingTrkSph = (TH2*)(tempTwoDimHist->Clone( "HitsMapSingTrkSph_normed" ) );
      else return kFALSE;
      
//-----------------------------------
      // Mapping of position for hits coming from multiple MC Tracks
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapMultTrkXY" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapMultTrkXY = (TH2*)(tempTwoDimHist->Clone( "HitsMapMultTrkXY_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapMultTrkXZ" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapMultTrkXZ = (TH2*)(tempTwoDimHist->Clone( "HitsMapMultTrkXZ_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapMultTrkYZ" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapMultTrkYZ = (TH2*)(tempTwoDimHist->Clone( "HitsMapMultTrkYZ_normed" ) );
      else return kFALSE;

   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapMultTrkAng" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapMultTrkAng = (TH2*)(tempTwoDimHist->Clone( "HitsMapMultTrkAng_normed" ) );
      else return kFALSE;
      
   tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( "TofTests_HitsMapMultTrkSph" ) );
   if( NULL != tempTwoDimHist )
      fhHitMapMultTrkSph = (TH2*)(tempTwoDimHist->Clone( "HitsMapMultTrkSph_normed" ) );
      else return kFALSE;

   for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
   {
            // Phase space
      tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( Form("TofTests_PtmRapGenTrk_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempTwoDimHist )
         fvhPtmRapGenTrk[iPartIdx] = (TH2*)(tempTwoDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
         
      tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( Form("TofTests_PtmRapStsPnt_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempTwoDimHist )
         fvhPtmRapStsPnt[iPartIdx] = (TH2*)(tempTwoDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
         
      tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( Form("TofTests_PtmRapTofPnt_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempTwoDimHist )
         fvhPtmRapTofPnt[iPartIdx] = (TH2*)(tempTwoDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
         
      tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( Form("TofTests_PtmRapTofHit_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempTwoDimHist )
         fvhPtmRapTofHit[iPartIdx] = (TH2*)(tempTwoDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
         
      tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( Form("TofTests_PtmRapTofHitSinglePnt_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempTwoDimHist )
         fvhPtmRapTofHitSinglePnt[iPartIdx] = (TH2*)(tempTwoDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
         
      tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( Form("TofTests_PtmRapTofHitSingleTrk_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempTwoDimHist )
         fvhPtmRapTofHitSingleTrk[iPartIdx] = (TH2*)(tempTwoDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
         
   //-----------------------------------
            // PLab
      tempOneDimHist = (TH1*) (fQaHistFile->FindObjectAny( Form("TofTests_PlabGenTrk_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempOneDimHist )
         fvhPlabGenTrk[iPartIdx] = (TH1*)(tempOneDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
         
      tempOneDimHist = (TH1*) (fQaHistFile->FindObjectAny( Form("TofTests_PlabStsPnt_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempOneDimHist )
         fvhPlabStsPnt[iPartIdx] = (TH1*)(tempOneDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
         
      tempOneDimHist = (TH1*) (fQaHistFile->FindObjectAny( Form("TofTests_PlabTofPnt_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempOneDimHist )
         fvhPlabTofPnt[iPartIdx] = (TH1*)(tempOneDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
         
      tempOneDimHist = (TH1*) (fQaHistFile->FindObjectAny( Form("TofTests_PlabTofHit_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempOneDimHist )
         fvhPlabTofHit[iPartIdx] = (TH1*)(tempOneDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
         
      tempOneDimHist = (TH1*) (fQaHistFile->FindObjectAny( Form("TofTests_PlabTofHitSinglePnt_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempOneDimHist )
         fvhPlabTofHitSinglePnt[iPartIdx] = (TH1*)(tempOneDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
         
      tempOneDimHist = (TH1*) (fQaHistFile->FindObjectAny( Form("TofTests_PlabTofHitSingleTrk_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempOneDimHist )
         fvhPlabTofHitSingleTrk[iPartIdx] = (TH1*)(tempOneDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
                                 
   //-----------------------------------
            // MC Tracks losses      
      tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( Form("TofTests_PtmRapGenTrkTofPnt_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempTwoDimHist )
         fvhPtmRapGenTrkTofPnt[iPartIdx] = (TH2*)(tempTwoDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
         
      tempTwoDimHist = (TH2*) (fQaHistFile->FindObjectAny( Form("TofTests_PtmRapGenTrkTofHit_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempTwoDimHist )
         fvhPtmRapGenTrkTofHit[iPartIdx] = (TH2*)(tempTwoDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
         
      tempOneDimHist = (TH1*) (fQaHistFile->FindObjectAny( Form("TofTests_PlabGenTrkTofPnt_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempOneDimHist )
         fvhPlabGenTrkTofPnt[iPartIdx] = (TH1*)(tempOneDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
         
      tempOneDimHist = (TH1*) (fQaHistFile->FindObjectAny( Form("TofTests_PlabGenTrkTofHit_%s", ksPartTag[iPartIdx].Data() ) ) );
      if( NULL != tempOneDimHist )
         fvhPlabGenTrkTofhit[iPartIdx] = (TH1*)(tempOneDimHist->Clone( "HitMapMultTrkSph_normed" ) );
         else return kFALSE;
      
   } // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)

   std::cout << "Histo recovery OK!" << std::endl;
//******* close the input files
   fDigiHistFile->Close();
   fClustHistFile->Close();
   fQaHistFile->Close();

   std::cout << "Files closing OK!" << std::endl;
   
//******* close the input files
   if( 1 < kiRebinFactX && 1 < kiRebinFactY )
   {
      fhPointMapXY      ->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhPointMapXZ      ->Rebin2D( kiRebinFactX, kiRebinFactZ);
      fhPointMapYZ      ->Rebin2D( kiRebinFactY, kiRebinFactZ);
      fhPointMapAng     ->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhPointMapSph     ->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhDigiMapXY       ->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhDigiMapXZ       ->Rebin2D( kiRebinFactX, kiRebinFactZ);
      fhDigiMapYZ       ->Rebin2D( kiRebinFactY, kiRebinFactZ);
      fhDigiMapAng      ->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhDigiMapSph      ->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhHitMapXY        ->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhHitMapXZ        ->Rebin2D( kiRebinFactX, kiRebinFactZ);
      fhHitMapYZ        ->Rebin2D( kiRebinFactY, kiRebinFactZ);
      fhHitMapAng       ->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhHitMapSph       ->Rebin2D( kiRebinFactX, kiRebinFactY);    
      // Mapping of position for hits coming from a single MC Point
      fhHitMapSingPntXY ->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhHitMapSingPntXZ ->Rebin2D( kiRebinFactX, kiRebinFactZ);
      fhHitMapSingPntYZ ->Rebin2D( kiRebinFactY, kiRebinFactZ);
      fhHitMapSingPntAng->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhHitMapSingPntSph->Rebin2D( kiRebinFactX, kiRebinFactY);
      // Mapping of position for hits coming from multiple MC Points
      fhHitMapMultPntXY ->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhHitMapMultPntXZ ->Rebin2D( kiRebinFactX, kiRebinFactZ);
      fhHitMapMultPntYZ ->Rebin2D( kiRebinFactY, kiRebinFactZ);
      fhHitMapMultPntAng->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhHitMapMultPntSph->Rebin2D( kiRebinFactX, kiRebinFactY);
      // Mapping of position for hits coming from a single MC Track
      fhHitMapSingTrkXY ->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhHitMapSingTrkXZ ->Rebin2D( kiRebinFactX, kiRebinFactZ);
      fhHitMapSingTrkYZ ->Rebin2D( kiRebinFactY, kiRebinFactZ);
      fhHitMapSingTrkAng->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhHitMapSingTrkSph->Rebin2D( kiRebinFactX, kiRebinFactY);
      // Mapping of position for hits coming from multiple MC Tracks
      fhHitMapMultTrkXY ->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhHitMapMultTrkXZ ->Rebin2D( kiRebinFactX, kiRebinFactZ);
      fhHitMapMultTrkYZ ->Rebin2D( kiRebinFactY, kiRebinFactZ);
      fhHitMapMultTrkAng->Rebin2D( kiRebinFactX, kiRebinFactY);
      fhHitMapMultTrkSph->Rebin2D( kiRebinFactX, kiRebinFactY);
      
      std::cout << "2D maps rebinning OK!" << std::endl;
   } // if( 1 < kiRebinFactX && 1 < kiRebinFactY )
//******* Creates the output histograms
   // Histograms bining, for now HardCode
   // TODO: recover from input histograms
      // xy - hit densities and rates
   Int_t nbinx=1500 / kiRebinFactX;
   Int_t nbiny=1000 / kiRebinFactY;
   Int_t nbinz=1500 / kiRebinFactZ;
   Double_t xrange=750.;
   Double_t yrange=500.;
   Double_t zmin  = fdWallPosZ -  50.;
   Double_t zmax  = fdWallPosZ + 100.;
      // angular densities for overlap check
   Int_t iNbBinThetaX  = 1200 / kiRebinFactX;
   Double_t dThetaXMin = -  60.0;
   Double_t dThetaXMax =    60.0;
   Int_t iNbBinThetaY  = 800 / kiRebinFactY;
   Double_t dThetaYMin = -  40.0;
   Double_t dThetaYMax =    40.0;
      // spherical densities for overlap check
   Int_t iNbBinTheta   = 180 / kiRebinFactX;
   Double_t dThetaMin  =   0;
   Double_t dThetaMax  =  TMath::Pi()*90/180;
   Int_t iNbBinPhi     = 180 / kiRebinFactY;
   Double_t dPhiMin    = - TMath::Pi();
   Double_t dPhiMax    =   TMath::Pi();
   
      // Physics coord mapping, 1 per particle type
         // Phase space
   Int_t    iNbBinsY   = 30;
   Double_t dMinY      = -1.;
   Double_t dMaxY      =  4.;
   Int_t    iNbBNinsPtm = 30;
   Double_t dMinPtm    =  0.0;
   Double_t dMaxPtm    =  2.5;
         // PLab
   Int_t    iNbBinsPlab = 100;
   Double_t dMinPlab    =   0.0;
   Double_t dMaxPlab    =  10.0;      
   
      // Geometric Mapping
   TH2 * fhHitEffMapXY  = new TH2D("TofQa_HitsEffMapXY",     "Efficiency of the Tof Hits generation VS position; X[cm]; Y[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbiny,-yrange,yrange);
   TH2 * fhHitEffMapXZ  = new TH2D("TofQa_HitsEffMapXZ",     "Efficiency of the Tof Hits generation VS position; X[cm]; Z[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbinz,zmin,zmax);
   TH2 * fhHitEffMapYZ  = new TH2D("TofQa_HitsEffMapYZ",     "Efficiency of the Tof Hits generation VS position; Y[cm]; Z[cm]; Eff. [\%]",
                              nbiny,-yrange,yrange,nbinz,zmin,zmax);
   TH2 * fhHitEffMapAng = new TH2D("TofQa_HitsEffMapAng",    "Efficiency of the Tof Hits generation VS position; #theta_{x}[Deg.]; #theta_{y}[Deg.]; Eff. [\%]",
                              iNbBinThetaX, dThetaXMin, dThetaXMax,
                              iNbBinThetaY, dThetaYMin, dThetaYMax);
   TH2 * fhHitEffMapSph = new TH2D("TofQa_HitsEffMapSph", "Efficiency of the Tof Hits generation VS position; #theta[rad.]; #phi[rad.]; Eff. [\%]",
                              iNbBinTheta, dThetaMin, dThetaMax,
                              iNbBinPhi,   dPhiMin,   dPhiMax); 
      // Mapping of position for hits coming from a single MC Point
   TH2 * fhHitEffMapSingPntXY  = new TH2D("TofQa_HitsEffMapSingPntXY",     
                              "Efficiency of the Tof Hits generation for hits from a single MC Pnt; X[cm]; Y[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbiny,-yrange,yrange);
   TH2 * fhHitEffMapSingPntXZ  = new TH2D("TofQa_HitsEffMapSingPntXZ",     
                              "Efficiency of the Tof Hits generation for hits from a single MC Pnt; X[cm]; Z[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbinz,zmin,zmax);
   TH2 * fhHitEffMapSingPntYZ  = new TH2D("TofQa_HitsEffMapSingPntYZ",     
                              "Efficiency of the Tof Hits generation for hits from a single MC Pnt; Y[cm]; Z[cm]; Eff. [\%]",
                              nbiny,-yrange,yrange,nbinz,zmin,zmax);
   TH2 * fhHitEffMapSingPntAng = new TH2D("TofQa_HitsEffMapSingPntAng",    
                              "Efficiency of the Tof Hits generation for hits from a single MC Pnt; #theta_{x}[Deg.]; #theta_{y}[Deg.]; Eff. [\%]",
                              iNbBinThetaX, dThetaXMin, dThetaXMax,
                              iNbBinThetaY, dThetaYMin, dThetaYMax);
   TH2 * fhHitEffMapSingPntSph = new TH2D("TofQa_HitsEffMapSingPntSph", 
                              "Efficiency of the Tof Hits generation for hits from a single MC Pnt; #theta[rad.]; #phi[rad.]; Eff. [\%]",
                              iNbBinTheta, dThetaMin, dThetaMax,
                              iNbBinPhi,   dPhiMin,   dPhiMax); 

   TH2 * fhHitRatioMapSingPntXY  = new TH2D("TofQa_HitsRatioMapSingPntXY",
                              "Tof Hits generation: Fraction of hits from a single MC Pnt; X[cm]; Y[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbiny,-yrange,yrange);
   TH2 * fhHitRatioMapSingPntXZ  = new TH2D("TofQa_HitsRatioMapSingPntXZ",
                              "Tof Hits generation: Fraction of hits from a single MC Pnt; X[cm]; Z[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbinz,zmin,zmax);
   TH2 * fhHitRatioMapSingPntYZ  = new TH2D("TofQa_HitsRatioMapSingPntYZ",
                              "Tof Hits generation: Fraction of hits from a single MC Pnt; Y[cm]; Z[cm]; Eff. [\%]",
                              nbiny,-yrange,yrange,nbinz,zmin,zmax);
   TH2 * fhHitRatioMapSingPntAng = new TH2D("TofQa_HitsRatioMapSingPntAng",
                              "Tof Hits generation: Fraction of hits from a single MC Pnt; #theta_{x}[Deg.]; #theta_{y}[Deg.]; Eff. [\%]",
                              iNbBinThetaX, dThetaXMin, dThetaXMax,
                              iNbBinThetaY, dThetaYMin, dThetaYMax);
   TH2 * fhHitRatioMapSingPntSph = new TH2D("TofQa_HitsRatioMapSingPntSph",
                              "Tof Hits generation: Fraction of hits from a single MC Pnt; #theta[rad.]; #phi[rad.]; Eff. [\%]",
                              iNbBinTheta, dThetaMin, dThetaMax,
                              iNbBinPhi,   dPhiMin,   dPhiMax);
      // Mapping of position for hits coming from multiple MC Points
   TH2 * fhHitEffMapMultPntXY  = new TH2D("TofQa_HitsEffMapMultPntXY",     
                              "Efficiency of the Tof Hits generation for hits from multiple MC Pnt; X[cm]; Y[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbiny,-yrange,yrange);
   TH2 * fhHitEffMapMultPntXZ  = new TH2D("TofQa_HitsEffMapMultPntXZ",     
                              "Efficiency of the Tof Hits generation for hits from multiple MC Pnt; X[cm]; Z[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbinz,zmin,zmax);
   TH2 * fhHitEffMapMultPntYZ  = new TH2D("TofQa_HitsEffMapMultPntYZ",     
                              "Efficiency of the Tof Hits generation for hits from multiple MC Pnt; Y[cm]; Z[cm]; Eff. [\%]",
                              nbiny,-yrange,yrange,nbinz,zmin,zmax);
   TH2 * fhHitEffMapMultPntAng = new TH2D("TofQa_HitsEffMapMultPntAng",    
                              "Efficiency of the Tof Hits generation for hits from multiple MC Pnt; #theta_{x}[Deg.]; #theta_{y}[Deg.]; Eff. [\%]",
                              iNbBinThetaX, dThetaXMin, dThetaXMax,
                              iNbBinThetaY, dThetaYMin, dThetaYMax);
   TH2 * fhHitEffMapMultPntSph = new TH2D("TofQa_HitsEffMapMultPntSph", 
                              "Efficiency of the Tof Hits generation for hits from multiple MC Pnt; #theta[rad.]; #phi[rad.]; Eff. [\%]",
                              iNbBinTheta, dThetaMin, dThetaMax,
                              iNbBinPhi,   dPhiMin,   dPhiMax); 
      // Mapping of position for hits coming from a single MC Track
   TH2 * fhHitEffMapSingTrkXY  = new TH2D("TofQa_HitsEffMapSingTrkXY",     
                              "Efficiency of the Tof Hits generation for hits from a single MC Trk; X[cm]; Y[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbiny,-yrange,yrange);
   TH2 * fhHitEffMapSingTrkXZ  = new TH2D("TofQa_HitsEffMapSingTrkXZ",     
                              "Efficiency of the Tof Hits generation for hits from a single MC Trk; X[cm]; Z[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbinz,zmin,zmax);
   TH2 * fhHitEffMapSingTrkYZ  = new TH2D("TofQa_HitsEffMapSingTrkYZ",     
                              "Efficiency of the Tof Hits generation for hits from a single MC Trk; Y[cm]; Z[cm]; Eff. [\%]",
                              nbiny,-yrange,yrange,nbinz,zmin,zmax);
   TH2 * fhHitEffMapSingTrkAng = new TH2D("TofQa_HitsEffMapSingTrkAng",    
                              "Efficiency of the Tof Hits generation for hits from a single MC Trk; #theta_{x}[Deg.]; #theta_{y}[Deg.]; Eff. [\%]",
                              iNbBinThetaX, dThetaXMin, dThetaXMax,
                              iNbBinThetaY, dThetaYMin, dThetaYMax);
   TH2 * fhHitEffMapSingTrkSph = new TH2D("TofQa_HitsEffMapSingTrkSph", 
                              "Efficiency of the Tof Hits generation for hits from a single MC Trk; #theta[rad.]; #phi[rad.]; Eff. [\%]",
                              iNbBinTheta, dThetaMin, dThetaMax,
                              iNbBinPhi,   dPhiMin,   dPhiMax);

   TH2 * fhHitRatioMapSingTrkXY  = new TH2D("TofQa_HitsRatioMapSingTrkXY",
                              "Tof Hits generation: Fraction of hits from a single MC Trk; X[cm]; Y[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbiny,-yrange,yrange);
   TH2 * fhHitRatioMapSingTrkXZ  = new TH2D("TofQa_HitsRatioMapSingTrkXZ",
                              "Tof Hits generation: Fraction of hits from a single MC Trk; X[cm]; Z[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbinz,zmin,zmax);
   TH2 * fhHitRatioMapSingTrkYZ  = new TH2D("TofQa_HitsRatioMapSingTrkYZ",
                              "Tof Hits generation: Fraction of hits from a single MC Trk; Y[cm]; Z[cm]; Eff. [\%]",
                              nbiny,-yrange,yrange,nbinz,zmin,zmax);
   TH2 * fhHitRatioMapSingTrkAng = new TH2D("TofQa_HitsRatioMapSingTrkAng",
                              "Tof Hits generation: Fraction of hits from a single MC Trk; #theta_{x}[Deg.]; #theta_{y}[Deg.]; Eff. [\%]",
                              iNbBinThetaX, dThetaXMin, dThetaXMax,
                              iNbBinThetaY, dThetaYMin, dThetaYMax);
   TH2 * fhHitRatioMapSingTrkSph = new TH2D("TofQa_HitsRatioMapSingTrkSph",
                              "Tof Hits generation: Fraction of hits from a single MC Trk; #theta[rad.]; #phi[rad.]; Eff. [\%]",
                              iNbBinTheta, dThetaMin, dThetaMax,
                              iNbBinPhi,   dPhiMin,   dPhiMax);
      // Mapping of position for hits coming from multiple MC Tracks
   TH2 * fhHitEffMapMultTrkXY  = new TH2D("TofQa_HitsEffMapMultTrkXY",     
                              "Efficiency of the Tof Hits generation for hits from multiple MC Trk; X[cm]; Y[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbiny,-yrange,yrange);
   TH2 * fhHitEffMapMultTrkXZ  = new TH2D("TofQa_HitsEffMapMultTrkXZ",     
                              "Efficiency of the Tof Hits generation for hits from multiple MC Trk; X[cm]; Z[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbinz,zmin,zmax);
   TH2 * fhHitEffMapMultTrkYZ  = new TH2D("TofQa_HitsEffMapMultTrkYZ",     
                              "Efficiency of the Tof Hits generation for hits from multiple MC Trk; Y[cm]; Z[cm]; Eff. [\%]",
                              nbiny,-yrange,yrange,nbinz,zmin,zmax);
   TH2 * fhHitEffMapMultTrkAng = new TH2D("TofQa_HitsEffMapMultTrkAng",    
                              "Efficiency of the Tof Hits generation for hits from multiple MC Trk; #theta_{x}[Deg.]; #theta_{y}[Deg.]; Eff. [\%]",
                              iNbBinThetaX, dThetaXMin, dThetaXMax,
                              iNbBinThetaY, dThetaYMin, dThetaYMax);
   TH2 * fhHitEffMapMultTrkSph = new TH2D("TofQa_HitsEffMapMultTrkSph", 
                              "Efficiency of the Tof Hits generation for hits from multiple MC Trk; #theta[rad.]; #phi[rad.]; Eff. [\%]",
                              iNbBinTheta, dThetaMin, dThetaMax,
                              iNbBinPhi,   dPhiMin,   dPhiMax); 


   TH2 * fhHitRatioMapMultTrkMultPntXY  = new TH2D("TofQa_HitsRatioMapMultTrkMultPntXY",
                              "Tof Hits generation: Fraction of hits from multi. Tracks among hits from multi. MC Pnt; X[cm]; Y[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbiny,-yrange,yrange);
   TH2 * fhHitRatioMapMultTrkMultPntXZ  = new TH2D("TofQa_HitsRatioMapMultTrkMultPntXZ",
                              "Tof Hits generation: Fraction of hits from multi. Tracks among hits from multi. MC Pnt; X[cm]; Z[cm]; Eff. [\%]",
                              nbinx,-xrange,xrange,nbinz,zmin,zmax);
   TH2 * fhHitRatioMapMultTrkMultPntYZ  = new TH2D("TofQa_HitsRatioMapMultTrkMultPntYZ",
                              "Tof Hits generation: Fraction of hits from multi. Tracks among hits from multi. MC Pnt; Y[cm]; Z[cm]; Eff. [\%]",
                              nbiny,-yrange,yrange,nbinz,zmin,zmax);
   TH2 * fhHitRatioMapMultTrkMultPntAng = new TH2D("TofQa_HitsRatioMapMultTrkMultPntAng",
                              "Tof Hits generation: Fraction of hits from multi. Tracks among hits from multi. MC Pnt; #theta_{x}[Deg.]; #theta_{y}[Deg.]; Eff. [\%]",
                              iNbBinThetaX, dThetaXMin, dThetaXMax,
                              iNbBinThetaY, dThetaYMin, dThetaYMax);
   TH2 * fhHitRatioMapMultTrkMultPntSph = new TH2D("TofQa_HitsRatioMapMultTrkMultPntSph",
                              "Tof Hits generation: Fraction of hits from multi. Tracks among hits from multi. MC Pnt; #theta[rad.]; #phi[rad.]; Eff. [\%]",
                              iNbBinTheta, dThetaMin, dThetaMax,
                              iNbBinPhi,   dPhiMin,   dPhiMax);

      // Physics coord mapping, 1 per particle type
         // Phase space
   std::vector<TH2 *> fvhPtmRapStsPntAcc;
   std::vector<TH2 *> fvhPtmRapTofPntAcc;
   std::vector<TH2 *> fvhPtmRapTofHitEff;
   std::vector<TH2 *> fvhPtmRapTofHitSinglePntRatio;
   std::vector<TH2 *> fvhPtmRapTofHitSingleTrkRatio;
   fvhPtmRapStsPntAcc.resize(kiNbPart);
   fvhPtmRapTofPntAcc.resize(kiNbPart);
   fvhPtmRapTofHitEff.resize(kiNbPart);
   fvhPtmRapTofHitSinglePntRatio.resize(kiNbPart);
   fvhPtmRapTofHitSingleTrkRatio.resize(kiNbPart);
   std::vector<TH2 *> fvhPtmRapTofPntEffSts;
   std::vector<TH2 *> fvhPtmRapTofHitEffSts;
   std::vector<TH2 *> fvhPtmRapTofHitSinglePntEffSts;
   std::vector<TH2 *> fvhPtmRapTofHitSingleTrkEffSts;
   fvhPtmRapTofPntEffSts.resize(kiNbPart);
   fvhPtmRapTofHitEffSts.resize(kiNbPart);
   fvhPtmRapTofHitSinglePntEffSts.resize(kiNbPart);
   fvhPtmRapTofHitSingleTrkEffSts.resize(kiNbPart);
         // PLab
   std::vector<TH1 *> fvhPlabStsPntAcc;
   std::vector<TH1 *> fvhPlabTofPntAcc;
   std::vector<TH1 *> fvhPlabTofHitEff;
   std::vector<TH1 *> fvhPlabTofHitSinglePntRatio;
   std::vector<TH1 *> fvhPlabTofHitSingleTrkRatio;
   fvhPlabStsPntAcc.resize(kiNbPart);
   fvhPlabTofPntAcc.resize(kiNbPart);
   fvhPlabTofHitEff.resize(kiNbPart);
   fvhPlabTofHitSinglePntRatio.resize(kiNbPart);
   fvhPlabTofHitSingleTrkRatio.resize(kiNbPart);
   std::vector<TH1 *> fvhPlabTofPntEffSts;
   std::vector<TH1 *> fvhPlabTofHitEffSts;
   std::vector<TH1 *> fvhPlabTofHitSinglePntEffSts;
   std::vector<TH1 *> fvhPlabTofHitSingleTrkEffSts;
   fvhPlabTofPntEffSts.resize(kiNbPart);
   fvhPlabTofHitEffSts.resize(kiNbPart);
   fvhPlabTofHitSinglePntEffSts.resize(kiNbPart);
   fvhPlabTofHitSingleTrkEffSts.resize(kiNbPart);
      // MC Tracks losses
   std::vector<TH2 *> fvhPtmRapGenTrkTofHitEff;
   std::vector<TH1 *> fvhPlabGenTrkTofhitEff;
   fvhPtmRapGenTrkTofHitEff.resize(kiNbPart);
   fvhPlabGenTrkTofhitEff.resize(kiNbPart);
   
   for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
   {
         // Phase space
      fvhPtmRapStsPntAcc[iPartIdx] = new TH2D( Form("TofQa_PtmRapStsPntAcc_%s", ksPartTag[iPartIdx].Data() ) ,
                              Form("P_{t}/M vs y acceptance for MC Track with STS points, %s; y; P_{t}/M; Acc. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsY,    dMinY,   dMaxY,
                              iNbBNinsPtm, dMinPtm, dMaxPtm);
      fvhPtmRapTofPntAcc[iPartIdx] = new TH2D( Form("TofQa_PtmRapTofPntAcc_%s", ksPartTag[iPartIdx].Data() ) ,
                              Form("P_{t}/M vs y acceptance from MC Track for TOF points, %s; y; P_{t}/M; Acc. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsY,    dMinY,   dMaxY,
                              iNbBNinsPtm, dMinPtm, dMaxPtm);
      fvhPtmRapTofHitEff[iPartIdx] = new TH2D( Form("TofQa_PtmRapTofHitEff_%s", ksPartTag[iPartIdx].Data() ) ,
                              Form("P_{t}/M vs y efficiency distribution for TOF Hits VS MC tracks(from MC Track), %s; y; P_{t}/M; Eff. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsY,    dMinY,   dMaxY,
                              iNbBNinsPtm, dMinPtm, dMaxPtm);
      fvhPtmRapTofHitSinglePntRatio[iPartIdx] = new TH2D( Form("TofQa_PtmRapTofHitSinglePntRatio_%s", ksPartTag[iPartIdx].Data() ),
                              Form("P_{t}/M vs y distribution of the ratio of TOF Hits from a single TOF Point (from MC Track), %s; y; P_{t}/M; Rat. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsY,    dMinY,   dMaxY,
                              iNbBNinsPtm, dMinPtm, dMaxPtm);
      fvhPtmRapTofHitSingleTrkRatio[iPartIdx] = new TH2D( Form("TofQa_PtmRapTofHitSingleTrkRatio_%s", ksPartTag[iPartIdx].Data() ),
                              Form("P_{t}/M vs y distribution of the ratio of TOF Hits from a single MC track (from MC Track), %s; y; P_{t}/M; Rat. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsY,    dMinY,   dMaxY,
                              iNbBNinsPtm, dMinPtm, dMaxPtm);
                              
      fvhPtmRapTofPntEffSts[iPartIdx] = new TH2D( Form("TofQa_PtmRapTofPntEffSts_%s", ksPartTag[iPartIdx].Data() ) ,
                              Form("P_{t}/M vs y distribution of the efficiency for TOF points VS STS (from MC Track), %s; y; P_{t}/M; Eff. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsY,    dMinY,   dMaxY,
                              iNbBNinsPtm, dMinPtm, dMaxPtm);
      fvhPtmRapTofHitEffSts[iPartIdx] = new TH2D( Form("TofQa_PtmRapTofHitEffSts_%s", ksPartTag[iPartIdx].Data() ) ,
                              Form("P_{t}/M vs y efficiency distribution for TOF Hits VS STS (from MC Track), %s; y; P_{t}/M; Eff. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsY,    dMinY,   dMaxY,
                              iNbBNinsPtm, dMinPtm, dMaxPtm);
      fvhPtmRapTofHitSinglePntEffSts[iPartIdx] = new TH2D( Form("TofQa_PtmRapTofHitSinglePntEffSts_%s", ksPartTag[iPartIdx].Data() ),
                              Form("P_{t}/M vs y efficiency distribution for TOF Hits from a single TOF Point VS STS (from MC Track), %s; y; P_{t}/M; Eff. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsY,    dMinY,   dMaxY,
                              iNbBNinsPtm, dMinPtm, dMaxPtm);
      fvhPtmRapTofHitSingleTrkEffSts[iPartIdx] = new TH2D( Form("TofQa_PtmRapTofHitSingleTrkEffSts_%s", ksPartTag[iPartIdx].Data() ),
                              Form("P_{t}/M vs y efficiency distribution for TOF Hits from a single MC track VS STS (from MC Track), %s; y; P_{t}/M; Eff. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsY,    dMinY,   dMaxY,
                              iNbBNinsPtm, dMinPtm, dMaxPtm);
         // PLab
      fvhPlabStsPntAcc[iPartIdx] = new TH1D( Form("TofQa_PlabStsPntAcc_%s", ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} acceptance for MC Track with STS points, %s; P_{lab} [GeV/c]; Acc. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
      fvhPlabTofPntAcc[iPartIdx] = new TH1D( Form("TofQa_PlabTofPntAcc_%s", ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} acceptance from MC Track for TOF points, %s; P_{lab} [GeV/c]; Acc. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
      fvhPlabTofHitEff[iPartIdx] = new TH1D( Form("TofQa_PlabTofHitEff_%s", ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} efficiency distribution for TOF Hits VS MC tracks(from MC Track), %s; P_{lab} [GeV/c]; Eff. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
      fvhPlabTofHitSinglePntRatio[iPartIdx] = new TH1D( Form("TofQa_PlabTofHitSinglePntRatio_%s", ksPartTag[iPartIdx].Data() ), 
                              Form("P_{lab} distribution of the ratio of TOF Hits from a single TOF Point (from MC Track), %s; P_{lab} [GeV/c]; Rat. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
      fvhPlabTofHitSingleTrkRatio[iPartIdx] = new TH1D( Form("TofQa_PlabTofHitSingleTrkRatio_%s", ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} distribution of the ratio of TOF Hits from a single MC track (from MC Track), %s; P_{lab} [GeV/c]; Rat. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
                              
      fvhPlabTofPntEffSts[iPartIdx] = new TH1D( Form("TofQa_PlabTofPntEffSts_%s", ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} distribution of the efficiency for TOF points VS STS (from MC Track), %s; P_{lab} [GeV/c]; Eff. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
      fvhPlabTofHitEffSts[iPartIdx] = new TH1D( Form("TofQa_PlabTofHitEffSts_%s", ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} efficiency distribution for TOF Hits VS STS (from MC Track), %s; P_{lab} [GeV/c]; Eff. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
      fvhPlabTofHitSinglePntEffSts[iPartIdx] = new TH1D( Form("TofQa_PlabTofHitSinglePntEffSts_%s", ksPartTag[iPartIdx].Data() ), 
                              Form("P_{lab} efficiency distribution for TOF Hits from a single TOF Point VS STS (from MC Track), %s; P_{lab} [GeV/c]; Eff. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
      fvhPlabTofHitSingleTrkEffSts[iPartIdx] = new TH1D( Form("TofQa_PlabTofHitSingleTrkEffSts_%s", ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} efficiency distribution for TOF Hits from a single MC track VS STS (from MC Track), %s; P_{lab} [GeV/c]; Eff. [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
         
         // MC Tracks losses
      fvhPtmRapGenTrkTofHitEff[iPartIdx] = new TH2D( Form("TofQa_PtmRapGenTrkTofHitEff_%s", ksPartTag[iPartIdx].Data() ) ,  
                              Form("P_{t}/M vs y efficiency distribution for MC tracks with TOF Hit(s), %s; y; P_{t}/M; Trk w. Hits [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsY,    dMinY,   dMaxY,
                              iNbBNinsPtm, dMinPtm, dMaxPtm);
      fvhPlabGenTrkTofhitEff[iPartIdx] = new TH1D( Form("TofQa_PlabGenTrkTofHit_%s", ksPartTag[iPartIdx].Data() ),
                              Form("P_{lab} efficiency distribution for MC tracks with TOF Hit(s), %s; P_{lab} [GeV/c]; Trk w. Hits [\%]", ksPartName[iPartIdx].Data()  ),
                              iNbBinsPlab, dMinPlab, dMaxPlab);
   } // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
   
   std::cout << "Histo creation OK!" << std::endl;
//******* Fills the output histograms
   
      // Geometric Mapping
   fhHitEffMapXY ->Divide( fhHitMapXY,  fhPointMapXY);
   fhHitEffMapXZ ->Divide( fhHitMapXZ,  fhPointMapXZ);
   fhHitEffMapYZ ->Divide( fhHitMapYZ,  fhPointMapYZ);
   fhHitEffMapAng->Divide( fhHitMapAng, fhPointMapAng);
   fhHitEffMapSph->Divide( fhHitMapSph, fhPointMapSph);
   
      // Mapping of position for hits coming from a single MC Point
   fhHitEffMapSingPntXY ->Divide( fhHitMapSingPntXY,  fhPointMapXY);
   fhHitEffMapSingPntXZ ->Divide( fhHitMapSingPntXZ,  fhPointMapXZ);
   fhHitEffMapSingPntYZ ->Divide( fhHitMapSingPntYZ,  fhPointMapYZ);
   fhHitEffMapSingPntAng->Divide( fhHitMapSingPntAng, fhPointMapAng);
   fhHitEffMapSingPntSph->Divide( fhHitMapSingPntSph, fhPointMapSph);

   fhHitRatioMapSingPntXY ->Divide( fhHitMapSingPntXY,  fhHitMapXY);
   fhHitRatioMapSingPntXZ ->Divide( fhHitMapSingPntXZ,  fhHitMapXZ);
   fhHitRatioMapSingPntYZ ->Divide( fhHitMapSingPntYZ,  fhHitMapYZ);
   fhHitRatioMapSingPntAng->Divide( fhHitMapSingPntAng, fhHitMapAng);
   fhHitRatioMapSingPntSph->Divide( fhHitMapSingPntSph, fhHitMapSph);
      // Mapping of position for hits coming from multiple MC Points
   fhHitEffMapMultPntXY ->Divide( fhHitMapMultPntXY,  fhPointMapXY);
   fhHitEffMapMultPntXZ ->Divide( fhHitMapMultPntXZ,  fhPointMapXZ);
   fhHitEffMapMultPntYZ ->Divide( fhHitMapMultPntYZ,  fhPointMapYZ);
   fhHitEffMapMultPntAng->Divide( fhHitMapMultPntAng, fhPointMapAng);
   fhHitEffMapMultPntSph->Divide( fhHitMapMultPntSph, fhPointMapSph);
      // Mapping of position for hits coming from a single MC Track
   fhHitEffMapSingTrkXY ->Divide( fhHitMapSingTrkXY,  fhPointMapXY);
   fhHitEffMapSingTrkXZ ->Divide( fhHitMapSingTrkXZ,  fhPointMapXZ);
   fhHitEffMapSingTrkYZ ->Divide( fhHitMapSingTrkYZ,  fhPointMapYZ);
   fhHitEffMapSingTrkAng->Divide( fhHitMapSingTrkAng, fhPointMapAng);
   fhHitEffMapSingTrkSph->Divide( fhHitMapSingTrkSph, fhPointMapSph);

   fhHitRatioMapSingTrkXY ->Divide( fhHitMapSingTrkXY,  fhHitMapXY);
   fhHitRatioMapSingTrkXZ ->Divide( fhHitMapSingTrkXZ,  fhHitMapXZ);
   fhHitRatioMapSingTrkYZ ->Divide( fhHitMapSingTrkYZ,  fhHitMapYZ);
   fhHitRatioMapSingTrkAng->Divide( fhHitMapSingTrkAng, fhHitMapAng);
   fhHitRatioMapSingTrkSph->Divide( fhHitMapSingTrkSph, fhHitMapSph);
      // Mapping of position for hits coming from multiple MC Tracks
   fhHitEffMapMultTrkXY ->Divide( fhHitMapMultTrkXY,  fhPointMapXY);
   fhHitEffMapMultTrkXZ ->Divide( fhHitMapMultTrkXZ,  fhPointMapXZ);
   fhHitEffMapMultTrkYZ ->Divide( fhHitMapMultTrkYZ,  fhPointMapYZ);
   fhHitEffMapMultTrkAng->Divide( fhHitMapMultTrkAng, fhPointMapAng);
   fhHitEffMapMultTrkSph->Divide( fhHitMapMultTrkSph, fhPointMapSph);

   fhHitRatioMapMultTrkMultPntXY ->Divide( fhHitMapMultTrkXY,  fhHitMapMultPntXY);
   fhHitRatioMapMultTrkMultPntXZ ->Divide( fhHitMapMultTrkXZ,  fhHitMapMultPntXZ);
   fhHitRatioMapMultTrkMultPntYZ ->Divide( fhHitMapMultTrkYZ,  fhHitMapMultPntYZ);
   fhHitRatioMapMultTrkMultPntAng->Divide( fhHitMapMultTrkAng, fhHitMapMultPntAng);
   fhHitRatioMapMultTrkMultPntSph->Divide( fhHitMapMultTrkSph, fhHitMapMultPntSph);

      // Physics coord mapping, 1 per particle type  
   for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
   {
         // Phase space
      fvhPtmRapStsPntAcc[iPartIdx]->Divide( fvhPtmRapStsPnt[iPartIdx], 
                                            fvhPtmRapGenTrk[iPartIdx]);
      fvhPtmRapTofPntAcc[iPartIdx]->Divide( fvhPtmRapTofPnt[iPartIdx], 
                                            fvhPtmRapGenTrk[iPartIdx]);
      fvhPtmRapTofHitEff[iPartIdx]->Divide( fvhPtmRapTofHit[iPartIdx], 
                                            fvhPtmRapGenTrk[iPartIdx]);
      fvhPtmRapTofHitSinglePntRatio[iPartIdx]->Divide( fvhPtmRapTofHitSinglePnt[iPartIdx], 
                                                       fvhPtmRapTofHit[iPartIdx]);;
      fvhPtmRapTofHitSingleTrkRatio[iPartIdx]->Divide( fvhPtmRapTofHitSingleTrk[iPartIdx], 
                                                       fvhPtmRapTofHit[iPartIdx]);
                                            
      fvhPtmRapTofPntEffSts[iPartIdx]->Divide( fvhPtmRapTofPnt[iPartIdx], 
                                               fvhPtmRapStsPnt[iPartIdx]);
      fvhPtmRapTofHitEffSts[iPartIdx]->Divide( fvhPtmRapTofHit[iPartIdx], 
                                               fvhPtmRapStsPnt[iPartIdx]);
      fvhPtmRapTofHitSinglePntEffSts[iPartIdx]->Divide( fvhPtmRapTofHitSinglePnt[iPartIdx], 
                                                        fvhPtmRapStsPnt[iPartIdx]);
      fvhPtmRapTofHitSingleTrkEffSts[iPartIdx]->Divide( fvhPtmRapTofHitSingleTrk[iPartIdx], 
                                                        fvhPtmRapStsPnt[iPartIdx]);
         // PLab
      fvhPlabStsPntAcc[iPartIdx]->Divide( fvhPlabStsPnt[iPartIdx], 
                                          fvhPlabGenTrk[iPartIdx]);
      fvhPlabTofPntAcc[iPartIdx]->Divide( fvhPlabTofPnt[iPartIdx], 
                                          fvhPlabGenTrk[iPartIdx]);
      fvhPlabTofHitEff[iPartIdx]->Divide( fvhPlabTofHit[iPartIdx], 
                                          fvhPlabGenTrk[iPartIdx]);
      fvhPlabTofHitSinglePntRatio[iPartIdx]->Divide( fvhPlabTofHitSinglePnt[iPartIdx], 
                                                     fvhPlabTofHit[iPartIdx]);
      fvhPlabTofHitSingleTrkRatio[iPartIdx]->Divide( fvhPlabTofHitSingleTrk[iPartIdx], 
                                                     fvhPlabTofHit[iPartIdx]);
                                            
      fvhPlabTofPntEffSts[iPartIdx]->Divide( fvhPlabTofPnt[iPartIdx], 
                                             fvhPlabStsPnt[iPartIdx]);
      fvhPlabTofHitEffSts[iPartIdx]->Divide( fvhPlabTofHit[iPartIdx], 
                                             fvhPlabStsPnt[iPartIdx]);
      fvhPlabTofHitSinglePntEffSts[iPartIdx]->Divide( fvhPlabTofHitSinglePnt[iPartIdx], 
                                                      fvhPlabStsPnt[iPartIdx]);
      fvhPlabTofHitSingleTrkEffSts[iPartIdx]->Divide( fvhPlabTofHitSingleTrk[iPartIdx], 
                                                      fvhPlabStsPnt[iPartIdx]);
      // MC Tracks losses 
      fvhPtmRapGenTrkTofHitEff[iPartIdx]->Divide( fvhPtmRapGenTrkTofHit[iPartIdx], 
                                                  fvhPtmRapGenTrkTofPnt[iPartIdx]);
      fvhPlabGenTrkTofhitEff[iPartIdx]  ->Divide( fvhPlabGenTrkTofhit[iPartIdx], 
                                                  fvhPlabGenTrkTofPnt[iPartIdx]);
   } // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)

   std::cout << "Histo filling OK!" << std::endl;
//******* Scales the output histograms to percentage
      // Geometric Mapping
   fhHitEffMapXY ->Scale( 100.0 );
   fhHitEffMapXZ ->Scale( 100.0 );
   fhHitEffMapYZ ->Scale( 100.0 );
   fhHitEffMapAng->Scale( 100.0 );
   fhHitEffMapSph->Scale( 100.0 );
   
      // Mapping of position for hits coming from a single MC Point
   fhHitEffMapSingPntXY ->Scale( 100.0 );
   fhHitEffMapSingPntXZ ->Scale( 100.0 );
   fhHitEffMapSingPntYZ ->Scale( 100.0 );
   fhHitEffMapSingPntAng->Scale( 100.0 );
   fhHitEffMapSingPntSph->Scale( 100.0 );

   fhHitRatioMapSingPntXY ->Scale( 100.0 );
   fhHitRatioMapSingPntXZ ->Scale( 100.0 );
   fhHitRatioMapSingPntYZ ->Scale( 100.0 );
   fhHitRatioMapSingPntAng->Scale( 100.0 );
   fhHitRatioMapSingPntSph->Scale( 100.0 );
      // Mapping of position for hits coming from multiple MC Points
   fhHitEffMapMultPntXY ->Scale( 100.0 );
   fhHitEffMapMultPntXZ ->Scale( 100.0 );
   fhHitEffMapMultPntYZ ->Scale( 100.0 );
   fhHitEffMapMultPntAng->Scale( 100.0 );
   fhHitEffMapMultPntSph->Scale( 100.0 );
      // Mapping of position for hits coming from a single MC Track
   fhHitEffMapSingTrkXY ->Scale( 100.0 );
   fhHitEffMapSingTrkXZ ->Scale( 100.0 );
   fhHitEffMapSingTrkYZ ->Scale( 100.0 );
   fhHitEffMapSingTrkAng->Scale( 100.0 );
   fhHitEffMapSingTrkSph->Scale( 100.0 );

   fhHitRatioMapSingTrkXY ->Scale( 100.0 );
   fhHitRatioMapSingTrkXZ ->Scale( 100.0 );
   fhHitRatioMapSingTrkYZ ->Scale( 100.0 );
   fhHitRatioMapSingTrkAng->Scale( 100.0 );
   fhHitRatioMapSingTrkSph->Scale( 100.0 );
      // Mapping of position for hits coming from multiple MC Tracks
   fhHitEffMapMultTrkXY ->Scale( 100.0 );
   fhHitEffMapMultTrkXZ ->Scale( 100.0 );
   fhHitEffMapMultTrkYZ ->Scale( 100.0 );
   fhHitEffMapMultTrkAng->Scale( 100.0 );
   fhHitEffMapMultTrkSph->Scale( 100.0 );

   fhHitRatioMapMultTrkMultPntXY ->Scale( 100.0 );
   fhHitRatioMapMultTrkMultPntXZ ->Scale( 100.0 );
   fhHitRatioMapMultTrkMultPntYZ ->Scale( 100.0 );
   fhHitRatioMapMultTrkMultPntAng->Scale( 100.0 );
   fhHitRatioMapMultTrkMultPntSph->Scale( 100.0 );

      // Physics coord mapping, 1 per particle type      
   for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
   {
         // Phase space
      fvhPtmRapStsPntAcc[iPartIdx]->Scale( 100.0 );
      fvhPtmRapTofPntAcc[iPartIdx]->Scale( 100.0 );
      fvhPtmRapTofHitEff[iPartIdx]->Scale( 100.0 );
      fvhPtmRapTofHitSinglePntRatio[iPartIdx]->Scale( 100.0 );
      fvhPtmRapTofHitSingleTrkRatio[iPartIdx]->Scale( 100.0 );
      fvhPtmRapTofPntEffSts[iPartIdx]->Scale( 100.0 );
      fvhPtmRapTofHitEffSts[iPartIdx]->Scale( 100.0 );
      fvhPtmRapTofHitSinglePntEffSts[iPartIdx]->Scale( 100.0 );
      fvhPtmRapTofHitSingleTrkEffSts[iPartIdx]->Scale( 100.0 );
         // PLab
      fvhPlabStsPntAcc[iPartIdx]->Scale( 100.0 );
      fvhPlabTofPntAcc[iPartIdx]->Scale( 100.0 );
      fvhPlabTofHitEff[iPartIdx]->Scale( 100.0 );
      fvhPlabTofHitSinglePntRatio[iPartIdx]->Scale( 100.0 );
      fvhPlabTofHitSingleTrkRatio[iPartIdx]->Scale( 100.0 );
      fvhPlabTofPntEffSts[iPartIdx]->Scale( 100.0 );
      fvhPlabTofHitEffSts[iPartIdx]->Scale( 100.0 );
      fvhPlabTofHitSinglePntEffSts[iPartIdx]->Scale( 100.0 );
      fvhPlabTofHitSingleTrkEffSts[iPartIdx]->Scale( 100.0 );
      // MC Tracks losses 
      fvhPtmRapGenTrkTofHitEff[iPartIdx]->Scale( 100.0 );
      fvhPlabGenTrkTofhitEff[iPartIdx]->Scale( 100.0 );
   } // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)

   std::cout << "Histo scaling OK!" << std::endl;
//******* Saves the output histograms
   fOutHistFile->cd();

   TDirectory *cdGeomMap = fOutHistFile->mkdir( "GeomMap" );
   cdGeomMap->cd();    // make the "GeomMap" directory the current directory
      // Geometric Mapping
   fhHitEffMapXY ->Write();
   fhHitEffMapXZ ->Write();
   fhHitEffMapYZ ->Write();
   fhHitEffMapAng->Write();
   fhHitEffMapSph->Write();
   
   TDirectory *cdMixMap = fOutHistFile->mkdir( "MixMap" );
   cdMixMap->cd();    // make the "MixMap" directory the current directory
      // Mapping of position for hits coming from a single MC Point
   fhHitEffMapSingPntXY ->Write();
   fhHitEffMapSingPntXZ ->Write();
   fhHitEffMapSingPntYZ ->Write();
   fhHitEffMapSingPntAng->Write();
   fhHitEffMapSingPntSph->Write();

   fhHitRatioMapSingPntXY ->Write();
   fhHitRatioMapSingPntXZ ->Write();
   fhHitRatioMapSingPntYZ ->Write();
   fhHitRatioMapSingPntAng->Write();
   fhHitRatioMapSingPntSph->Write();
      // Mapping of position for hits coming from multiple MC Points
   fhHitEffMapMultPntXY ->Write();
   fhHitEffMapMultPntXZ ->Write();
   fhHitEffMapMultPntYZ ->Write();
   fhHitEffMapMultPntAng->Write();
   fhHitEffMapMultPntSph->Write();
      // Mapping of position for hits coming from a single MC Track
   fhHitEffMapSingTrkXY ->Write();
   fhHitEffMapSingTrkXZ ->Write();
   fhHitEffMapSingTrkYZ ->Write();
   fhHitEffMapSingTrkAng->Write();
   fhHitEffMapSingTrkSph->Write();

   fhHitRatioMapSingTrkXY ->Write();
   fhHitRatioMapSingTrkXZ ->Write();
   fhHitRatioMapSingTrkYZ ->Write();
   fhHitRatioMapSingTrkAng->Write();
   fhHitRatioMapSingTrkSph->Write();
      // Mapping of position for hits coming from multiple MC Tracks
   fhHitEffMapMultTrkXY ->Write();
   fhHitEffMapMultTrkXZ ->Write();
   fhHitEffMapMultTrkYZ ->Write();
   fhHitEffMapMultTrkAng->Write();
   fhHitEffMapMultTrkSph->Write();

   fhHitRatioMapMultTrkMultPntXY ->Write();
   fhHitRatioMapMultTrkMultPntXZ ->Write();
   fhHitRatioMapMultTrkMultPntYZ ->Write();
   fhHitRatioMapMultTrkMultPntAng->Write();
   fhHitRatioMapMultTrkMultPntSph->Write();

      // Physics coord mapping, 1 per particle type      
   TDirectory *cdPhysMap = fOutHistFile->mkdir( "PhysMap" );
   cdPhysMap->cd();    // make the "PhysMap" directory the current directory
   for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
   {
         // Phase space
      fvhPtmRapStsPntAcc[iPartIdx]->Write();
      fvhPtmRapTofPntAcc[iPartIdx]->Write();
      fvhPtmRapTofHitEff[iPartIdx]->Write();
      fvhPtmRapTofHitSinglePntRatio[iPartIdx]->Write();
      fvhPtmRapTofHitSingleTrkRatio[iPartIdx]->Write();
      fvhPtmRapTofPntEffSts[iPartIdx]->Write();
      fvhPtmRapTofHitEffSts[iPartIdx]->Write();
      fvhPtmRapTofHitSinglePntEffSts[iPartIdx]->Write();
      fvhPtmRapTofHitSingleTrkEffSts[iPartIdx]->Write();
         // PLab
      fvhPlabStsPntAcc[iPartIdx]->Write();
      fvhPlabTofPntAcc[iPartIdx]->Write();
      fvhPlabTofHitEff[iPartIdx]->Write();
      fvhPlabTofHitSinglePntRatio[iPartIdx]->Write();
      fvhPlabTofHitSingleTrkRatio[iPartIdx]->Write();
      fvhPlabTofPntEffSts[iPartIdx]->Write();
      fvhPlabTofHitEffSts[iPartIdx]->Write();
      fvhPlabTofHitSinglePntEffSts[iPartIdx]->Write();
      fvhPlabTofHitSingleTrkEffSts[iPartIdx]->Write();
      // MC Tracks losses 
      fvhPtmRapGenTrkTofHitEff[iPartIdx]->Write();
      fvhPlabGenTrkTofhitEff[iPartIdx]->Write();
   } // for( Int_t iPartIdx = 0; iPartIdx < kiNbPart; iPartIdx++)
   
   std::cout << "Histo writing OK!" << std::endl;
//******* Closes the output file
   fOutHistFile->Close();

   std::cout << "out file closing OK!" << std::endl;
   
   return kTRUE;
}
