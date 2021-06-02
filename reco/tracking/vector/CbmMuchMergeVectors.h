/** @file CbmMuchMergeVectors.h
  * @copyright Copyright (C) 2014-2018 Veksler and Baldin Laboratory of High Energy Physics, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Alexander Zinchenko [committer] **/

/** CbmMuchMergeVectors.h
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2014
 **
 ** Task class for vector merging in MUCH.
 ** Input: TClonesArray of CbmMuchTrack
 ** Output: TClonesArray of CbmMuchTrack
 **
 **/

#ifndef CBMMUCHMERGEVECTORS_H_
#define CBMMUCHMERGEVECTORS_H_ 1

#include "CbmMuchGeoScheme.h"

#include "FairTask.h"
#include "FairTrackParam.h"

#include <TDecompLU.h>

#include <map>

class CbmMuchTrack;
class TClonesArray;

class CbmMuchMergeVectors : public FairTask {
public:
  /** Default constructor **/
  CbmMuchMergeVectors();

  /** Destructor **/
  virtual ~CbmMuchMergeVectors();

  /** Initialisation **/
  virtual InitStatus Init();

  /** Task execution **/
  virtual void Exec(Option_t* opt);

  /** Finish at the end **/
  virtual void Finish();

  /** SetParContainers **/
  virtual void SetParContainers();

  /** Accessors **/
  Int_t GetNofTracks() { return fNofTracks; };
  Int_t GetNofStat() const { return fgkStat; }

  void PassAbsorber(Int_t ist, Double_t* zabs, Double_t x0, FairTrackParam& parFirst, TMatrixFSym& cov,
                    Int_t pFlag = 0);

private:
  // Some constants
  static const Int_t fgkPlanes = 8;  // Number of straw planes per station
  static const Int_t fgkStat   = 5;  //2;          // Number of stations (including STS tracks)

private:
  CbmMuchGeoScheme* fGeoScheme;  // Geometry scheme
  TClonesArray* fTrackArray;     // Output array of CbmMuchTrack
  Int_t fNofTracks;              // Number of tracks created
  TClonesArray* fHits;           // Input array of CbmMuchHit
  TClonesArray* fGemHits;        // Input array of CbmMuchHit
  TClonesArray* fPoints;         // Input array of CbmMuchPoint
  TClonesArray* fDigiMatches;    // Input array of CbmMatch
  TClonesArray* fVecArray;       // Input array of CbmMuchTrack
  TClonesArray* fTracksSts;      // Input array of CbmStsTrack
  TClonesArray* fTrStsMatch;     // Input array of CbmTrackMatch
  TClonesArray* fTracksLit;      // Input array of LIT MuchTrack

  Int_t fNstat;                                      // Number of MUCH stations
  Int_t fNdoubl;                                     // Number of straw doublets per station
  Int_t fStatFirst;                                  // First straw station No.
  std::map<Int_t, CbmMuchTrack*> fVectors[fgkStat];  // track vectors for stations
  Double_t fDz[fgkPlanes];                           // geometrical constants (Z-distances from layer 0)
  Double_t fCosa[fgkPlanes];                         // geometrical constants (cos of stereo angles)
  Double_t fSina[fgkPlanes];                         // geometrical constants (sin of stereo angles)
  Double_t fCutChi2[9];                              // Chi2-cuts for each absorber
  Double_t fZ0[9];                                   // Z-positions of the first layers
  Double_t fRmin[9];                                 // inner radii of stations
  Double_t fRmax[9];                                 // outer radii of stations
  std::map<Int_t, TMatrixDSym*> fMatr;               // system matrices (for different hit layer patterns)
  Double_t fZabs0[9][2];                             // Z-positions of absorber faces
  Double_t fX0abs[9];                                // radiation lengths of the absorbers

  void GetVectors();
  void MatchVectors();
  void MergeVectors();
  void AddTrack(Int_t ista0, CbmMuchTrack* tr1, CbmMuchTrack* tr2, Int_t indx1, Int_t indx2,
                //TMatrixF &par, Double_t c2);
                FairTrackParam& parOk, Double_t c2);
  void RemoveClones(Int_t ibeg, Int_t iabs, std::map<Int_t, CbmMuchTrack*>& mapMerged);
  void SelectTracks();
  void AddStation1();
  void AddTrack1(Int_t ista0, CbmMuchTrack* tr1, CbmMuchTrack* tr2, Int_t indx1, Int_t indx2, FairTrackParam& parOk,
                 Double_t c2);

  CbmMuchMergeVectors(const CbmMuchMergeVectors&);
  CbmMuchMergeVectors& operator=(const CbmMuchMergeVectors&);

  ClassDef(CbmMuchMergeVectors, 0)
};

#endif
