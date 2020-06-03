// -----------------------------------------------------------------------
// -----                  CbmTofTrackFitterKF                        -----
// -----             Created 29/11/05  by D. Kresan                  -----
// -----------------------------------------------------------------------
#ifndef CBMTOFTRACKFITTERKF
#define CBMTOFTRACKFITTERKF

#include "CbmTofTrackFitter.h"

class TClonesArray;
class CbmTofTrack;
class CbmTofTracklet;
class CbmKFTrack;


class CbmTofTrackFitterKF : public CbmTofTrackFitter {

private:
    TClonesArray *fArrayTofHit;   // Array of TOF hits
    Int_t         fVerbose;       // Verbosity level
    Int_t         fPid;           // Mass hypothesis
    CbmKFTrack   *fKfTrack;       // KF track

public:
    CbmTofTrackFitterKF();
    CbmTofTrackFitterKF(Int_t verbose, Int_t pid);
    virtual ~CbmTofTrackFitterKF();

    void Init();
    Int_t DoFit(CbmTofTracklet* pTrack);
    Int_t DoFit(CbmTofTrack* pTrack);

  /** Extrapolate track param. to any Z position 
   *
   * @param track    already fitted STS track
   * @param z        new z-position
   * @param e_track  (return value) extrapolated track parameters
   */
    void Extrapolate(const FairTrackParam* trackPar, Double_t z, FairTrackParam *e_track );
    /*
    void Extrapolate( CbmTofTracklet* track, Double_t z, FairTrackParam *e_track );
    */
    inline Int_t GetVerbose() {  return fVerbose; }
    inline Int_t GetPid()     {  return fPid;     }

    inline void SetVerbose(Int_t verbose) {  fVerbose = verbose; }
    inline void SetPid    (Int_t pid)     {  fPid = pid;         }


    ClassDef(CbmTofTrackFitterKF, 1);

 private:
  void SetKFHits(CbmKFTrack &T, CbmTofTracklet* track);
  CbmTofTrackFitterKF(const CbmTofTrackFitterKF&);
  void operator=(const CbmTofTrackFitterKF&);
};


#endif


