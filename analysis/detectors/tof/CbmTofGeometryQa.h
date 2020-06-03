/** @file CbmTofGeometryQa.h
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @date 27/08/2015
 **/

/** @class CbmTofGeometryQa
 ** @brief QA class for the TOF event based Hit finder tasks (clusterizers, ...)
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @version 1.0
 **/
#ifndef _CbmTofGeometryQa_H_
#define _CbmTofGeometryQa_H_

#include "FairTask.h"

class FairMCEventHeader;
class CbmTofGeoHandler;
class CbmTofCell;
// Geometry
class CbmTofGeoHandler;
class CbmTofDetectorId;
class CbmTofCell;

class CbmTofDigiPar;
class CbmTofDigiBdfPar;

class TClonesArray;
class TH1;
class TH2;
class TH3;
class TProfile2D;
class TString;

class CbmTofGeometryQa : public FairTask {

   public:
       CbmTofGeometryQa();
       CbmTofGeometryQa(const char* name, Int_t verbose = 1);
       virtual ~CbmTofGeometryQa();

       virtual InitStatus Init();
       virtual void Exec(Option_t* option);
       virtual void Finish();

       /**
        ** @brief Inherited from FairTask.
        **/
       virtual void SetParContainers();

       Bool_t   RegisterInputs();

       Bool_t   SetHistoFileName( TString sFilenameIn );

       Bool_t   SetWallPosZ( Double_t dWallPosCm = 1000);

       void     SetCentralityDepFlag( Bool_t bFlag = kTRUE ) { fbCentDepOn = bFlag; }
       void     SetSphereApproxCheckFlag( Bool_t bFlag = kTRUE ) { fbSphAppOn = bFlag; }

   private:
      Bool_t   CreateHistos();
      Bool_t   FillHistos();
      Bool_t   NormalizeMapHistos();
      Bool_t   NormalizeNormHistos();
      Bool_t   WriteHistos();
      Bool_t   DeleteHistos();

      /**
      ** @brief Load the geometry: for now just resizing the Digis temporary vectors
      **/
      Bool_t   LoadGeometry();

      CbmTofGeometryQa(const CbmTofGeometryQa&);
      CbmTofGeometryQa operator=(const CbmTofGeometryQa&);

      Int_t         fEvents;            // Number of processed events

      // Geometry infos
      CbmTofGeoHandler * fGeoHandler;
      CbmTofDetectorId * fTofId;
      CbmTofCell       * fChannelInfo;
      Int_t iNbSmTot;
      std::vector< Int_t >                               fvTypeSmOffs; // Offset in SM index for first SM of each SM type
      Int_t iNbRpcTot;
      std::vector< std::vector< Int_t > >                fvSmRpcOffs;  // Offset in RPC index for first RPC of each SM
      Int_t fiNbChTot;
      std::vector< std::vector< std::vector< Int_t > > > fvRpcChOffs;  // Offset in channel index for first channel of each RPC

      // Parameters

      // Data IO
      FairMCEventHeader     * fMCEventHeader; // MC event header
//      TClonesArray          * fStsPointsColl; // STS MC points
      TClonesArray          * fTofPointsColl; // TOF MC points
      TClonesArray          * fMcTracksColl;  // MC tracks
      TClonesArray          * fRealTofPointsColl; // Realistics TOF MC points
      TClonesArray          * fRealTofMatchColl;  // Index of Realistics TOF MC points for each MC Point (CbmMatch)
      Bool_t                  fbRealPointAvail;

      // Histograms
         // Output file name and path
      TString fsHistoOutFilename;
         // Position of the TOF wall on Z axis for centering histos with Z
      Double_t fdWallPosZ;
         // Geometric Mapping
      Bool_t             fbCentDepOn; // Switch on study of dependence on centrality
      std::vector<TH2 *> fvhTrackAllStartZCent; // Dependence of Track origin on centrality, if TOF points
      std::vector<TH2 *> fvhTrackSecStartZCent; // Dependence of Track origin on centrality, if TOF points
      std::vector<TH3 *> fvhTrackAllStartXZCent; // Dependence of Track origin on centrality, if TOF points
      std::vector<TH3 *> fvhTofPntAllAngCent; // Dependence of Tof Point position (angular) on centrality
      std::vector<TH2 *> fvhTrackAllStartXZ;    // Track origin mapping, if TOF points
      std::vector<TH2 *> fvhTrackAllStartYZ;    // Track origin mapping, if TOF points
      TH2 * fhTrackMapXY;  // Only when creating normalization histos
      TH2 * fhTrackMapXZ;  // Only when creating normalization histos
      TH2 * fhTrackMapYZ;  // Only when creating normalization histos
      TH2 * fhTrackMapAng; // Only when creating normalization histos
      TH2 * fhTrackMapSph; // Only when creating normalization histos
      TH2 * fhTrackMapAngPrimAll;  // For angular acceptance study
      TH2 * fhTrackMapAngPrimSts;  // For angular acceptance study
      TH2 * fhTrackMapAngPrimRich; // For angular acceptance study
      TH2 * fhTrackMapAngPrimMuch; // For angular acceptance study
      TH2 * fhTrackMapAngPrimTrd;  // For angular acceptance study
      TH2 * fhTrackMapAngPrimTof;  // For angular acceptance study
/*
      TH2 * fhTrackMapAngSecAll;  // For angular acceptance study
      TH2 * fhTrackMapAngSecSts;  // For angular acceptance study
      TH2 * fhTrackMapAngSecRich; // For angular acceptance study
      TH2 * fhTrackMapAngSecMuch; // For angular acceptance study
      TH2 * fhTrackMapAngSecTrd;  // For angular acceptance study
      TH2 * fhTrackMapAngSecTof;  // For angular acceptance study
*/
      TH2 * fhPointMapXY;
      TH2 * fhPointMapXZ;
      TH2 * fhPointMapYZ;
      TH2 * fhPointMapAng;
      TH2 * fhPointMapSph;
      TH2 * fhRealPointMapXY;
      TH2 * fhRealPointMapXZ;
      TH2 * fhRealPointMapYZ;
      TH2 * fhRealPointMapAng;
      TH2 * fhRealPointMapSph;
      TH2 * fhPointMapAngWithSts;  // For angular acceptance study
      TH2 * fhPointMapAngWithRich; // For angular acceptance study
      TH2 * fhPointMapAngWithMuch; // For angular acceptance study
      TH2 * fhPointMapAngWithTrd;  // For angular acceptance study

         // Errors relative to spherical approx
      Bool_t       fbSphAppOn;
      TProfile2D * fhPointSphAprRadiusErrMapXY;
      TProfile2D * fhPointSphAprRadiusErrMapXZ;
      TProfile2D * fhPointSphAprRadiusErrMapYZ;
      TProfile2D * fhPointSphAprRadiusErrMapAng;
      TProfile2D * fhPointSphAprRadiusErrMapSph;
      TProfile2D * fhPointSphAprZposErrMapXY;
      TProfile2D * fhPointSphAprZposErrMapXZ;
      TProfile2D * fhPointSphAprZposErrMapYZ;
      TProfile2D * fhPointSphAprZposErrMapAng;
      TProfile2D * fhPointSphAprZposErrMapSph;

         // Physics coord mapping, 1 per particle type
            // Phase space
               // Primary tracks
      std::vector<TH2 *> fvhPtmRapGenTrk;
      std::vector<TH2 *> fvhPtmRapStsPnt;
      std::vector<TH2 *> fvhPtmRapTofPnt;
               // Secondary tracks
      std::vector<TH2 *> fvhPtmRapSecGenTrk;
      std::vector<TH2 *> fvhPtmRapSecStsPnt;
      std::vector<TH2 *> fvhPtmRapSecTofPnt;

            // PLab
               // Primary tracks
      std::vector<TH1 *> fvhPlabGenTrk;
      std::vector<TH1 *> fvhPlabStsPnt;
      std::vector<TH1 *> fvhPlabTofPnt;
               // Secondary tracks
      std::vector<TH1 *> fvhPlabSecGenTrk;
      std::vector<TH1 *> fvhPlabSecStsPnt;
      std::vector<TH1 *> fvhPlabSecTofPnt;

            // MC Tracks losses
               // Primary tracks
      std::vector<TH2 *> fvhPtmRapGenTrkTofPnt;
      std::vector<TH1 *> fvhPlabGenTrkTofPnt;
      std::vector<TH1 *> fvhPlabStsTrkTofPnt;
               // Secondary tracks
      std::vector<TH2 *> fvhPtmRapSecGenTrkTofPnt;
      std::vector<TH1 *> fvhPlabSecGenTrkTofPnt;
      std::vector<TH1 *> fvhPlabSecStsTrkTofPnt;

      ClassDef(CbmTofGeometryQa, 1);
};

#endif

