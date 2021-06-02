/** @file CbmGlobalTrackingTof.cxx
  * @copyright Copyright (C) 2017-2019 Laboratory of Information Technologies, Joint Institute for Nuclear Research, Dubna
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Timur Ablyazimov [committer] **/

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "CbmGlobalTrackingTof.h"

#include "CbmPixelHit.h"
#include "CbmTofAddress.h"
#include "CbmTofHit.h"
#include "base/CbmLitToolFactory.h"
#include "utils/CbmLitConverter.h"

#include "TGeoBBox.h"
#include "TGeoManager.h"
#include "TMath.h"

#include <iostream>
#include <utility>

TrackPropagatorPtr fPropagator;
TrackPropagatorPtr fLinePropagator;
TrackUpdatePtr fFilter;

using std::cout;
using std::endl;
using std::list;
using std::map;
using std::pair;
using std::set;
using std::swap;

/* (VF) Unused function
static bool GaussSolve(double coeffs[3][4], double result[3])
{
   int indices[3];
   
   for (int i = 0; i < 3; ++i)
      indices[i] = i;
   
   for (int i = 2; i > 0; --i)
   {
      if (0 == coeffs[i][i + 1])
      {
         for (int j = i; j > 0; --j)
         {
            if (0 != coeffs[i][j])
            {
               for (int k = 0; k < 3; ++k)
                  swap(coeffs[k][j], coeffs[k][i + 1]);
               
               swap(indices[j - 1], indices[i]);
               break;
            }
         }
      }
      
      double den = coeffs[i][i + 1];
      
      if (0 == den)
         return false;
      
      for (int j = 0; j < i; ++j)
      {
         for (int k = 0; k <= i; ++k)
            coeffs[j][k] += -coeffs[j][i + 1] * coeffs[i][k] / den;
      }
   }
   
   double solution[3];
   
   for (int i = 0; i < 3; ++i)
   {
      double s = coeffs[i][0];
      
      for (int j = 0; j < i; ++j)
         s += coeffs[i][j + 1] * solution[j];
      
      if (0 == coeffs[i][i + 1])
         return false;
      
      solution[i] = -s / coeffs[i][i + 1];
   }
   
   for (int i = 0; i < 3; ++i)
      result[indices[i]] = solution[i];
   
   return true;
}
*/

struct Point {
  double x;
  double y;
  double z;
};

struct Line {
  double x0;
  double y0;
  double z0;

  double cosX;
  double cosY;
  double cosZ;
};

struct Segment {
  Point a;
  Point b;
};

struct Plane {
  double x0;
  double y0;
  double z0;

  double cosX1;
  double cosY1;
  double cosZ1;

  double cosX2;
  double cosY2;
  double cosZ2;
};

/* (VF) Unused function
static bool Intersect(const Plane& plane, const Line& line, double result[3])
{
   double coeffs[3][4] = {
      { plane.x0 - line.x0, plane.cosX1, plane.cosX2, -line.cosX },
      { plane.y0 - line.y0, plane.cosY1, plane.cosY2, -line.cosY },
      { plane.z0 - line.z0, plane.cosZ1, plane.cosZ2, -line.cosZ }
   };
   
   return GaussSolve(coeffs, result);
}
*/

struct Rectangle {
  Plane plane;
  double s1;
  double s2;
};

/* (VF) Unused function
static bool Intersect(const Rectangle& rect, const Line& line, double result[3])
{
   if (!Intersect(rect.plane, line, result))
      return false;
   
   return 0 <= result[0] && result[0] <= rect.s1 && 0 <= result[1] && result[1] <= rect.s2;
}
*/

/* (VF) Unused function
static bool Intersect(const Rectangle& rect, const Line& line)
{
   double result[3];
   
   if (!Intersect(rect.plane, line, result))
      return false;
   
   return 0 <= result[0] && result[0] <= rect.s1 && 0 <= result[1] && result[1] <= rect.s2;
}
*/

/* (VF) Unused function
static bool Intersect(const Rectangle& rect, const Segment& segment)
{
   double len = TMath::Sqrt((segment.b.x - segment.a.x) * (segment.b.x - segment.a.x) +
      (segment.b.y - segment.a.y) * (segment.b.y - segment.a.y) +
      (segment.b.z - segment.a.z) * (segment.b.z - segment.a.z));
   double cosX = (segment.b.x - segment.a.x) / len;
   double cosY = (segment.b.y - segment.a.y) / len;
   double cosZ = (segment.b.z - segment.a.z) / len;
   Line line = { segment.a.x, segment.a.y, segment.a.z, cosX, cosY, cosZ };
   double result[3];
   
   if (!Intersect(rect.plane, line, result))
      return false;
   
   return 0 <= result[2] && result[2] <= len;
}
*/

struct Direction {
  double cosX;
  double cosY;
  double cosZ;
};

struct TBin {
  list<Int_t> fHitInds;

  TBin() : fHitInds() {}

  void Clear() { fHitInds.clear(); }
};

#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
struct Cuboid {
  double width;
  double height;
  double thickness;
  double minX;
  double maxX;
  double minY;
  double maxY;
  double minZ;
  double maxZ;
  Direction dirWidth;
  Direction dirHeight;
  Direction dirThickness;
  Point vertices[2][2][2];
  Segment edges[12];
  Rectangle faces[3][2];

  //Plane plane1;
  //Plane plane2;

  int fNofTBins;
  TBin* fTBins;

  Int_t fFullModId;

  void Clear()
  {
    for (int i = 0; i < fNofTBins; ++i)
      fTBins[i].Clear();
  }

  void Calc(int nofTBins = 0)
  {
    fNofTBins = nofTBins;

    if (fNofTBins) fTBins = new TBin[fNofTBins];

    dirWidth     = {(vertices[0][0][1].x - vertices[0][0][0].x) / width,
                (vertices[0][0][1].y - vertices[0][0][0].y) / width,
                (vertices[0][0][1].z - vertices[0][0][0].z) / width};
    dirHeight    = {(vertices[0][1][0].x - vertices[0][0][0].x) / height,
                 (vertices[0][1][0].y - vertices[0][0][0].y) / height,
                 (vertices[0][1][0].z - vertices[0][0][0].z) / height};
    dirThickness = {(vertices[1][0][0].x - vertices[0][0][0].x) / thickness,
                    (vertices[1][0][0].y - vertices[0][0][0].y) / thickness,
                    (vertices[1][0][0].z - vertices[0][0][0].z) / thickness};

    minX = 1000000;
    maxX = -1000000;
    minY = 1000000;
    maxY = -1000000;
    minZ = 1000000;
    maxZ = -1000000;

    int dims[3];
    int ind = 0;

    for (int i = 0; i < 2; ++i) {
      dims[0] = i;

      for (int j = 0; j < 2; ++j) {
        dims[1] = j;

        for (int k = 0; k < 2; ++k) {
          dims[2] = k;

          for (int l = 0; l < 3; ++l) {
            if (0 == dims[l]) {
              int dims2[3] = {dims[0], dims[1], dims[2]};
              dims2[l]     = 1;
              edges[ind++] = {{vertices[dims[0]][dims[1]][dims[2]].x, vertices[dims[0]][dims[1]][dims[2]].y,
                               vertices[dims[0]][dims[1]][dims[2]].z},
                              {vertices[dims2[0]][dims2[1]][dims2[2]].x, vertices[dims2[0]][dims2[1]][dims2[2]].y,
                               vertices[dims2[0]][dims2[1]][dims2[2]].z}};
            }
          }

          if (minX > vertices[i][j][k].x) minX = vertices[i][j][k].x;

          if (maxX < vertices[i][j][k].x) maxX = vertices[i][j][k].x;

          if (minY > vertices[i][j][k].y) minY = vertices[i][j][k].y;

          if (maxY < vertices[i][j][k].y) maxY = vertices[i][j][k].y;

          if (minZ > vertices[i][j][k].z) minZ = vertices[i][j][k].z;

          if (maxZ < vertices[i][j][k].z) maxZ = vertices[i][j][k].z;
        }
      }
    }

    for (int i = 0; i < 3; ++i) {
      int j        = (i + 1) % 3;
      int k        = (i + 2) % 3;
      int dims0[3] = {0, 0, 0};
      int dims1[3] = {0, 0, 0};
      int dims2[3] = {0, 0, 0};
      dims1[j]     = 1;
      dims2[k]     = 1;
      double len1 =
        TMath::Sqrt((vertices[dims1[0]][dims1[1]][dims1[2]].x - vertices[dims0[0]][dims0[1]][dims0[2]].x)
                      * (vertices[dims1[0]][dims1[1]][dims1[2]].x - vertices[dims0[0]][dims0[1]][dims0[2]].x)
                    + (vertices[dims1[0]][dims1[1]][dims1[2]].y - vertices[dims0[0]][dims0[1]][dims0[2]].y)
                        * (vertices[dims1[0]][dims1[1]][dims1[2]].y - vertices[dims0[0]][dims0[1]][dims0[2]].y)
                    + (vertices[dims1[0]][dims1[1]][dims1[2]].z - vertices[dims0[0]][dims0[1]][dims0[2]].z)
                        * (vertices[dims1[0]][dims1[1]][dims1[2]].z - vertices[dims0[0]][dims0[1]][dims0[2]].z));
      double cosX1 = (vertices[dims1[0]][dims1[1]][dims1[2]].x - vertices[dims0[0]][dims0[1]][dims0[2]].x) / len1;
      double cosY1 = (vertices[dims1[0]][dims1[1]][dims1[2]].y - vertices[dims0[0]][dims0[1]][dims0[2]].y) / len1;
      double cosZ1 = (vertices[dims1[0]][dims1[1]][dims1[2]].z - vertices[dims0[0]][dims0[1]][dims0[2]].z) / len1;

      double len2 =
        TMath::Sqrt((vertices[dims2[0]][dims2[1]][dims2[2]].x - vertices[dims0[0]][dims0[1]][dims0[2]].x)
                      * (vertices[dims2[0]][dims2[1]][dims2[2]].x - vertices[dims0[0]][dims0[1]][dims0[2]].x)
                    + (vertices[dims2[0]][dims2[1]][dims2[2]].y - vertices[dims0[0]][dims0[1]][dims0[2]].y)
                        * (vertices[dims2[0]][dims2[1]][dims2[2]].y - vertices[dims0[0]][dims0[1]][dims0[2]].y)
                    + (vertices[dims2[0]][dims2[1]][dims2[2]].z - vertices[dims0[0]][dims0[1]][dims0[2]].z)
                        * (vertices[dims2[0]][dims2[1]][dims2[2]].z - vertices[dims0[0]][dims0[1]][dims0[2]].z));
      double cosX2 = (vertices[dims2[0]][dims2[1]][dims2[2]].x - vertices[dims0[0]][dims0[1]][dims0[2]].x) / len2;
      double cosY2 = (vertices[dims2[0]][dims2[1]][dims2[2]].y - vertices[dims0[0]][dims0[1]][dims0[2]].y) / len2;
      double cosZ2 = (vertices[dims2[0]][dims2[1]][dims2[2]].z - vertices[dims0[0]][dims0[1]][dims0[2]].z) / len2;

      faces[i][0] = {{vertices[dims0[0]][dims0[1]][dims0[2]].x, vertices[dims0[0]][dims0[1]][dims0[2]].y,
                      vertices[dims0[0]][dims0[1]][dims0[2]].z, cosX1, cosY1, cosZ1, cosX2, cosY2, cosZ2},
                     len1,
                     len2};

      dims0[i] = 1;
      dims1[i] = 1;
      dims2[i] = 1;
      len1     = TMath::Sqrt((vertices[dims1[0]][dims1[1]][dims1[2]].x - vertices[dims0[0]][dims0[1]][dims0[2]].x)
                           * (vertices[dims1[0]][dims1[1]][dims1[2]].x - vertices[dims0[0]][dims0[1]][dims0[2]].x)
                         + (vertices[dims1[0]][dims1[1]][dims1[2]].y - vertices[dims0[0]][dims0[1]][dims0[2]].y)
                             * (vertices[dims1[0]][dims1[1]][dims1[2]].y - vertices[dims0[0]][dims0[1]][dims0[2]].y)
                         + (vertices[dims1[0]][dims1[1]][dims1[2]].z - vertices[dims0[0]][dims0[1]][dims0[2]].z)
                             * (vertices[dims1[0]][dims1[1]][dims1[2]].z - vertices[dims0[0]][dims0[1]][dims0[2]].z));
      cosX1    = (vertices[dims1[0]][dims1[1]][dims1[2]].x - vertices[dims0[0]][dims0[1]][dims0[2]].x) / len1;
      cosY1    = (vertices[dims1[0]][dims1[1]][dims1[2]].y - vertices[dims0[0]][dims0[1]][dims0[2]].y) / len1;
      cosZ1    = (vertices[dims1[0]][dims1[1]][dims1[2]].z - vertices[dims0[0]][dims0[1]][dims0[2]].z) / len1;

      len2  = TMath::Sqrt((vertices[dims2[0]][dims2[1]][dims2[2]].x - vertices[dims0[0]][dims0[1]][dims0[2]].x)
                           * (vertices[dims2[0]][dims2[1]][dims2[2]].x - vertices[dims0[0]][dims0[1]][dims0[2]].x)
                         + (vertices[dims2[0]][dims2[1]][dims2[2]].y - vertices[dims0[0]][dims0[1]][dims0[2]].y)
                             * (vertices[dims2[0]][dims2[1]][dims2[2]].y - vertices[dims0[0]][dims0[1]][dims0[2]].y)
                         + (vertices[dims2[0]][dims2[1]][dims2[2]].z - vertices[dims0[0]][dims0[1]][dims0[2]].z)
                             * (vertices[dims2[0]][dims2[1]][dims2[2]].z - vertices[dims0[0]][dims0[1]][dims0[2]].z));
      cosX2 = (vertices[dims2[0]][dims2[1]][dims2[2]].x - vertices[dims0[0]][dims0[1]][dims0[2]].x) / len2;
      cosY2 = (vertices[dims2[0]][dims2[1]][dims2[2]].y - vertices[dims0[0]][dims0[1]][dims0[2]].y) / len2;
      cosZ2 = (vertices[dims2[0]][dims2[1]][dims2[2]].z - vertices[dims0[0]][dims0[1]][dims0[2]].z) / len2;

      faces[i][1] = {{vertices[dims0[0]][dims0[1]][dims0[2]].x, vertices[dims0[0]][dims0[1]][dims0[2]].y,
                      vertices[dims0[0]][dims0[1]][dims0[2]].z, cosX1, cosY1, cosZ1, cosX2, cosY2, cosZ2},
                     len1,
                     len2};
    }
  }
};

static bool Inside(const Cuboid& cuboid, const Point& point)
{
  const Point& O = cuboid.vertices[0][0][0];
  double projX   = (point.x - O.x) * cuboid.dirWidth.cosX + (point.y - O.y) * cuboid.dirWidth.cosY
                 + (point.z - O.z) * cuboid.dirWidth.cosZ;

  if (0 > projX || projX > cuboid.width) return false;

  double projY = (point.x - O.x) * cuboid.dirHeight.cosX + (point.y - O.y) * cuboid.dirHeight.cosY
                 + (point.z - O.z) * cuboid.dirHeight.cosZ;

  if (0 > projY || projY > cuboid.height) return false;

  double projZ = (point.x - O.x) * cuboid.dirThickness.cosX + (point.y - O.y) * cuboid.dirThickness.cosY
                 + (point.z - O.z) * cuboid.dirThickness.cosZ;

  if (0 > projZ || projZ > cuboid.thickness) return false;

  return true;
}

static bool Intersect(const Cuboid& cuboid1, const Cuboid& cuboid2)
{
  // Check if cuboid2 is inside cuboid1
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      for (int k = 0; k < 2; ++k) {
        if (Inside(cuboid1, cuboid2.vertices[i][j][k])) return true;
      }
    }
  }

  // Check if cuboid1 is inside cuboid2
  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 2; ++j) {
      for (int k = 0; k < 2; ++k) {
        if (Inside(cuboid2, cuboid1.vertices[i][j][k])) return true;
      }
    }
  }

  // Check if one edge of the cuboid2 intersects one face of the cuboid1
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 2; ++j) {
      const Rectangle& face = cuboid1.faces[i][j];

      for (int k = 0; k < 12; ++k) {
        const Segment& edge = cuboid2.edges[k];

        if (Intersect(face, edge)) return true;
      }
    }
  }

  // Check if one edge of the cuboid1 intersects one face of the cuboid2
  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 2; ++j) {
      const Rectangle& face = cuboid2.faces[i][j];

      for (int k = 0; k < 12; ++k) {
        const Segment& edge = cuboid1.edges[k];

        if (Intersect(face, edge)) return true;
      }
    }
  }

  return false;
}
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS

struct XBin {
#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
  list<Cuboid*> fCuboids;

  void Clear()
  {
    for (list<Cuboid*>::iterator i = fCuboids.begin(); i != fCuboids.end(); ++i) {
      Cuboid* cuboid = *i;
      cuboid->Clear();
    }
  }
#else  //CBM_GLOBALTB_TOF_3D_CUBOIDS
  int fNofTBins;
  TBin* fTBins;
  explicit XBin(int nofTBins) : fNofTBins(nofTBins), fTBins(new TBin[fNofTBins]) {}

  void Clear()
  {
    for (int i = 0; i < fNofTBins; ++i) {
      fTBins[i].Clear();
    }
  }
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS
};

struct YBin {
  int fNofXBins;
  XBin* fXBins;

  YBin(int nofXBins, int nofTBins)
    : fNofXBins(nofXBins)
    , fXBins(reinterpret_cast<XBin*>(new unsigned char[fNofXBins * sizeof(XBin)]))
  {
    for (int i = 0; i < fNofXBins; ++i)
#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
      new (&fXBins[i]) XBin;
#else  //CBM_GLOBALTB_TOF_3D_CUBOIDS
      new (&fXBins[i]) XBin(nofTBins);
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS
  }

  void Clear()
  {
    for (int i = 0; i < fNofXBins; ++i)
      fXBins[i].Clear();
  }
};

struct ZBin {
  int fNofYBins;
  YBin* fYBins;

  ZBin(int nofYBins, int nofXBins, int nofTBins)
    : fNofYBins(nofYBins)
    , fYBins(reinterpret_cast<YBin*>(new unsigned char[fNofYBins * sizeof(YBin)]))
  {
    for (int i = 0; i < fNofYBins; ++i)
      new (&fYBins[i]) YBin(nofXBins, nofTBins);
  }

  void Clear()
  {
    for (int i = 0; i < fNofYBins; ++i)
      fYBins[i].Clear();
  }
};

CbmGlobalTrackingTofGeometry::CbmGlobalTrackingTofGeometry()
  : fC(0)
  , fPDG(211)
  , fChi2Cut(50)
  ,
#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
  fCuboids()
  ,
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS
  fNofTBins(1000)
  , fNofXBins(60)
  , fNofYBins(60)
  , fNofZBins(5)
  , fZBins(0)
  , fTBinSize(100)
  , fMinX(1000000)
  , fMaxX(-1000000)
  , fXBinSize(0)
  , fMinY(1000000)
  , fMaxY(-1000000)
  , fYBinSize(0)
  , fMinZ(1000000)
  , fMaxZ(-1000000)
  , fZBinSize(0)
  , fStartTime(0)
  , fEndTime(0)
  , fTofHits(0)  //, fPropagator()
{
}

CbmGlobalTrackingTofGeometry::~CbmGlobalTrackingTofGeometry()
{
#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
  for (list<Cuboid*>::iterator i = fCuboids.begin(); i != fCuboids.end(); ++i)
    delete *i;
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS
}

static void FindGeoChild(TGeoNode* node, const char* name, list<TGeoNode*>& results)
{
  Int_t nofChildren = node->GetNdaughters();

  for (Int_t i = 0; i < nofChildren; ++i) {
    TGeoNode* child = node->GetDaughter(i);
    TString childName(child->GetName());

    if (childName.Contains(name, TString::kIgnoreCase)) results.push_back(child);
  }
}

bool CbmGlobalTrackingTofGeometry::Read()
{
  fC                        = 1.e-7 * TMath::C();
  fPropagator               = CbmLitToolFactory::CreateTrackPropagator("lit");
  fLinePropagator           = CbmLitToolFactory::CreateTrackPropagator("line");
  fFilter                   = CbmLitToolFactory::CreateTrackUpdate("kalman");
  TGeoNavigator* pNavigator = gGeoManager->GetCurrentNavigator();
  gGeoManager->cd("/cave_1");
  list<TGeoNode*> tofNodes;
  FindGeoChild(gGeoManager->GetCurrentNode(), "tof", tofNodes);

  if (tofNodes.empty()) return false;

  TGeoNode* tofNode = tofNodes.front();
  pNavigator->CdDown(tofNode);

  list<TGeoNode*> tofModules;
  FindGeoChild(tofNode, "module", tofModules);

  for (list<TGeoNode*>::iterator i = tofModules.begin(); i != tofModules.end(); ++i) {
    TGeoNode* tofModule  = *i;
    const char* modName  = tofModule->GetName();
    const char* firstUnd = strchr(modName, '_');
    const char* lastUnd  = strrchr(modName, '_');

    if (0 == firstUnd || 0 == lastUnd) continue;

    //int modType = atoi(firstUnd + 1); (VF) unused
    //int modNum = atoi(lastUnd + 1); (VF) unused

    pNavigator->CdDown(tofModule);
    list<TGeoNode*> tofGasBoxes;
    FindGeoChild(tofModule, "gas_box", tofGasBoxes);

    for (list<TGeoNode*>::iterator j = tofGasBoxes.begin(); j != tofGasBoxes.end(); ++j) {
      TGeoNode* tofGasBox = *j;
      pNavigator->CdDown(tofGasBox);
      list<TGeoNode*> tofModuleCounters;
      FindGeoChild(tofGasBox, "counter", tofModuleCounters);

      for (list<TGeoNode*>::iterator k = tofModuleCounters.begin(); k != tofModuleCounters.end(); ++k) {
        TGeoNode* tofModuleCounter = *k;
        const char* counterName    = tofModuleCounter->GetName();

        const char* lastUnd2 = strrchr(counterName, '_');

        if (0 == lastUnd2) continue;

        //int counterNum = atoi(lastUnd2 + 1); (VF) unused

        pNavigator->CdDown(tofModuleCounter);
        TGeoVolume* tofModuleCounterVol       = gGeoManager->GetCurrentVolume();
        const TGeoBBox* tofModuleCounterShape = static_cast<const TGeoBBox*>(tofModuleCounterVol->GetShape());
        Double_t halfwidth                    = tofModuleCounterShape->GetDX();
        Double_t halfheight                   = tofModuleCounterShape->GetDY();
        Double_t halfthickness                = tofModuleCounterShape->GetDZ();

#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
        Cuboid* cuboid = new Cuboid({2 * halfwidth, 2 * halfheight, 2 * halfthickness});
        cuboid->fFullModId =
          CbmTofAddress::GetModFullId(CbmTofAddress::GetUniqueAddress(modNum, counterNum, 0, 0, modType));
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS

        /*Int_t modId0 = CbmTofAddress::GetModFullId(CbmTofAddress::GetUniqueAddress(modNum, counterNum, 0, 0, modType));
            Int_t modId1 = CbmTofAddress::GetModFullId(CbmTofAddress::GetUniqueAddress(modNum, counterNum, 1, 0, modType));
            Int_t modId2 = CbmTofAddress::GetModFullId(CbmTofAddress::GetUniqueAddress(modNum, counterNum, 2, 0, modType));
            Int_t modId3 = CbmTofAddress::GetModFullId(CbmTofAddress::GetUniqueAddress(modNum, counterNum, 3, 0, modType));
            Int_t modId0_1 = CbmTofAddress::GetModFullId(CbmTofAddress::GetUniqueAddress(modNum, counterNum, 0, 1, modType));
            Int_t modId1_1 = CbmTofAddress::GetModFullId(CbmTofAddress::GetUniqueAddress(modNum, counterNum, 1, 1, modType));
            Int_t modId2_1 = CbmTofAddress::GetModFullId(CbmTofAddress::GetUniqueAddress(modNum, counterNum, 2, 1, modType));
            Int_t modId3_1 = CbmTofAddress::GetModFullId(CbmTofAddress::GetUniqueAddress(modNum, counterNum, 3, 1, modType));*/

        for (int t = 0; t < 2; ++t)  // t means thickness
        {
          for (int h = 0; h < 2; ++h)  // h means height
          {
            for (int w = 0; w < 2; ++w)  // w means width
            {
              Double_t localCoords[3] = {w > 0 ? halfwidth : -halfwidth, h > 0 ? halfheight : -halfheight,
                                         t > 0 ? halfthickness : -halfthickness};
              Double_t globalCoords[3];
              gGeoManager->LocalToMaster(localCoords, globalCoords);
#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
              cuboid->vertices[t][h][w] = {globalCoords[0], globalCoords[1], globalCoords[2]};
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS

              if (fMinX > globalCoords[0]) fMinX = globalCoords[0];

              if (fMaxX < globalCoords[0]) fMaxX = globalCoords[0];

              if (fMinY > globalCoords[1]) fMinY = globalCoords[1];

              if (fMaxY < globalCoords[1]) fMaxY = globalCoords[1];

              if (fMinZ > globalCoords[2]) fMinZ = globalCoords[2];

              if (fMaxZ < globalCoords[2]) fMaxZ = globalCoords[2];
            }
          }
        }

#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
        cuboid->Calc(fNofTBins);
        fCuboids.push_back(cuboid);
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS
        pNavigator->CdUp();
      }

      pNavigator->CdUp();
    }

    pNavigator->CdUp();
  }

  pNavigator->CdUp();

  fXBinSize = (fMaxX - fMinX) / fNofXBins;
  fYBinSize = (fMaxY - fMinY) / fNofYBins;
  fZBinSize = (fMaxZ - fMinZ) / fNofZBins;

  // Create search bins
  fZBins = reinterpret_cast<ZBin*>(new unsigned char[fNofZBins * sizeof(ZBin)]);

  for (int i = 0; i < fNofZBins; ++i)
    new (&fZBins[i]) ZBin(fNofYBins, fNofXBins, fNofTBins);

#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
  // Create references to the existing cuboids
  for (list<Cuboid*>::iterator iter = fCuboids.begin(); iter != fCuboids.end(); ++iter) {
    Cuboid* cuboid = *iter;
    int zIndMin    = GetZInd(cuboid->minZ);
    int zIndMax    = GetZInd(cuboid->maxZ);
    int yIndMin    = GetYInd(cuboid->minY);
    int yIndMax    = GetYInd(cuboid->maxY);
    int xIndMin    = GetXInd(cuboid->minX);
    int xIndMax    = GetXInd(cuboid->maxX);

    for (int zInd = zIndMin; zInd <= zIndMax; ++zInd) {
      ZBin& zBin = fZBins[zInd];

      for (int yInd = yIndMin; yInd <= yIndMax; ++yInd) {
        YBin& yBin = zBin.fYBins[yInd];

        for (int xInd = xIndMin; xInd <= xIndMax; ++xInd) {
          XBin& xBin     = yBin.fXBins[xInd];
          Cuboid cuboid2 = {fXBinSize, fYBinSize, fZBinSize};

          for (int i = 0; i < 2; ++i) {
            for (int j = 0; j < 2; ++j) {
              for (int k = 0; k < 2; ++k)
                cuboid2.vertices[i][j][k] = {fMinX + (xInd + k) * fXBinSize, fMinY + (yInd + j) * fYBinSize,
                                             fMinZ + (zInd + i) * fZBinSize};
            }
          }

          cuboid2.Calc();

          if (Intersect(*cuboid, cuboid2)) xBin.fCuboids.push_back(cuboid);
        }
      }
    }
  }  // for (list<Cuboid*>::iterator i = fCuboids.begin(); i != fCuboids.end(); ++i)
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS

  return true;
}

void CbmGlobalTrackingTofGeometry::Clear()
{
  for (int i = 0; i < fNofZBins; ++i)
    fZBins[i].Clear();
}

int globalNofHits  = 0;
int globalNofHitsT = 0;
int globalNofHitsM = 0;

void CbmGlobalTrackingTofGeometry::Prepare(timetype startTime)
{
  Clear();
  fStartTime = startTime;
  fEndTime   = fStartTime + fNofTBins * fTBinSize;

  int nofHits = fTofHits->GetEntriesFast();
  globalNofHits += nofHits;

  for (int i = 0; i < nofHits; ++i) {
    const CbmTofHit* hit = static_cast<const CbmTofHit*>(fTofHits->At(i));
    scaltype z           = hit->GetZ();

    if (z < fMinZ || z > fMaxZ) continue;

    int zInd   = GetZInd(z);
    ZBin& zBin = fZBins[zInd];
    scaltype y = hit->GetY();

    if (y < fMinY || y > fMaxY) continue;

    int yInd   = GetYInd(y);
    YBin& yBin = zBin.fYBins[yInd];
    scaltype x = hit->GetX();

    if (x < fMinX || x > fMaxX) continue;

    int xInd   = GetXInd(x);
    XBin& xBin = yBin.fXBins[xInd];

#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
    if (xBin.fCuboids.empty()) continue;
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS

    timetype t = hit->GetTime();

    if (t < fStartTime || t > fEndTime) continue;

    ++globalNofHitsT;

    int tInd = GetTInd(t);

#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
    Int_t address  = hit->GetAddress();
    Int_t moduleId = CbmTofAddress::GetModFullId(address);

    for (list<Cuboid*>::iterator j = xBin.fCuboids.begin(); j != xBin.fCuboids.end(); ++j) {
      Cuboid* mrpc = *j;

      if (mrpc->fFullModId == moduleId) {
        mrpc->fTBins[tInd].fHitInds.push_back(i);
        ++globalNofHitsM;
      }
    }
#else  //CBM_GLOBALTB_TOF_3D_CUBOIDS
    TBin& tBin = xBin.fTBins[tInd];
    tBin.fHitInds.push_back(i);
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS
  }
}

int nofMRPCIntersections  = 0;
int nofToFIntersections   = 0;
int nofMRPCIntersectionsT = 0;

void CbmGlobalTrackingTofGeometry::Find(FairTrackParam& trackParams, timetype trackTime, timetype errT,
                                        Int_t& tofHitInd, Double_t& length)
//void CbmGlobalTrackingTofGeometry::Find(scaltype x0, scaltype errX, scaltype y0, scaltype errY, scaltype z0, scaltype t0, scaltype errT,
//scaltype tx, scaltype errTx, scaltype ty, scaltype errTy, Int_t& tofHitInd)
//void CbmGlobalTrackingTofGeometry::Find(scaltype x0, scaltype errXSq, scaltype y0, scaltype errYSq, scaltype z0, scaltype t0, scaltype errT,
//scaltype tx, scaltype errTx, scaltype ty, scaltype errTy, Int_t& tofHitInd)
{
  tofHitInd               = -1;
  Double_t stsTrackLength = length;
  length                  = 0;
  //double x0 = trackParams.GetX();

  //if (x0 < fMinX || x0 > fMaxX)
  //return;

  //double y0 = trackParams.GetY();

  //if (y0 < fMinY || y0 > fMaxY)
  //return;

  //double z0 = trackParams.GetZ();
  //double t0 = fStartTime;//trackParams.GetTime();
  //double tx = trackParams.GetTx();
  //double ty = trackParams.GetTy();

  //double deltaZ1 = fMinZ - z0;
  //double x1 = x0 + tx * deltaZ1;
  //double y1 = y0 + ty * deltaZ1;
  double z1 = fMinZ;
  CbmTrackParam cbmTrackParams;
  cbmTrackParams.Set(trackParams, trackTime, errT);
  CbmLitTrackParam litTrackParams;
  CbmLitConverterFairTrackParam::FairTrackParamToCbmLitTrackParam(&cbmTrackParams, &litTrackParams);

  if (fPropagator->Propagate(&litTrackParams, z1, fPDG, 0, &length) == kLITERROR) {
    length = 0;
    return;
  }

  double x1 = litTrackParams.GetX();
  double y1 = litTrackParams.GetY();

  //if (fMinX > x1 || x1 > fMaxX || fMinY > y1 || y1 > fMaxY)
  //return;

  // First check if track exits from the ToF XYZ manifold from the back face.
  double deltaZMax = fMaxZ - z1;
  double tx        = litTrackParams.GetTx();
  double xMax      = x1 + tx * deltaZMax;
  double ty        = litTrackParams.GetTy();
  double yMax      = y1 + ty * deltaZMax;
  double normLen   = TMath::Sqrt(1 + tx * tx + ty * ty);
  //double t1 = trackTime + z1 * normLen / fC;
  double t1 = trackTime + (stsTrackLength + length) / fC;
  //double t1 = litTrackParams.GetTime();
  double zMax;

  map<int, map<int, map<int, double>>> inds;

  /*for (int i = 0; i < 25; ++i)
   {
      double val = litTrackParams.GetCovariance(i);
      int qq = 0;
   }*/

  double deltaX = 4 * TMath::Sqrt(litTrackParams.GetCovariance(0));
  double deltaY = 4 * TMath::Sqrt(litTrackParams.GetCovariance(6));
  double deltaT = 4 * errT;
  //double deltaT = 4 * litTrackParams.GetTimeError();
  //Find(x1 - deltaX, y1 - deltaY, z1, tx, ty, inds);
  //Find(x1 + deltaX, y1 - deltaY, z1, tx, ty, inds);
  //Find(x1 - deltaX, y1 + deltaY, z1, tx, ty, inds);
  //Find(x1 + deltaX, y1 + deltaY, z1, tx, ty, inds);


  if (fMinX <= xMax && xMax <= fMaxX && fMinY <= yMax && yMax <= fMaxY) zMax = fMaxZ;
  else if (ty > 0 && (fMaxY - y1) / ty < fMaxZ - z1 && fMinX <= x1 + tx * (fMaxY - y1) / ty
           && x1 + tx * (fMaxY - y1) / ty <= fMaxX) {
    zMax = z1 + (fMaxY - y1) / ty;
    yMax = fMaxY;
    xMax = x1 + tx * (fMaxY - y1) / ty;
  }
  else if (ty < 0 && (fMinY - y1) / ty < fMaxZ - z1 && fMinX <= x1 + tx * (fMinY - y1) / ty
           && x1 + tx * (fMinY - y1) / ty <= fMaxX) {
    zMax = z1 + (fMinY - y1) / ty;
    yMax = fMinY;
    xMax = x1 + tx * (fMinY - y1) / ty;
  }
  else if (tx > 0 && (fMaxX - x1) / tx < fMaxZ - z1 && fMinY <= y1 + ty * (fMaxX - x1) / tx
           && y1 + ty * (fMaxX - x1) / tx <= fMaxY) {
    zMax = z1 + (fMaxX - x1) / tx;
    yMax = y1 + ty * (fMaxX - x1) / tx;
    xMax = fMaxX;
  }
  else if (tx < 0 && (fMinX - x1) / tx < fMaxZ - z1 && fMinY <= y1 + ty * (fMinX - x1) / tx
           && y1 + ty * (fMinX - x1) / tx <= fMaxY) {
    zMax = z1 + (fMinX - x1) / tx;
    yMax = y1 + ty * (fMinX - x1) / tx;
    xMax = fMinX;
  }
  else
    return;

  ++nofToFIntersections;

#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
  set<const Cuboid*> cuboidSet;
#else  //CBM_GLOBALTB_TOF_3D_CUBOIDS
  double minChi2              = std::numeric_limits<double>::max();
  const CbmTofHit* mergingHit = 0;
  double deltaLength          = 0;
  Line line                   = {x1, y1, z1, tx / normLen, ty / normLen, 1 / normLen};
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS

  set<const TBin*> neighbourhood;

  int zMinInd = GetZInd(fMinZ);
  int zMaxInd = GetZInd(zMax);

  for (int zInd = zMinInd; zInd <= zMaxInd; ++zInd) {
    const ZBin& zBin = fZBins[zInd];
    double startZ    = fMinZ + zInd * fZBinSize;
    double stopZ     = startZ + fZBinSize;
    int yIndDelta    = ty < 0 ? -1 : 1;
    double startY    = y1 + (startZ - z1) * ty;  // - yIndDelta * deltaY;
    double stopY     = y1 + (stopZ - z1) * ty;   // + yIndDelta * deltaY;
    int yStartInd    = GetYInd(startY);
    int yStopInd     = GetYInd(stopY);

    for (int yInd = yStartInd; true; yInd += yIndDelta) {
      //const YBin& yBin = zBin.fYBins[yInd]; (VF) unused
      double startZy;
      double stopZy;

      if (0 == ty) {
        startZy = startZ;
        stopZy  = stopZ;
      }
      else if (0 > ty) {
        startZy = z1 + (fMinY + (yInd + 1) * fYBinSize - y1) / ty;
        stopZy  = z1 + (fMinY + yInd * fYBinSize - y1) / ty;
      }
      else {
        startZy = z1 + (fMinY + yInd * fYBinSize - y1) / ty;
        stopZy  = z1 + (fMinY + (yInd + 1) * fYBinSize - y1) / ty;
      }

      if (startZy < startZ) startZy = startZ;

      if (stopZy > stopZ) stopZy = stopZ;

      //int xIndDelta = tx < 0 ? -1 : 1; (VF) unused
      double startX = x1 + (startZy - z1) * tx;  // - xIndDelta * deltaX;
      double stopX  = x1 + (stopZy - z1) * tx;   // + xIndDelta * deltaX;
      //int xStartInd = GetXInd(startX); (VF) unused
      //int xStopInd = GetXInd(stopX); (VF) unused
      //#ifndef CBM_GLOBALTB_TOF_3D_CUBOIDS
      double extLen = ((startZy + stopZy) / 2 - z1) * normLen;
      double extT   = t1 + extLen / fC;
      //int tInd = (extT - fStartTime) / fTBinSize;
      int tLowInd  = (extT - deltaT - fStartTime) / fTBinSize;
      int tHighInd = (extT + deltaT - fStartTime) / fTBinSize;

      if (tHighInd >= 0 || tLowInd < fNofTBins) {
        if (tLowInd < 0) tLowInd = 0;

        if (tHighInd >= fNofTBins) tHighInd = fNofTBins - 1;

        double startY2 = startY < stopY ? startY - deltaY : stopY - deltaY;
        double stopY2  = startY < stopY ? stopY + deltaY : startY + deltaY;
        int yStartInd2 = GetYInd(startY2);
        int yStopInd2  = GetYInd(stopY2);
        double startX2 = startX < stopX ? startX - deltaX : stopX - deltaX;
        double stopX2  = startX < stopX ? stopX + deltaX : startX + deltaX;
        int xStartInd2 = GetXInd(startX2);
        int xStopInd2  = GetXInd(stopX2);

        for (int yInd2 = yStartInd2; yInd2 <= yStopInd2; ++yInd2) {
          const YBin& yBin2 = zBin.fYBins[yInd2];

          for (int xInd2 = xStartInd2; xInd2 <= xStopInd2; ++xInd2) {
            const XBin& xBin2 = yBin2.fXBins[xInd2];

            for (int tInd2 = tLowInd; tInd2 <= tHighInd; ++tInd2)
              neighbourhood.insert(&xBin2.fTBins[tInd2]);
          }
        }
      }

      if (yInd == yStopInd) break;
    }
  }

  for (set<const TBin*>::const_iterator tBinIter = neighbourhood.begin(); tBinIter != neighbourhood.end(); ++tBinIter) {
    const TBin* tBin = *tBinIter;

    for (list<Int_t>::const_iterator hitIndIter = tBin->fHitInds.begin(); hitIndIter != tBin->fHitInds.end();
         ++hitIndIter) {
      Int_t hitInd         = *hitIndIter;
      const CbmTofHit* hit = static_cast<const CbmTofHit*>(fTofHits->At(hitInd));
      double L01Sq         = (hit->GetX() - x1) * (hit->GetX() - x1) + (hit->GetY() - y1) * (hit->GetY() - y1)
                     + (hit->GetZ() - z1) * (hit->GetZ() - z1);
      //double L01 = TMath::Sqrt(L01Sq);
      double L02   = (hit->GetX() - x1) * line.cosX + (hit->GetY() - y1) * line.cosY + (hit->GetZ() - z1) * line.cosZ;
      double extT2 = t1 + L02 / fC;
      /*double x = x1 + L02 * line.cosX;
         double y = y1 + L02 * line.cosY;
         double z = z1 + L02 * line.cosZ;
         double extT2 = t1 + TMath::Sqrt(x * x + y * y + z * z) / fC;*/
      double chi2 = (L01Sq - L02 * L02)
                      / (litTrackParams.GetCovariance(0) + hit->GetDx() * hit->GetDx() + litTrackParams.GetCovariance(5)
                         + hit->GetDy() * hit->GetDy())
                    + (hit->GetTime() - extT2) * (hit->GetTime() - extT2)
                        / (/*litTrackParams.GetTimeError() * litTrackParams.GetTimeError()*/
                           errT * errT + hit->GetTimeError() * hit->GetTimeError());

      if (chi2 < minChi2) {
        tofHitInd   = hitInd;
        mergingHit  = hit;
        minChi2     = chi2;
        deltaLength = L02;
      }
    }
  }

  if (minChi2 > fChi2Cut) {
    tofHitInd = -1;
    length    = 0;
  }
  else {
    length += deltaLength;
    fLinePropagator->Propagate(&litTrackParams, mergingHit->GetZ(), fPDG);
    CbmLitPixelHit litHit;
    CbmLitConverter::CbmPixelHitToCbmLitPixelHit(mergingHit, tofHitInd, &litHit);
    double mergingChi2 = 0;
    fFilter->Update(&litTrackParams, &litHit, mergingChi2);
    CbmLitConverterFairTrackParam::CbmLitTrackParamToFairTrackParam(&litTrackParams, &trackParams);
  }

#if 0
         if (tInd >= 0 && tInd < fNofTBins)
         {
//#endif//CBM_GLOBALTB_TOF_3D_CUBOIDS
            for (int xInd = xStartInd; true; xInd += xIndDelta)
            {
               const XBin& xBin = yBin.fXBins[xInd];

#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
            for (list<Cuboid*>::const_iterator i = xBin.fCuboids.begin(); i != xBin.fCuboids.end(); ++i)
               cuboidSet.insert(*i);
#else  //CBM_GLOBALTB_TOF_3D_CUBOIDS
               const TBin& tBin = xBin.fTBins[tInd];
               
               for (list<Int_t>::const_iterator hitIndIter = tBin.fHitInds.begin(); hitIndIter != tBin.fHitInds.end(); ++hitIndIter)
               {
                  Int_t hitInd = *hitIndIter;
                  const CbmTofHit* hit = static_cast<const CbmTofHit*> (fTofHits->At(hitInd));
                  double L01Sq = (hit->GetX() - x1) * (hit->GetX() - x1) + (hit->GetY() - y1) * (hit->GetY() - y1) + (hit->GetZ() - z1) * (hit->GetZ() - z1);
                  //double L01 = TMath::Sqrt(L01Sq);
                  double L02 = (hit->GetX() - x1) * line.cosX + (hit->GetY() - y1) * line.cosY + (hit->GetZ() - z1) * line.cosZ;
                  double extT2 = t1 + L02 / fC;
                  double chi2 = (L01Sq - L02 * L02) / (litTrackParams.GetCovariance(0) + hit->GetDx() * hit->GetDx() + litTrackParams.GetCovariance(6) + hit->GetDy() * hit->GetDy()) +
                     (hit->GetTime() - extT2) * (hit->GetTime() - extT2) / (litTrackParams.GetTimeError() * litTrackParams.GetTimeError() + hit->GetTimeError() * hit->GetTimeError());
            
                  if (chi2 < minChi2)
                  {
                     tofHitInd = hitInd;
                     minChi2 = chi2;
                  }
               }
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS
            
               if (xInd == xStopInd)
                  break;
            }
#ifndef CBM_GLOBALTB_TOF_3D_CUBOIDS
         }
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS
         
         if (yInd == yStopInd)
            break;
      }
   }
#endif  // 0

  /*double minChi2 = std::numeric_limits<double>::max();
   Line line = { x1, y1, z1, tx / normLen, ty / normLen, 1 / normLen };
   
   for (map<int, map<int, map<int, double> > >::const_iterator i = inds.begin(); i != inds.end(); ++i)
   {
      const map<int, map<int, double> >& yInds = i->second;
      
      for (map<int, map<int, double> >::const_iterator j = yInds.begin(); j != yInds.end(); ++j)
      {
         const map<int, double>& xInds = j->second;
         
         for (map<int, double>::const_iterator k = xInds.begin(); k != xInds.end(); ++k)
         {
            const XBin& xBin = fZBins[i->first].fYBins[j->first].fXBins[k->first];
            double extT = t1 + k->second / fC;
            int tInd = (extT - fStartTime) / fTBinSize;
            const TBin& tBin = xBin.fTBins[tInd];

            for (list<Int_t>::const_iterator hitIndIter = tBin.fHitInds.begin(); hitIndIter != tBin.fHitInds.end(); ++hitIndIter)
            {
               Int_t hitInd = *hitIndIter;
               const CbmTofHit* hit = static_cast<const CbmTofHit*> (fTofHits->At(hitInd));
               double L01Sq = (hit->GetX() - x1) * (hit->GetX() - x1) + (hit->GetY() - y1) * (hit->GetY() - y1) + (hit->GetZ() - z1) * (hit->GetZ() - z1);
               //double L01 = TMath::Sqrt(L01Sq);
               double L02 = (hit->GetX() - x1) * line.cosX + (hit->GetY() - y1) * line.cosY + (hit->GetZ() - z1) * line.cosZ;
               double extT2 = t1 + L02 / fC;
               double chi2 = (L01Sq - L02 * L02) / (litTrackParams.GetCovariance(0) + hit->GetDx() * hit->GetDx() + litTrackParams.GetCovariance(6) + hit->GetDy() * hit->GetDy()) +
                  (hit->GetTime() - extT2) * (hit->GetTime() - extT2) / (errT * errT + hit->GetTimeError() * hit->GetTimeError());

               if (chi2 < minChi2) {
                  tofHitInd = hitInd;
                  minChi2 = chi2;
               }
            }
         }
      }
   }*/

#ifdef CBM_GLOBALTB_TOF_3D_CUBOIDS
  if (cuboidSet.empty()) return;

  bool inMRPC  = false;
  bool inMRPCT = false;

  double minChi2 = std::numeric_limits<double>::max();
  double normLen = TMath::Sqrt(1 + tx * tx + ty * ty);
  Line line      = {x1, y1, z1, tx / normLen, ty / normLen, 1 / normLen};

  for (set<const Cuboid*>::const_iterator i = cuboidSet.begin(); i != cuboidSet.end(); ++i) {
    const Cuboid* cuboid   = *i;
    const Rectangle& face1 = cuboid->faces[0][0];
    const Rectangle& face2 = cuboid->faces[0][1];
    double result[3];

    if (Intersect(face1, line, result) || Intersect(face2, line, result)) {
      inMRPC      = true;
      double extT = t0 + result[2] / fC;
      int tInd    = (extT - fStartTime) / fTBinSize;

      if (tInd < 0 || tInd >= fNofTBins) continue;

      inMRPCT = true;

      //const TBin& tBin = xBin.f
      double extX = x1 + line.cosX * result[2];
      double extY = y1 + line.cosY * result[2];
      double extZ = z1 + line.cosZ * result[2];
      TBin& tBin  = cuboid->fTBins[tInd];

      for (list<Int_t>::const_iterator hitIndIter = tBin.fHitInds.begin(); hitIndIter != tBin.fHitInds.end();
           ++hitIndIter) {
        Int_t hitInd         = *hitIndIter;
        const CbmTofHit* hit = static_cast<const CbmTofHit*>(fTofHits->At(hitInd));
        double chi2          = (hit->GetX() - extX) * (hit->GetX() - extX) / (errX * errX + hit->GetDx() * hit->GetDx())
                      + (hit->GetY() - extY) * (hit->GetY() - extY) / (errY * errY + hit->GetDy() * hit->GetDy())
                      + (hit->GetTime() - extT) * (hit->GetTime() - extT)
                          / (litTrackParams.GetTimeError() * litTrackParams.GetTimeError()
                             + hit->GetTimeError() * hit->GetTimeError());

        if (chi2 < minChi2) {
          tofHitInd = hitInd;
          minChi2   = chi2;
        }
      }
    }
  }

  if (inMRPC) ++nofMRPCIntersections;

  if (inMRPCT) ++nofMRPCIntersectionsT;
#endif  //CBM_GLOBALTB_TOF_3D_CUBOIDS
}
