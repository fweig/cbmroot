/* Copyright (C) 2006-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

/**
* \file CbmRichRingFinderHough.h
*
* \brief Main class for ring finder based on Hough Transform implementation.
*
* \author Semen Lebedev
* \date 2008
**/

#ifndef CBM_RICH_RING_FINDER_HOUGH
#define CBM_RICH_RING_FINDER_HOUGH

#include "CbmRichRingFinder.h"

#include <vector>

class CbmRichRingFinderHoughImpl;
class CbmRichRingFinderHoughSimd;
class CbmRichRing;
class CbmRichRingLight;

#define HOUGH_SERIAL
//#define HOUGH_SIMD

using std::vector;

/**
* \class CbmRichRingFinderHough
*
* \brief Main class for ring finder based on Hough Transform implementation.
*
* \author Semen Lebedev
* \date 2008
**/
class CbmRichRingFinderHough : public CbmRichRingFinder {

public:
  /**
    * \brief Standard constructor.
    */
  CbmRichRingFinderHough();

  /**
    * \brief Destructor.
    */
  virtual ~CbmRichRingFinderHough();

  /**
	 * \brief Inherited from CbmRichRingFinder.
	 */
  virtual void Init();

  /**
	 * \brief Inherited from CbmRichRingFinder.
	 */
  virtual Int_t DoFind(CbmEvent* event, TClonesArray* rHitArray, TClonesArray* rProjArray, TClonesArray* rRingArray);

  void SetUseAnnSelect(bool use) { fUseAnnSelect = use; }

private:
  Int_t fEventNum      = 0;
  Bool_t fUseAnnSelect = true;

// choose between serial and SIMD implementation of the ring finder
#ifdef HOUGH_SERIAL
  CbmRichRingFinderHoughImpl* fHTImpl = nullptr;
#endif

#ifdef HOUGH_SIMD
  CbmRichRingFinderHoughSimd* fHTImpl = nullptr;
#endif

  /**
	 * \brief Add found rings to the output TClonesArray.
	 * \param[out] rRingArray Output array of CbmRichRing.
	 * \param[in] rHitArray  Array of CbmRichHit.
	 * \param[in] rings Found rings.
	 */
  void AddRingsToOutputArray(CbmEvent* event, TClonesArray* rRingArray, TClonesArray* rHitArray,
                             const vector<CbmRichRingLight*>& rings);

  /**
	 * \brief Copy constructor.
	 */
  CbmRichRingFinderHough(const CbmRichRingFinderHough&);

  /**
    * \brief Assignment operator.
    */
  CbmRichRingFinderHough& operator=(const CbmRichRingFinderHough&);
};

#endif
