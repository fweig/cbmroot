
#ifndef RICH_CbmRichNavigationUtil
#define RICH_CbmRichNavigationUtil

#include "TObject.h"
#include "FairTrackParam.h"
#include "TVector3.h"
#include "FairLogger.h"


class CbmRichNavigationUtil {
    
public:
    
    static string FindIntersection(
                                   const FairTrackParam* par,
                                   TVector3& crossPoint,
                                   const string& volumeName)
    {
        TVector3 dirCos, pos;
        pos.SetXYZ(par->GetX(), par->GetY(), par->GetZ());
        Double_t nx, ny, nz;
        GetDirCos(par,nx, ny, nz);
        dirCos.SetXYZ(nx, ny, nz);
        
        return FindIntersection(dirCos, pos, crossPoint, volumeName);
    }
    
    static string FindIntersection(
                                   const TVector3& dirCos,
                                   const TVector3& pos,
                                   TVector3& crossPoint,
                                   const string& volumeName)
    {
      //  if (volumeName == "pmt_pixel")cout << "InitTrack: " << pos.X() << " " <<  pos.Y() << " " << pos.Z() << " " << dirCos.X() << " " << dirCos.Y()<< " " << dirCos.Z() << endl;
        gGeoManager->InitTrack(pos.X(), pos.Y(), pos.Z(), dirCos.X(), dirCos.Y(), dirCos.Z());
        
        if(gGeoManager->IsOutside()) { return string(""); }
        
        do {
            gGeoManager->PushPoint();
            string name = string(gGeoManager->GetCurrentNode()->GetName());
            string fullPath = string(gGeoManager->GetPath());
            Double_t x = gGeoManager->GetCurrentPoint()[0];
            Double_t y = gGeoManager->GetCurrentPoint()[1];
            Double_t z = gGeoManager->GetCurrentPoint()[2];
           // if (volumeName == "pmt_pixel")cout << "volumeName:" << volumeName << " " << "name:" << name << " x:"<< x << " y:" << y << " z:" << z << endl;
            
            std::size_t found = name.find(volumeName);
            if (found != std::string::npos) {
              //  if (volumeName == "pmt_pixel")cout << "volumeName found" << endl;
                crossPoint.SetXYZ(x, y, z);
                gGeoManager->PopPoint();
                return fullPath;
            }
            
            
            Double_t step = 25.;
            gGeoManager->FindNextBoundaryAndStep(step);
            if(gGeoManager->IsOutside()) {
                //if (volumeName == "pmt_pixel")std::cout << "Error! CbmRichNavigationUtil::FindIntersections: Outside geometry.\n";
                gGeoManager->PopDummy();
                return string("");
            }
            // Check for NaN values
            if (std::isnan(gGeoManager->GetCurrentPoint()[0]) ||
                std::isnan(gGeoManager->GetCurrentPoint()[1]) ||
                std::isnan(gGeoManager->GetCurrentPoint()[2]) ) {
               // if (volumeName == "pmt_pixel")std::cout << "Error! CbmRichNavigationUtil::FindIntersections: NaN values.\n";
                gGeoManager->PopDummy();
                return string("");
            }
            
            gGeoManager->PopDummy();
            
        } while (true);
        
        return string("");
    }
    
    static void GetDirCos(
                          const FairTrackParam* par,
                          Double_t& nx, Double_t& ny,
                          Double_t& nz)
    {
        Double_t p  = (std::abs(par->GetQp()) != 0.) ? 1. / std::abs(par->GetQp()) : 1.e20;
        Double_t pz = std::sqrt(p * p / (par->GetTx() * par->GetTx() + par->GetTy() * par->GetTy() + 1));
        Double_t px = par->GetTx() * pz;
        Double_t py = par->GetTy() * pz;
        TVector3 unit = TVector3(px, py, pz).Unit();
        nx = unit.X();
        ny = unit.Y();
        nz = unit.Z();
    }
    
};

#endif