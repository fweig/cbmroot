class TGeoMatrix;
class TGeoVolume;
class FairModule;
class TString;

namespace Cbm {
  namespace GeometryUtils {
    void PrintMedia();
    void PrintMaterials();
    void CorrectMediaId();
    void ReAssignMediaId();
    void RemoveDuplicateMaterials();
    void RemoveDuplicateMedia();

    void ImportRootGeometry(TString& filename, 
                            FairModule* mod, 
                            TGeoMatrix* mat = nullptr);

    bool IsNewGeometryFile(TString& filename);    
    bool IsNewGeometryFile(TString& filename, 
                           TString& volumeName,
                           TGeoMatrix** matrix);

    void AssignMediumAtImport(TGeoVolume* v);
    void ExpandNodes(TGeoVolume* volume, FairModule* mod);
  }
}
