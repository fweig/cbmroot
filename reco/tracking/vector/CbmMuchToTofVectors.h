/** CbmMuchToTofVectors.h
 *@author A.Zinchenko <Alexander.Zinchenko@jinr.ru>
 *@since 2017
 **
 ** Task class for vector matching between CBM MUCH and TOF.
 ** Input: 
 ** Output:
 **
 **/

#ifndef CBMMUCHTOTOFVECTORS_H_
#define CBMMUCHTOTOFVECTORS_H_ 1

#include "FairTask.h"
#include <TLine.h>

#include <map>
#include <set>
#include <vector>

class CbmMuchTrack;
class CbmMCDataArray;

class CbmMuchToTofVectors : public FairTask
{
public:
  /** Default constructor **/
  CbmMuchToTofVectors();
  
  /** Destructor **/
  virtual ~CbmMuchToTofVectors();
  
  /** Initialisation **/
  virtual InitStatus Init();
  
  /** Task execution **/
  virtual void Exec(
		    Option_t* opt);
  
  /** Finish at the end **/
  virtual void Finish();
  
  /** SetParContainers **/
  virtual void SetParContainers();
  
  /** Accessors **/
  Int_t GetNofTracks()           { return fNofTracks; };
  
 private:
  // Some constants
  static const Int_t fgkPlanes = 4;             // Number of TRD layers

 private:
  TClonesArray* fTrackArray;                      // Output array of CbmMuchTracks
  Int_t fNofTracks;                               // Number of tracks created
  TClonesArray* fHits;                            // Input array of CbmTofHit
  TClonesArray* fHitMatches;                      // Input array of CbmMatch
  //TClonesArray* fPoints;                          // Input array of CbmTofPoint
  CbmMCDataArray* fPoints;                        // Input array of CbmTofPoint
  TClonesArray* fDigis;                           // Input array of CbmDigi
  TClonesArray* fDigiMatches;                     // Input array of CbmMatch
  TClonesArray* fMuchTracks;                       // Input array of 
  Double_t fZ[2];                                 // TOF location in Z
  Double_t fErrX;                                 // hit measurement error in X
  Double_t fErrY;                                 // hit measurement error in Y
  Double_t fCutChi2;                              // Chi2-cuts 
  std::multimap<Double_t,Int_t> fHitX;            // hit index vs X
  std::multimap<Double_t,Int_t> fHitY;            // hit index vs Y
  std::map<Int_t,Double_t> fHitTime;              // hit time (min. of all contributing points)
  std::map<Int_t,std::set<Int_t> > fHitIds;       // hit index - to - contributing track IDs 
  std::vector<CbmMuchTrack*> fVectors;            // track vectors for stations
  std::multimap<Double_t,TLine> fLineX;           // projected TRD vectors vs min X
  Double_t fXy[fgkPlanes][5];                     // hit float data
  Double_t fXyi[fgkPlanes][3];                    // hit int data

  void GetHits();         // get TRD hits
  void MakeVectors();     // make vectors
  void MatchTofToMuch();   // match TOF hits to TRD vectors
  void CheckParams();     // check vector parameters (apriory constraints)
  void RemoveClones();    // remove clones
  void StoreVectors();    // store vectors
  void ProcessPlane(Int_t lay, Int_t patt, Int_t flag); // process next plane
  void FindLine(Int_t patt, Double_t *pars);            // line fit
  Double_t FindChi2(Int_t patt, Double_t *pars);        // fit chi2
  void AddVector(Int_t patt, Double_t chi2, Double_t *pars); // add vector to temp. container
  void SetTrackId(CbmMuchTrack *vec);                   // set track ID

  CbmMuchToTofVectors(const CbmMuchToTofVectors&);
  CbmMuchToTofVectors& operator=(const CbmMuchToTofVectors&);
  
  ClassDef(CbmMuchToTofVectors,0)
};

#endif
