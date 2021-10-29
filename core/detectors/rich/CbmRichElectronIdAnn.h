/* Copyright (C) 2008-2020 UGiessen/JINR-LIT, Giessen/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer], Andrey Lebedev */

/**
* \file CbmRichElectronIdAnn.h
*
* \brief Implementation of the electron identification algorithm in the RICH detector using
* Artificial Neural Network(ANN).
*
* \author Semen Lebedev
* \date 2008
**/

#ifndef CBM_RICH_ELECTRONID_ANN
#define CBM_RICH_ELECTRONID_ANN

#include "TClonesArray.h"  // for ROOTCLING

#include <string>

class CbmRichRing;
class TMultiLayerPerceptron;

using std::string;

/**
* \class CbmRichElectronIdAnn
*
* \brief Implementation of the electron identification algorithm in the RICH detector using
* Artificial Neural Network(ANN).
*
* \author Semen Lebedev
* \date 2008
**/
class CbmRichElectronIdAnn {
private:
  /**
	* \brief Standard constructor.
	*/
  CbmRichElectronIdAnn();

public:
  /**
	 * Return Instance of CbmRichGeoManager.
	 */
  static CbmRichElectronIdAnn& GetInstance()
  {
    static CbmRichElectronIdAnn fInstance;
    return fInstance;
  }


  /**
    * \brief Destructor.
    */
  virtual ~CbmRichElectronIdAnn();

  /**
    * \brief Calculate output value of the ANN.
    * \param[in] ring Found and fitted ring.
    * \param[in] momentum Momentum of the track attached to this ring.
    * \return ANN output value.
    */
  double CalculateAnnValue(int globalTrackIndex, double momentum);

  /**
    * \brief Set path to the file with ANN weights.
    * \param[in] fileName path to the file with ANN weights.
    */
  //void SetAnnWeights(const string& fileName){fAnnWeights = fileName;}

private:
  string fAnnWeights;          // path to the file with weights for ANN
  TMultiLayerPerceptron* fNN;  // Pointer to the ANN

  TClonesArray* fGlobalTracks;
  TClonesArray* fRichRings;

  /**
    * \brief Initialize ANN before use.
    */
  void Init();

private:
  /**
    * \brief Copy constructor.
    */
  CbmRichElectronIdAnn(const CbmRichElectronIdAnn&);

  /**
    * \brief Assignment operator.
    */
  CbmRichElectronIdAnn& operator=(const CbmRichElectronIdAnn&);
};

#endif
