// -------------------------------------------------------------------------
// -----               CbmPointSetArrayDraw header file                -----
// -----                Created 18/06/22  by J. Brandt                 -----
// -----               Following class FairPointSetDraw                -----
// -------------------------------------------------------------------------


/** CbmPointSetArrayDraw
 * @author J. Brandt
 * @since 22.06.18
 *   Task to display pointsets in array
 **
 **/

#ifndef CBMPOINTSETARRAYDRAW_H
#define CBMPOINTSETARRAYDRAW_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t, Bool_t, kTRUE, Color_t, Opt...

#include <FairTask.h>  // for FairTask, InitStatus

class CbmPointSetArray;
class FairEventManager;
class TClonesArray;
class TObject;
class TVector3;

class CbmPointSetArrayDraw : public FairTask {

public:
  /** Default constructor **/
  CbmPointSetArrayDraw();


  /** Standard constructor
    *@param name        Name of task
    *@param colorMode   coloring of points
    *@param markerMode  how to mark points
    *@param iVerbose    Verbosity level
    **/
  CbmPointSetArrayDraw(const char* name,
                       Int_t colorMode,
                       Int_t markerMode,
                       Int_t iVerbose = 1,
                       Bool_t render  = kTRUE);

  /** Destructor **/
  virtual ~CbmPointSetArrayDraw();

  // Setters
  /** Set verbosity level. For this task and all of the subtasks. **/
  void SetVerbose(Int_t iVerbose) { fVerbose = iVerbose; }
  void SetColorMode(Int_t colorMode) { fColorMode = colorMode; }
  void SetMarkerMode(Int_t markerMode) { fMarkerMode = markerMode; }
  void SetRender(Bool_t render) { fRender = render; }
  // Accessors
  Int_t GetColorMode() const { return fColorMode; }
  Int_t GetMarkerMode() const { return fMarkerMode; }

  /** Executed task **/
  virtual void Exec(Option_t* option);
  /** Reset task **/
  void Reset();

protected:
  TVector3 GetVector(TObject* obj);    //Get 3D Vector of Hit
  Double_t GetTime(TObject* obj);      //Get Time of Hit
  Int_t GetClusterSize(TObject* obj);  //Get ClusterSize of TofHit
  Double_t GetTot(TObject* obj);       //Get ToT of TofHit
  Int_t GetPointId(TObject* obj);      //Get RefId of Hit

  void DetermineTimeOffset();  //Determine TimeOffset and time of latest hit

  virtual void SetParContainers();
  /** Initialise taks **/
  virtual InitStatus Init();
  /** Action after each event **/
  virtual void Finish();

  Int_t fVerbose;                   //Verbosity level
  TClonesArray* fPointList;         //Array containing list of hits
  FairEventManager* fEventManager;  //Pointer to Event Manager
  CbmPointSetArray*
    fl;  //Pointer to CbmPointSetArray -> Cbm class for displaying array of Hit-sets -> TEvePointSetArray
  Color_t fColor;        //Color of Hit-Markers
  Style_t fStyle;        //Style of Hit-Markers
  Double_t fTimeOffset;  //Time Offset on Hits to scale first hit to 0
  Double_t fTimeMax;     //Max Time of Hits in TofHit
  Int_t fColorMode;      //Int determining how points get color-coded
  Int_t fMarkerMode;     //Int determining how marker-size of points gets coded
  Bool_t fRender;        //Boolean whether points shown on default

private:
  CbmPointSetArrayDraw(const CbmPointSetArrayDraw&);
  CbmPointSetArrayDraw& operator=(const CbmPointSetArrayDraw&);

  ClassDef(CbmPointSetArrayDraw, 1);
};


#endif
