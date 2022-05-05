/* Copyright (C) 2018-2022 Horia Hulubei National Institute of Physics and Nuclear Engineering, Bucharest
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci [committer] */

#ifndef CBMTRDHITMC_H
#define CBMTRDHITMC_H

#include "CbmTrdCluster.h"
#include "CbmTrdHit.h"
#include "CbmTrdPoint.h"

#include <string>  // for ToString
#include <vector>  // for fTrdPoints

/** \class CbmTrdHitMC
 * \brief  TRD hit to MC point correlation class
 * \author Alexandru Bercuci <abercuci@niham.nipne.ro>
 * \date 02.05.2022
 * 
 * The class packs the whole history of a TrdHit from the set of MC points generating the signals, 
 * to information on the digits structure in the cluster and the reconstructed observables.
 * The class should be used to make error parametrization, pile-up estimations, etc. 
 *
 * To describe main functionality ... 
 */

class CbmTrdDigi;
class CbmTrdHitMC : public CbmTrdHit {
public:
  enum eCbmTrdHitMCshape
  {
    kRT = 0,  // open left - open right shape
    kRR,      // open left - closed right shape
    kTT,      // closed left - open right shape
    kTR       // closed left - closed right shape
  };

  /** \brief Default constructor.*/
  CbmTrdHitMC();

  /** \brief Standard constructor.*/
  CbmTrdHitMC(const CbmTrdHit& hit);

  /** \brief Destructor. */
  virtual ~CbmTrdHitMC();

  /** \brief Copy cluster details.*/
  void AddCluster(const CbmTrdCluster* c);

  /** \brief Add MC points to the hit. The first time this function is called is for the best matched MC point
   * \param p: pointer to the point being added 
   * \param t: time of the event to which the point belongs  
   * \param m: mass of the particle producing the point  
   * \return the number of points in the list 
   */
  size_t AddPoint(const CbmTrdPoint* p, double t, int id);
  /** \brief Add signal values in the increasing order of pad index
   * \param d digi from ch/pad
   * \param t0 relative time in the cluster
   * \return the number of signals in the cluster 
   */
  size_t AddSignal(const CbmTrdDigi* d, uint64_t t0);

  /** \brief return MC pile-up size*/
  std::string GetErrorMsg() const { return fErrMsg; }

  /** \brief Register a MC point
   * \param idx index of point being requested. by default the best fit is returned. 
   */
  const CbmTrdPoint* GetPoint(uint idx = 0) const;

  /** \brief return signal at position
   * \param idx index of signal counting from left to right looking upstream (from FEE). 
   */
  double GetSignal(uint idx = 0) const;

  /** \brief return MC pile-up size*/
  size_t GetNPoints() const { return fTrdPoints.size(); }

  /** \brief return cluster size*/
  size_t GetNSignals() const { return fTrdSignals.size(); }

  /** \brief return cluster shape according to the eCbmTrdHitMCshape definitions*/
  eCbmTrdHitMCshape GetClShape() const;

  /** \brief Calculate residuals in the bending plane.*/
  double GetDx() const;

  /** \brief Calculate error in the bending plane.*/
  double GetSx() const;

  /** \brief Calculate residuals for the azimuth direction.*/
  double GetDy() const;

  /** \brief Calculate error for the azimuth direction.*/
  double GetSy() const;

  /** \brief Calculate residuals for time.*/
  double GetDt() const;

  /** \brief Store error message.*/
  void SetErrorMsg(std::string msg) { fErrMsg = msg; }

  /** \brief Applies to TRD2D and remove 0 charges from the boundaries of the cluster.**/
  size_t PurgeSignals();

  /** \brief Verbosity functionality.**/
  virtual std::string ToString() const;

private:
  /** \brief Copy Constructor.*/
  CbmTrdHitMC(const CbmTrdHitMC&) = default;
  /** \brief Assignment operator.*/
  CbmTrdHitMC& operator=(const CbmTrdHitMC&) = default;

  std::string fErrMsg                             = "";  //< error message from the QA task
  std::vector<std::pair<double, int>> fTrdSignals = {};  //< list of signal/time in cluster
  std::vector<std::tuple<CbmTrdPoint, double, int>> fTrdPoints =
    {};                     //< list of MC points together with the event time and particle PDG code producing them
  CbmTrdCluster fCluster;   //< data from the cluster
  static int fSx[5][2];     //< x error parametrization as function of cluster size and incident direction
  ClassDef(CbmTrdHitMC, 1)  // Hit to MC point data correlation
};

#endif
