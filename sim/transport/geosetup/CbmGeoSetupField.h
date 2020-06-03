/** @file CbmGeoSetupField.h
 ** @author Evgeny Lavrik <e.lavrik@gsi.de>
 ** @since 01.10.2019
 **/

#ifndef CBMGEOSETUPFIELD_H
#define CBMGEOSETUPFIELD_H 1

#include <Rtypes.h>            // for ClassDef
#include <RtypesCore.h>        // for Double_t, Int_t
#include <TGeoMatrix.h>        // for TGeoTranslation

#include <string>              // for string



/** @class CbmGeoSetup
 ** @brief Data transfer object to represent the magnetic field in CBM setup
 **
 ** Has no real functionality apart from getters and setters
 **/
class CbmGeoSetupField
{
  public:
    Int_t GetId() { return fId; };
    std::string GetTag() { return fTag; };
    std::string GetAuthor() { return fAuthor; };
    std::string GetDate() { return fDate; };
    std::string GetDescription() { return fDescription; };
    std::string GetFilePath() { return fFilePath; };
    std::string GetRevision() { return fRevision; };
    Double_t GetScale() { return fScale; };
    TGeoTranslation& GetMatrix() { return fMatrix; };

    void SetId(Int_t value) { fId = value; };
    void SetTag(std::string value) { fTag = value; };
    void SetAuthor(std::string value) { fAuthor = value; };
    void SetDate(std::string value) { fDate = value; };
    void SetDescription(std::string value) { fDescription = value; };
    void SetFilePath(std::string value) { fFilePath = value; };
    void SetRevision(std::string value) { fRevision = value; };
    void SetScale(Double_t value) { fScale = value; };
    void SetMatrix(TGeoTranslation value) { fMatrix = value; };

  private:
    Int_t fId{};
    std::string fTag{};
    std::string fAuthor{};
    std::string fDate{};
    std::string fDescription{};
    std::string fFilePath{};
    std::string fRevision{};
    Double_t fScale{1.};
    TGeoTranslation fMatrix{};

    ClassDefNV(CbmGeoSetupField, 1);
};

#endif /* CBMGEOSETUPFIELD_H */
