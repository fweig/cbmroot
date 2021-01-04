// -------------------------------------------------------------------------
// -----                   CbmTofExtendTracks header file                -----
// -----                  Created 21/12/20  by N. Herrmann              -----
// -------------------------------------------------------------------------

/** CbmTofExtendTracks
 **
 **/


#ifndef CBMTOFEXTENDTRACKS
#define CBMTOFEXTENDTRACKS 1

#include "FairTask.h"
//#include "CbmTofTypes.h"
#include "TTimeStamp.h"
#include <map>
#include <vector>

class CbmTofTrackFinder;
class CbmTofTrackFitter;
class CbmTofTrackletParam;
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
class CbmPixelHit;
class CbmTofHit;
class CbmMatch;
class CbmEvent;
class CbmStsHit;
class CbmMuchPixelHit;
class CbmRichHit;

class CbmTofExtendTracks : public FairTask {
  friend class CbmTofFindTracks;
  friend class CbmTofTrackFinderNN;
  friend class CbmTofAnaTestbeam;

public:
  /** Default constructor **/
  CbmTofExtendTracks();


  /** Standard constructor 
   **
   *@param name   Name of class
   *@param title  Task title
   *@param finder Pointer to STS track finder concrete class
   **/
  CbmTofExtendTracks(const char* name,
                     const char* title         = "FairTask",
                     CbmTofTrackFinder* finder = NULL);

  /** Destructor **/
  virtual ~CbmTofExtendTracks();

  inline static CbmTofExtendTracks* Instance() { return fInstance; }

  /** Initialisation at beginning of each event **/
  virtual InitStatus Init();

  // Initialize other parameters not included in parameter classes.
  Bool_t InitParameters();

  /** Task execution **/
  virtual void Exec(Option_t* opt);
  virtual void ExecExtend(Option_t* opt, CbmEvent* tEvent = NULL);

  /** Finish at the end of each event **/
  virtual void Finish();

  /** SetParContainers **/
  virtual void SetParContainers();

  virtual void CreateHistograms();

  virtual void FindVertex();

  virtual void FillHistograms(CbmEvent* tEvent = NULL);


  virtual void Line3Dfit(std::vector<CbmPixelHit*>, CbmTofTrackletParam*);
  Double_t GetFitX(Double_t, CbmTofTrackletParam*);
  Double_t GetFitY(Double_t, CbmTofTrackletParam*);
  Double_t GetFitT(Double_t, CbmTofTrackletParam*);

  virtual void TrkAddStation(Int_t iStation);

  /** Accessors **/
  CbmTofTrackFinder* GetFinder() { return fFinder; };
  CbmTofTrackFitter* GetFitter() { return fFitter; };

  inline void SetTrkHitsMin(Int_t i) { fiTrkHitsMin = i; };
  inline void SetCutDX(Double_t val) { fdTrkCutDX = val; };
  inline void SetCutDY(Double_t val) { fdTrkCutDY = val; };
  inline void SetCutDT(Double_t val) { fdTrkCutDT = val; };
  inline void SetChi2Max(Double_t val) { fdChi2Max = val; };
  inline void SetCorSrc(Int_t i) { fiCorSrc = i; };
  inline void SetCorMode(Int_t i) { fiCorMode = i; };
  inline void SetAddStations(Int_t i) { fiAddStations = i; };
  inline void SetReqStations(Int_t i) { fiReqStations = i; };
  inline void SetStationUT(Int_t i) { fiStationUT = i; };
  inline void SetCutStationMaxHitMul(Int_t i) { fiCutStationMaxHitMul = i; };
  inline void SetNTrkTofMax(Int_t i) { fiNTrkTofMax = i; };

  inline void SetCalParFileName(TString CalParFileName) {
    fCalParFileName = CalParFileName;
  }
  inline void SetCalOutFileName(TString CalOutFileName) {
    fCalOutFileName = CalOutFileName;
  }

  /** Set concrete track finder **/
  void UseFinder(CbmTofTrackFinder* finder) { fFinder = finder; };
  void UseFitter(CbmTofTrackFitter* fitter) { fFitter = fitter; };

  inline Double_t GetVertexT() const { return fVTX_T; }
  inline Double_t GetVertexX() const { return fVTX_X; }
  inline Double_t GetVertexY() const { return fVTX_Y; }
  inline Double_t GetVertexZ() const { return fVTX_Z; }

private:
  static CbmTofExtendTracks* fInstance;
  CbmTofTrackFinder* fFinder;           // Pointer to TrackFinder concrete class
  CbmTofTrackFitter* fFitter;           // Pointer to TrackFitter concrete class
  CbmTofTrackletTools* fTrackletTools;  // Pointer to Tracklet tools class
  CbmTofCalibrator* fTofCalibrator;     // Pointer to Calibrator
  TClonesArray* fEventsColl;            // CBMEvents (time based)
  TClonesArray* fTofHitArrayIn;         // Input array of TOF hits
  TClonesArray* fStsHitArrayIn;         // Input array of TOF hits
  TClonesArray* fMuchHitArrayIn;        // Input array of TOF hits
  TClonesArray* fRichHitArrayIn;        // Input array of TOF hits
  TClonesArray* fTofMatchArrayIn;       // Input array of TOF hit matches
  TClonesArray* fTofHitArray;           // Output array of recalibrated TOF hits
  TClonesArray* fTofTrackArrayIn;       // Input array of CbmTofTracks
  TClonesArray*
    fTrackArrayOut;  // Output array of CbmTofTracks in CbmEvent mode
  std::vector<std::vector<Int_t>> fvTofHitIndex;   // Index of hit in TS
  std::vector<Int_t> fvTofTrackIndex;              // Index of track in TS
  std::vector<std::vector<Int_t>> fvStsHitIndex;   // Index of hit in TS
  std::vector<std::vector<Int_t>> fvMuchHitIndex;  // Index of hit in TS
  std::vector<std::vector<Int_t>> fvRichHitIndex;  // Index of hit in TS
  std::vector<Double_t> fvTofStationZ;             // Z position of Tof stations
  std::vector<Double_t> fvStsStationZ;             // Z position of Tof stations
  std::vector<Double_t> fvMuchStationZ;            // Z position of Tof stations
  std::vector<Double_t> fvRichStationZ;            // Z position of Tof stations

  std::vector<std::vector<CbmPixelHit*>>
    fvAllHitPointer;  // Pointer to hits in TS
  std::vector<std::vector<CbmPixelHit*>> fvTrkCalHits;
  std::vector<CbmTofTrackletParam*> fvTrkPar;

  std::map<Int_t, Int_t> fMapStationZ;
  std::map<Int_t, Int_t>::iterator itMapStationZ;

  std::vector<Double_t> fvToff;  // station correction parameter
  std::vector<Double_t> fvXoff;  // station correction parameter
  std::vector<Double_t> fvYoff;  // station correction parameter
  std::vector<Double_t> fvZoff;  // station correction parameter

  std::vector<Double_t> fvTsig;  // station matching parameter
  std::vector<Double_t> fvXsig;  // station matching parameter
  std::vector<Double_t> fvYsig;  // station matching parameter
  std::vector<Double_t> fvZsig;  // station matching parameter

  CbmTofExtendTracks(const CbmTofExtendTracks&);
  CbmTofExtendTracks& operator=(const CbmTofExtendTracks&);

  // Control histograms
  TH2* fhMulCorTrkTof;
  TH2* fhMulCorTrkSts;
  TH2* fhMulCorTrkMuch;
  TH2* fhMulCorTrkRich;

  TH2* fhPosCorTrkTof;
  TH2* fhPosCorTrkSts;
  TH2* fhPosCorTrkMuch;
  TH2* fhPosCorTrkRich;

  std::vector<TH2*> fhTrkStationDX;
  std::vector<TH2*> fhTrkStationDY;
  std::vector<TH2*> fhTrkStationDZ;
  std::vector<TH2*> fhTrkStationDT;
  std::vector<TH2*> fhTrkStationNHits;
  std::vector<std::vector<TH2*>> fhTrkStationDXDY;

  std::vector<TH2*> fhTrkPullDX;
  std::vector<TH2*> fhTrkPullDY;
  std::vector<TH2*> fhTrkPullDT;

  TH1* fhExt_Toff;
  TH1* fhExt_Xoff;
  TH1* fhExt_Yoff;
  TH1* fhExt_Zoff;

  TH1* fhExt_Tsig;
  TH1* fhExt_Xsig;
  TH1* fhExt_Ysig;
  TH1* fhExt_Zsig;

  std::vector<TH2*> fhExt_TrkSizVel;
  std::vector<TH2*> fhExt_TrkSizChiSq;

  TH1* fhVTXNorm;
  TH2* fhVTX_XY0;
  TH2* fhVTX_DT0_Norm;

  TH2* fhExtSutXY_Found;
  TH2* fhExtSutXY_Missed;
  TH3* fhExtSutXY_DX;
  TH3* fhExtSutXY_DY;
  TH3* fhExtSutXY_DT;

  Bool_t LoadCalParameter();
  Bool_t WriteHistos();
  Bool_t UpdateCalHistos();

  TString fCalParFileName;  // name of the file name with Calibration Parameters
  TString fCalOutFileName;
  TFile* fCalParFile;  // pointer to Calibration Parameter file

  Double_t fVTXNorm;   // Number of Hits contributing to Vertex determination
  Double_t fVTX_T;     // measured event wise t0
  Double_t fVTX_X;     // measured event wise vertex x
  Double_t fVTX_Y;     // measured event wise vertex y
  Double_t fVTX_Z;     // measured event wise vertex z
  Double_t fT0MAX;     // range of calibration histogram
  Int_t fiTrkHitsMin;  // Min number of hits required for tracks
  Double_t fdTrkCutDX;
  Double_t fdTrkCutDY;
  Double_t fdTrkCutDT;
  Double_t fdChi2Max;   // Max accepted matching chi2
  Int_t fiCorSrc;       // correction source (0 - all hits, 1 pulls)
  Int_t fiCorMode;      // correction update mode
  Int_t fiAddStations;  // extend tracks (station number *100)
  Int_t fiReqStations;  // request station for track (station number *100)
  Int_t fiStationUT;    // station under test
  Int_t fiCutStationMaxHitMul;  //max hit multiplicity for any station
  Int_t fiNTrkTofMax;           // maximum number of TofTracks per event
  Int_t fiEvent;                // Number of processed events
  // ToF geometry variables

  ClassDef(CbmTofExtendTracks, 1);
};

#endif
