// ----------------------------------------------------------------------------
// -----                    CbmMvdHitfinderTB header file                    -----
// -----                   Created by C. Dritsa (2009)                    -----
// -----                   Maintained by M.Deveaux (m.deveaux(att)gsi.de) -----
// ----------------------------------------------------------------------------

#ifndef CBMMVDHITFINDERTB_H
#define CBMMVDHITFINDERTB_H 1

#include "FairTask.h"
#include "TStopwatch.h"

class CbmMvdDetector;


class CbmMvdHitfinderTB : public FairTask
{
 
 public:

  /** Default constructor **/  
  CbmMvdHitfinderTB();


  /** Standard constructor 
  *@param name  Task name

  **/
  CbmMvdHitfinderTB(const char* name,Int_t iVerbose = 1);
  CbmMvdHitfinderTB(const char* name,Int_t mode, Int_t iVerbose);

  /** Destructor **/
  virtual ~CbmMvdHitfinderTB();

  void Exec(Option_t* opt);
  void UseClusterfinder(Bool_t clusterfinderFlag ){useClusterfinder = clusterfinderFlag;}; //* enable use of external clusterfinder
  void ShowDebugHistos(){fShowDebugHistos = kTRUE;}

protected:
 
 


private:

 CbmMvdDetector* fDetector;
 
 TClonesArray* fInputDigis;
 TClonesArray* fInputCluster;
 TClonesArray* fHits;

 UInt_t fHitfinderPluginNr;
 Bool_t useClusterfinder;
 Bool_t fShowDebugHistos; 
 TStopwatch     fTimer;        ///< ROOT timer
 Int_t fmode;

// -----   Private methods   ---------------------------------------------
 /** Intialisation **/
  virtual InitStatus Init();


  /** Reinitialisation **/
  virtual InitStatus ReInit();


  /** Virtual method Finish **/
  virtual void Finish();


  /** Register the output arrays to the IOManager **/
  void Register();


  /** Clear the hit arrays **/
  void Reset();	 

  void GetMvdGeometry();

  /** Print digitisation parameters **/
  void PrintParameters();

private:
  CbmMvdHitfinderTB(const CbmMvdHitfinderTB&);
  CbmMvdHitfinderTB operator=(const CbmMvdHitfinderTB&);

ClassDef(CbmMvdHitfinderTB,1);
};
    
    
#endif   		     
