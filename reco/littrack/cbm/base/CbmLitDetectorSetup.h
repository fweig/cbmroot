/* Copyright (C) 2012-2017 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

/**
 * \file CbmLitDetectorSetup.h
 * \brief Helper class to access detector presence.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2012
 */
#ifndef CBMLITDETECTORSETUP_H_
#define CBMLITDETECTORSETUP_H_

#include "CbmDefs.h"

#include <map>
#include <string>
using std::map;
using std::string;

/**
 * \class CbmLitDetectorSetup
 * \brief Helper class to access detector presence.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2012
 */
class CbmLitDetectorSetup {
public:
  /**
    * \brief Constructor.
    */
  CbmLitDetectorSetup();

  /**
    * \brief Destructor.
    */
  virtual ~CbmLitDetectorSetup();

  /**
    * \brief Set detector presence manually.
    * \param[in] detId Detector identificator.
    * \param[in] isDet True if detector is in the setup.
    */
  void SetDet(ECbmModuleId detId, bool isDet);

  /**
    * \brief Return detector presence in setup.
    * \param[in] detId Detector identificator.
    * \return Detector presence in setup.
    */
  bool GetDet(ECbmModuleId detId) const;

  /**
    * \brief Set electron setup of detector.
    * \param[in] isElectronSetup True if electron setup.
    */
  void SetElectronSetup(bool isElectronSetup) { fIsElectronSetup = isElectronSetup; }

  /**
    * \brief Return true if electron setup is detected.
    * \return True if electron setup is detected.
    */
  bool GetElectronSetup() const { return fIsElectronSetup; }

  /**
    * \brief Set muon setup of detector.
    * \param[in] isMuonSetup True if muon setup.
    */
  void SetMuonSetup(bool isMuonSetup) { fIsMuonSetup = isMuonSetup; }

  /**
    * \brief Return true if muon setup is detected.
    * \return True if muon setup is detected.
    */
  bool GetMuonSetup() const { return fIsMuonSetup; }

  /**
    * \brief Determines detector presence using TGeoManager.
    */
  void DetermineSetup();

  /**
    * \brief Return string representation of class.
    * \return String representation of class.
    */
  string ToString() const;

private:
  /**
    * \brief Check detector presence using TGeoManager.
    * \param[in] name Name of the detector to be checked.
    * \return True if detector was found in TGeoManager.
    */
  bool CheckDetectorPresence(const std::string& name) const;

  bool fIsElectronSetup;         // If "electron" setup detected than true
  bool fIsMuonSetup;             // If "muon" setup detected than true
  map<ECbmModuleId, bool> fDet;  // Map DetectorId to detector presence
};

#endif /* CBMLITDETECTORSETUP_H_ */
