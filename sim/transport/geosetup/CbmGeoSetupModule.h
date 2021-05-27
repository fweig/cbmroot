/** @file CbmGeoSetupModule.h
 ** @author Evgeny Lavrik <e.lavrik@gsi.de>
 ** @since 01.10.2019
 **/

#ifndef CBMGEOSETUPMODULE_H
#define CBMGEOSETUPMODULE_H 1

#include "CbmDefs.h"  // for ECbmModuleId

#include <Rtypes.h>      // for ClassDef
#include <RtypesCore.h>  // for Int_t, Bool_t
#include <TGeoMatrix.h>  // for TGeoHMatrix

#include <string>  // for string

/** @class CbmGeoSetup
 ** @brief Data transfer object to represent the CBM Detector setup module
 **
 ** Has no real functionality apart from getters and setters
 **/
class CbmGeoSetupModule {
public:
  Int_t GetId() { return fId; };
  ECbmModuleId GetModuleId() { return fModuleId; };
  std::string GetTag() { return fTag; };
  std::string GetName() { return fName; };
  std::string GetAuthor() { return fAuthor; };
  std::string GetDate() { return fDate; };
  std::string GetDescription() { return fDescription; };
  std::string GetFilePath() { return fFilePath; };
  std::string GetRevision() { return fRevision; };
  TGeoHMatrix& GetMatrix() { return fMatrix; };
  Bool_t GetActive() { return fActive; };

  void SetId(Int_t value) { fId = value; };
  void SetModuleId(ECbmModuleId value) { fModuleId = value; };
  void SetTag(std::string value) { fTag = value; };
  void SetName(std::string value) { fName = value; };
  void SetAuthor(std::string value) { fAuthor = value; };
  void SetDate(std::string value) { fDate = value; };
  void SetDescription(std::string value) { fDescription = value; };
  void SetFilePath(std::string value) { fFilePath = value; };
  void SetRevision(std::string value) { fRevision = value; };
  void SetMatrix(TGeoHMatrix value) { fMatrix = value; };
  void SetActive(Bool_t value) { fActive = value; };

private:
  Int_t fId {};
  ECbmModuleId fModuleId {};
  std::string fTag {};
  std::string fName {};
  std::string fAuthor {};
  std::string fDate {};
  std::string fDescription {};
  std::string fFilePath {};
  std::string fRevision {};
  TGeoHMatrix fMatrix {};
  Bool_t fActive {};

  ClassDefNV(CbmGeoSetupModule, 2);
};

#endif /* CBMGEOSETUPMODULE_H */
