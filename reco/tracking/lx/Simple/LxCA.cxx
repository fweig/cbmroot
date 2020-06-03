#include "LxCA.h"
#include <iostream>
#include "Lx.h"
#pragma GCC diagnostic ignored "-Weffc++"

#ifdef CLUSTER_MODE
#include "kdtree++/kdtree.hpp"
#endif//CLUSTER_MODE

#include <sys/time.h>
#ifdef BEST_SIX_POINTS
#include <set>
#endif//BEST_SIX_POINTS

#include <cmath>
#include "base/CbmLitToolFactory.h"
#include "utils/CbmLitConverterFairTrackParam.h"

using namespace std;

scaltype tx_limits[LXSTATIONS];
scaltype tx_limits_sq[LXSTATIONS];
scaltype ty_limits[LXSTATIONS];
scaltype ty_limits_sq[LXSTATIONS];
scaltype x_disp_left_limits[LXSTATIONS];
scaltype x_disp_left_limits_sq[LXSTATIONS];
scaltype y_disp_left_limits[LXSTATIONS];
scaltype y_disp_left_limits_sq[LXSTATIONS];
scaltype x_disp_right_limits[LXSTATIONS];
scaltype x_disp_right_limits_sq[LXSTATIONS];
scaltype y_disp_right_limits[LXSTATIONS];
scaltype y_disp_right_limits_sq[LXSTATIONS];
/*scaltype x_coords[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
scaltype tx_vals[LXSTATIONS - 1][LXMAXPOINTSONSTATION];
scaltype x_errs[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
scaltype y_coords[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
scaltype ty_vals[LXSTATIONS][LXMAXPOINTSONSTATION];
scaltype y_errs[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
scaltype z_coords[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
LxPoint* point_refs[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
bool use_points[LXSTATIONS - 1][LXMAXPOINTSONSTATION];
int points_counts[LXSTATIONS][LXLAYERS];*/

void LxSpace::InitGlobalCAArrays()
{
  memset(x_coords, 0, sizeof(scaltype) * LXSTATIONS * LXLAYERS * LXMAXPOINTSONSTATION);
  memset(tx_vals, 0, sizeof(scaltype) * (LXSTATIONS - 1) * LXLAYERS * LXMAXPOINTSONSTATION);
  memset(x_errs, 0, sizeof(scaltype) * LXSTATIONS * LXLAYERS * LXMAXPOINTSONSTATION);
  memset(y_coords, 0, sizeof(scaltype) * LXSTATIONS * LXLAYERS * LXMAXPOINTSONSTATION);
  memset(ty_vals, 0, sizeof(scaltype) * (LXSTATIONS - 1) * LXLAYERS * LXMAXPOINTSONSTATION);
  memset(y_errs, 0, sizeof(scaltype) * LXSTATIONS * LXLAYERS * LXMAXPOINTSONSTATION);
  memset(z_coords, 0, sizeof(scaltype) * LXSTATIONS * LXLAYERS * LXMAXPOINTSONSTATION);
  memset(point_refs, 0, sizeof(LxPoint*) * LXSTATIONS * LXLAYERS * LXMAXPOINTSONSTATION);
  memset(points_counts, 0, sizeof(int) * LXSTATIONS * LXLAYERS);

  for (int i = 0; i < stationsInAlgo - 2; ++i)
    memset(&use_points[i][0], 0, sizeof(use_points[i]));

  for (int i = stationsInAlgo - 2; i < LXSTATIONS - 1; ++i)
    memset(&use_points[i][0], 1, sizeof(use_points[LXSTATIONS - 2]));
}

//#define LXSIMDIZE

#ifdef LXSIMDIZE

#include "immintrin.h"

#define vectype __m256
#define veclen 8
#define vec_load_ps _mm256_load_ps
#define vec_store_ps _mm256_store_ps
#define vec_set1_ps _mm256_set1_ps
#define vec_add_ps _mm256_add_ps
#define vec_sub_ps _mm256_sub_ps
#define vec_mul_ps _mm256_mul_ps
#define vec_div_ps _mm256_div_ps
#define vec_cmp_ps _mm256_cmp_ps
#define vec_or_ps _mm256_or_ps
#define vec_sqrt_ps _mm256_sqrt_ps

void LxSpace::RefineMiddlePoints()
{
  for (int station_number = LXSTATIONS - 1; station_number >= stationsInAlgo - 1; --station_number)
  {
    for (int i = 0; i < points_counts[station_number][1]; ++i)
    {
      scaltype xC = x_coords[station_number][1][i];
      scaltype yC = y_coords[station_number][1][i];
      scaltype zC = z_coords[station_number][1][i];
      scaltype tx = xC / zC;
      scaltype ty = yC / zC;
      scaltype xErrCSq = x_errs[station_number][1][i] * x_errs[station_number][1][i];
      scaltype yErrCSq = y_errs[station_number][1][i] * y_errs[station_number][1][i];
      bool validC = false;

      for (int j = 0; j < points_counts[station_number][2]; ++j)
      {
        scaltype zR = z_coords[station_number][2][j];
        scaltype deltaZ = zR - zC;
        scaltype xR = x_coords[station_number][2][j];
        scaltype xErrRSq = x_errs[station_number][2][j] * x_errs[station_number][2][j];
        scaltype x = xC + tx * deltaZ;
        scaltype deltaX = x - xR;
        scaltype deltaXSq = deltaX * deltaX;

        if (deltaXSq > 16 * (xErrCSq + xErrRSq) + x_disp_right_limits_sq[station_number])
          continue;

        scaltype yR = y_coords[station_number][2][j];
        scaltype yErrRSq = y_errs[station_number][2][j] * y_errs[station_number][2][j];
        scaltype y = yC + ty * deltaZ;
        scaltype deltaY = y - yR;
        scaltype deltaYSq = deltaY * deltaY;

        if (deltaYSq > 16 * (yErrCSq + yErrRSq) + y_disp_right_limits_sq[station_number])
          continue;

        validC = true;
        break;
      }

      if (validC)
        continue;

      for (int j = 0; j < points_counts[station_number][0]; ++j)
      {
        scaltype zL = z_coords[station_number][0][j];
        scaltype deltaZ = zL - zC;
        scaltype xL = x_coords[station_number][0][j];
        scaltype xErrLSq = x_errs[station_number][0][j] * x_errs[station_number][0][j];
        scaltype x = xC + tx * deltaZ;
        scaltype deltaX = x - xL;
        scaltype deltaXSq = deltaX * deltaX;

        if (deltaXSq > 16 * (xErrCSq + xErrLSq) + x_disp_left_limits_sq[station_number])
          continue;

        scaltype yL = y_coords[station_number][0][j];
        scaltype yErrLSq = y_errs[station_number][0][j] * y_errs[station_number][0][j];
        scaltype y = yC + ty * deltaZ;
        scaltype deltaY = y - yL;
        scaltype deltaYSq = deltaY * deltaY;

        if (deltaYSq > 16 * (yErrCSq + yErrLSq) + y_disp_left_limits_sq[station_number])
          continue;

        validC = true;
        break;
      }

      if (!validC)
      {
        use_points[station_number - 1][i] = 0;
        point_refs[station_number][1][i]->valid = false;
      }
    }
  }
}

//#include <omp.h>

void LxSpace::CalcTangents(int station_number)
{
  int l_station_number = station_number - 1;

  if (points_counts[station_number][1] > 200)
  {
    //omp_set_num_threads(4);
//#pragma omp parallel for
  for (int i = 0; i < points_counts[station_number][1]; i += veclen)
  {
    if (!*reinterpret_cast<unsigned long long*> (&use_points[l_station_number][i]))
      continue;

    vectype x = vec_load_ps(&x_coords[station_number][1][i]);
    vectype y = vec_load_ps(&y_coords[station_number][1][i]);
    vectype z = vec_load_ps(&z_coords[station_number][1][i]);
    vectype tx = vec_div_ps(x, z);
    vectype ty = vec_div_ps(y, z);
    vec_store_ps(&tx_vals[l_station_number][i], tx);
    vec_store_ps(&ty_vals[l_station_number][i], ty);
  }
  }
  else
  {
    for (int i = 0; i < points_counts[station_number][1]; i += veclen)
    {
      if (!*reinterpret_cast<unsigned long long*> (&use_points[l_station_number][i]))
        continue;

      vectype x = vec_load_ps(&x_coords[station_number][1][i]);
      vectype y = vec_load_ps(&y_coords[station_number][1][i]);
      vectype z = vec_load_ps(&z_coords[station_number][1][i]);
      vectype tx = vec_div_ps(x, z);
      vectype ty = vec_div_ps(y, z);
      vec_store_ps(&tx_vals[l_station_number][i], tx);
      vec_store_ps(&ty_vals[l_station_number][i], ty);
    }
  }
}

void LxSpace::BuildRaysGlobal()
{
  vectype tx_coeff_sq = vec_set1_ps(errorTxCoeffSq);
  vectype ty_coeff_sq = vec_set1_ps(errorTyCoeffSq);
  scaltype res_buf[veclen];
  scaltype tx_buf[veclen];
  scaltype ty_buf[veclen];
  scaltype dtx_buf[veclen];
  scaltype dty_buf[veclen];
  int iter;

//#pragma omp parallel for
  for (int i = LXSTATIONS - 1; i > 0; --i)
  {
    vectype tx_limit_sq = vec_set1_ps(tx_limits_sq[i]);
    vectype ty_limit_sq = vec_set1_ps(ty_limits_sq[i]);
    CalcTangents(i);
    int i2 = i - 1;
    int i3 = i2 - 1;

    for (int j = 0; j < points_counts[i][1]; ++j)
    {
      if (!use_points[i2][j])
        continue;

      vectype r_x = vec_set1_ps(x_coords[i][1][j]);
      vectype r_dx = vec_set1_ps(x_errs[i][1][j]);
      vectype r_y = vec_set1_ps(y_coords[i][1][j]);
      vectype r_dy = vec_set1_ps(y_errs[i][1][j]);
      vectype r_z = vec_set1_ps(z_coords[i][1][j]);
      vectype tx0 = vec_set1_ps(tx_vals[i2][j]);
      vectype ty0 = vec_set1_ps(ty_vals[i2][j]);

      vectype r_dx_sq = vec_mul_ps(r_dx, r_dx);
      vectype r_dy_sq = vec_mul_ps(r_dy, r_dy);

//#pragma omp parallel for
      for (int k = 0; k < points_counts[i2][1]; k += veclen)
      {
        vectype l_x = vec_load_ps(&x_coords[i2][1][k]);
        vectype l_dx = vec_load_ps(&x_errs[i2][1][k]);
        vectype l_y = vec_load_ps(&y_coords[i2][1][k]);
        vectype l_dy = vec_load_ps(&y_errs[i2][1][k]);
        vectype l_z = vec_load_ps(&z_coords[i2][1][k]);

        vectype delta_x = vec_sub_ps(l_x, r_x);
        vectype delta_y = vec_sub_ps(l_y, r_y);
        vectype delta_z = vec_sub_ps(l_z, r_z);
        vectype delta_z_sq = vec_mul_ps(delta_z, delta_z);

        vectype tx = vec_div_ps(delta_x, delta_z);
        vectype ty = vec_div_ps(delta_y, delta_z);
        vectype l_dx_sq = vec_mul_ps(l_dx, l_dx);
        vectype l_dy_sq = vec_mul_ps(l_dy, l_dy);

        vectype dtx_sq = vec_add_ps(r_dx_sq, l_dx_sq);
        dtx_sq = vec_div_ps(dtx_sq, delta_z_sq);
        vectype dtx = vec_sqrt_ps(dtx_sq);
        dtx_sq = vec_mul_ps(dtx_sq, tx_coeff_sq);
        dtx_sq = vec_add_ps(dtx_sq, tx_limit_sq);
        vectype dty_sq = vec_add_ps(r_dy_sq, l_dy_sq);
        dty_sq = vec_div_ps(dty_sq, delta_z_sq);
        vectype dty = vec_sqrt_ps(dty_sq);
        dty_sq = vec_mul_ps(dty_sq, ty_coeff_sq);
        dty_sq = vec_add_ps(dtx_sq, ty_limit_sq);

        vectype diff_tx = vec_sub_ps(tx, tx0);
        vectype diff_ty = vec_sub_ps(ty, ty0);
        vectype diff_tx_sq = vec_mul_ps(diff_tx, diff_tx);
        vectype diff_ty_sq = vec_mul_ps(diff_ty, diff_ty);

        vectype res_tx = vec_cmp_ps(diff_tx_sq, dtx_sq, _CMP_GT_OS);
        vectype res_ty = vec_cmp_ps(diff_ty_sq, dty_sq, _CMP_GT_OS);
        vectype res = vec_or_ps(res_tx, res_ty);
        vec_store_ps(res_buf, res);
        vec_store_ps(tx_buf, tx);
        vec_store_ps(ty_buf, ty);
        vec_store_ps(dtx_buf, dtx);
        vec_store_ps(dty_buf, dty);
        iter = k;

        for (int l = 0; l < veclen; ++l)
        {
          if (res_buf[l])
          {
            ++iter;
            continue;
          }

          if (iter >= points_counts[i2][1])
            break;

          point_refs[i][1][j]->CreateRay(point_refs[i2][1][iter], tx_buf[l], ty_buf[l], dtx_buf[l], dty_buf[l]);

          if (i3 > -1)
            use_points[i3][iter] = true;

          ++iter;
        }
      }
    }
  }
}

#else//LXSIMDIZE

#define _mm_malloc(X, Y) malloc((X))
#define _mm_free free

scaltype x_coords[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
scaltype tx_vals[LXSTATIONS - 1][LXMAXPOINTSONSTATION];
scaltype x_errs[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
scaltype y_coords[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
scaltype ty_vals[LXSTATIONS][LXMAXPOINTSONSTATION];
scaltype y_errs[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
scaltype z_coords[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
LxPoint* point_refs[LXSTATIONS][LXLAYERS][LXMAXPOINTSONSTATION];
bool use_points[LXSTATIONS - 1][LXMAXPOINTSONSTATION];
int points_counts[LXSTATIONS][LXLAYERS];

static void CalcTangents(int station_number)
{
  int l_station_number = station_number - 1;

  for (int i = 0; i < points_counts[station_number][1]; ++i)
  {
    if (!use_points[l_station_number][i])
      continue;

    tx_vals[l_station_number][i] = x_coords[station_number][1][i] / z_coords[station_number][1][i];
    ty_vals[l_station_number][i] = y_coords[station_number][1][i] / z_coords[station_number][1][i];
  }
}

void BuildRaysGlobal()
{
  for (int i = LXSTATIONS - 1; i > 0; --i)
  {
    scaltype tx_limit_sq = tx_limits[i] * tx_limits[i];
    scaltype ty_limit_sq = ty_limits[i] * ty_limits[i];
    CalcTangents(i);
    int i2 = i - 1;
    int i3 = i2 - 1;

    for (int j = 0; j < points_counts[i][1]; ++j)
    {
      if (!use_points[i2][j])
        continue;

      scaltype r_x = x_coords[i][1][j];
      scaltype r_y = y_coords[i][1][j];
      scaltype r_z = z_coords[i][1][j];
      scaltype r_dx_sq = x_errs[i][1][j] * x_errs[i][1][j];
      scaltype r_dy_sq = y_errs[i][1][j] * y_errs[i][1][j];

      for (int k = 0; k < points_counts[i2][1]; ++k)
      {
        scaltype delta_x = x_coords[i2][1][k] - r_x;
        scaltype delta_y = y_coords[i2][1][k] - r_y;
        scaltype delta_z = z_coords[i2][1][k] - r_z;
        scaltype delta_z_sq = delta_z * delta_z;

        scaltype tx = delta_x / delta_z;
        scaltype ty = delta_y / delta_z;
        scaltype l_dx_sq = x_errs[i2][1][k] * x_errs[i2][1][k];
        scaltype l_dy_sq = y_errs[i2][1][k] * y_errs[i2][1][k];

        scaltype dtx_sq = (r_dx_sq + l_dx_sq) / delta_z_sq;
        scaltype dtx = sqrt(dtx_sq);
        dtx_sq *= errorTxCoeffSq;
        dtx_sq += tx_limit_sq;
        scaltype dty_sq = (r_dy_sq + l_dy_sq) / delta_z_sq;
        scaltype dty = sqrt(dty_sq);
        dty_sq *= errorTyCoeffSq;
        dty_sq += ty_limit_sq;

        scaltype diff_tx = tx - tx_vals[i2][j];
        scaltype diff_ty = ty - ty_vals[i2][j];
        scaltype diff_tx_sq = diff_tx * diff_tx;
        scaltype diff_ty_sq = diff_ty * diff_ty;

        if ((diff_tx_sq > dtx_sq) || (diff_ty_sq > dty_sq))
          continue;

        point_refs[i][1][j]->CreateRay(point_refs[i2][1][k], tx, ty, dtx, dty);

        if (i3 > -1)
          use_points[i3][k] = true;
      }
    }
  }
}

#endif//LXSIMDIZE

///////////////////////////////////////////////////////////////////////////////////////////////////
// Global declarations
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef CLUSTER_MODE
static scaltype squareRoot2 = sqrt(2.0);
static scaltype errorXcoeff = 4.0;
static scaltype errorYcoeff = 4.0;
static scaltype errorTxCoeff = 4.0;
static scaltype errorTyCoeff = 4.0;
#else//CLUSTER_MODE
static scaltype errorXcoeff = 4.0;
static scaltype errorYcoeff = 4.0;
#endif//CLUSTER_MODE

static scaltype errorTxBreakCoeff = 4.0;
static scaltype errorTyBreakCoeff = 4.0;

// These ..Ext.. coefficients are applied to squares of uncertainties. So they can be bigger than the previous.
//static scaltype errorExtXcoeff = 16;
//static scaltype errorExtYcoeff = 16;
//static scaltype errorExtTxCoeff = 16;
//static scaltype errorExtTyCoeff = 16;

//static LxFinder* finderInstance = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LxTrackCandidate
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct LxTrackCandidate
{
  LxRay* rays[LXSTATIONS - 1];// Rays are stored left to right.
  Int_t length;
  scaltype chi2;

  LxTrackCandidate(LxRay** rr, Int_t len, scaltype c2) : length(len), chi2(c2)
  {
    for (int i = 0; i < len; ++i)
      rays[i] = rr[i];

    for (int i = len; i < LXSTATIONS - 1; ++i)
      rays[i] = 0;
  }
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LxTrack
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef CLUSTER_MODE
LxTrack::LxTrack(LxTrackCandidate* tc) : matched(false), length(tc->length), chi2(tc->chi2), mcTrack(0),
    aX(0), bX(0), aY(0), bY(0), restoredPoints(0), externalTrack(0)
#ifdef USE_KALMAN_FIT
    , x(0), y(0), z(0), dx(0), dy(0), tx(0), ty(0), dtx(0), dty(0)
#endif//USE_KALMAN_FIT
{
  memset(rays, 0, sizeof(rays));
  memset(points, 0, sizeof(points));

  for (Int_t i = 0; i < length; ++i)
  {
    LxRay* ray = tc->rays[i];
    rays[i] = ray;
    LxPoint* point = ray->source;
    point->used = true;
    point->track = this;

    for (list<LxPoint*>::iterator j = ray->clusterPoints.begin(); j != ray->clusterPoints.end(); ++j)
    {
      LxPoint* p = *j;
      p->track = this;
    }
  }

  LxRay* ray = tc->rays[0];
  LxPoint* point = ray->end;
  point->used = true;
  point->track = this;
}
#else//CLUSTER_MODE
LxTrack::LxTrack(LxTrackCandidate* tc) 
  : externalTrack(NULL) 
#ifdef LX_EXT_LINK_SOPH
  , extTrackCandidates()
#else//LX_EXT_LINK_SOPH
  , extLinkChi2()     
#endif
  , matched(false)
  , mcTrack(NULL)
#ifdef CALC_LINK_WITH_STS_EFF
  , mcTracks()
#endif//CALC_LINK_WITH_STS_EFF
  , length(tc->length), rays(), points(), chi2(tc->chi2)
  ,  aX(0), bX(0), aY(0), bY(0), restoredPoints(0)
#ifdef USE_KALMAN_FIT
    , x(0), y(0), z(0), dx(0), dy(0), tx(0), ty(0), dtx(0), dty(0)
#endif//USE_KALMAN_FIT
  , clone(false), distanceOk(false), oppCharged(false), triggering(false)
{
  memset(rays, 0, sizeof(rays));
  memset(points, 0, sizeof(points));

  for (int i = 0; i < length; ++i)
  {
    LxRay* ray = tc->rays[i];
    rays[i] = ray;
    LxPoint* point = ray->source;
    point->used = true;
    point->track = this;
    points[(i + 1) * LXLAYERS + LXMIDDLE] = point;

    if (point->artificial)
      ++restoredPoints;

    LxStation* station = ray->station;

    for (int j = 0; j < LXLAYERS; ++j)
    {
      if (LXMIDDLE == j)
        continue;

      LxLayer* layer = station->layers[j];
      LxPoint* p = layer->PickNearestPoint(ray);

      if (0 != p && !p->used)
      {
        p->used = true;
        p->track = this;
        points[(i + 1) * LXLAYERS + j] = p;
      }
    }
  }

  LxRay* ray = tc->rays[0];
  LxPoint* point = ray->end;
  point->used = true;
  point->track = this;
  points[LXMIDDLE] = point;

  if (point->artificial)
    ++restoredPoints;

  LxStation* station = ray->station->space->stations[0];

  for (int i = 0; i < LXLAYERS; ++i)
  {
    if (LXMIDDLE == i)
      continue;

    LxLayer* layer = station->layers[i];
    scaltype diffZ = layer->zCoord - ray->end->z;
    scaltype x = ray->end->x + ray->tx * diffZ;
    scaltype y = ray->end->y + ray->ty * diffZ;
    LxPoint* p = layer->PickNearestPoint(x, y);

    if (0 != p && !p->used)
    {
      p->used = true;
      p->track = this;
      points[i] = p;
    }
  }

  //cout << "This track contains: " << restoredPoints << " restored points" << endl;
}
#endif//CLUSTER_MODE

static inline void Ask()
{
  char symbol;

  cout << "ask>";

  do
  {
    cin.get(symbol);
  } while (symbol != '\n');

  cout << endl;
}

#ifdef USE_KALMAN_FIT
struct KFParams
{
  scaltype coord, tg, C11, C12, C21, C22;
};

void LxTrack::Fit()
{
  LxRay* firstRay = rays[LXSTATIONS - LXFIRSTSTATION - 2];
  LxPoint* firstPoint = firstRay->source;
  KFParams params[2] = {
      { firstPoint->x, firstRay->tx, firstPoint->dx, 0, 0, 1.0 },
      { firstPoint->y, firstRay->ty, firstPoint->dy, 0, 0, 1.0 }
  };

  LxPoint* prevPoint = firstPoint;
  scaltype aChi2 = 0;

  for (Int_t i = LXSTATIONS - LXFIRSTSTATION - 2; i >= 0; --i)
  {
    LxRay* ray = rays[i];
    LxPoint* point = ray->end;
    LxStation* station = point->layer->station;
    scaltype m[2] = { point->x, point->y };
    scaltype V[2] = { point->dx * point->dx, point->dy * point->dy };
    KFParams pPrev[2] = { params[0], params[1] };
    scaltype deltaZ = point->z - prevPoint->z;
    scaltype deltaZ2 = deltaZ * deltaZ;

    for (Int_t k = 0; k <= 1; ++k)
    {
      // Extrapolate.
      params[k].coord += pPrev[k].tg * deltaZ;// params[k].tg is unchanged.

      // Filter.
      params[k].C11 += pPrev[k].C12 * deltaZ + pPrev[k].C21 * deltaZ + pPrev[k].C22 * deltaZ2 + station->MSNoise[k][0][0];
      params[k].C12 += pPrev[k].C22 * deltaZ + station->MSNoise[k][0][1];
      params[k].C21 += pPrev[k].C22 * deltaZ + station->MSNoise[k][1][0];
      params[k].C22 += station->MSNoise[k][1][1];

      scaltype S = 1.0 / (V[k] + params[k].C11);
      scaltype Kcoord = params[k].C11 * S;
      scaltype Ktg = params[k].C21 * S;
      scaltype dzeta = m[k] - params[k].coord;
      params[k].coord += Kcoord * dzeta;
      params[k].tg += Ktg * dzeta;
      params[k].C21 -= params[k].C11 * Ktg;
      params[k].C22 -= params[k].C12 * Ktg;
      params[k].C11 *= 1.0 - Kcoord;
      params[k].C12 *= 1.0 - Kcoord;
      aChi2 += dzeta * S * dzeta;
    }

    prevPoint = point;
  }

  x = params[0].coord;
  dx = sqrt(params[0].C11);
  tx = params[0].tg;
  dtx = sqrt(params[0].C22);
  y = params[1].coord;
  dy = sqrt(params[1].C11);
  ty = params[1].tg;
  dty = sqrt(params[1].C22);
  z = rays[0]->end->z;
}
#else//USE_KALMAN_FIT
void LxTrack::Fit()
{
  scaltype sumZ = 0;
  scaltype sumZ2 = 0;
  scaltype sumX = 0;
  scaltype sumZX = 0;
  scaltype sumY = 0;
  scaltype sumZY = 0;

  for (int i = 0; i < LXSTATIONS - LXFIRSTSTATION; ++i)
  {
    LxPoint* point = points[LXLAYERS * i + LXMIDDLE];
    scaltype z = point->z;
    scaltype x = point->x;
    scaltype y = point->y;

    sumZ += z;
    sumZ2 += z * z;
    sumX += x;
    sumZX += z * x;
    sumY += y;
    sumZY += z * y;
  }

  aX = (LXSTATIONS * sumZX - sumZ * sumX) / (LXSTATIONS * sumZ2 - sumZ * sumZ);
  bX = (sumX - aX * sumZ) / LXSTATIONS;
  aY = (LXSTATIONS * sumZY - sumZ * sumY) / (LXSTATIONS * sumZ2 - sumZ * sumZ);
  bY = (sumY - aY * sumZ) / LXSTATIONS;

  //LxPoint* point = points[LXLAYERS * (LXSTATIONS - 1) + LXMIDDLE];
  //cout << "bX = " << bX << " , aX = " << aX << " , X / Z = " << point->x / point->z << endl;
  //cout << "bY = " << bY << " , aY = " << aY << " , Y / Z = " << point->y / point->z << endl;
  //Ask();
}
#endif//USE_KALMAN_FIT

#ifdef LX_EXT_LINK_SOPH
void LxTrack::Rebind()
{
  externalTrack = 0;

  for (list<pair<LxExtTrack*, scaltype> >::iterator i = extTrackCandidates.begin(); i != extTrackCandidates.end(); ++i)
  {
    LxExtTrack* extTrack = i->first;
    scaltype aChi2 = i->second;

    if (0 == extTrack->recoTrack.first)
    {
      extTrack->recoTrack.first = this;
      extTrack->recoTrack.second = aChi2;
      externalTrack = extTrack;
      break;
    }
    else if (aChi2 < extTrack->recoTrack.second)
    {
      LxTrack* anotherTrack = extTrack->recoTrack.first;
      extTrack->recoTrack.first = this;
      extTrack->recoTrack.second = aChi2;
      externalTrack = extTrack;
      anotherTrack->Rebind();
      break;
    }
  }
}
#endif// LX_EXT_LINK_SOPH

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LxPoint
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LxPoint::~LxPoint()
{
  for (list<LxRay*>::iterator i = rays.begin(); i != rays.end(); ++i)
    delete *i;
}

void LxPoint::CreateRay(LxPoint* lPoint, scaltype tx, scaltype ty, scaltype dtx, scaltype dty)
{
  rays.push_back(new LxRay(this, lPoint, tx, ty, dtx, dty
#ifdef CLUSTER_MODE
      , 4
#endif//CLUSTER_MODE
      ));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LxRay
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LxRay::LxRay(LxPoint* s, LxPoint* e
#ifdef CLUSTER_MODE
    , Int_t l
#endif//CLUSTER_MODE
    ) 
 : tx((e->x - s->x) / (e->z - s->z)), ty((e->y - s->y) / (e->z - s->z)),
   dtx(sqrt(e->dx * e->dx + s->dx * s->dx) / (s->z - e->z)),
   dty(sqrt(e->dy * e->dy + s->dy * s->dy) / (s->z - e->z)), 
   source(s), end(e), 
   station(s->layer->station), neighbours()
#ifdef CLUSTER_MODE
   , level(l), used(false), neighbourhood(), clusterPoints()
#endif//CLUSTER_MODE
#ifdef USE_KALMAN
   , kalman()
#endif
{
}

LxRay::LxRay(LxPoint* s, LxPoint* e, scaltype Tx, scaltype Ty, scaltype Dtx, scaltype Dty
#ifdef CLUSTER_MODE
    , Int_t l
#endif//CLUSTER_MODE
    ) 
 : tx(Tx), ty(Ty), dtx(Dtx), dty(Dty),  
   source(s), end(e), station(s->layer->station), neighbours()
#ifdef CLUSTER_MODE
   , level(l), used(false), neighbourhood(), clusterPoints()
#endif//CLUSTER_MODE
#ifdef USE_KALMAN
   , kalman()           
#endif

{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LxLayer
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LxLayer::LxLayer(LxStation* st, int lNum) : station(st),
    layerNumber(lNum), zCoord(0)
{
}

LxLayer::~LxLayer()
{
  Clear();
}

void LxLayer::Clear()
{
  for (list<LxPoint*>::iterator i = points.begin(); i != points.end(); ++i)
    delete *i;

  points.clear();
}

LxPoint* LxLayer::PickNearestPoint(scaltype x, scaltype y)
{
  LxPoint* result = 0;
  scaltype minR2 = 0;

  for (list<LxPoint*>::iterator i = points.begin(); i != points.end(); ++i)
  {
    LxPoint* point = *i;

    if (point->used)
      continue;

    scaltype diffX = point->x - x;
    scaltype diffY = point->y - y;

    if (diffX < 0)
      diffX = -diffX;

    if (diffX > errorXcoeff * point->dx)
      continue;

    if (diffY < 0)
      diffY = -diffY;

    if (diffY > errorYcoeff * point->dy)
      continue;

    scaltype r2 = diffX * diffX + diffY * diffY;

    if (0 == result || r2 < minR2)
    {
      result = point;
      minR2 = r2;
    }
  }

  return result;
}

LxPoint* LxLayer::PickNearestPoint(LxRay* ray)
{
  LxPoint* point = ray->source;
  scaltype diffZ = zCoord - point->z;
  scaltype x = point->x + ray->tx * diffZ;
  scaltype y = point->y + ray->ty * diffZ;
  return PickNearestPoint(x, y);
}

LxPoint* LxLayer::PickNearestPoint(scaltype x, scaltype y, scaltype deltaX, scaltype deltaY)
{
  LxPoint* result = 0;
  scaltype minR2 = 0;
  scaltype xLBound = x - deltaX;
  scaltype xUBound = x + deltaX;
  scaltype yLBound = y - deltaY;
  scaltype yUBound = y + deltaY;

  for (list<LxPoint*>::iterator i = points.begin(); i != points.end(); ++i)
  {
    LxPoint* point = *i;

    if (point->x < xLBound || point->x > xUBound || point->y < yLBound || point->y > yUBound)
      continue;

    scaltype diffX = point->x - x;
    scaltype diffY = point->y - y;
    scaltype r2 = diffX * diffX + diffY * diffY;

    if (0 == result || r2 < minR2)
    {
      result = point;
      minR2 = r2;
    }
  }

  return result;
}

bool LxLayer::HasPointInRange(scaltype x, scaltype y, scaltype deltaX, scaltype deltaY)
{
  scaltype xLBound = x - deltaX;
  scaltype xUBound = x + deltaX;
  scaltype yLBound = y - deltaY;
  scaltype yUBound = y + deltaY;

  for (list<LxPoint*>::iterator i = points.begin(); i != points.end(); ++i)
  {
    LxPoint* point = *i;

    if (xLBound < point->x && point->x < xUBound && yLBound < point->y && point->y < yUBound)
      return true;
  }

  return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LxStation
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// These arrays are filled with data obtained during Monte Carlo simulations.
//static scaltype txLimits[] = { 0., 0.13, 0.11, 0.11, 0.125, 0.17 };
//static scaltype tyLimits[] = { 0., 0.125, 0.07, 0.07, 0.1, 0.21 };

//static scaltype txBreakLimits[] = { 0., 0.085, 0.09, 0.13, 0.21, 0. };
//static scaltype tyBreakLimits[] = { 0., 0.07, 0.085, 0.11, 0.23, 0. };

//static scaltype txBreakSigmas[] = { 0., 0.01242, 0.0115, 0.0134, 0.02955, 0. };
//static scaltype tyBreakSigmas[] = { 0., 0.01473, 0.01174, 0.0136, 0.02827, 0. };

static scaltype dispersions01XSmall[] = { 0.6, 1.0, 0.8, 0.8, 1.2, 1.2 };
static scaltype dispersions01XBig[] = { 1.9, 1.8, 1.6, 1.6, 1.9, 2.9 };
static scaltype dispersions01YSmall[] = { 0.4, 0.5, 0.7, 0.7, 1.0, 1.0 };
static scaltype dispersions01YBig[] = { 1.2, 1.3, 1.3, 1.3, 1.6, 2.9 };

static scaltype dispersions02XSmall[] = { 0.7, 1.2, 1.2, 1.2, 1.1, 1.4 };
static scaltype dispersions02XBig[] = { 2.0, 1.9, 1.6, 1.7, 1.8, 2.3 };
static scaltype dispersions02YSmall[] = { 0.2, 0.6, 1.0, 0.9, 0.9, 1.4 };
static scaltype dispersions02YBig[] = { 1.5, 1.3, 1.3, 1.4, 1.7, 2.2 };

#ifdef CLUSTER_MODE
struct KDRayWrap
{
  LxRay* ray;
  scaltype data[4];
  static bool destroyRays;

  KDRayWrap(scaltype x, scaltype y, LxRay* r) : ray(r)
  {
    data[0] = x;
    data[1] = y;
    data[2] = ray->tx;
    data[3] = ray->ty;
  }

  KDRayWrap(scaltype x, scaltype y, scaltype tx, scaltype ty) : ray(0)// This constructor is used when setting search-range bounds.
  {
    data[0] = x;
    data[1] = y;
    data[2] = tx;
    data[3] = ty;
  }

  ~KDRayWrap()
  {
    //if (destroyRays)
      //delete ray;
  }

  // Stuff required by libkdtree++
  typedef scaltype value_type;

  value_type operator[] (size_t n) const
  {
    return data[n];
  }
};

bool KDRayWrap::destroyRays = false;

typedef KDTree::KDTree<4, KDRayWrap> KDRaysStorageType;
#endif//CLUSTER_MODE

LxStation::LxStation(LxSpace* sp, int stNum) : space(sp), stationNumber(stNum), zCoord(0), txLimit(0), tyLimit(0),
    txBreakLimit(0), tyBreakLimit(0), txBreakSigma(0), tyBreakSigma(0), disp01XSmall(dispersions01XSmall[stNum]),
    disp01XBig(dispersions01XBig[stNum]), disp01YSmall(dispersions01YSmall[stNum]), disp01YBig(dispersions01YBig[stNum]),
    disp02XSmall(dispersions02XSmall[stNum]), disp02XBig(dispersions02XBig[stNum]), disp02YSmall(dispersions02YSmall[stNum]),
    disp02YBig(dispersions02YBig[stNum])
#ifdef CLUSTER_MODE
    ,raysHandle(0), clusteredRaysHandle(0), clusterXLimit(0), clusterXLimit2(0), clusterYLimit(0), clusterYLimit2(0),
    clusterTxLimit(0), clusterTxLimit2(0), clusterTyLimit(0), clusterTyLimit2(0)
#endif//CLUSTER_MODE
{
  for (int i = 0; i < LXLAYERS; ++i)
    layers.push_back(new LxLayer(this, i));

#ifdef CLUSTER_MODE
  raysHandle = new KDRaysStorageType;
  clusteredRaysHandle = new KDRaysStorageType;
#endif//CLUSTER_MODE
}

LxStation::~LxStation()
{
  Clear();
#ifdef CLUSTER_MODE
  KDRaysStorageType* rays = static_cast<KDRaysStorageType*> (raysHandle);
  delete rays;
  KDRaysStorageType* clusteredRays = static_cast<KDRaysStorageType*> (clusteredRaysHandle);
  delete clusteredRays;
#endif//CLUSTER_MODE
}

void LxStation::Clear()
{
  for (vector<LxLayer*>::iterator i = layers.begin(); i != layers.end(); ++i)
    (*i)->Clear();

#ifdef CLUSTER_MODE
  KDRaysStorageType* rays = static_cast<KDRaysStorageType*> (raysHandle);
  KDRayWrap::destroyRays = true;
  rays->clear();
  KDRaysStorageType* clusteredRays = static_cast<KDRaysStorageType*> (clusteredRaysHandle);
  clusteredRays->clear();
  KDRayWrap::destroyRays = false;

  for (list<LxPoint*>::iterator i = clusteredPoints.begin(); i != clusteredPoints.end(); ++i)
    delete *i;

  clusteredPoints.clear();

  for (Int_t i = 0; i < 2 * LXLAYERS; ++i)
  {
    for (vector<list<LxRay*>*>::iterator j = clusters[i].begin(); j != clusters[i].end(); ++j)
      delete *j;

    clusters[i].clear();
  }
#endif//CLUSTER_MODE
}

void LxStation::RestoreMiddlePoints()
{
  LxLayer* lLayer = layers[0];
  LxLayer* mLayer = layers[LXMIDDLE];
  LxLayer* rLayer = layers[2];
  list<LxPoint*>& mPoints = mLayer->points;
  list<LxPoint*>& rPoints = rLayer->points;

  // 1. Loop through the points of the middle layer and check if they have at least on corresponding point on either left or right layer.
  for (list<LxPoint*>::iterator i = mPoints.begin(); i != mPoints.end(); ++i)
  {
    LxPoint* point = *i;
    scaltype tx = point->x / point->z;
    scaltype ty = point->y / point->z;
    scaltype diffZ = lLayer->zCoord - point->z;
    scaltype x = point->x + tx * diffZ;
    scaltype y = point->y + ty * diffZ;

    if (!lLayer->HasPointInRange(x, y, disp01XBig, disp01YBig))
    {
      diffZ = rLayer->zCoord - point->z;
      x = point->x + tx * diffZ;
      y = point->y + tx * diffZ;

      if (!rLayer->HasPointInRange(x, y, disp01XBig, disp01YBig))
        point->valid = false;
    }
  }

  // 2. Loop through the points of the right station. If there are corresponding points in the middle station -- it is OK.
  //    If there are no -- check corresponding points on the left station. If such points exist, choose the point nearest
  //    to the predicted value among them and reconstruct a middle point as a point of intersection of the segment between
  //    the left and corresponding right points and the middle layer.
  for (list<LxPoint*>::iterator i = rPoints.begin(); i != rPoints.end(); ++i)
  {
    LxPoint* rPoint = *i;
    scaltype tx = rPoint->x / rPoint->z;
    scaltype ty = rPoint->y / rPoint->z;
    scaltype diffZ = mLayer->zCoord - rPoint->z;
    scaltype x = rPoint->x + tx * diffZ;
    scaltype y = rPoint->y + ty * diffZ;

    if (mLayer->HasPointInRange(x, y, disp01XBig, disp01YBig))
      continue;

    diffZ = lLayer->zCoord - rPoint->z;
    x = rPoint->x + tx * diffZ;
    y = rPoint->y + ty * diffZ;

    LxPoint* lPoint = lLayer->PickNearestPoint(x, y, disp02XSmall, disp02YSmall);

    if (0 == lPoint)
      continue;

    x = (lPoint->x + rPoint->x) / 2;
    y = (lPoint->y + rPoint->y) / 2;
    mLayer->AddPoint(-1, x, y, mLayer->zCoord, rPoint->dx, rPoint->dy, rPoint->dz, true);
  }
}

void LxStation::BuildRays()
{
  if (stationNumber < LXFIRSTSTATION + 1)
    return;

  LxLayer* rLayer = layers[LXMIDDLE];
  LxStation* lStation = space->stations[stationNumber - 1];// Pointer to 'left' station. 'This' station is 'right'.
  LxLayer* lLayer = lStation->layers[LXMIDDLE];

  for (list<LxPoint*>::iterator rIter = rLayer->points.begin(); rIter != rLayer->points.end(); ++rIter)
  {
    LxPoint* rPoint = *rIter;

    if (!rPoint->valid)
      continue;

    scaltype tx1 = rPoint->x / rPoint->z;
    scaltype ty1 = rPoint->y / rPoint->z;

    for (list<LxPoint*>::iterator lIter = lLayer->points.begin(); lIter != lLayer->points.end(); ++lIter)
    {
      LxPoint* lPoint = *lIter;

      if (!lPoint->valid)
        continue;

      scaltype diffZ = lPoint->z - rPoint->z;
      scaltype tx = (lPoint->x - rPoint->x) / diffZ;
      scaltype ty = (lPoint->y - rPoint->y) / diffZ;
      scaltype dtx = -sqrt(lPoint->dx * lPoint->dx + rPoint->dx * rPoint->dx) / diffZ;
      scaltype dty = -sqrt(lPoint->dy * lPoint->dy + rPoint->dy * rPoint->dy) / diffZ;
      scaltype diffTx = tx - tx1;
      scaltype diffTy = ty - ty1;

      if (diffTx < 0)
        diffTx = -diffTx;

      if (diffTy < 0)
        diffTy = -diffTy;

      if (diffTx > txLimit + dtx * errorTxCoeff || diffTy > tyLimit + dty * errorTyCoeff)
        continue;

      rPoint->CreateRay(lPoint, tx, ty, dtx, dty);
    }
  }
}

#ifdef CLUSTER_MODE
static void AddRayData(LxRay* ray, scaltype& lX, scaltype& lY, scaltype& lDx2, scaltype& lDy2,
    scaltype& rX, scaltype& rY, scaltype& rDx2, scaltype& rDy2)
{
  LxPoint* lPoint = ray->end;
  scaltype deltaZ = lPoint->layer->station->zCoord - lPoint->z;
  lX += lPoint->x + ray->tx * deltaZ;
  lY += lPoint->y + ray->ty * deltaZ;
  lDx2 += lPoint->dx * lPoint->dx;
  lDy2 += lPoint->dy * lPoint->dy;

  LxPoint* rPoint = ray->source;
  deltaZ = rPoint->layer->station->zCoord - rPoint->z;
  rX += rPoint->x + ray->tx * deltaZ;
  rY += rPoint->y + ray->ty * deltaZ;
  rDx2 += rPoint->dx * rPoint->dx;
  rDy2 += rPoint->dy * rPoint->dy;
}

void LxStation::InsertClusterRay(Int_t levels, Int_t cardinality, LxRay* clusterRay)
{
  if (levels < (LXLAYERS - 1) * (LXLAYERS - 1))
    return;

  levels -= (LXLAYERS - 1) * (LXLAYERS - 1);

  while (clusters[levels].size() < cardinality)
    clusters[levels].push_back(new list<LxRay*>);

  clusters[levels][cardinality - 1]->push_back(clusterRay);
}

#ifdef BEST_SIX_POINTS
struct LxLess
{
  bool operator() (pair<LxPoint*, LxPoint*> lVal, pair<LxPoint*, LxPoint*> rVal) const
  {
    if (lVal.first < rVal.first)
      return true;

    return lVal.second < rVal.second;
  }
};

struct LxRaysCandidates
{
  LxRay* data[LXLAYERS * LXLAYERS];
  scaltype chi2;

  LxRaysCandidates() : chi2(-1.0)
  {
    memset(data, 0, sizeof(data));
  }
};
#endif//BEST_SIX_POINTS

void LxStation::BuildRays2()
{
  if (stationNumber < LXFIRSTSTATION + 1)
      return;

  KDRaysStorageType* rays = static_cast<KDRaysStorageType*> (raysHandle);
  LxStation* lStation = space->stations[stationNumber - 1];// Pointer to 'left' station. 'This' station is 'right'.

  for (Int_t i = 0; i < LXLAYERS; ++i)
  {
    LxLayer* rLayer = layers[i];

    for (Int_t j = 0; j < LXLAYERS; ++j)
    {
      LxLayer* lLayer = lStation->layers[j];

      for (list<LxPoint*>::iterator rIter = rLayer->points.begin(); rIter != rLayer->points.end(); ++rIter)
      {
        LxPoint* rPoint = *rIter;
        scaltype tx1 = rPoint->x / rPoint->z;
        scaltype ty1 = rPoint->y / rPoint->z;
        scaltype zShift = zCoord - rPoint->z;

        for (list<LxPoint*>::iterator lIter = lLayer->points.begin(); lIter != lLayer->points.end(); ++lIter)
        {
          LxPoint* lPoint = *lIter;
          scaltype diffZ = lPoint->z - rPoint->z;
          scaltype tx = (lPoint->x - rPoint->x) / diffZ;
          scaltype ty = (lPoint->y - rPoint->y) / diffZ;
          scaltype dtx = -sqrt(lPoint->dx * lPoint->dx + rPoint->dx * rPoint->dx) / diffZ;
          scaltype dty = -sqrt(lPoint->dy * lPoint->dy + rPoint->dy * rPoint->dy) / diffZ;
          scaltype diffTx = abs(tx - tx1);
          scaltype diffTy = abs(ty - ty1);

          if (diffTx > txLimit + dtx * errorTxCoeff || diffTy > tyLimit + dty * errorTyCoeff)
            continue;

          scaltype x = rPoint->x + tx * zShift;
          scaltype y = rPoint->y + ty * zShift;
          LxRay* ray = new LxRay(rPoint, lPoint, LXLAYERS * i + j);
          KDRayWrap rayWrap(x, y, ray);
          rays->insert(rayWrap);
          rPoint->rays.push_back(ray);
        }
      }
    }
  }

  timeval bTime, eTime;
  Int_t exeDuration;
  static Int_t maxExeDuration = 0;
  static Int_t goodClusters = 0;
  gettimeofday(&bTime, 0);

  for (KDRaysStorageType::iterator i = rays->begin(); i != rays->end(); ++i)
  {
    KDRayWrap& wrap = const_cast<KDRayWrap&> (*i);
    LxRay* ray = wrap.ray;
    KDTree::_Region<4, KDRayWrap, scaltype, KDTree::_Bracket_accessor<KDRayWrap>, std::less<KDTree::_Bracket_accessor<KDRayWrap>::result_type> > range(wrap);
    scaltype limitX = errorXcoeff * sqrt(clusterXLimit2 + 2.0 * ray->source->dx * ray->source->dx);
    scaltype limitY = errorYcoeff * sqrt(clusterYLimit2 + 2.0 * ray->source->dy * ray->source->dy);
    scaltype limitTx = errorTxCoeff * sqrt(clusterTxLimit2 + 2.0 * ray->dtx * ray->dtx);
    scaltype limitTy = errorTyCoeff * sqrt(clusterTyLimit2 + 2.0 * ray->dty * ray->dty);
    KDRayWrap boundsWrap(wrap.data[0] - limitX, wrap.data[1] - limitY, wrap.data[2] - limitTx, wrap.data[3] - limitTy);
    range.set_low_bound(boundsWrap, 0);
    range.set_low_bound(boundsWrap, 1);
    range.set_low_bound(boundsWrap, 2);
    range.set_low_bound(boundsWrap, 3);
    boundsWrap.data[0] = wrap.data[0] + limitX;
    boundsWrap.data[1] = wrap.data[1] + limitY;
    boundsWrap.data[2] = wrap.data[2] + limitTx;
    boundsWrap.data[3] = wrap.data[3] + limitTy;
    range.set_high_bound(boundsWrap, 0);
    range.set_high_bound(boundsWrap, 1);
    range.set_high_bound(boundsWrap, 2);
    range.set_high_bound(boundsWrap, 3);
    list<KDRayWrap> neighbours;
    rays->find_within_range(range, back_insert_iterator<list<KDRayWrap> > (neighbours));
    Int_t level_occupancy[LXLAYERS * LXLAYERS] = {};

    for (list<KDRayWrap>::iterator j = neighbours.begin(); j != neighbours.end(); ++j)
    {
      KDRayWrap& w = *j;
      LxRay* r = w.ray;
      level_occupancy[r->level] = 1;

      if (ray != r)
        ray->neighbourhood.push_back(r);
    }

    Int_t levels = 0;

    for (Int_t j = 0; j < LXLAYERS * LXLAYERS; ++j)
      levels += level_occupancy[j];

    InsertClusterRay(levels, ray->neighbourhood.size() + 1, ray);
  }// for (KDRaysStorageType::iterator i = rays->begin(); i != rays->end(); ++i)

  KDRaysStorageType* clusteredRays = static_cast<KDRaysStorageType*> (clusteredRaysHandle);

  for (Int_t i = 2 * LXLAYERS - 1; i >= 0; --i)
  {
#ifdef DENSE_CLUSTERS_FIRST
    for (Int_t j = clusters[i].size() - 1; j >= 0; --j)
#else//DENSE_CLUSTERS_FIRST
    for (Int_t j = 0; j < clusters[i].size(); ++j)
#endif//DENSE_CLUSTERS_FIRST
    {
      list<LxRay*>* clusterRays = clusters[i][j];

      for (list<LxRay*>::iterator k = clusterRays->begin(); k != clusterRays->end(); ++k)
      {
        LxRay* ray = *k;

        if (0 == ray)
          continue;

#ifdef BEST_SIX_POINTS
        if (ray->source->used || ray->end->used)
        //if (ray->used)
          continue;
#else//BEST_SIX_POINTS
        if (ray->used)
          continue;
#endif//BEST_SIX_POINTS

        Int_t levels = 0;
        Int_t level_occupancy[LXLAYERS * LXLAYERS] = {};
        scaltype lX = 0;
        scaltype lY = 0;
        scaltype lDx2 = 0;
        scaltype lDy2 = 0;
        scaltype lZ = ray->end->layer->station->zCoord;
        scaltype rX = 0;
        scaltype rY = 0;
        scaltype rDx2 = 0;
        scaltype rDy2 = 0;
        scaltype rZ = ray->source->layer->station->zCoord;
        scaltype deltaZ = rZ - lZ;
        AddRayData(ray, lX, lY, lDx2, lDy2, rX, rY, rDx2, rDy2);
        level_occupancy[ray->level] = 1;
        Int_t numRays = 1;

#ifdef BEST_RAYS_ONLY
        pair<LxRay*, scaltype> bestNeighbours[LXLAYERS * LXLAYERS];// Neigbours and chi2 to the cluster owner.

        for (Int_t l = 0; l < LXLAYERS * LXLAYERS; ++l)
        {
          bestNeighbours[l].first = 0;
          bestNeighbours[l].second = 0;
        }

#ifdef BEST_SIX_POINTS
        set<LxPoint*> leftBestPoints[LXLAYERS];
        set<LxPoint*> rightBestPoints[LXLAYERS];
        map<pair<LxPoint*, LxPoint*>, LxRay*, LxLess> bestRaysMap[LXLAYERS * LXLAYERS];
        LxPoint* rayLeftPoint = ray->end;
        Int_t rayLeftInd = rayLeftPoint->layer->layerNumber;
        leftBestPoints[rayLeftInd].insert(rayLeftPoint);
        LxPoint* rayRightPoint = ray->source;
        Int_t rayRightInd = rayRightPoint->layer->layerNumber;
        rightBestPoints[rayRightInd].insert(rayRightPoint);
        bestRaysMap[ray->level][make_pair(rayLeftPoint, rayRightPoint)] = ray;

        for (list<LxRay*>::iterator l = ray->neighbourhood.begin(); l != ray->neighbourhood.end(); ++l)
        {
          LxRay* r = *l;

          /*if (r->used)
            continue;

          if (r->level == ray->level)
            continue;*/

          LxPoint* rLeftPoint = r->end;
          Int_t rLeftInd = rLeftPoint->layer->layerNumber;

          if (rLeftPoint->used)
            continue;

          LxPoint* rRightPoint = r->source;
          Int_t rRightInd = rRightPoint->layer->layerNumber;

          if (rRightPoint->used)
            continue;

          if (rLeftInd == rayLeftInd && rRightInd == rayRightInd)
            continue;

          leftBestPoints[rLeftInd].insert(rLeftPoint);
          rightBestPoints[rRightInd].insert(rRightPoint);
          bestRaysMap[r->level][make_pair(rLeftPoint, rRightPoint)] = r;
        }
#endif//BEST_SIX_POINTS

#endif//BEST_RAYS_ONLY

#ifdef BEST_SIX_POINTS
        LxRaysCandidates bestRaysCandidates;

        for (set<LxPoint*>::iterator r0 = rightBestPoints[0].begin(); r0 != rightBestPoints[0].end(); ++r0)
        {
          for (set<LxPoint*>::iterator r1 = rightBestPoints[1].begin(); r1 != rightBestPoints[1].end(); ++r1)
          {
            for (set<LxPoint*>::iterator r2 = rightBestPoints[2].begin(); r2 != rightBestPoints[2].end(); ++r2)
            {
              for (set<LxPoint*>::iterator l0 = leftBestPoints[0].begin(); l0 != leftBestPoints[0].end(); ++l0)
              {
                for (set<LxPoint*>::iterator l1 = leftBestPoints[1].begin(); l1 != leftBestPoints[1].end(); ++l1)
                {
                  for (set<LxPoint*>::iterator l2 = leftBestPoints[2].begin(); l2 != leftBestPoints[2].end(); ++l2)
                  {
                    LxRaysCandidates cand;

                    for (Int_t rInd = 0; rInd < LXLAYERS; ++rInd)
                    {
                      LxPoint* rPoint = 0;

                      switch(rInd)
                      {
                      case 0:
                        rPoint = *r0;
                        break;

                      case 1:
                        rPoint = *r1;
                        break;

                      default:
                        rPoint = *r2;
                      }

                      for (Int_t lInd = 0; lInd < LXLAYERS; ++lInd)
                      {
                        LxPoint* lPoint = 0;

                        switch(lInd)
                        {
                        case 0:
                          lPoint = *l0;
                          break;

                        case 1:
                          lPoint = *l1;
                          break;

                        default:
                          lPoint = *l2;
                        }

                        Int_t level = rPoint->layer->layerNumber * LXLAYERS + lPoint->layer->layerNumber;
                        map<pair<LxPoint*, LxPoint*>, LxRay*, LxLess>::iterator rIter = bestRaysMap[level].find(make_pair(lPoint, rPoint));

                        if (rIter == bestRaysMap[level].end())
                          continue;

                        if (level == ray->level)
                          continue;

                        cand.data[level] = rIter->second;
                      }
                    }

                    Int_t candLevels = 0;
                    cand.chi2 = 0;

                    for (Int_t l = 0; l < LXLAYERS * LXLAYERS; ++l)
                    {
                      if (0 == cand.data[l])
                        continue;

                      ++candLevels;
                      LxRay* r = cand.data[l];
                      scaltype diffTx = r->tx - ray->tx;
                      scaltype diffTy = r->ty - ray->ty;
                      scaltype diffZ = ray->source->z - r->source->z;
                      scaltype diffX = r->source->x + r->tx * diffZ - ray->source->x;
                      scaltype diffY = r->source->y + r->ty * diffZ - ray->source->y;
                      cand.chi2 += diffTx * diffTx / (ray->dtx * ray->dtx) + diffTy * diffTy / (ray->dty * ray->dty) +
                          diffX * diffX / (ray->source->dx * ray->source->dx) + diffY * diffY / (ray->source->dy * ray->source->dy);
                    }

                    if (candLevels < LXLAYERS * LXLAYERS - 1)
                      continue;

                    if (bestRaysCandidates.chi2 < 0 || bestRaysCandidates.chi2 > cand.chi2)
                      bestRaysCandidates = cand;
                  }
                }
              }
            }
          }
        }

        if (bestRaysCandidates.chi2 < 0)
          continue;

        levels = LXLAYERS * LXLAYERS;

        for (Int_t l = 0; l < LXLAYERS * LXLAYERS; ++l)
        {
          bestNeighbours[l].first = bestRaysCandidates.data[l];
          bestNeighbours[l].second = 0;
        }

#else//BEST_SIX_POINTS
        for (list<LxRay*>::iterator l = ray->neighbourhood.begin(); l != ray->neighbourhood.end(); ++l)
        {
          LxRay* r = *l;

          if (r->used)
            continue;

#ifdef BEST_RAYS_ONLY
          if (r->level == ray->level)
            continue;
#endif//BEST_RAYS_ONLY

          scaltype diffTx = r->tx - ray->tx;
          scaltype diffTy = r->ty - ray->ty;
          scaltype diffZ = ray->source->z - r->source->z;
          scaltype diffX = r->source->x + r->tx * diffZ - ray->source->x;
          scaltype diffY = r->source->y + r->ty * diffZ - ray->source->y;
          scaltype aChi2 = diffTx * diffTx / (ray->dtx * ray->dtx) + diffTy * diffTy / (ray->dty * ray->dty) +
              diffX * diffX / (ray->source->dx * ray->source->dx) + diffY * diffY / (ray->source->dy * ray->source->dy);

#ifdef BEST_RAYS_ONLY
          if (0 == bestNeighbours[r->level].first || aChi2 < bestNeighbours[r->level].second)
          {
            bestNeighbours[r->level].first = r;
            bestNeighbours[r->level].second = aChi2;
          }
#else//BEST_RAYS_ONLY
          AddRayData(r, lX, lY, lDx2, lDy2, rX, rY, rDx2, rDy2);
#endif//BEST_RAYS_ONLY

          level_occupancy[r->level] = 1;
          ++numRays;
        }

        for (Int_t l = 0; l < LXLAYERS * LXLAYERS; ++l)
          levels += level_occupancy[l];
#endif//BEST_SIX_POINTS

        if (levels < LXLAYERS * LXLAYERS/*(LXLAYERS - 1) * (LXLAYERS - 1)*/)
          continue;
        else if (levels < i + (LXLAYERS - 1) * (LXLAYERS - 1)
#if defined(DENSE_CLUSTERS_FIRST) && !defined(BEST_RAYS_ONLY)
            || numRays < j + 1
#endif//defined(DENSE_CLUSTERS_FIRST) && !defined(BEST_RAYS_ONLY)
            )
        {
          InsertClusterRay(levels, numRays, ray);// It should be safe to leave ray also in its previous place.
          continue;
        }

#ifdef BEST_RAYS_ONLY
        numRays = 1;

        for (Int_t l = 0; l < LXLAYERS * LXLAYERS; ++l)
        {
          LxRay* r = bestNeighbours[l].first;

          if (0 == r)
            continue;

          AddRayData(r, lX, lY, lDx2, lDy2, rX, rY, rDx2, rDy2);
#ifdef BEST_SIX_POINTS
          r->source->used = true;
          r->end->used = true;
#else//BEST_SIX_POINTS
          r->used = true;
#endif//BEST_SIX_POINTS
          ++numRays;
        }
#endif//BEST_RAYS_ONLY

        lX /= numRays;
        lY /= numRays;
        scaltype lDx = ray->end->dx;//sqrt(lDx2) / numRays;
        scaltype lDy = ray->end->dy;//sqrt(lDy2) / numRays;
        rX /= numRays;
        rY /= numRays;
        scaltype rDx = ray->source->dx;//sqrt(rDx2) / numRays;
        scaltype rDy = ray->source->dy;//sqrt(rDy2) / numRays;

        LxPoint* lPoint = new LxPoint(lX, lY, lZ, lDx, lDy, 0, ray->end->layer, -1);
        clusteredPoints.push_back(lPoint);
        LxPoint* rPoint = new LxPoint(rX, rY, rZ, rDx, rDy, 0, ray->source->layer, -1);
        clusteredPoints.push_back(rPoint);
        LxRay* clRay = new LxRay(rPoint, lPoint, LXLAYERS + LXMIDDLE);
        clRay->clusterPoints.push_back(ray->source);
        clRay->clusterPoints.push_back(ray->end);
#ifdef REMEMBER_CLUSTERED_RAYS_IN_POINTS
        ray->source->leftClusteredRay = clRay;
        ray->end->rightClusteredRay = clRay;
#endif// REMEMBER_CLUSTERED_RAYS_IN_POINTS
#ifdef BEST_SIX_POINTS
        ray->source->used = true;
        ray->end->used = true;
        //ray->used = true;
#else//BEST_SIX_POINTS
        ray->used = true;
#endif//BEST_SIX_POINTS

#ifdef BEST_RAYS_ONLY
        for (Int_t l = 0; l < LXLAYERS * LXLAYERS; ++l)
        {
          LxRay* r = bestNeighbours[l].first;

          if (0 == r)
            continue;

          clRay->clusterPoints.push_back(r->source);
          clRay->clusterPoints.push_back(r->end);
#ifdef REMEMBER_CLUSTERED_RAYS_IN_POINTS
          ray->source->leftClusteredRay = clRay;
          ray->end->rightClusteredRay = clRay;
#endif// REMEMBER_CLUSTERED_RAYS_IN_POINTS
        }

#ifdef REMOVE_SUBCLUSTER
        for (list<LxRay*>::iterator l = ray->neighbourhood.begin(); l != ray->neighbourhood.end(); ++l)
        {
          LxRay* r = *l;

#ifdef BEST_SIX_POINTS
          //if (r->source->used || r->end->used)
            //continue;
#else//BEST_SIX_POINTS
          if (r->used)
            continue;
#endif//BEST_SIX_POINTS

          /*scaltype diffTx = r->tx - ray->tx;

          if (diffTx < 0)
            diffTx = -diffTx;

          if (diffTx > errorTxCoeff * ray->dtx * 2)
            continue;

          scaltype diffTy = r->ty - ray->ty;

          if (diffTy < 0)
            diffTy = -diffTy;

          if (diffTy > errorTyCoeff * ray->dty * 2)
            continue;

          scaltype diffZ = ray->source->z - r->source->z;
          scaltype diffX = ray->source->x - r->source->x - r->tx * diffZ;

          if (diffX < 0)
            diffX = -diffX;

          if (diffX > errorXcoeff * ray->source->dx * 2)
            continue;

          scaltype diffY = ray->source->y - r->source->y - r->ty * diffZ;

          if (diffY < 0)
            diffY = -diffY;

          if (diffY > errorYcoeff * ray->source->dy * 2)
            continue;*/

          clRay->clusterPoints.push_back(r->source);
          clRay->clusterPoints.push_back(r->end);
#ifdef BEST_SIX_POINTS
          //r->source->used = true;
          //r->end->used = true;
#else//BEST_SIX_POINTS
          r->used = true;
#endif//BEST_SIX_POINTS
        }
#endif//REMOVE_SUBCLUSTER

#else//BEST_RAYS_ONLY
        for (list<LxRay*>::iterator l = ray->neighbourhood.begin(); l != ray->neighbourhood.end(); ++l)
        {
          LxRay* r = *l;

          if (r->used)
            continue;

          clRay->clusterPoints.push_back(r->source);
          clRay->clusterPoints.push_back(r->end);
          r->used = true;
        }
#endif//BEST_RAYS_ONLY

        KDRayWrap clRayWrap(rX, rY, clRay);
        clusteredRays->insert(clRayWrap);
      }// for (list<LxRay*>::iterator k = rays->begin(); k != rays->end(); ++k)
    }// for (vector<list<LxRay*>*>::reverse_iterator j = clusters[i].rbegin(); j != clusters[i].rend(); ++j)
  }// for (Int_t i = 2 * LXLAYERS - 1; i >= 0; --i)

  gettimeofday(&eTime, 0);
  exeDuration = (eTime.tv_sec - bTime.tv_sec) * 1000000 + eTime.tv_usec - bTime.tv_usec;

  if (exeDuration > maxExeDuration)
    maxExeDuration = exeDuration;

  cout << "Max execution duration was: " << maxExeDuration << endl;
}
#endif//CLUSTER_MODE

void LxStation::ConnectNeighbours()
{
  if (stationNumber < LXFIRSTSTATION + 2)
    return;

  LxLayer* layer = layers[LXMIDDLE];
  LxStation* lStation = space->stations[stationNumber - 1];

  for (list<LxPoint*>::iterator i = layer->points.begin(); i != layer->points.end(); ++i)
  {
    LxPoint* rPoint = *i;

    if (!rPoint->valid)
      continue;

    for (list<LxRay*>::iterator j = rPoint->rays.begin(); j != rPoint->rays.end(); ++j)
    {
      LxRay* rRay = *j;
      LxPoint* ePoint = rRay->end;

      if (!ePoint->valid)
        continue;

      for (list<LxRay*>::iterator k = ePoint->rays.begin(); k != ePoint->rays.end(); ++k)
      {
        LxRay* lRay = *k;
        scaltype diffTx = lRay->tx - rRay->tx;
        scaltype diffTy = lRay->ty - rRay->ty;

        if (diffTx < 0)
          diffTx = -diffTx;

        if (diffTy < 0)
          diffTy = -diffTy;

        scaltype dtxb = sqrt(lRay->dtx * lRay->dtx + rRay->dtx * rRay->dtx);
        scaltype dtyb = sqrt(lRay->dty * lRay->dty + rRay->dty * rRay->dty);

        if (diffTx > lStation->txBreakLimit + dtxb * errorTxBreakCoeff ||
            diffTy > lStation->tyBreakLimit + dtyb * errorTyBreakCoeff)
          continue;

        rRay->neighbours.push_back(lRay);
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// LxSpace
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LxSpace::LxSpace() : muchStsBreakX(0), muchStsBreakY(0), muchStsBreakTx(0), muchStsBreakTy(0), stationsInAlgo(LXSTATIONS)
{
  for (int i = 0; i < LXSTATIONS; ++i)
    stations.push_back(new LxStation(this, i));

  x_coords = reinterpret_cast<scal_coords*> (_mm_malloc(sizeof(scaltype) * LXSTATIONS * LXLAYERS * LXMAXPOINTSONSTATION, 32));
  tx_vals = reinterpret_cast<scal_tans*> (_mm_malloc(sizeof(scaltype) * (LXSTATIONS - 1) * LXMAXPOINTSONSTATION, 32));
  x_errs = reinterpret_cast<scal_coords*> (_mm_malloc(sizeof(scaltype) * LXSTATIONS * LXLAYERS * LXMAXPOINTSONSTATION, 32));
  y_coords = reinterpret_cast<scal_coords*> (_mm_malloc(sizeof(scaltype) * LXSTATIONS * LXLAYERS * LXMAXPOINTSONSTATION, 32));
  ty_vals = reinterpret_cast<scal_tans*> (_mm_malloc(sizeof(scaltype) * (LXSTATIONS - 1) * LXMAXPOINTSONSTATION, 32));
  y_errs = reinterpret_cast<scal_coords*> (_mm_malloc(sizeof(scaltype) * LXSTATIONS * LXLAYERS * LXMAXPOINTSONSTATION, 32));
  z_coords = reinterpret_cast<scal_coords*> (_mm_malloc(sizeof(scaltype) * LXSTATIONS * LXLAYERS * LXMAXPOINTSONSTATION, 32));
}

LxSpace::~LxSpace()
{
  _mm_free(x_coords);
  _mm_free(tx_vals);
  _mm_free(x_errs);
  _mm_free(y_coords);
  _mm_free(ty_vals);
  _mm_free(y_errs);
  _mm_free(z_coords);
}

void LxSpace::Clear()
{
  for (vector<LxStation*>::iterator i = stations.begin(); i != stations.end(); ++i)
    (*i)->Clear();

  for (list<LxTrack*>::iterator i = tracks.begin(); i != tracks.end(); ++i)
    delete *i;

  tracks.clear();
  extTracks.clear();
}

void LxSpace::RestoreMiddlePoints()
{
  for (int i = LXFIRSTSTATION; i < LXSTATIONS; ++i)
    stations[i]->RestoreMiddlePoints();
}

void LxSpace::BuildRays()
{
  for (int i = LXFIRSTSTATION + 1; i < LXSTATIONS; ++i)
    stations[i]->BuildRays();
}

#ifdef CLUSTER_MODE
void LxSpace::BuildRays2()
{
  for (Int_t i = LXSTATIONS - 1; i > LXFIRSTSTATION; --i)
  {
    stations[i]->BuildRays2();

    if (i <= LXFIRSTSTATION + 1)
      continue;

    for (Int_t j = 0; j < LXLAYERS; ++j)
    {
      LxLayer* layer = stations[i - 1]->layers[j];

      for (list<LxPoint*>::iterator k = layer->points.begin(); k != layer->points.end(); ++k)
      {
        LxPoint* point = *k;
        point->used = false;
      }
    }
  }
}

void LxSpace::ConnectNeighbours2()
{
  for (Int_t i = LXSTATIONS - 1; i > LXFIRSTSTATION + 1; --i)
  {
    KDRaysStorageType* leftRays = static_cast<KDRaysStorageType*> (stations[i - 1]->clusteredRaysHandle);
    KDRaysStorageType* rightRays = static_cast<KDRaysStorageType*> (stations[i]->clusteredRaysHandle);
    scaltype txSigma = stations[i - 1]->txBreakSigma;
    scaltype txSigma2 = txSigma * txSigma;
    scaltype tySigma = stations[i - 1]->tyBreakSigma;
    scaltype tySigma2 = tySigma * tySigma;
    scaltype deltaZ = stations[i - 1]->zCoord - stations[i]->zCoord;
    scaltype deltaZ2 = deltaZ * deltaZ;
    scaltype deltaZ23 = 1.0;//deltaZ2 / 3.0;

    for (KDRaysStorageType::iterator j = rightRays->begin(); j != rightRays->end(); ++j)
    {
      KDRayWrap& wrap = const_cast<KDRayWrap&> (*j);
      LxRay* ray = wrap.ray;
      KDTree::_Region<4, KDRayWrap, scaltype, KDTree::_Bracket_accessor<KDRayWrap>,
        std::less<KDTree::_Bracket_accessor<KDRayWrap>::result_type> > range(wrap);
      LxPoint* point = ray->end;
      scaltype x = wrap.data[0] + wrap.data[2] * deltaZ;
      scaltype y = wrap.data[1] + wrap.data[3] * deltaZ;
      scaltype tx = wrap.data[2];
      scaltype ty = wrap.data[3];
      scaltype xRange = 4 * sqrt(2 * point->dx * point->dx + txSigma2 * deltaZ23);
      scaltype yRange = 4 * sqrt(2 * point->dy * point->dy + tySigma2 * deltaZ23);
      scaltype txRange = 4 * sqrt(2 * ray->dtx * ray->dtx + txSigma2);
      scaltype tyRange = 4 * sqrt(2 * ray->dty * ray->dty + tySigma2);
      KDRayWrap boundsWrap(x - xRange, y - yRange, tx - txRange, ty - tyRange);
      range.set_low_bound(boundsWrap, 0);
      range.set_low_bound(boundsWrap, 1);
      range.set_low_bound(boundsWrap, 2);
      range.set_low_bound(boundsWrap, 3);
      boundsWrap.data[0] = x + xRange;
      boundsWrap.data[1] = y + yRange;
      boundsWrap.data[2] = tx + txRange;
      boundsWrap.data[3] = ty + tyRange;
      range.set_high_bound(boundsWrap, 0);
      range.set_high_bound(boundsWrap, 1);
      range.set_high_bound(boundsWrap, 2);
      range.set_high_bound(boundsWrap, 3);
      list<KDRayWrap> neighbours;
      leftRays->find_within_range(range, back_insert_iterator<list<KDRayWrap> > (neighbours));

      for (list<KDRayWrap>::iterator k = neighbours.begin(); k != neighbours.end(); ++k)
      {
        KDRayWrap& w = *k;
        LxRay* r = w.ray;
        ray->neighbours.push_back(r);
      }
    }
  }
}

void LxSpace::BuildCandidates2(LxRay* ray, LxRay** rays, list<LxTrackCandidate*>& candidates, scaltype chi2)
{
  int level = ray->station->stationNumber - LXFIRSTSTATION - 1;
  rays[level] = ray;

  if (0 == level)
  {
    LxTrackCandidate* tc = new LxTrackCandidate(rays, LXSTATIONS - LXFIRSTSTATION - 1, chi2);
    candidates.push_back(tc);
    return;
  }

  LxPoint* point = ray->end;

  for(list<LxRay*>::iterator i = ray->neighbours.begin(); i != ray->neighbours.end(); ++i)
  {
    LxRay* lRay = *i;

    if (lRay->source->used)
      continue;

    LxPoint* lPoint = lRay->source;
    LxStation* station = lRay->station;
    scaltype dx = point->x - lPoint->x;
    scaltype dx2 = dx * dx;
    scaltype sigmaX2 = point->dx * point->dx + lPoint->dx * lPoint->dx;
    scaltype dy = point->y - lPoint->y;
    scaltype dy2 = dy * dy;
    scaltype sigmaY2 = point->dy * point->dy + lPoint->dy * lPoint->dy;
    scaltype dtx = ray->tx - lRay->tx;
    scaltype dtx2 = dtx * dtx;
    scaltype sigmaTx2 = ray->dtx * ray->dtx + lRay->dtx * lRay->dtx + station->txBreakSigma * station->txBreakSigma;
    scaltype dty = ray->ty - lRay->ty;
    scaltype dty2 = dty * dty;
    scaltype sigmaTy2 = ray->dty * ray->dty + lRay->dty * lRay->dty + station->tyBreakSigma * station->tyBreakSigma;

    // Continue process of track building.
    BuildCandidates2(lRay, rays, candidates, chi2 + dx2 / sigmaX2 + dy2 / sigmaY2 + dtx2 / sigmaTx2 + dty2 / sigmaTy2);
  }
}

void LxSpace::Reconstruct2()
{
  LxStation* startStation = stations[LXSTATIONS - 1];
  KDRaysStorageType* startRays = static_cast<KDRaysStorageType*> (startStation->clusteredRaysHandle);

  for (KDRaysStorageType::iterator i = startRays->begin(); i != startRays->end(); ++i)
  {
    KDRayWrap& wrap = const_cast<KDRayWrap&> (*i);
    LxRay* ray = wrap.ray;
    LxRay* rays[LXSTATIONS - LXFIRSTSTATION - 1];
    list<LxTrackCandidate*> candidates;
    scaltype chi2 = 0;
    BuildCandidates2(ray, rays, candidates, chi2);
    LxTrackCandidate* bestCandidate = 0;

    for (list<LxTrackCandidate*>::iterator j = candidates.begin(); j != candidates.end(); ++j)
    {
      LxTrackCandidate* candidate = *j;

      if (0 == bestCandidate || candidate->chi2 < bestCandidate->chi2)
        bestCandidate = candidate;
    }

    if (0 != bestCandidate)
    {
      LxTrack* track = new LxTrack(bestCandidate);
      tracks.push_back(track);
    }

    for (list<LxTrackCandidate*>::iterator j = candidates.begin(); j != candidates.end(); ++j)
      delete *j;
  }// for (KDRaysStorageType::iterator i = rays->begin(); i != rays->end(); ++i)

  cout << "LxSpace::Reconstruct() found: " << tracks.size() << " tracks" << endl;
}
#endif//CLUSTER_MODE

void LxSpace::ConnectNeighbours()
{
  for (int i = LXFIRSTSTATION + 2; i < LXSTATIONS; ++i)
    stations[i]->ConnectNeighbours();
}

void LxSpace::BuildCandidates(int endStNum, LxRay* ray, LxRay** rays, list<LxTrackCandidate*>& candidates, scaltype chi2)
{
  int level = ray->station->stationNumber - 1;
  rays[level] = ray;

  if (0 == level)
  {
#ifdef USE_KALMAN
    LxTrackCandidate* tc = new LxTrackCandidate(rays, LXSTATIONS - 1, ray->kalman.chi2);
#else// USE_KALMAN
    LxTrackCandidate* tc = new LxTrackCandidate(rays, endStNum, chi2);
#endif//USE_KALMAN
    candidates.push_back(tc);
    return;
  }

  for(list<LxRay*>::iterator i = ray->neighbours.begin(); i != ray->neighbours.end(); ++i)
  {
    LxRay* lRay = *i;

    if (lRay->source->used)
      continue;

    LxStation* station = lRay->station;
    //scaltype dtx = ray->tx - lRay->tx;
    //dtx /= station->txBreakSigma;
    //scaltype dty = ray->ty - lRay->ty;
    //dty /= station->tyBreakSigma;

    scaltype dtx = ray->tx - lRay->tx;
    scaltype dtx2 = dtx * dtx;
    scaltype sigmaTx2 = ray->dtx * ray->dtx + lRay->dtx * lRay->dtx + station->txBreakSigma * station->txBreakSigma;
    scaltype dty = ray->ty - lRay->ty;
    scaltype dty2 = dty * dty;
    scaltype sigmaTy2 = ray->dty * ray->dty + lRay->dty * lRay->dty + station->tyBreakSigma * station->tyBreakSigma;

#ifdef USE_KALMAN
    LxKalmanParams& kPrev = ray->kalman;
    LxKalmanParams& kalman = lRay->kalman;
    kalman.tx = kPrev.tx;
    kalman.ty = kPrev.ty;
    kalman.C11 = kPrev.C11 + station->txBreakSigma * station->txBreakSigma;
    kalman.C22 = kPrev.C22 + station->tyBreakSigma * station->tyBreakSigma;
    scaltype S11 = 1 / (kalman.C11 + lRay->dtx * lRay->dtx);
    scaltype S22 = 1 / (kalman.C22 + lRay->dty * lRay->dty);
    scaltype K11 = kalman.C11 * S11;
    scaltype K22 = kalman.C22 * S22;
    scaltype zetaTx = lRay->tx - kalman.tx;
    scaltype zetaTy = lRay->ty - kalman.ty;
    kalman.tx += K11 * zetaTx;
    kalman.ty += K22 * zetaTy;
    kalman.C11 = (1.0 - K11) * kalman.C11;
    kalman.C22 = (1.0 - K22) * kalman.C22;
    kalman.chi2 = kPrev.chi2 + zetaTx * S11 * zetaTx + zetaTy * S22 * zetaTy;
#endif//USE_KALMAN

    // Continue process of track building.
    //BuildCandidates(lRay, rays, candidates, chi2 + dtx * dtx + dty * dty);
    BuildCandidates(endStNum, lRay, rays, candidates, chi2 + dtx2 / sigmaTx2 + dty2 / sigmaTy2);
  }
}

void LxSpace::Reconstruct()
{
  for (int endStNum = LXSTATIONS - 1; endStNum >= stationsInAlgo - 1; --endStNum)
  {
    LxStation* startStation = stations[endStNum];
    LxLayer* startLayer = startStation->layers[LXMIDDLE];
    list<LxPoint*>& startPoints = startLayer->points;

    for (list<LxPoint*>::iterator i = startPoints.begin(); i != startPoints.end(); ++i)
    {
      LxPoint* point = *i;

      if (point->used)
        continue;

      if (!point->valid)
        continue;

      LxRay* rays[endStNum];
      list<LxTrackCandidate*> candidates;
      list<LxRay*>& startRays = point->rays;

      for (list<LxRay*>::iterator j = startRays.begin(); j != startRays.end(); ++j)
      {
        LxRay* ray = *j;
        scaltype chi2 = 0;

#ifdef USE_KALMAN
        LxKalmanParams& kalman = ray->kalman;
        kalman.tx = ray->tx;
        kalman.ty = ray->ty;
        kalman.C11 = ray->dtx * ray->dtx;
        kalman.C22 = ray->dty * ray->dty;
        kalman.chi2 = 0;
#endif//USE_KALMAN

        BuildCandidates(endStNum, ray, rays, candidates, chi2);
      }

      LxTrackCandidate* bestCandidate = 0;

      for (list<LxTrackCandidate*>::iterator j = candidates.begin(); j != candidates.end(); ++j)
      {
        LxTrackCandidate* candidate = *j;

        if (0 == bestCandidate || candidate->chi2 < bestCandidate->chi2)
          bestCandidate = candidate;
      }

      if (0 != bestCandidate)
      {
        LxTrack* track = new LxTrack(bestCandidate);
        tracks.push_back(track);
      }

      for (list<LxTrackCandidate*>::iterator j = candidates.begin(); j != candidates.end(); ++j)
        delete *j;
    }
  }// for (int stNum = LXSTATIONS - 1; stNum >= stationsInAlgo - 1; --stNum)

  for (list<LxTrack*>::iterator i = tracks.begin(); i != tracks.end(); ++i)
  {
    LxTrack* track = *i;
    //cout << "LxSpace::Reconstruct(): found track with length = " << track->length << endl << "With points:";

    for (int j = 0; j < track->length * LXLAYERS; ++j)
    {
      LxPoint* point = track->points[j];

      //if (point)
        //cout << " " << point->z;
      //else
        //cout << " *";
    }

    //cout << endl;
  }

  RemoveClones();
}

void LxSpace::RemoveClones()
{
  for (list<LxTrack*>::iterator i = tracks.begin(); i != tracks.end(); ++i)
  {
    LxTrack* firstTrack = *i;
    list<LxTrack*>::iterator i2 = i;
    ++i2;

    if (i2 == tracks.end())
      break;

    LxTrack* secondTrack = *i2;
    Int_t neighbourPoints = 0;
    int minLength = firstTrack->length < secondTrack->length ? firstTrack->length : secondTrack->length;

    for (Int_t j = 0; j < minLength * LXLAYERS; ++j)
    {
      LxPoint* point1 = firstTrack->points[j];
      LxPoint* point2 = secondTrack->points[j];

      if (0 == point1 || 0 == point2)
        continue;

      scaltype dx = point1->dx > point2->dx ? point1->dx : point2->dx;
      scaltype dy = point1->dy > point2->dy ? point1->dy : point2->dy;

      if (abs(point2->x - point1->x) < 5.0 * dx && abs(point2->y - point1->y) < 5.0 * dy)
        ++neighbourPoints;
    }

    if (neighbourPoints < minLength * LXLAYERS / 2)
      continue;

    if (firstTrack->length > secondTrack->length)
      secondTrack->clone = true;
    else if (secondTrack->length > firstTrack->length)
      firstTrack->clone = true;
    else if (firstTrack->chi2 < secondTrack->chi2)
      secondTrack->clone = true;
    else
      firstTrack->clone = true;
  }
}

void LxSpace::FitTracks()
{
  for (list<LxTrack*>::iterator i = tracks.begin(); i != tracks.end(); ++i)
    (*i)->Fit();
}

void LxSpace::JoinExtTracks()
{
  /*scaltype sigmaX = 0.8548;//0.89;//1.202;
  scaltype sigmaX2 = sigmaX * sigmaX;
  scaltype sigmaY = 0.6233;//1.061;
  scaltype sigmaY2 = sigmaY * sigmaY;
  scaltype sigmaTx = 0.02349;//0.02426;
  scaltype sigmaTx2 = sigmaTx * sigmaTx;
  scaltype sigmaTy = 0.007941;//0.01082;
  scaltype sigmaTy2 = sigmaTy * sigmaTy;
  scaltype deltaMuPlus = -0.01883;
  scaltype sigmaTxMuPlus = 0.01105;
  scaltype sigmaTxMuPlus2 = sigmaTxMuPlus * sigmaTxMuPlus;
  scaltype deltaMuMinus = 0.020;
  scaltype sigmaTxMuMinus = 0.0118;
  scaltype sigmaTxMuMinus2 = sigmaTxMuMinus * sigmaTxMuMinus;
  scaltype covXTx = 0.155612;
  scaltype covYTy = 0.157198;*/
  //scaltype cutCoeff = 5.0;
#ifdef USE_OLD_STS_LINKING_RULE
  scaltype sigmaX2 = muchStsBreakX * muchStsBreakX;
  scaltype sigmaY2 = muchStsBreakY * muchStsBreakY;
  scaltype sigmaTx2 = muchStsBreakTx * muchStsBreakTx;
  scaltype sigmaTy2 = muchStsBreakTy * muchStsBreakTy;
#endif//USE_OLD_STS_LINKING_RULE

  TrackPropagatorPtr fPropagator = CbmLitToolFactory::CreateTrackPropagator("lit");

  for (list<LxTrack*>::iterator i = tracks.begin(); i != tracks.end(); ++i)
  {
    LxTrack* track = *i;

    if (track->clone)
      continue;

#ifdef USE_KALMAN_FIT
    scaltype x = track->x;
    scaltype y = track->y;
    scaltype z = track->z;
    scaltype tx0 = track->tx;
    scaltype ty = track->ty;
    scaltype dxMuch = track->dx;
    scaltype dyMuch = track->dy;
    scaltype dtxMuch = track->dtx;
    scaltype dtyMuch = track->dty;
#else//USE_KALMAN_FIT
    LxRay* ray = track->rays[0];
    LxPoint* point = ray->end;
    scaltype x = point->x;
    scaltype y = point->y;
    scaltype z = point->z;
    //scaltype tx0 = ray->tx;
    scaltype tx = ray->tx;
    scaltype ty = ray->ty;
    scaltype dxMuch = point->dx;
    scaltype dyMuch = point->dy;
    scaltype dtxMuch = ray->dtx;
    scaltype dtyMuch = ray->dty;
#endif//USE_KALMAN_FIT

    // External track are already filtered by P and Pt.
    for (list<LxExtTrack>::iterator j = extTracks.begin(); j != extTracks.end(); ++j)
    {
      LxExtTrack* extTrack = &(*j);
      const FairTrackParam* firstParam = extTrack->track->GetParamFirst();
      const FairTrackParam* lastParam = extTrack->track->GetParamLast();

#ifndef USE_OLD_STS_LINKING_RULE
      CbmLitTrackParam litLastParam;
      CbmLitConverterFairTrackParam::FairTrackParamToCbmLitTrackParam(lastParam, &litLastParam);

      if (kLITERROR == fPropagator->Propagate(&litLastParam, stations[0]->zCoord, 13))
        continue;

      scaltype deltaX = abs(litLastParam.GetX() - x);
      scaltype deltaY = abs(litLastParam.GetY() - y);
      scaltype deltaTx = abs(litLastParam.GetTx() - tx0);
      scaltype deltaTy = abs(litLastParam.GetTy() - ty);
      scaltype sigmaX2 = dxMuch * dxMuch + litLastParam.GetCovariance(0);
      scaltype sigmaX = sqrt(sigmaX2);
      scaltype sigmaY2 = dyMuch * dyMuch + litLastParam.GetCovariance(5);
      scaltype sigmaY = sqrt(sigmaY2);
      scaltype sigmaTx2 = dtxMuch * dtxMuch + litLastParam.GetCovariance(9);
      scaltype sigmaTx = sqrt(sigmaTx2);
      scaltype sigmaTy2 = dtyMuch * dtyMuch + litLastParam.GetCovariance(12);
      scaltype sigmaTy = sqrt(sigmaTy2);

      //if (deltaX > cutCoeff * sigmaX || deltaY > cutCoeff * sigmaY ||
          //deltaTx > cutCoeff * sigmaTx || deltaTy > cutCoeff * sigmaTy)
      //{
        //continue;
      //}

      scaltype chi2 = deltaX * deltaX / sigmaX2 + deltaY * deltaY / sigmaY2 +
          deltaTx * deltaTx / sigmaTx2 + deltaTy * deltaTy / sigmaTy2;
#else//USE_OLD_STS_LINKING_RULE

      //if ((tx0 - lastParam->GetTx()) * (lastParam->GetTx() - firstParam->GetTx()) < 0)
        //continue;

      /*scaltype muchCharge = tx0 - firstParam->GetTx();
      scaltype tx = muchCharge > 0 ? tx0 + deltaMuPlus : tx0 + deltaMuMinus;

      if (muchCharge > 0)
      {
        sigmaTx = sigmaTxMuPlus;
        sigmaTx2 = sigmaTxMuPlus2;
      }
      else
      {
        sigmaTx = sigmaTxMuMinus;
        sigmaTx2 = sigmaTxMuMinus2;
      }*/

      scaltype deltaZ = lastParam->GetZ() - z;
      scaltype deltaX = abs(lastParam->GetX() - x - tx * deltaZ);
      scaltype dxSts2 = lastParam->GetCovariance(0, 0);
      scaltype dySts2 = lastParam->GetCovariance(1, 1);
      scaltype dtxSts2 = lastParam->GetCovariance(2, 2);
      scaltype dtySts2 = lastParam->GetCovariance(3, 3);
      //scaltype sigmaXMeas2 = dxMuch * dxMuch + dxSts2 - covXTx * deltaZ;// deltaZ is negative.
      scaltype sigmaXMeas2 = dxMuch * dxMuch + dxSts2 + dtxMuch * dtxMuch * deltaZ * deltaZ;
      //scaltype sigmaXMeas = sqrt(sigmaXMeas2);
      //scaltype sigmaYMeas2 = dyMuch * dyMuch + dySts2 - covYTy * deltaZ;// deltaZ is negative.
      scaltype sigmaYMeas2 = dyMuch * dyMuch + dySts2 + dtyMuch * dtyMuch * deltaZ * deltaZ;
      //scaltype sigmaYMeas = sqrt(sigmaYMeas2);
      scaltype sigmaTxMeas2 = dtxMuch * dtxMuch + dtxSts2;
      //scaltype sigmaTxMeas = sqrt(sigmaTxMeas2);
      scaltype sigmaTyMeas2 = dtyMuch * dtyMuch + dtySts2;
      //scaltype sigmaTyMeas = sqrt(sigmaTyMeas2);

      //if (deltaX > cutCoeff * sqrt(sigmaX2 + sigmaXMeas2))
        //continue;

      scaltype deltaY = abs(lastParam->GetY() - y - ty * deltaZ);

      //if (deltaY > cutCoeff * sqrt(sigmaY2 + sigmaYMeas2))
        //continue;

      scaltype deltaTx = abs(lastParam->GetTx() - tx);

      //if (deltaTx > cutCoeff * sqrt(sigmaTx2 + sigmaTxMeas2))
        //continue;

      scaltype deltaTy = abs(lastParam->GetTy() - ty);

      //if (deltaTy > cutCoeff * sqrt(sigmaTy2 + sigmaTyMeas2))
        //continue;

      // Take the charge sign into account.
      //scaltype stsCharge = firstParam->GetQp();
      //scaltype angMomInv =  muchCharge / stsCharge;
      //scaltype dAmi = abs(sqrt(dtxMuch * dtxMuch + firstParam->GetCovariance(2, 2)) / stsCharge);

      // Check if the MUCH track projection to XZ plane angle fit the STS track momentum.
      //if (0.18 - dAmi > angMomInv || 0.52 + dAmi < angMomInv)
      //if (0.26 - dAmi > angMomInv || 0.44 + dAmi < angMomInv)
        //continue;

      scaltype chi2 = deltaX * deltaX / (sigmaX2 + sigmaXMeas2) + deltaY * deltaY / (sigmaY2 + sigmaYMeas2) +
          deltaTx * deltaTx / (sigmaTx2 + sigmaTxMeas2) + deltaTy * deltaTy / (sigmaTy2 + sigmaTyMeas2);
#endif//USE_OLD_STS_LINKING_RULE

#ifdef LX_EXT_LINK_SOPH
      list<pair<LxExtTrack*, scaltype> >::iterator k = track->extTrackCandidates.begin();

      for (; k != track->extTrackCandidates.end() && chi2 >= k->second; ++k)
        ;

      pair<LxExtTrack*, scaltype> linkDesc(extTrack, chi2);
      track->extTrackCandidates.insert(k, linkDesc);
#else//LX_EXT_LINK_SOPH
      if (0 == track->externalTrack || track->extLinkChi2 > chi2)
      {
        track->externalTrack = extTrack;
        track->extLinkChi2 = chi2;
      }
#endif//LX_EXT_LINK_SOPH
    }// for (list<LxExtTrack>::iterator j = extTracks.begin(); j != extTracks.end(); ++j)

#ifdef LX_EXT_LINK_SOPH
    for (list<pair<LxExtTrack*, scaltype> >::iterator j = track->extTrackCandidates.begin();
        j != track->extTrackCandidates.end(); ++j)
    {
      LxExtTrack* extTrack = j->first;
      scaltype chi2 = j->second;

      if (0 == extTrack->recoTrack.first)
      {
        extTrack->recoTrack.first = track;
        extTrack->recoTrack.second = chi2;
        track->externalTrack = extTrack;
        break;
      }
      else if (chi2 < extTrack->recoTrack.second)
      {
        LxTrack* anotherTrack = extTrack->recoTrack.first;
        extTrack->recoTrack.first = track;
        extTrack->recoTrack.second = chi2;
        track->externalTrack = extTrack;
        anotherTrack->Rebind();
        break;
      }
    }
#endif// LX_EXT_LINK_SOPH
  }// for (list<LxTrack*>::iterator i = tracks.begin(); i != tracks.end(); ++i)
}

void LxSpace::CheckArray(scaltype xs[LXSTATIONS][LXLAYERS], scaltype ys[LXSTATIONS][LXLAYERS],
    scaltype zs[LXSTATIONS][LXLAYERS],
    scaltype xDisp2Limits[LXSTATIONS], scaltype yDisp2Limits[LXSTATIONS], scaltype tx2Limits[LXSTATIONS],
      scaltype ty2Limits[LXSTATIONS], scaltype txBreak2Limits[LXSTATIONS], scaltype tyBreak2Limits[LXSTATIONS])
{
  for (int i = 0; i < LXSTATIONS; ++i)
  {
    scaltype diffZ = zs[i][0] - zs[i][1];
    scaltype tx = xs[i][1] / zs[i][1];
    scaltype ty = ys[i][1] / zs[i][1];
    scaltype dispXL = xs[i][0] - tx * diffZ - xs[i][1];
    scaltype dispYL = ys[i][0] - ty * diffZ - ys[i][1];

    if (dispXL < 0)
      dispXL = -dispXL;

    if (dispYL < 0)
      dispYL = -dispYL;

    diffZ = zs[i][2] - zs[i][1];
    scaltype dispXR = xs[i][2] - tx * diffZ - xs[i][1];
    scaltype dispYR = ys[i][2] - ty * diffZ - ys[i][1];

    if (dispXR < 0)
      dispXR = -dispXR;

    if (dispYR < 0)
      dispYR = -dispYR;

    scaltype dispX = dispXL < dispXR ? dispXL : dispXR;
    scaltype dispY = dispYL < dispYR ? dispYL : dispYR;

    if (stations[i]->disp01XBig - dispX < xDisp2Limits[i])
      xDisp2Limits[i] = stations[i]->disp01XBig - dispX;

    if (stations[i]->disp01YBig - dispY < yDisp2Limits[i])
      yDisp2Limits[i] = stations[i]->disp01YBig - dispY;

    if (i > 0)
    {
      diffZ = zs[i][1] - zs[i - 1][1];
      scaltype tx1 = xs[i][1] / zs[i][1];
      tx = (xs[i][1] - xs[i - 1][1]) / diffZ;
      scaltype dtx = tx - tx1;

      if (dtx < 0)
        dtx = -dtx;

      scaltype ty1 = ys[i][1] / zs[i][1];
      ty = (ys[i][1] - ys[i - 1][1]) / diffZ;
      scaltype dty = ty - ty1;

      if (dty < 0)
        dty = -dty;

      if (stations[i]->txLimit - dtx < tx2Limits[i])
        tx2Limits[i] = stations[i]->txLimit - dtx;

      if (stations[i]->tyLimit - dty < ty2Limits[i])
        ty2Limits[i] = stations[i]->tyLimit - dty;

      if (i < LXSTATIONS - 1)
      {
        diffZ = zs[i + 1][1] - zs[i][1];
        scaltype tx2 = (xs[i + 1][1] - xs[i][1]) / diffZ;
        scaltype ty2 = (ys[i + 1][1] - ys[i][1]) / diffZ;
        scaltype txBreak = tx2 - tx;
        scaltype tyBreak = ty2 - ty;

        if (txBreak < 0)
          txBreak = -txBreak;

        if (tyBreak < 0)
          tyBreak = -tyBreak;

        if (stations[i]->txBreakLimit - txBreak < txBreak2Limits[i])
          txBreak2Limits[i] = stations[i]->txBreakLimit - txBreak;

        if (stations[i]->tyBreakLimit - tyBreak < tyBreak2Limits[i])
          tyBreak2Limits[i] = stations[i]->tyBreakLimit - tyBreak;
      }
    }
  }
}

void LxSpace::CheckArray(scaltype xs[LXSTATIONS][LXLAYERS], scaltype ys[LXSTATIONS][LXLAYERS],
    scaltype zs[LXSTATIONS][LXLAYERS], list<LxPoint*> pts[LXSTATIONS][LXLAYERS], int level,
    scaltype xDisp2Limits[LXSTATIONS], scaltype yDisp2Limits[LXSTATIONS], scaltype tx2Limits[LXSTATIONS],
      scaltype ty2Limits[LXSTATIONS], scaltype txBreak2Limits[LXSTATIONS], scaltype tyBreak2Limits[LXSTATIONS])
{
  if (LXSTATIONS * LXLAYERS == level)
  {
    CheckArray(xs, ys, zs, xDisp2Limits, yDisp2Limits, tx2Limits, ty2Limits, txBreak2Limits, tyBreak2Limits);
    return;
  }

  int stNum = level / LXLAYERS;
  int lyNum = level % LXLAYERS;
  list<LxPoint*>& points = pts[stNum][lyNum];

  for (list<LxPoint*>::iterator i = points.begin(); i != points.end(); ++i)
  {
    LxPoint* point = *i;

    xs[stNum][lyNum] = point->x;
    ys[stNum][lyNum] = point->y;
    zs[stNum][lyNum] = point->z;

    CheckArray(xs, ys, zs, pts, level + 1, xDisp2Limits, yDisp2Limits, tx2Limits, ty2Limits, txBreak2Limits,
        tyBreak2Limits);
  }
}

void LxSpace::CheckArray(ostream& out, LxMCTrack& track)
{
  scaltype xs[LXSTATIONS][LXLAYERS];
  scaltype ys[LXSTATIONS][LXLAYERS];
  scaltype zs[LXSTATIONS][LXLAYERS];
  list<LxPoint*> pts[LXSTATIONS][LXLAYERS];
  int inits[LXSTATIONS][LXLAYERS];
  scaltype xDisp2Limits[LXSTATIONS];
  scaltype yDisp2Limits[LXSTATIONS];
  scaltype tx2Limits[LXSTATIONS];
  scaltype ty2Limits[LXSTATIONS];
  scaltype txBreak2Limits[LXSTATIONS];
  scaltype tyBreak2Limits[LXSTATIONS];
  bool busyHits[LXSTATIONS];

  for (int i = 0; i < LXSTATIONS; ++i)
  {
    for (int j = 0; j < LXLAYERS; ++j)
      inits[i][j] = 0;

    xDisp2Limits[i] = stations[i]->disp01XBig;
    yDisp2Limits[i] = stations[i]->disp01YBig;
    tx2Limits[i] = stations[i]->txLimit;
    ty2Limits[i] = stations[i]->tyLimit;
    txBreak2Limits[i] = stations[i]->txBreakLimit;
    tyBreak2Limits[i] = stations[i]->tyBreakLimit;
    busyHits[i] = false;
  }

  for (vector<LxMCPoint*>::iterator i = track.Points.begin(); i != track.Points.end(); ++i)
  {
    LxMCPoint* point = *i;

    for (list<LxPoint*>::iterator j = point->lxPoints.begin(); j != point->lxPoints.end(); ++j)
    {
      LxPoint* lxPoint = *j;
      pts[point->stationNumber][point->layerNumber].push_back(lxPoint);

      if (lxPoint->used)
        busyHits[point->stationNumber] = true;
    }

    inits[point->stationNumber][point->layerNumber] += point->lxPoints.size();
  }

  bool ini = true;

  for (int i = 0; i < LXSTATIONS; ++i)
  {
    for (int j = 0; j < LXLAYERS; ++j)
      ini = ini && 0 < inits[i][j];
  }

  if (!ini)
  {
    out << "CheckArray: track does not contain all the points" << endl;
    return;
  }

  CheckArray(xs, ys, zs, pts, 0, xDisp2Limits, yDisp2Limits, tx2Limits, ty2Limits, txBreak2Limits,
      tyBreak2Limits);

  for (int i = 0; i < LXSTATIONS; ++i)
  {
    out << "CheckArray on station " << i << ": ";
    out << "dispX to limit: " << xDisp2Limits[i];
    out << "; dispY to limit: " << yDisp2Limits[i];

    if (i > 0)
    {
      out << "; Tx to limit: " << tx2Limits[i];
      out << "; Ty to limit: " << ty2Limits[i];

      if (i < LXSTATIONS - 1)
      {
        out << "; tx break to limit: " << txBreak2Limits[i];
        out << "; ty break to limit: " << tyBreak2Limits[i];
      }
    }

    if (busyHits[i])
      out << "; have busy hits";

    out << endl;
  }

  out << endl;
}
