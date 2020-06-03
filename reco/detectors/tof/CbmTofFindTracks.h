// -------------------------------------------------------------------------
// -----                   CbmTofFindTracks header file                -----
// -----                  Created 25/04/15  by N. Herrmann              -----
// -----                 according to the CbmTrdFindTracks             -----
// -------------------------------------------------------------------------

/** CbmTofFindTracks
 **
 ** Task class for track finding in the TOF.
 ** Input:  TClonesArray of CbmTofHit
 ** Output: TClonesArray of CbmTofTrack
 **
 ** Uses as track finding algorithm classes derived from CbmTofTrackFinder.
 **/


#ifndef CBMTOFFINDTRACKS
#define CBMTOFFINDTRACKS 1

#include "FairTask.h"
//#include "CbmTofTypes.h"
#include <vector>
#include <map>
#include "TTimeStamp.h"

class CbmTofTrackFinder;
class CbmTofTrackFitter;
class CbmTofTrackletTools;
class CbmTofCalibrator;
class TClonesArray;
class TH1;
class TH2;
class TH3;
// Geometry
class CbmTofGeoHandler;
class CbmTofDetectorId;
class CbmTofDigiPar;
class CbmTofDigiBdfPar;
class CbmTofAddress;
class CbmTofHit;
class CbmMatch;

class CbmTofFindTracks : public FairTask
{
  friend class CbmTofTrackFinderNN;
  friend class CbmTofAnaTestbeam;

 public:

  /** Default constructor **/
  CbmTofFindTracks();


  /** Standard constructor 
   **
   *@param name   Name of class
   *@param title  Task title
   *@param finder Pointer to STS track finder concrete class
   **/
  CbmTofFindTracks(const char* name, const char* title = "FairTask", 
		   CbmTofTrackFinder* finder = NULL);

  /** Destructor **/
  virtual ~CbmTofFindTracks();
  
  inline static CbmTofFindTracks *Instance(){ return fInstance; }
  
  /** Initialisation at beginning of each event **/
  virtual InitStatus Init();

  // Initialize other parameters not included in parameter classes.
  Bool_t   InitParameters();

  /** Task execution **/
  virtual void Exec(Option_t* opt);
  virtual void ExecFind(Option_t* opt);

  /** Finish at the end of each event **/
  virtual void Finish();

  /** SetParContainers **/
  virtual void SetParContainers();

  virtual void CreateHistograms();

  virtual void FillUHits();

  virtual Bool_t CheckHit2Track(CbmTofHit *pHit);

  virtual void FindVertex();

  virtual void FillHistograms();

  /** Accessors **/
  CbmTofTrackFinder* GetFinder() { return fFinder; };
  CbmTofTrackFitter* GetFitter() { return fFitter; };
  Int_t GetNofTracks()           { return fNofTracks; };
  Int_t GetNofStations()         { return fNTofStations; };

  /** Set concrete track finder **/
  void UseFinder(CbmTofTrackFinder* finder) { fFinder = finder; };
  void UseFitter(CbmTofTrackFitter* fitter) { fFitter = fitter; };

  inline void SetMinNofHits (Int_t i)         { fMinNofHits = i-1;};
  inline void SetNStations (Int_t i)          { fNTofStations = i;};
  inline void SetNReqStations (Int_t i)        { fNReqStations = i;};

  inline Int_t GetMinNofHits() const   { return fMinNofHits+1;}
  inline Int_t GetNStations() const    { return fNTofStations;}
  inline Int_t GetNReqStations() const  { return fNReqStations;}

  void SetStations    (Int_t ival);
  void SetStation     (Int_t iVal, Int_t iModType, Int_t iModId, Int_t iRpcId);
  void SetBeamCounter (Int_t iModType, Int_t iModId, Int_t iRpcId);
  void PrintSetup();

  inline Int_t GetAddrOfStation(Int_t iVal) {return fMapStationRpcId[iVal]; } 
  inline Int_t GetDetIndSize()              {return fMapRpcIdParInd.size(); }
  Int_t GetStationOfAddr(Int_t iAddr);
  void  PrintMapRpcIdParInd();

  inline Int_t GetStationType(Int_t i) { return fStationType[i]; }
  inline Int_t GetTypeStation(Int_t i) { return fTypeStation[i]; }
  inline Int_t GetCorMode   ()  const  { return fiCorMode;}
  inline Int_t GetBeamCounter() const  { return fiBeamCounter;}
  inline Int_t GetEventNumber() const  { return fiEvent;}
  inline Double_t  GetTtTarg()  const  { return fTtTarg; }

  inline Double_t GetSigT() const  { return fSIGT;}
  inline Double_t GetSigX() const  { return fSIGX;}
  inline Double_t GetSigY() const  { return fSIGY;}
  inline Double_t GetSigZ() const  { return fSIGZ;}
  inline Bool_t InspectEvent() const  { return fInspectEvent;}

  Double_t GetSigT(Int_t iAddr);
  Double_t GetSigX(Int_t iAddr);
  Double_t GetSigY(Int_t iAddr);
  Double_t GetSigZ(Int_t iAddr);
  Double_t GetTOff(Int_t iAddr);

  inline void SetSIGT     (Double_t dval){ fSIGT = dval;}
  inline void SetSIGX     (Double_t dval){ fSIGX = dval;}
  inline void SetSIGY     (Double_t dval){ fSIGY = dval;}
  inline void SetSIGZ     (Double_t dval){ fSIGZ = dval;}
  inline void SetUseSigCalib  (Bool_t bval){ fbUseSigCalib = bval;}
  inline void SetRefVelMean   (Double_t dval){ fdRefVelMean = dval;}
  inline void SetRefDVel          (Double_t dval){ fdRefDVel = dval;}

  inline void SetCorMode       (Int_t ival){ fiCorMode     = ival;}
  inline void SetCalParFileName(TString CalParFileName) { fCalParFileName = CalParFileName; }
  inline void SetCalOutFileName(TString CalOutFileName) { fCalOutFileName = CalOutFileName; }
  inline void SetTtTarg(Double_t val){ fTtTarg=val; }
  inline void SetT0MAX(Double_t val){ fT0MAX=val; }

  inline void SetStationMaxHMul (Int_t ival) { fiStationMaxHMul = ival; }
  inline void MarkStationFired  (Int_t iSt)  { fStationHMul[iSt]++; }
  Int_t GetNStationsFired();
  void  ResetStationsFired();

  inline void SetBeamMomentumLab (Double_t dval) { fdBeamMomentumLab = dval; }
  inline void SetRemoveSignalPropagationTime (Bool_t bval) { fbRemoveSignalPropagationTime = bval; }
  inline void SetBeamMaxHMul (Int_t ival) { fiBeamMaxHMul = ival; }
  inline void SetCalOpt (Int_t ival) { fiCalOpt = ival; }

  inline Double_t GetVertexT() const { return fVTX_T; }
  inline Double_t GetVertexX() const { return fVTX_X; }
  inline Double_t GetVertexY() const { return fVTX_Y; }
  inline Double_t GetVertexZ() const { return fVTX_Z; }

 private:

  static CbmTofFindTracks *fInstance;
  CbmTofTrackFinder* fFinder;    // Pointer to TrackFinder concrete class
  CbmTofTrackFitter* fFitter;    // Pointer to TrackFitter concrete class
  CbmTofTrackletTools* fTrackletTools;    // Pointer to Tracklet tools class
  CbmTofCalibrator* fTofCalibrator;    // Pointer to Calibrator
  TClonesArray* fEventsColl;     // CBMEvents (time based)
  TClonesArray* fTofHitArrayIn;  // Input array of TOF hits
  TClonesArray* fTofMatchArrayIn;  // Input array of TOF hit matches
  TClonesArray* fTofHitArray;    // Output array of recalibrated TOF hits
  TClonesArray* fTrackArray;     // Output array of CbmTofTracks 
  TClonesArray* fTrackArrayOut;  // Output array of CbmTofTracks in CbmEvent mode 
  TClonesArray* fTofUHitArray;   // Output array of unused TOF hits

  Int_t fMinNofHits;             // minimal number of Tof Hits for filling histos 
  Int_t fNofTracks;              // Number of tracks created
  Int_t fNTofStations;           // Number of Tof Stations
  Int_t fNReqStations;           // Number of requested Stations
  Bool_t fInspectEvent;          // analyse event flag 

  std::vector<Int_t> fStationType; // Station SM type 
  std::vector<Int_t> fStationHMul; // Station Hit Multiplicity 
  std::vector<Int_t> fRpcAddr;        // vector of RPC addresses 
  std::map <Int_t, Int_t> fMapStationRpcId; 
  std::map <Int_t, Int_t> fMapRpcIdParInd; 

  CbmTofFindTracks(const CbmTofFindTracks&);
  CbmTofFindTracks& operator=(const CbmTofFindTracks&);

  void CheckMaxHMul();

  // Control histograms 
  TH1* fhTrklMul;
  TH1* fhTrklChi2;
  TH1* fhAllHitsStation;
  TH1* fhAllHitsSmTypes;
  TH1* fhUsedHitsStation;

  TH2* fhTrackingTimeNhits;
  TH2* fhTrklMulNhits;
  TH2* fhTrklMulMaxMM;
  TH3* fhTrklMul3D;
  TH2* fhTrklHMul;
  TH2* fhTrklZ0xHMul;
  TH2* fhTrklZ0yHMul;
  TH2* fhTrklTxHMul;
  TH2* fhTrklTyHMul;
  TH2* fhTrklTtHMul;
  TH2* fhTrklVelHMul;
  TH2* fhTrklT0HMul;
  TH2* fhTrklT0Mul;
  TH2* fhTrklDT0SmMis;
  TH2* fhTrklDT0StMis2;
  TH2* fhTrklXY0_0;
  TH2* fhTrklXY0_1;
  TH2* fhTrklXY0_2;

  std::vector<TH1 *> vhPullX;
  std::vector<TH1 *> vhPullY;
  std::vector<TH1 *> vhPullZ;
  std::vector<TH1 *> vhPullT;
  std::vector<TH1 *> vhPullTB;
  std::vector<TH2 *> vhResidualTBWalk;
  std::vector<TH2 *> vhResidualYWalk;
  std::vector<TH2 *> vhXY_AllTracks;        // for monitoring
  std::vector<TH2 *> vhXY_AllStations;      // for efficiency estimation
  std::vector<TH2 *> vhXY_MissedStation;    // for efficiency estimation
  std::vector<TH3 *> vhXY_DX;
  std::vector<TH3 *> vhXY_DY;
  std::vector<TH3 *> vhXY_DT;
  std::vector<TH3 *> vhXY_TOT;
  std::vector<TH3 *> vhXY_CSZ;
  std::vector<TH3 *> vhUDXDY_DT;
  std::vector<TH3 *> vhUCDXDY_DT;

  TH1* fhVTXNorm;
  TH2* fhVTX_XY0;
  TH2* fhVTX_DT0_Norm;

  Int_t fTypeStation[100];       // FIXME fixed array size
  TString  fOutHstFileName;      // name of the histogram output file name with Calibration Parameters

  Bool_t   LoadCalParameter();
  Bool_t   WriteHistos();
  TString       fCalParFileName;      // name of the file name with Calibration Parameters
  TString       fCalOutFileName;
  TFile*        fCalParFile;          // pointer to Calibration Parameter file 
  TH2*          fhPullT_Smt;           // Time calibration histo
  TH1*          fhPullT_Smt_Off;       // Time calibration histo
  TH2*          fhPullX_Smt;           // position calibration histo
  TH1*          fhPullX_Smt_Off;       // position calibration histo
  TH2*          fhPullY_Smt;           // position calibration histo
  TH1*          fhPullY_Smt_Off;       // position calibration histo
  TH2*          fhPullZ_Smt;           // position calibration histo
  TH1*          fhPullZ_Smt_Off;       // position calibration histo
  TH1*          fhPullT_Smt_Width;      // position calibration histo
  TH1*          fhPullX_Smt_Width;      // position calibration histo
  TH1*          fhPullY_Smt_Width;      // position calibration histo
  TH1*          fhPullZ_Smt_Width;      // position calibration histo
  TH2*          fhTOff_Smt;              // Time calibration histo
  TH1*          fhTOff_Smt_Off;          // Time calibration histo
  TH2*          fhDeltaTt_Smt;           // Time calibration histo
  TH2*          fhTOff_HMul2;              // Time calibration histo
  Int_t         fiCorMode;
  Int_t         fiBeamCounter;
  Int_t         fiStationMaxHMul;
  Double_t      fTtTarg;                // expected average slope in ps/cm 
  Double_t      fVTXNorm;               // Number of Hits contributing to Vertex determination 
  Double_t      fVTX_T;                 // measured event wise t0 
  Double_t      fVTX_X;                 // measured event wise vertex x  
  Double_t      fVTX_Y;                 // measured event wise vertex y
  Double_t      fVTX_Z;                 // measured event wise vertex z
  Double_t      fT0MAX;                 // range of calibration histogram 
  Int_t         fiEvent;                // Number of processed events
  // ToF geometry variables

  CbmTofGeoHandler      * fGeoHandler;
  CbmTofDetectorId      * fTofId;
  CbmTofDigiPar         * fDigiPar;
  CbmTofDigiBdfPar      * fDigiBdfPar;

  Double_t fSIGT;
  Double_t fSIGX;
  Double_t fSIGY;
  Double_t fSIGZ;
  Bool_t   fbUseSigCalib;
  Double_t fdRefVelMean;
  Double_t fdRefDVel;

  TTimeStamp fStart;
  TTimeStamp fStop;
  Double_t   fdTrackingTime;

  Double_t   fdBeamMomentumLab;        // beam momentum in lab frame [AGeV/c]
  Bool_t     fbRemoveSignalPropagationTime;
  Int_t      fiBeamMaxHMul;
  Int_t      fiCalOpt;

  ClassDef(CbmTofFindTracks,1);

};

#endif
