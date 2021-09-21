/* Copyright (C) 2021 Goethe-University Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pascal Raisig [committer] */

/**
 * @file CbmTrddEdxUtils.h
 * @author Pascal Raisig (praisig@ikf.uni-frankfurt.de)
 * @brief Class containing definitions and functions correlated to the dEdx behavior of the CbmTrd
 * @version 0.1
 * @date 2021-04-08
 * 
 * 
 */

#ifndef CBMTRDDEDXUTILS_H
#define CBMTRDDEDXUTILS_H

#include <Rtypes.h>
#include <RtypesCore.h>


class CbmTrddEdxUtils {
public:
  /**
     * @brief Construct a new CbmTrd dEdx Utils object
     * 
     */
  CbmTrddEdxUtils();

  /**
     * @brief Copy constructor (not implemented!)
     * 
     */
  CbmTrddEdxUtils(const CbmTrddEdxUtils&) = delete;

  /**
     * @brief Assignment operator (not implemented!)
     * 
     * @return CbmTrddEdxUtils& 
     */
  CbmTrddEdxUtils& operator=(const CbmTrddEdxUtils&);

  /**
     * @brief Destroy the CbmTrd dEdx Utils object
     * 
     */
  virtual ~CbmTrddEdxUtils();

  /**
     * @brief Mip De Dx kev/cm for XeC02 8020 see https://cbm-wiki.gsi.de/foswiki/bin/view/TRD/      TrdParameterList
     * 
     * @return Double_t 
     */
  static Double_t MipDeDx() { return 5.0; }

  /**
     * @brief Mean number of primary electron per cm created by a MIP (current value corresponds to GEANT3 PR20210408 - see e.g. https://www.uni-frankfurt.de/96680075/Doktorarbeit_Etienne_Bechtel.pdf)
     * 
     * @return Double_t 
     */
  static Double_t MipMeanPrimaryEles() { return 20.5; }

  /**
    * @brief Target percentage of the dynamic range for the MIP energy deposition on the central pad. 
    * 
    * @return Double_t 
   */
  static Double_t MipCaliTarget() { return 0.07; }

  /**
     * @brief Get the Mip normalized Bethe-Bloch dEdx value 
     * Returns Q/Q_MIP based on a given parametrization of the Bethe-Bloch function
     * In the current status it returns the values for the ALEPH/ALICE-TRD parametrization 
     * see https://arxiv.org/pdf/1709.02743.pdf
     * @param betaGamma
     * @return Double_t 
     */
  static Double_t GetMipNormedBB(Double_t betaGamma);

  /**
    * @brief Mean charge fraction on the central pad assuming a flat distrubtion all over the pad.
    * 
    * @return Double_t 
   */
  static Double_t MeanChargeCentrPadPRF() { return 0.65; }

  // protected:
  // private:
public:
  ClassDef(CbmTrddEdxUtils, 2)
};
#endif  // CBMTRDDEDEXUTILS_H
