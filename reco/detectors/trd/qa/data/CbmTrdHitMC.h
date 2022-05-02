/* Copyright (C) 2018-2022 Horia Hulubei National Institute of Physics and Nuclear Engineering, Bucharest
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci [committer] */

#ifndef CBMTRDHITMC_H
#define CBMTRDHITMC_H

#include "CbmTrdHit.h"
#include "CbmTrdPoint.h"

#include <vector>  // for vector

/** \class CbmTrdHitMC
 * \brief  TRD hit to MC point correlation class
 * \author Alexandru Bercuci <abercuci@niham.nipne.ro>
 * \date 02.05.2022
 * 
 * The class
 */

class CbmTrdHitMC : public CbmTrdHit {
public:
  /** \brief Default constructor.*/
  CbmTrdHitMC();
  /** \brief Standard constructor.*/
  CbmTrdHitMC(const CbmTrdHit& hit);
  /** \brief Destructor. */
  virtual ~CbmTrdHitMC();

  /** \brief Add MC points to the hit. The first time this function is called is for the best matched MC point
   * \param p: pointer to the point being added 
   * \param t: time of the event to which the point belongs  
   * \param m: mass of the particle producing the point  
   * \return the number of points in the list 
   */
  size_t AddPoint(const CbmTrdPoint* p, double t, double m);

  /** \brief Retrieve a MC point
   * \param idx index of point being requested. by default the best fit is returned. 
   */
  const CbmTrdPoint* GetPoint(uint idx = 0) const;

  /** \brief Calculate residuals in the bending plane.*/
  double GetDx() const;
  /** \brief Calculate residuals for the azimuth direction.*/
  double GetDy() const;
  /** \brief Calculate residuals for time.*/
  double GetDt() const;

private:
  /** \brief Copy Constructor.*/
  CbmTrdHitMC(const CbmTrdHitMC&) = default;
  /** \brief Assignment operator.*/
  CbmTrdHitMC& operator=(const CbmTrdHitMC&) = default;

  std::vector<std::tuple<CbmTrdPoint, double, double>> fTrdPoints = {};
  ClassDef(CbmTrdHitMC, 1)  // Hit to MC point data correlation
};

#endif
