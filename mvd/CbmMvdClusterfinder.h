// ----------------------------------------------------------------------------
// -----               CbmMvdClusterfinder header file                    -----
// -----                   Created by P.Sitzmann 03.12.2014               -----
// ----------------------------------------------------------------------------


#ifndef CBMMVDCLUSTERFINDER_H
#define CBMMVDCLUSTERFINDER_H 1


#include "FairTask.h"

#include "TStopwatch.h"

class CbmMvdDetector;
class TClonesArray;
class TString;
class CbmDigiManager;


class CbmMvdClusterfinder : public FairTask {

public:
  /** Default constructor **/
  CbmMvdClusterfinder();


  /** Standard constructor 
  *@param name  Task name
  *@param mode  0 = MAPS, 1 = Ideal
  **/
  CbmMvdClusterfinder(const char* name, Int_t mode = 0, Int_t iVerbose = 1);


  /** Destructor **/
  ~CbmMvdClusterfinder();

  void Exec(Option_t* opt);

  void ShowDebugHistos() { fShowDebugHistos = kTRUE; }

private:
  /** Hit producer mode (0 = MAPS, 1 = Ideal) **/
  Int_t fMode;
  Bool_t fShowDebugHistos;
  CbmMvdDetector* fDetector;
  CbmDigiManager* fDigiMan;

  TClonesArray* fCluster;

  UInt_t fClusterPluginNr;

  TString fBranchName;  // Name of input branch (MvdDigi)


  TStopwatch fTimer;  ///< ROOT timer


  // -----   Private methods   ---------------------------------------------
  /** Intialisation **/
  virtual InitStatus Init();


  /** Reinitialisation **/
  virtual InitStatus ReInit();


  /** Virtual method Finish **/
  virtual void Finish();


  /** Register the output arrays to the IOManager **/
  void Register();

  void GetMvdGeometry();


  /** Clear the hit arrays **/
  void Reset();


  /** Print digitisation parameters **/
  void PrintParameters();

private:
  CbmMvdClusterfinder(const CbmMvdClusterfinder&);
  CbmMvdClusterfinder operator=(const CbmMvdClusterfinder&);

  ClassDef(CbmMvdClusterfinder, 1);
};


#endif
