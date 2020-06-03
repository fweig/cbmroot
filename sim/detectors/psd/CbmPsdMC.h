/** @file CbmPsdMC.h
 ** @author Yuri Kharlov <yuri.kharlov@ihep.ru>
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 18.09.2017 Major revision: Use geometry file instead of hard-coded geometry.
 **/


#ifndef CBMPSDMC_H
#define CBMPSDMC_H 1

#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TString.h"
#include "FairDetector.h"
#include "FairRootManager.h"

class FairVolume;


/** @class CbmPsdMC
 ** @brief Class for the MC transport of the CBM-PSD
 ** @author V.Friese <v.friese@gsi.de>
 ** @version 2.0
 **
 ** The CbmPsdMC defines the behaviour of the PSD system during
 ** transport simulation. It constructs the PSD transport geometry
 ** and creates objects of type CbmPsdPoints.
 **/
class CbmPsdMC : public FairDetector
{

 public:

    /** @brief Constructor
     ** @param active   If set true, ProcessHits will be called
     ** @param name     Name of detector object
     **/
  CbmPsdMC(Bool_t active = kTRUE, const char* name = "PSDMC");


  /** Destructor **/
  virtual ~CbmPsdMC();


  /** @brief Check whether a volume is sensitive.
   ** @param(name)  Volume name
   ** @value        kTRUE if volume is sensitive, else kFALSE
   **
   ** The decision is based on the volume name (has to contain "scint").
   ** Virtual from FairModule.
   **/
  virtual Bool_t CheckIfSensitive(std::string name) {
      return ( TString(name).Contains("scint", TString::kIgnoreCase)
               ? kTRUE : kFALSE );
  }


  /** @brief Construct the PSD geometry in the TGeoManager.
   **
   ** Only ROOT geometries are supported. The file must contain a top
   ** volume the name of which starts with "psd" and a TGeoMatrix for
   ** the placement of the top psd volume in the cave.
   ** Virtual from FairModule.
   **/
  virtual void ConstructGeometry();


  /** @brief Action at end of event
   **
   ** Short status log and Reset().
   ** Virtual from FairDetector.
   **/
  virtual void EndOfEvent();


  /** Accessor to the hit collection **/
  /** @brief Get output array of CbmPsdPoints
   ** @param iColl Number of collection. Must be zero, since there is only one.
   ** @value Pointer to TClonesArray with CbmPsdPoints
   **/
  virtual TClonesArray* GetCollection(Int_t iColl) const {
    return ( iColl ? nullptr : fPsdPoints );
  }


  /** @brief Screen log
   ** Prints current number of StsPoints in array.
   ** Virtual from TObject.
   **/
  virtual void Print(Option_t* opt = "") const;



  /** @brief Stepping action
   ** @param volume  Pointer to the current volume
   ** @value Always kTRUE
   **
   ** Defines the action to be taken when a step is inside the
   ** active volume. Creates CbmPsdPoints and adds them to the
   ** collection.
   ** Abstract from FairDetector.
   **/
  virtual Bool_t  ProcessHits(FairVolume* volume = 0);


  /** @brief Register the output array
   **
   ** Abstract from FairDetector.
   **/
  virtual void  Register() {
    FairRootManager::Instance()->Register("PsdPoint", GetName(),
                                          fPsdPoints, kTRUE);
  }


  /** @brief Clear output array
   **
   ** Abstract from FairDetector.
   **/
  virtual void Reset() {
    fPsdPoints->Delete();
  };


  /** @brief Define the PSD position in the cave
   ** @param xPos  x Position in the cave [cm]
   ** @param zPos  z position in the cave [cm]
   ** @param rotY  Rotation angle around y axis [degrees]
   **
   ** When this method is invoked, the transformation matrix read from
   ** the geometry file will be overridden with a translation by
   ** (xPos, 0, zPos) and a rotation around the y axis by rotY degrees.
   **/
  void SetPosition(Double_t xPos, Double_t zPos, Double_t rotY) {
    fPosX = xPos;
    fPosZ = zPos;
    fRotY = rotY;
    fUserPlacement = kTRUE;
  }


 private:

  Double_t   fPosX;    //  x position of PSD centre in cave [cm]
  Double_t   fPosZ;    //  z position of PSD centre in cave [cm]
  Double_t   fRotY;    //  Rotation angle of PSD around Y axis [degrees]
  Bool_t fUserPlacement;  // Flag to override placement from file
  TClonesArray*  fPsdPoints;         //! Output array

  /** Track information to be temporarily stored **/
  Int_t          fTrackID;           //!  track index
  Int_t          fAddress;           //!  address (module and layer)
  TLorentzVector fPos;               //!  position
  TLorentzVector fMom;               //!  momentum
  Double_t       fTime;              //!  time
  Double_t       fLength;            //!  length
  Double_t       fEloss;             //!  energy loss
  Int_t          fLayerID;           //!  layer ID
  Int_t          fModuleID;          //!  module ID

  /** @brief Register all sensitive volumes
   ** @param node Pointer to start node
   **
   ** Starting from the specified node, the entire node tree is expanded
   ** and all volumes which satisfy the CheckIfSensitive() criterion
   ** are added to the list of sensitive volumes.
   */
  void RegisterSensitiveVolumes(TGeoNode* node);


  /** Prevent copy constructor and assignment operator **/
  CbmPsdMC(const CbmPsdMC&) = delete;
  CbmPsdMC operator=(const CbmPsdMC&) = delete;

  
  ClassDef(CbmPsdMC,1)

};


#endif  //? CBMPSDMC_H
