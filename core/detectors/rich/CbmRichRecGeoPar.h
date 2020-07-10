/**
 * \file CbmRichRecGeoPar.h
 *
 * \brief RICH geometry parameters for the reconstruction. This class is used for convinient storing
 * of the basic RICH geometry parameters like: PMT and mirrors position, dimensions and tilting.
 *
 * \author Lebedev Semen
 * \date 2013
 **/

#ifndef CBM_RICH_REC_GEO_PAR
#define CBM_RICH_REC_GEO_PAR

#include <RtypesCore.h>  // for Double_t, Bool_t
#include <TMath.h>       // for Pi, Abs, IsNaN, Min, Max
#include <TVector3.h>    // for TVector3

#include <FairLogger.h>  // for LOG
#include <map>           // for map
#include <string>        // for string

enum CbmRichGeometryType {
  CbmRichGeometryTypeNotDefined  = 0,
  CbmRichGeometryTypeTwoWings    = 1,
  CbmRichGeometryTypeCylindrical = 2
};

/**
 * \class CbmRichRecGeoPar
 *
 * \brief PMT parameters for the RICH geometry.
 *
 * \author Lebedev Semen
 * \date 2016
 **/
class CbmRichRecGeoParPmt {
public:
  /**
     * \brief Default constructor.
     */
  CbmRichRecGeoParPmt()
    : fTheta(0.)
    , fPhi(0.)
    , fX(0.)
    , fY(0.)
    , fZ(0.)
    , fPlaneX(0.)
    , fPlaneY(0.)
    , fPlaneZ(0.)
    , fWidth(0.)
    , fHeight(0.)
    , fPmtPositionIndexX(0.) {
    ;
  }


  Double_t fTheta;  // angle by which photodetector was tilted around X-axis
  Double_t fPhi;    // angle by which photodetector was tilted around Y-axis

  //PMT position is used for rotation method
  Double_t fX;  // X-coordinate of photodetector
  Double_t fY;  // Y-coordinate of photodetector
  Double_t fZ;  // Z-coordinate of photodetector

  // PMt plane positions is used in projection producer (analytical)
  Double_t fPlaneX;  // X-coordinate of photodetector plane
  Double_t fPlaneY;  // Y-coordinate of photodetector plane
  Double_t fPlaneZ;  // Z-coordinate of photodetector plane

  Double_t fWidth;   // TGeoBBox->GetDX(), half of the camera quater
  Double_t fHeight;  // TGeoBBox->GetDY(), half of the camera quater

  // needed for cylindrcal geometry
  Int_t fPmtPositionIndexX;  // index of the pmt block in X coordinate
};


/**
 * \class CbmRichRecGeoPar
 *
 * \brief RICH geometry parameters for the reconstruction. This class is used for convinient storing
 * of the basic RICH geometry parameters like: PMT and mirrors position, dimensions and tilting.
 *
 * \author Lebedev Semen
 * \date 2013
 **/
class CbmRichRecGeoPar {

public:
  /**
     * \brief Default constructor.
     */
  CbmRichRecGeoPar()
    : fPmt()
    , fGeometryType(CbmRichGeometryTypeNotDefined)
    , fPmtMap()
    , fPmtStripGap(0.)
    , fNRefrac(0.)
    , fMirrorX(0.)
    , fMirrorY(0.)
    , fMirrorZ(0.)
    , fMirrorR(0.)
    , fMirrorTheta(0.) {
    ;
  }

  /**
     * \brief Destructor.
     */
  ~CbmRichRecGeoPar() { ; }

  /**
     * \brief Print geometry parameters
     */
  void Print() {
    LOG(info) << "-I- RICH geometry parameters";

    if (fGeometryType == CbmRichGeometryTypeTwoWings) {
      LOG(info) << "Geometry type: CbmRichGeometryTypeTwoWings";
      PrintTwoWings();
    } else if (fGeometryType == CbmRichGeometryTypeCylindrical) {
      LOG(info) << "Geometry type: CbmRichGeometryTypeCylindrical";
      PrintCylindrical();
    } else {
      LOG(info) << "ERROR, geometry type is not correct";
    }

    PrintMirror();
  }

  /**
     * \brief Print geometry parameters for two wings geometry
     */
  void PrintTwoWings() {
    LOG(info) << "PMT position in (x,y,z) [cm]: " << fPmt.fX << "  " << fPmt.fY
              << "  " << fPmt.fZ;
    LOG(info) << "PMT plane position in (x,y,z) [cm]: " << fPmt.fPlaneX << "  "
              << fPmt.fPlaneY << "  " << fPmt.fPlaneZ;
    LOG(info) << "PMT width and height [cm]: " << fPmt.fWidth << "  "
              << fPmt.fHeight;
    LOG(info) << "PMT was rotated around x (theta) by "
              << fPmt.fTheta * 180. / TMath::Pi() << " degrees";
    LOG(info) << "PMT was rotated around y (phi) by "
              << fPmt.fPhi * 180. / TMath::Pi() << " degrees";
  }

  /**
     * \brief Print geometry parameters for cylindrical geometry
     */
  void PrintCylindrical() {
    LOG(info) << "PMT strip gap " << fPmtStripGap << " [cm]";

    typedef std::map<std::string, CbmRichRecGeoParPmt>::iterator it_type;
    for (it_type iterator = fPmtMap.begin(); iterator != fPmtMap.end();
         iterator++) {
      LOG(info) << "Geo path:" << iterator->first;
      LOG(info) << "PMT position in (x,y,z) [cm]: " << iterator->second.fX
                << "  " << iterator->second.fY << "  " << iterator->second.fZ;
      LOG(info) << "PMT plane position in (x,y,z) [cm]: "
                << iterator->second.fPlaneX << "  " << iterator->second.fPlaneY
                << "  " << iterator->second.fPlaneZ;
      LOG(info) << "PMT width and height [cm]: " << iterator->second.fWidth
                << "  " << iterator->second.fHeight;
      LOG(info) << "PMT was rotated around x (theta) by "
                << iterator->second.fTheta * 180. / TMath::Pi() << " degrees";
      LOG(info) << "PMT was rotated around y (phi) by "
                << iterator->second.fPhi * 180. / TMath::Pi() << " degrees";
    }
  }

  void PrintMirror() {
    LOG(info) << "Refractive index for lowest photon energies: " << fNRefrac
              << ", (n-1)*10000: " << (fNRefrac - 1.0) * 10000.0;
    LOG(info) << "Mirror center (x,y,z): " << fMirrorX << " " << fMirrorY << " "
              << fMirrorZ;
    LOG(info) << "Mirror radius: " << fMirrorR;
    LOG(info) << "Mirror rotation angle: " << fMirrorTheta * 180. / TMath::Pi()
              << " degrees";
  }

  CbmRichRecGeoParPmt GetGeoRecPmtByBlockPathOrClosest(const std::string& path,
                                                       TVector3* pos) {
    typedef std::map<std::string, CbmRichRecGeoParPmt>::iterator it_type;
    for (it_type it = fPmtMap.begin(); it != fPmtMap.end(); it++) {
      if (path.find(it->first) != std::string::npos) { return it->second; }
    }

    // if nothing is found we search for the closest strip block
    // closest we define by X position
    if (TMath::IsNaN(pos->X()) || TMath::IsNaN(pos->Y())
        || TMath::IsNaN(pos->Z())) {
      CbmRichRecGeoParPmt par;
      return par;
    }

    double minDist = 999999999.;
    CbmRichRecGeoParPmt minPar;
    for (it_type it = fPmtMap.begin(); it != fPmtMap.end(); it++) {
      double x = it->second.fPlaneX;
      double y = it->second.fPlaneY;
      if ((pos->Y() > 0) == (y > 0)) {
        double d = TMath::Abs(x - pos->X());
        if (d < minDist) {
          minDist = d;
          minPar  = it->second;
        }
      }
    }
    //LOG(info) << "minIt->first :" << minIt->first;
    // LOG(info) << "pos:" << pos->X() << " " << pos->Y() << " " << pos->Z() << " plane:" << minIt->second.fPlaneX << " " << minIt->second.fPlaneY << " " << minIt->second.fPlaneZ;

    return minPar;
  }


public:
  CbmRichRecGeoParPmt
    fPmt;  // PMT parameters for 2-wings geometry CbmRichGeometryTypeTwoWings

  CbmRichGeometryType fGeometryType;

  std::map<std::string, CbmRichRecGeoParPmt>
    fPmtMap;  // PMT parameter map for CbmRichGeometryTypeCylindrical, string is geo path to PMT block
  Double_t
    fPmtStripGap;  // [cm] Gap between pmt strips, only valid for CbmRichGeometryTypeCylindrical


  Double_t fNRefrac;  // refraction index

  Double_t fMirrorX;  // X-coordinate of mirror center
  Double_t fMirrorY;  // Y-coordinate of mirror center
  Double_t fMirrorZ;  // Z-coordinate of mirror center
  Double_t fMirrorR;  // mirror radius

  Double_t fMirrorTheta;  // mirror rotation angle around X-axis
};

/**
 * \class CbmRichPmtPlaneMinMax
 *
 * \brief This class is used to store pmt_pixel min and max positions.
 *
 * \author Lebedev Semen
 * \date 2016
 **/
class CbmRichPmtPlaneMinMax {
public:
  CbmRichPmtPlaneMinMax()
    : fMinPmtX(9999999.0)
    , fMaxPmtX(-9999999.0)
    , fMinPmtY(9999999.0)
    , fMaxPmtY(-9999999.0)
    , fMinPmtZ(9999999.0)
    , fMaxPmtZ(-9999999.0) {}

  void AddPoint(Double_t x, Double_t y, Double_t z) {
    fMinPmtX = TMath::Min(fMinPmtX, x);
    fMaxPmtX = TMath::Max(fMaxPmtX, x);
    fMinPmtY = TMath::Min(fMinPmtY, y);
    fMaxPmtY = TMath::Max(fMaxPmtY, y);
    fMinPmtZ = TMath::Min(fMinPmtZ, z);
    fMaxPmtZ = TMath::Max(fMaxPmtZ, z);
  }

  Double_t GetMeanX() { return (fMinPmtX + fMaxPmtX) / 2.; }

  Double_t GetMeanY() { return (fMinPmtY + fMaxPmtY) / 2.; }

  Double_t GetMeanZ() { return (fMinPmtZ + fMaxPmtZ) / 2.; }

private:
  Double_t fMinPmtX;
  Double_t fMaxPmtX;
  Double_t fMinPmtY;
  Double_t fMaxPmtY;
  Double_t fMinPmtZ;
  Double_t fMaxPmtZ;
};


#endif
