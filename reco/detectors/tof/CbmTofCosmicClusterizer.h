/** @file CbmTofCosmicClusterizer.h
 ** @author nh adopted from CbmTofTestBeamClusterizer.h
 ** @author Norbert Herrmann <herrmann@physi.uni-heidelberg.de>
 ** @date 03.12.2017
 **/
/** @class CbmTofSimpClusterizer
 ** @brief Simple Cluster building and hit producing for CBM ToF using Digis as input
 ** @author Pierre-Alain Loizeau <loizeau@physi.uni-heidelberg.de>
 ** @version 1.0
 **/
#ifndef CBMTOFCOSMICCLUSTERIZER_H
#define CBMTOFCOSMICCLUSTERIZER_H  1

// TOF Classes and includes
// Input/Output
//class CbmTofPoint;
class CbmTofDigi;
class CbmTofHit;
class CbmMatch;
   // Geometry
class CbmTofGeoHandler;
class CbmTofDetectorId;
class CbmTofDigiPar;
class CbmTofDigiBdfPar;
class CbmTofCell;
class CbmTofFindTracks;

class TTofCalibData;
class TTrbHeader;

// FAIR classes and includes
#include "FairTask.h"
#include "CbmTofAddress.h"    // in cbmdata/tof

// ROOT Classes and includes
class TClonesArray;
class TF1;
class TH1;
class TH2;
class TH3;
class TProfile;
class TString;
#include "TTimeStamp.h"

// C++ Classes and includes
#include <vector>
#include <map>
#include <list>

class CbmTofCosmicClusterizer : public FairTask
{

   friend class CbmTofAnaTestbeam;

   public:
      inline static CbmTofCosmicClusterizer *Instance(){ return fInstance; }

      /**
       ** @brief Constructor.
       **/
      CbmTofCosmicClusterizer();

      /**
       ** @brief Constructor.
       **/
      CbmTofCosmicClusterizer(const char *name, Int_t verbose = 1, Bool_t writeDataInOut = kTRUE);
      /**
       ** @brief Destructor.
       **/
      virtual ~CbmTofCosmicClusterizer();

      /**
       ** @brief Inherited from FairTask.
       **/
      virtual InitStatus Init();

      /**
       ** @brief Inherited from FairTask.
       **/
      virtual void SetParContainers();

      /**
       ** @brief Inherited from FairTask.
       **/
      virtual void Exec(Option_t * option);

      /**
       ** @brief Inherited from FairTask.
       **/
      virtual void Finish();
      virtual void Finish(Double_t calMode);

      inline void SetCalMode    (Int_t iMode)           { fCalMode     = iMode;}
      inline void SetCalSel     (Int_t iSel)            { fCalSel      = iSel;}
      inline void SetCalSmType  (Int_t iCalSmType)      
      { fCalSmAddr   = CbmTofAddress::GetUniqueAddress(0,0,0,0,iCalSmType);}
      inline void SetCalSmAddr  (Int_t iCalSmAddr)      { fCalSmAddr   = iCalSmAddr;}
      inline void SetCalRpc  (Int_t iCalRpc)      
      { if(iCalRpc != 0) {
	  Int_t iSign = iCalRpc/TMath::Abs(iCalRpc);
	  iCalRpc*=iSign;  // always a positive number 
	  Int_t iRpc = iCalRpc%10;
	  iCalRpc=(iCalRpc - iRpc)/10;
	  Int_t iSm = iCalRpc%10;
	  iCalRpc=(iCalRpc - iSm)/10;
	  iRpc=0; // ignore RPC number 
	  fCalSmAddr   = iSign*CbmTofAddress::GetUniqueAddress(iSm,iRpc,0,0,iCalRpc);}
	else { fCalSmAddr=0; }
      }
      inline void SetCaldXdYMax (Double_t dCaldXdYMax)  { fdCaldXdYMax = dCaldXdYMax;}
      inline void SetCalCluMulMax (Int_t ival)          { fiCluMulMax  = ival;}
      inline void SetTRefId     (Int_t Id)              { fTRefMode    = Id;}
      inline void SetDutId      (Int_t Id)              { fDutId       = Id;}
      inline void SetDutSm      (Int_t Id)              { fDutSm       = Id;}
      inline void SetDutRpc     (Int_t Id)              { fDutRpc      = Id;}
      inline void SetSelId      (Int_t Id)              { fSelId       = Id;}
      inline void SetSelSm      (Int_t Id)              { fSelSm       = Id;}
      inline void SetSelRpc     (Int_t Id)              { fSelRpc      = Id;}
      inline void SetTRefDifMax (Double_t val)          { fTRefDifMax  = val;}
      inline void SetdTRefMax   (Double_t val)          { fdTRefMax    = val;}
      inline void PosYMaxScal   (Double_t val)          { fPosYMaxScal = val;}
      inline void SetTotMax     (Double_t val)          { fTotMax      = val;}
      inline void SetTotMin     (Double_t val)          { fTotMin      = val;}
      inline void SetTotMean    (Double_t val)          { fTotMean     = val;}
      inline void SetDelTofMax  (Double_t val)          { fdDelTofMax  = val;}
      inline void SetTotPreRange(Double_t val)          { fTotPreRange = val;}
      inline void SetMaxTimeDist(Double_t val)          { fMaxTimeDist = val;}
      inline void SetChannelDeadtime(Double_t val)      { fdChannelDeadtime = val;}
      inline void SetMemoryTime (Double_t val)          { fdMemoryTime = val;}
      inline void SetYFitMin    (Double_t val)          { fdYFitMin = val;}
      inline void SetSel2Id     (Int_t ival)            { fSel2Id = ival;}
      inline void SetSel2Sm     (Int_t ival)            { fSel2Sm = ival;}
      inline void SetSel2Rpc    (Int_t ival)            { fSel2Rpc = ival;}
      inline void SetSel2MulMax (Int_t Id)              { fSel2MulMax    = Id;}

      inline void SetOutHstFileName(TString OutHstFileName) { fOutHstFileName = OutHstFileName; }
      inline void SetCalParFileName(TString CalParFileName) { fCalParFileName = CalParFileName; }

      inline void SetEnableMatchPosScaling(Bool_t bval) { fEnableMatchPosScaling = bval; }
      inline void SetEnableAvWalk(Bool_t bval)          { fEnableAvWalk = bval; }
      inline void SetPs2Ns(Bool_t bval)                 { fbPs2Ns = bval; }
      inline void SetTimePeriod(Double_t dval)          { fdTimePeriod = dval; }
      inline void SetCorMode(Int_t ival)                { fiCorMode = ival; }

      //static Double_t  f1_xboxe(double *x, double *par); // Fit function 
      virtual void fit_ybox(const char *hname);                      // Fit
      virtual void fit_ybox(TH1 *h, Double_t dy);                    // Fit
      virtual void fit_ybox(TH1 *h, Double_t dy, Double_t* fpar);    // Fit
      virtual void CheckLHMemory();                    // Check consistency of stored last hits
      virtual void CleanLHMemory();                    // Cleanup
      virtual Bool_t AddNextChan(Int_t iSmType, Int_t iSm, Int_t iRpc, Int_t iLastChan, Double_t dLastPosX,  Double_t dLastPosY, Double_t dLastTime, Double_t dLastTot); // needed for time based data  
      virtual void LH_store(Int_t iSmType, Int_t iSm,  Int_t iRpc, Int_t iChm, CbmTofHit *pHit);
      virtual Double_t TimeInPeriod(Double_t dTime);

   protected:

   private:
      static CbmTofCosmicClusterizer *fInstance;
      /**
       ** @brief Copy constructor.
       **/
      CbmTofCosmicClusterizer(const CbmTofCosmicClusterizer&);
      /**
       ** @brief Copy operator.
       **/
      CbmTofCosmicClusterizer& operator=(const CbmTofCosmicClusterizer&);

      // Functions common for all clusters approximations
      /**
       ** @brief Recover pointer on input TClonesArray: TofPoints, TofDigis...
       **/
      Bool_t   RegisterInputs();
      /**
       ** @brief Create and register output TClonesArray of Tof Hits.
       **/
      Bool_t   RegisterOutputs();
      /**
       ** @brief Initialize other parameters not included in parameter classes.
       **/
      Bool_t   InitParameters();
      /**
       ** @brief Initialize other parameters not included in parameter classes.
       **/
      Bool_t   InitCalibParameter();
      /**
       ** @brief Load the geometry: for now just resizing the Digis temporary vectors
       **/
      Bool_t   LoadGeometry();
      /**
       ** @brief Delete the geometry related arrays: for now just clearing the Digis temporary vectors
       **/
      Bool_t   DeleteGeometry();

      // Histogramming functions
      Bool_t   CreateHistos();
      Bool_t   FillHistos();
      Bool_t   WriteHistos();
      Bool_t   DeleteHistos();

      /**
       ** @brief Build clusters out of ToF Digis and store the resulting info in a TofHit.
       **/
      Bool_t   BuildClusters();
      Bool_t   MergeClusters();

      // ToF geometry variables
      CbmTofGeoHandler      * fGeoHandler;
      CbmTofDetectorId      * fTofId;
      CbmTofDigiPar         * fDigiPar;
      CbmTofCell            * fChannelInfo;
      CbmTofDigiBdfPar      * fDigiBdfPar;

      TTrbHeader            *fTrbHeader;

      // Input variables 
      TClonesArray          * fTofPointsColl; // TOF MC points
      TClonesArray          * fMcTracksColl;  // MC tracks
      TClonesArray          * fTofDigisColl;  // TOF Digis

      // Output variables
      Bool_t                  fbWriteHitsInOut;
      Bool_t                  fbWriteDigisInOut;
      TClonesArray          * fTofCalDigisColl;   // Calibrated TOF Digis 
      TClonesArray          * fTofHitsColl;       // TOF hits
      TClonesArray          * fTofDigiMatchColl;  // TOF Digi Links
      Int_t  fiNbHits;                            // Index of the CbmTofHit TClonesArray

      // Generic
      Int_t fVerbose;

      // Intermediate storage variables
      std::vector< std::vector< std::vector< std::vector< CbmTofDigi* > > > >
               fStorDigiExp; //[nbType][nbSm*nbRpc][nbCh][nDigis]
      std::vector< std::vector< std::vector< std::vector< Int_t > > > >
               fStorDigiInd; //[nbType][nbSm*nbRpc][nbCh][nDigis]
      /*
      std::vector< std::vector< std::vector< std::vector< std::vector< CbmTofDigi* > > > > >
               fStorDigi; //[nbType][nbSm][nbRpc][nbCh][nDigis]
      std::vector< std::vector< std::vector< std::vector< std::vector< CbmTofDigiExp* > > > > >
               fStorDigiExp; //[nbType][nbSm][nbRpc][nbCh][nDigis]
      */
      std::vector< std::vector< std::vector< Int_t > > > fviClusterMul; //[nbType][nbSm][nbRpc]
      std::vector< std::vector< std::vector< Int_t > > > fviClusterSize; //[nbType][nbRpc][nClusters]
      std::vector< std::vector< std::vector< Int_t > > > fviTrkMul; //[nbType][nbRpc][nClusters]
      std::vector< std::vector< std::vector< Double_t > > > fvdX; //[nbType][nbRpc][nClusters]
      std::vector< std::vector< std::vector< Double_t > > > fvdY; //[nbType][nbRpc][nClusters]
      std::vector< std::vector< std::vector< Double_t > > > fvdDifX; //[nbType][nbRpc][nClusters]
      std::vector< std::vector< std::vector< Double_t > > > fvdDifY; //[nbType][nbRpc][nClusters]
      std::vector< std::vector< std::vector< Double_t > > > fvdDifCh; //[nbType][nbRpc][nClusters]

      // Histograms
      TH1* fhClustBuildTime;
      TH1* fhHitsPerTracks;
      TH1* fhPtsPerHit;
      TH1* fhTimeResSingHits;
      TH2* fhTimeResSingHitsB;
      TH2* fhTimePtVsHits;
      TH1* fhClusterSize;
      TH2* fhClusterSizeType;
      TH1* fhTrackMul;
      TH2* fhClusterSizeMulti;
      TH2* fhTrk1MulPos;
      TH2* fhHiTrkMulPos;
      TH2* fhAllTrkMulPos;
      TH2* fhMultiTrkProbPos;
      TH1* fhDigSpacDifClust;
      TH1* fhDigTimeDifClust;
      TH2* fhDigDistClust;
      TH2* fhClustSizeDifX;
      TH2* fhClustSizeDifY;
      TH2* fhChDifDifX;
      TH2* fhChDifDifY;
      TH2* fhCluMulCorDutSel;

      std::vector< TH2* > fhRpcDigiCor;        //[nbDet]
      std::vector< TH1* > fhRpcCluMul;         //[nbDet]
      std::vector< TH1* > fhRpcCluRate;        //[nbDet]
      std::vector< TH2* > fhRpcCluPosition;    //[nbDet]
      std::vector< TProfile* > fhRpcCluPositionEvol;//[nbDet]
      std::vector< TProfile* > fhRpcCluTimeEvol;    //[nbDet]
      std::vector< TH2* > fhRpcCluDelPos;      //[nbDet]
      std::vector< TH2* > fhRpcCluDelMatPos;   //[nbDet]
      std::vector< TH2* > fhRpcCluTOff;        //[nbDet] 
      std::vector< TH2* > fhRpcCluDelTOff;     //[nbDet] 
      std::vector< TH2* > fhRpcCluDelMatTOff;  //[nbDet] 
      std::vector< TH2* > fhRpcCluTrms;        //[nbDet] 
      std::vector< TH2* > fhRpcCluTot;         //[nbDet]
      std::vector< TH2* > fhRpcCluSize;        //[nbDet]
      std::vector< TH2* > fhRpcCluAvWalk;      //[nbDet]
      std::vector< TH2* > fhRpcCluAvLnWalk;    //[nbDet]
      std::vector< std::vector< std::vector<TH2 *> > >fhRpcCluWalk; // [nbDet][nbCh][nSide]
      std::vector< TH2* > fhSmCluPosition; //[nbSmTypes]
      std::vector< TH2* > fhSmCluTOff; 
      std::vector< TProfile* > fhSmCluSvel; 
      std::vector< std::vector< TProfile* > > fhSmCluFpar; 
      std::vector< TH1* > fhRpcDTLastHits;         //[nbDet]
      std::vector< TH1* > fhRpcDTLastHits_Tot;     //[nbDet]
      std::vector< TH1* > fhRpcDTLastHits_CluSize; //[nbDet]

      std::vector< std::vector< TH1* > > fhTRpcCluMul;      //[nbDet][nbSel]
      std::vector< std::vector< TH2* > > fhTRpcCluPosition; //[nbDet][nbSel]
      std::vector< std::vector< TH2* > > fhTRpcCluTOff;     //[nbDet] [nbSel]
      std::vector< std::vector< TH2* > > fhTRpcCluTot;      // [nbDet][nbSel]
      std::vector< std::vector< TH2* > > fhTRpcCluSize;     // [nbDet][nbSel]
      std::vector< std::vector< TH2* > > fhTRpcCluAvWalk;   // [nbDet][nbSel]
      std::vector< std::vector< TH2* > > fhTRpcCluDelTof;   // [nbDet][nbSel]
      std::vector< std::vector< TH2* > > fhTRpcCludXdY;     // [nbDet][nbSel]
      std::vector< std::vector< std::vector< std::vector<TH2 *> > > >fhTRpcCluWalk; // [nbDet][nbSel][nbCh][nSide]
      std::vector< std::vector<TH3 *> >  fhTRpcCluWalk2; // [nbDet][nbSel]

      std::vector< std::vector< TH2* > > fhTSmCluPosition; //[nbSmTypes][nbSel]
      std::vector< std::vector< TH2* > > fhTSmCluTOff;     //[nbSmTypes][nbSel]
      std::vector< std::vector< TH2* > > fhTSmCluTRun;     //[nbSmTypes][nbSel]
      std::vector< std::vector< TH2* > > fhTRpcCluTOffDTLastHits;
      std::vector< std::vector< TH2* > > fhTRpcCluTotDTLastHits;
      std::vector< std::vector< TH2* > > fhTRpcCluSizeDTLastHits;
      std::vector< std::vector< TH2* > > fhTRpcCluMemMulDTLastHits;

      std::vector< TH1* > fhSeldT;  //[nbSel] 

      std::vector< std::vector< std::vector< std::vector< Double_t > > > > fvCPDelTof;   //[nSMT][nRpc][nbClDelTofBinX][nbSel]
      std::vector< std::vector< std::vector< std::vector< Double_t > > > > fvCPTOff;     //[nSMT][nRpc][nCh][nbSide]
      std::vector< std::vector< std::vector< std::vector< Double_t > > > > fvCPTotGain;  //[nSMT][nRpc][nCh][nbSide]
      std::vector< std::vector< std::vector< std::vector< Double_t > > > > fvCPTotOff;  //[nSMT][nRpc][nCh][nbSide]
      std::vector< std::vector< std::vector< std::vector< std::vector< Double_t > > > > > fvCPWalk; //[nSMT][nRpc][nCh][nbSide][nbWalkBins]

      std::vector< std::vector< std::vector< std::vector< std::list< CbmTofHit * > > > > > fvLastHits;  //[nSMT[nSm][nRpc][nCh][NHits]

      // Digis quality
      Int_t fiNbSameSide;
      TH1* fhNbSameSide;
      TH1* fhNbDigiPerChan;

      // Control
      TTimeStamp fStart;
      TTimeStamp fStop;

      // Calib
      Double_t dTRef;
      Double_t fdTRefMax;
      Int_t    fCalMode;
      Int_t    fCalSel;
      Int_t    fCalSmAddr;
      Double_t fdCaldXdYMax;
      Int_t    fiCluMulMax;
      Int_t    fTRefMode;
      Int_t    fTRefHits;
      Int_t    fDutId;
      Int_t    fDutSm;
      Int_t    fDutRpc;
      Int_t    fDutAddr;
      Int_t    fSelId;
      Int_t    fSelSm;
      Int_t    fSelRpc;
      Int_t    fSelAddr;
      Int_t    fSel2Id;
      Int_t    fSel2Sm;
      Int_t    fSel2Rpc;
      Int_t    fSel2Addr;
      Int_t    fSel2MulMax;

      std::map<UInt_t, UInt_t> fDetIdIndexMap;
      std::vector< Int_t >  fviDetId;
      std::vector< Int_t > vDigiIndRef;

      Double_t fPosYMaxScal;
      Double_t fTRefDifMax;
      Double_t fTotMax;
      Double_t fTotMin;
      Double_t fTotOff;
      Double_t fTotMean;
      Double_t fdDelTofMax;
      Double_t fTotPreRange;
      Double_t fMaxTimeDist;
      Double_t fdChannelDeadtime;
      Double_t fdMemoryTime;
      Double_t fdYFitMin;
      Double_t fdTimePeriod;
      Int_t    fiCorMode;

      Bool_t fEnableMatchPosScaling;
      Bool_t fEnableAvWalk;
      Bool_t fbPs2Ns;  // convert input raw digis from ps to ns 

      TString       fCalParFileName;      // name of the file name with Calibration Parameters
      TString       fOutHstFileName;      // name of the histogram output file name with Calibration Parameters
      TFile*        fCalParFile;          // pointer to Calibration Parameter file 

      // Constants or setting parameters
      Int_t    fiNevtBuild;
      Int_t    fiMsgCnt;

      Double_t fdTOTMax;
      Double_t fdTOTMin;
      Double_t fdTTotMean;

      Double_t fdMaxTimeDist; // Isn't this just a local variable? Why make it global and preset?!?
      Double_t fdMaxSpaceDist; // Isn't this just a local variable? Why make it global and preset?!?

      Double_t fdEvent;

   ClassDef(CbmTofCosmicClusterizer, 1);
};

#endif // CBMTOFCOSMICCLUSTERIZER_H
