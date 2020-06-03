#ifndef LXCA_INCLUDED
#define LXCA_INCLUDED

#pragma GCC diagnostic ignored "-Weffc++"

#include "Rtypes.h"
#include <list>
#include <math.h>
#include "LxSettings.h"
#include "LxMC.h"

struct LxPoint;

#define errorTxCoeff 4.0
#define errorTxCoeffSq errorTxCoeff * errorTxCoeff
#define errorTyCoeff 4.0
#define errorTyCoeffSq errorTyCoeff * errorTyCoeff

extern scaltype tx_limits[LXSTATIONS];
extern scaltype tx_limits_sq[LXSTATIONS];
extern scaltype ty_limits[LXSTATIONS];
extern scaltype ty_limits_sq[LXSTATIONS];
extern scaltype x_disp_left_limits[LXSTATIONS];
extern scaltype x_disp_left_limits_sq[LXSTATIONS];
extern scaltype y_disp_left_limits[LXSTATIONS];
extern scaltype y_disp_left_limits_sq[LXSTATIONS];
extern scaltype x_disp_right_limits[LXSTATIONS];
extern scaltype x_disp_right_limits_sq[LXSTATIONS];
extern scaltype y_disp_right_limits[LXSTATIONS];
extern scaltype y_disp_right_limits_sq[LXSTATIONS];
/*extern scaltype x_coords[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
extern scaltype tx_vals[LXSTATIONS - 1][LXMAXPOINTSONSTATION];
extern scaltype x_errs[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
extern scaltype y_coords[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
extern scaltype ty_vals[LXSTATIONS][LXMAXPOINTSONSTATION];
extern scaltype y_errs[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
extern scaltype z_coords[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
extern LxPoint* point_refs[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
extern bool use_points[LXSTATIONS - 1][LXMAXPOINTSONSTATION];
extern int points_counts[LXSTATIONS][LXLAYERS];

void InitGlobalCAArrays();
void BuildRaysGlobal();*/

struct LxRay;
struct LxLayer;
struct LxTrack;
struct LxStation;
struct LxSpace;
struct LxTrackCandidate;

struct LxPoint
{
  scaltype x, y, z;
  scaltype dx, dy, dz;
  bool used;
  bool valid;
  bool artificial;
  LxTrack* track;
  std::list<LxRay*> rays;
  LxLayer* layer;
  Int_t hitId;
#ifdef MAKE_EFF_CALC
  std::list<LxMCPoint*> mcPoints;
#endif//MAKE_EFF_CALC

#ifdef REMEMBER_CLUSTERED_RAYS_IN_POINTS
  LxRay* leftClusteredRay;
  LxRay* rightClusteredRay;
#endif//REMEMBER_CLUSTERED_RAYS_IN_POINTS

  LxPoint(scaltype X, scaltype Y, scaltype Z, scaltype Dx, scaltype Dy, scaltype Dz, LxLayer* lay, int hId, bool isArtificial = false) :
      x(X), y(Y), z(Z), dx(Dx), dy(Dy), dz(Dz), used(false), valid(true), artificial(isArtificial), track(NULL), rays(), layer(lay), hitId(hId)
#ifdef REMEMBER_CLUSTERED_RAYS_IN_POINTS
      , leftClusteredRay(0), rightClusteredRay(0)
#endif//REMEMBER_CLUSTERED_RAYS_IN_POINTS
        {}
  ~LxPoint();
  void CreateRay(LxPoint* lPoint, scaltype tx, scaltype ty, scaltype dtx, scaltype dty);
};

#ifdef USE_KALMAN
struct LxKalmanParams
{
  scaltype tx, ty;
  scaltype C11, C22;
  scaltype chi2;
};
#endif//USE_KALMAN

struct LxRay
{
  scaltype tx, ty;
  scaltype dtx, dty;
  LxPoint* source;
  LxPoint* end;
  LxStation* station;
  std::list<LxRay*> neighbours;
#ifdef CLUSTER_MODE
  Int_t level;
  bool used;
  std::list<LxRay*> neighbourhood;
  std::list<LxPoint*> clusterPoints;
#endif//CLUSTER_MODE

#ifdef USE_KALMAN
  LxKalmanParams kalman;
#endif//USE_KALMAN
  LxRay(LxPoint* s, LxPoint* e
#ifdef CLUSTER_MODE
      , Int_t
#endif//CLUSTER_MODE
      );
  LxRay(LxPoint* s, LxPoint* e, scaltype Tx, scaltype Ty, scaltype Dtx, scaltype Dty
#ifdef CLUSTER_MODE
      , Int_t
#endif//CLUSTER_MODE
      );
};

struct LxLayer
{
  std::list<LxPoint*> points;
  LxStation* station;
  int layerNumber;
  scaltype zCoord;
  LxLayer(LxStation* st, int lNum);
  ~LxLayer();
  void Clear();

  LxPoint* AddPoint(int hitId, scaltype x, scaltype y, scaltype z, scaltype dx, scaltype dy, scaltype dz, bool isArtificial = false)
  {
    LxPoint* result = new LxPoint(x, y, z, dx, dy, dz, this, hitId, isArtificial);
    points.push_back(result);
    return result;
  }

  LxPoint* PickNearestPoint(scaltype x, scaltype y);// Used in track building.
  LxPoint* PickNearestPoint(LxRay* ray);// Used in track building.
  LxPoint* PickNearestPoint(scaltype x, scaltype y, scaltype deltaX, scaltype deltaY);// Used in middle point building.
  bool HasPointInRange(scaltype x, scaltype y, scaltype deltaX, scaltype deltaY);
};

#ifdef CLUSTER_MODE
typedef void* kdt_rays_handle;
#endif//CLUSTER_MODE

struct LxStation
{
  std::vector<LxLayer*> layers;
#ifdef CLUSTER_MODE
  kdt_rays_handle raysHandle;
  std::vector<std::list<LxRay*>*> clusters[2 * LXLAYERS];
  kdt_rays_handle clusteredRaysHandle;
  std::list<LxPoint*> clusteredPoints;
  scaltype clusterXLimit;
  scaltype clusterXLimit2;
  scaltype clusterYLimit;
  scaltype clusterYLimit2;
  scaltype clusterTxLimit;
  scaltype clusterTxLimit2;
  scaltype clusterTyLimit;
  scaltype clusterTyLimit2;
#endif//CLUSTER_MODE
  LxSpace* space;
  int stationNumber;
  scaltype zCoord;
  scaltype txLimit;
  scaltype tyLimit;
  scaltype txBreakLimit;
  scaltype tyBreakLimit;
  scaltype txBreakSigma;
  scaltype tyBreakSigma;
  scaltype disp01XSmall;// 'disp' -- means: dispersion.
  scaltype disp01XBig;
  scaltype disp01YSmall;
  scaltype disp01YBig;
  scaltype disp02XSmall;
  scaltype disp02XBig;
  scaltype disp02YSmall;
  scaltype disp02YBig;
#ifdef USE_KALMAN_FIT
  scaltype MSNoise[2][2][2];
#endif//USE_KALMAN_FIT
  LxStation(LxSpace* sp, int stNum);
  ~LxStation();
  void Clear();

  LxPoint* AddPoint(int layerNumber, int hitId, scaltype x, scaltype y, scaltype z, scaltype dx, scaltype dy, scaltype dz)
  {
    return layers[layerNumber]->AddPoint(hitId, x, y, z, dx, dy, dz);
  }

  void RestoreMiddlePoints();
  void BuildRays();
#ifdef CLUSTER_MODE
  void BuildRays2();
  void InsertClusterRay(Int_t levels, Int_t cardinality, LxRay* clusterRay);
#endif//CLUSTER_MODE
  void ConnectNeighbours();
};

struct LxExtTrack
{
  CbmStsTrack* track;
  Int_t extId;
  LxMCTrack* mcTrack;
#ifdef LX_EXT_LINK_SOPH
  std::pair<LxTrack*, scaltype> recoTrack;
#endif//LX_EXT_LINK_SOPH

  LxExtTrack() : track(0), extId(-1), mcTrack(0)
#ifdef LX_EXT_LINK_SOPH
      , recoTrack(0, 0)
#endif//LX_EXT_LINK_SOPH
  {}
};

struct LxTrack
{
  LxExtTrack* externalTrack;
#ifdef LX_EXT_LINK_SOPH
  std::list<std::pair<LxExtTrack*, scaltype> > extTrackCandidates;
#else//LX_EXT_LINK_SOPH
  scaltype extLinkChi2;
#endif//LX_EXT_LINK_SOPH
  bool matched;
  LxMCTrack* mcTrack;
#ifdef CALC_LINK_WITH_STS_EFF
  std::list<LxMCTrack*> mcTracks;
#endif//CALC_LINK_WITH_STS_EFF
  int length;
  LxRay* rays[LXSTATIONS - 1];// Rays are stored left to right.
  LxPoint* points[LXSTATIONS * LXLAYERS];
  scaltype chi2;
  scaltype aX;
  scaltype bX;
  scaltype aY;
  scaltype bY;
  int restoredPoints;
#ifdef USE_KALMAN_FIT
  scaltype x, y, z, dx, dy, tx, ty, dtx, dty;
#endif//USE_KALMAN_FIT
  bool clone;
  // The following variables used in triggering:
  bool distanceOk;
  bool oppCharged;
  bool triggering;
  // .

  explicit LxTrack(LxTrackCandidate* tc);
  void Fit();
#ifdef LX_EXT_LINK_SOPH
  void Rebind();
#endif//LX_EXT_LINK_SOPH
};

typedef scaltype scal_coords[LXLAYERS][LXMAXPOINTSONSTATION];
typedef scaltype scal_tans[LXMAXPOINTSONSTATION];

struct LxSpace
{
  scal_coords* x_coords;
  scal_tans* tx_vals;
  scal_coords* x_errs;
  scal_coords* y_coords;
  scal_tans* ty_vals;
  scal_coords* y_errs;
  scal_coords* z_coords;
  LxPoint* point_refs[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
  bool use_points[LXSTATIONS - 1][LXMAXPOINTSONSTATION];
  int points_counts[LXSTATIONS][LXLAYERS];
  void InitGlobalCAArrays();
  void CalcTangents(int station_number);
  void BuildRaysGlobal();
  void RefineMiddlePoints();

  std::vector<LxStation*> stations;
  std::list<LxTrack*> tracks;
  std::list<LxExtTrack> extTracks;
  scaltype muchStsBreakX;
  scaltype muchStsBreakY;
  scaltype muchStsBreakTx;
  scaltype muchStsBreakTy;

  Int_t stationsInAlgo;

  LxSpace();
  ~LxSpace();
  void Clear();

  LxPoint* AddPoint(int stationNumber, int layerNumber, int hitId, scaltype x, scaltype y, scaltype z, scaltype dx, scaltype dy, scaltype dz)
  {
    return stations[stationNumber]->AddPoint(layerNumber, hitId, x, y, z, dx, dy, dz);
  }

  void RestoreMiddlePoints();
  void BuildRays();
#ifdef CLUSTER_MODE
  void BuildRays2();
  void ConnectNeighbours2();
  void BuildCandidates2(LxRay* ray, LxRay** rays, std::list<LxTrackCandidate*>& candidates, scaltype chi2);
  void Reconstruct2();
#endif//CLUSTER_MODE
  void ConnectNeighbours();
  void BuildCandidates(int endStNum, LxRay* ray, LxRay** rays, std::list<LxTrackCandidate*>& candidates, scaltype chi2);
  void Reconstruct();
  void RemoveClones();
  void FitTracks();
  void JoinExtTracks();
  void CheckArray(scaltype xs[LXSTATIONS][LXLAYERS], scaltype ys[LXSTATIONS][LXLAYERS],
      scaltype zs[LXSTATIONS][LXLAYERS],
      scaltype xDisp2Limits[LXSTATIONS], scaltype yDisp2Limits[LXSTATIONS], scaltype tx2Limits[LXSTATIONS],
        scaltype ty2Limits[LXSTATIONS], scaltype txBreak2Limits[LXSTATIONS], scaltype tyBreak2Limits[LXSTATIONS]);
  void CheckArray(scaltype xs[LXSTATIONS][LXLAYERS], scaltype ys[LXSTATIONS][LXLAYERS],
      scaltype zs[LXSTATIONS][LXLAYERS], std::list<LxPoint*> pts[LXSTATIONS][LXLAYERS], int level,
      scaltype xDisp2Limits[LXSTATIONS], scaltype yDisp2Limits[LXSTATIONS], scaltype tx2Limits[LXSTATIONS],
        scaltype ty2Limits[LXSTATIONS], scaltype txBreak2Limits[LXSTATIONS], scaltype tyBreak2Limits[LXSTATIONS]);
  void CheckArray(std::ostream& out, LxMCTrack& track);
};

#endif//LXCA_INCLUDED
