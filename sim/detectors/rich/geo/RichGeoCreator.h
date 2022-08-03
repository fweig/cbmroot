/* Copyright (C) 2022-2022 UGiessen/GSI, Giessen/Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

#ifndef RICH_GEO_CREATOR
#define RICH_GEO_CREATOR

#include "TObject.h"

#include <string>

#include <cmath>

#include "RichGeoMaterials.h"

class TGeoVolumeAssembly;
class TGeoVolume;
class TGeoShape;
class TGeoMatrix;
class TGeoCompositeShape;
class TGeoRotation;

class RichGeoCreator : public TObject {
public:
  RichGeoCreator();

  virtual ~RichGeoCreator();

  void Create();

  void SetGeoName(const std::string& geoName) { fGeoName = geoName; }
  void SetAddShieldingBox(bool addSchildingBox) { fAddShieldingBox = addSchildingBox; }
  void SetVolumeColors(bool setVolumeColors) { fSetVolumeColors = setVolumeColors; }

private:
  RichGeoMaterials fMaterials;

  std::string fGeoName  = "rich";
  bool fAddShieldingBox = true;
  bool fSetVolumeColors = false;

  // General RICH parameters
  double fDegToRad              = 1.74532925199433E-02;
  double fRichOrigZ             = 180.;   //cm
  double fAccAngle              = 25.;    //deg
  double fRichLength            = 190.;   //cm
  double fRichHeight            = 500.;   //cm
  double fRichWidth             = 526.8;  //cm
  double fRichEntranceY         = 180.;   //cm
  double fRichCoveringThickness = 0.5;    //cm
  double fRichEntranceThickness = 0.025;  //cm
  double fRichExitThickness     = 0.5;    //cm
  double fRichAddZ              = 20.;    //cm

  // Pipe parameters
  double fPipeOuterRadiusEntrance = 7.42;   //cm
  double fPipeThicknessEntrance   = 0.148;  //cm
  double fPipeOuterRadiusExit     = 16.16;  //cm
  double fPipeThicknessExit       = 0.32;   //cm
  double fPipeCylPartLen          = 10.0;   //cm

  // Mirror parameters
  double fMirrorRadius     = 300.;    //cm
  double fMirrorThickness  = 0.6;     //cm
  double fMirrorPos        = 350.;    //cm
  double fMirrorRot        = 12.;     //deg
  double fMirrorPhiSize    = 8.425;   //deg
  double fMirrorPhiStep    = 8.5;     //deg
  double fMirrorThetaSize  = 8.088;   //deg
  double fMirrorThetaStep  = 8.1625;  //deg
  double fMirrorThetaGap   = 0.0745;  //deg
  double fMirrorSupportAdd = 4.5;     //cm
  double fMirrorGapY       = 0.1;     // cm

  // PMT parameters
  int fNofPixels           = 8;
  double fPixelSize        = 0.6;    //cm
  double fPixelSizeEdge    = 0.625;  //cm
  double fPixelThickness   = 0.05;   //cm
  double fPmtSize          = 5.2;    //cm
  double fPmtDepth         = 3.871;  //cm
  double fCameraRot        = 15.;    //deg
  double fCameraShiftY     = 2.5;    //cm
  double fCameraShiftZ     = 5.;     //cm
  double fCameraRadius     = 169.7;  //cm
  double fCameraGap        = 0.1;    //cm
  double fCameraTouchWidth = 15.75;  //cm

  // Sensitive plane parameters
  double fSensPlaneZ = -70.;  //cm

  // Calculated parameters. These parameters are calculated in CalculateParams()
  double fCameraFullRotX    = 0.;
  double fCameraOriginY     = 0.;
  double fCameraOriginZ     = 0.;
  double fMirrorOriginY     = 0.;
  double fMirrorOriginZ     = 0.;
  double fAlpha             = 0.;
  double fCameraHeight      = 0.;
  double fCameraTouchRadius = 0.;
  double fCameraSegmentAng  = 0.;

  void CalculateParams();

  TGeoVolume* CreatePmt();
  TGeoVolume* CreateCameraModule();
  TGeoVolume* CreateCameraStrip();
  TGeoVolumeAssembly* CreateCameraContainer();
  TGeoVolumeAssembly* CreateMirror();
  TGeoVolumeAssembly* CreateMainFrame();
  TGeoVolume* CreateGas();
  TGeoVolume* CreateRichContainer();
  TGeoVolume* CreateSensitivePlane();
  TGeoVolume* CreateRichEntrance();
  TGeoVolume* CreateRichExit();
  TGeoVolume* CreatePipe();
  TGeoVolume* CreatePipeCyl();
  TGeoVolumeAssembly* CreateSchieldingBox();
  TGeoVolumeAssembly* CreateBeltAssembly();
  TGeoVolumeAssembly* CreateMirrorSupportBelts();
  TGeoVolumeAssembly* CreateMirrorSupportPillars();

  void CreateRich();

  double ToDeg(double rad) { return rad / fDegToRad; }
  double ToRad(double deg) { return deg * fDegToRad; }

  TGeoCompositeShape* MakeSubtraction(const std::string& name, TGeoShape* left, TGeoShape* right,
                                      TGeoMatrix* lmat = nullptr, TGeoMatrix* rmat = nullptr);
  TGeoCompositeShape* MakeUnion(const std::string& name, TGeoShape* left, TGeoShape* right, TGeoMatrix* lmat = nullptr,
                                TGeoMatrix* rmat = nullptr);
  TGeoCompositeShape* MakeIntersection(const std::string& name, TGeoShape* left, TGeoShape* right,
                                       TGeoMatrix* lmat = nullptr, TGeoMatrix* rmat = nullptr);

  TGeoCombiTrans* MakeCombiTrans(double dx, double dy, double dz, double rotX, double rotY, double rotZ);
  TGeoTranslation* MakeTrans(double dx, double dy, double dz);
  TGeoRotation* MakeRot(double rotX, double rotY, double rotZ);

  ClassDef(RichGeoCreator, 1)
};

#endif
