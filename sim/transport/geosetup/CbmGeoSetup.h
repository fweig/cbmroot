/** @file CbmGeoSetup.h
 ** @author Evgeny Lavrik <e.lavrik@gsi.de>
 ** @since 01.10.2019
 **/

#ifndef CBMSETUPSETUP_H
#define CBMSETUPSETUP_H 1

#include <Rtypes.h>            // for ClassDef
#include <RtypesCore.h>        // for Int_t

#include <map>                 // for map
#include <string>              // for string

#include "CbmGeoSetupModule.h" // for CbmGeoSetupModule
#include "CbmGeoSetupField.h"  // for CbmGeoSetupField
#include "CbmGeoSetupMedia.h"  // for CbmGeoSetupMedia
#include "CbmDefs.h"           // for ECbmModuleId 

/** @class CbmGeoSetup
 ** @brief Data transfer object to represent the CBM Detector setup
 **
 ** Has no real functionality apart from getters and setters
 ** It has a module map designated with module id (\ref ECbmModuleId)
 ** and CbmSetupModule (\ref CbmSetupModule), description of magnetic field
 ** and material properties.
 **
 ** Properties of this class are accessed from CbmSetup via CbmGeoSetupProvider
 **/
class CbmGeoSetup
{
  public:
    Int_t GetId() { return fId; };
    std::string GetName() { return fName; };
    std::string GetTag() { return fTag; };
    std::string GetAuthor() { return fAuthor; };
    std::string GetDate() { return fDate; };
    std::string GetRevision() { return fRevision; };
    std::string GetDescription() { return fDescription; };
    std::map<ECbmModuleId, CbmGeoSetupModule>& GetModuleMap() { return fModuleMap; };
    CbmGeoSetupField& GetField() { return fField; };
    CbmGeoSetupMedia& GetMedia() { return fMedia; };

    void SetId(Int_t value) { fId = value; };
    void SetName(std::string value) { fName = value; };
    void SetTag(std::string value) { fTag = value; };
    void SetAuthor(std::string value) { fAuthor = value; };
    void SetDate(std::string value) { fDate = value; };
    void SetRevision(std::string value) { fRevision = value; };
    void SetDescription(std::string value) { fDescription = value; };
    void SetModuleMap(std::map<ECbmModuleId, CbmGeoSetupModule> value) { fModuleMap = value; };
    void SetField(CbmGeoSetupField value) { fField = value; };
    void SetMedia(CbmGeoSetupMedia value) { fMedia = value; };

  private:
    Int_t fId{};
    std::string fName{};
    std::string fTag{};
    std::string fAuthor{};
    std::string fDate{};
    std::string fRevision{};
    std::string fDescription{};
    std::map<ECbmModuleId, CbmGeoSetupModule> fModuleMap{};
    CbmGeoSetupField fField{};
    CbmGeoSetupMedia fMedia{};

    ClassDefNV(CbmGeoSetup, 2);
};

#endif /* CBMSETUPSETUP_H */
