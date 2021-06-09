/* Copyright (C) 2004-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese, Andrey Lebedev, Denis Bertini [committer], Florian Uhlig */

/**
 * \file CbmTrd.h
 * \author V.Friese <v.friese@gsi.de>
 * \date 27.07.2004
 * \brief Defines the active detector TRD. Constructs the geometry and registers MCPoints.
 **/

#ifndef CBMTRD_H_
#define CBMTRD_H_

#include "FairDetector.h"

#include "TLorentzVector.h"
#include "TVector3.h"

#include <string>

class CbmTrdPoint;
class CbmTrdGeoHandler;
class FairVolume;
class TClonesArray;
class TGeoCombiTrans;

/**
 * \class CbmTrd
 * \author V.Friese <v.friese@gsi.de>
 * \date 27.07.2004
 * \brief Defines the active detector TRD. Constructs the geometry and registers MCPoints.
 **/
class CbmTrd : public FairDetector {
public:
  /**
    * \brief Default constructor.
    **/
  CbmTrd();

  /**
    * \brief Standard constructor.
    * \param name Detector name.
    * \param active Sensitivity flag
   **/
  CbmTrd(const char* name, Bool_t active);

  CbmTrd(const CbmTrd&) = delete;
  CbmTrd operator=(const CbmTrd&) = delete;

  /**
    * \brief Destructor.
    **/
  virtual ~CbmTrd();

  /**
    * \brief Defines the action to be taken when a step is inside the active volume.
    * Creates CbmTrdPoints and adds them to the collection.
    * \param vol Pointer to the active volume.
    **/
  virtual Bool_t ProcessHits(FairVolume* vol = NULL);

  /**
    * \brief If verbosity level is set, print hit collection at the end of the event and resets it afterwards.
    **/
  virtual void EndOfEvent();

  /**
    * \brief Registers the hit collection in the ROOT manager.
    */
  virtual void Register();

  /**
    * \brief Accessor to the hit collection.
    **/
  virtual TClonesArray* GetCollection(Int_t iColl) const;

  /**
    * \brief Screen output of hit collection.
    **/
  virtual void Print(Option_t* = "") const;

  /**
    * \brief Clears the hit collection.
    **/
  virtual void Reset();

  /**
    * \brief Copies the hit collection with a given track index offset.
    * \param cl1 Origin.
    * \param cl2 Target.
    * \param offset Index offset.
    **/
  virtual void CopyClones(TClonesArray* cl1, TClonesArray* cl2, Int_t offset);

  /**
    * \brief Constructs the TRD geometry.
    **/
  virtual void ConstructGeometry();

  Bool_t CheckIfSensitive(std::string name);

  void UseGlobalPhysicsProcesses(Bool_t use) { fUseGlobalPhysicsProcesses = use; }

  void SetTransformationMatrix(TGeoMatrix* mat) { fCombiTrans = mat; }

private:
  /** Track information to be stored until the track leaves the
   active volume. **/
  TLorentzVector fPosIn;   //! position at entrance
  TLorentzVector fMomIn;   //! momentum at entrance
  TLorentzVector fPosOut;  //! position at exit
  TLorentzVector fMomOut;  //! momentum at exit
  Double32_t fTime;        //! time
  Double32_t fLength;      //! length
  Double32_t fELoss;       //! energy loss
  Int_t fPosIndex;         //! position index

  TClonesArray* fTrdPoints;       //! MC point collection
  CbmTrdGeoHandler* fGeoHandler;  //! Interface to gMC and gGeoManager

  Bool_t fUseGlobalPhysicsProcesses;  //! weather to follow the global switch for physics cuts for the TRDgas

  TGeoMatrix* fCombiTrans;  //! Transformation matrix for geometry positioning

  /**
    * \brief Resets the private members for the track parameters.
    **/
  void ResetParameters();
  virtual void Initialize();
  virtual void SetSpecialPhysicsCuts();

  virtual void ConstructRootGeometry(TGeoMatrix* shift = NULL);


  ClassDef(CbmTrd, 10);
};


inline void CbmTrd::ResetParameters()
{
  fPosIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomIn.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fPosOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fMomOut.SetXYZM(0.0, 0.0, 0.0, 0.0);
  fTime = fLength = fELoss = 0;
  fPosIndex                = 0;
}

#endif
