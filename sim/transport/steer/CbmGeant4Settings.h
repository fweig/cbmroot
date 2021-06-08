/* Copyright (C) 2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

/** @file CbmGeant4Settings.h
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @since 21.01.2020
 **/

#ifndef CBMGEANT4SETTINGS_H
#define CBMGEANT4SETTINGS_H 1

/** @class CbmGeant4Settings
 ** @brief User interface class to define the Geant4 simulation settings
 ** @author Florian Uhlig <f.uhlig@gsi.de>
 ** @since 21.01.2020
 **/

#include "CbmVMCSettings.h"

#include <array>
#include <string>
#include <vector>

class TVirtualMC;

class CbmGeant4Settings : public CbmVMCSettings {
public:
  CbmGeant4Settings()                         = default;
  ~CbmGeant4Settings()                        = default;
  CbmGeant4Settings(const CbmGeant4Settings&) = delete;
  CbmGeant4Settings& operator=(const CbmGeant4Settings&) = delete;

  /** @brief Set all parameters defined in this class
   ** @param[in] vmc Pointer to the VirtualMC class
   **
   **/
  void Init(TVirtualMC*);

  /** @brief Define the Geant4 run configuration
   ** @param[in] navigationEngine Define geometry input and navigation (default geomROOT)
   ** @param[in] physicsLists Define the physiscs lists (default QGSP_BERT_EMV+optical)
   ** @Param[in] specialProcesses Define special processes (default stepLimiter)
   **/
  void SetG4RunConfig(std::string navigationEngine, std::string physicsLists, std::string specialProcesses)
  {
    fG4RunConfig[0] = navigationEngine;
    fG4RunConfig[1] = physicsLists;
    fG4RunConfig[2] = specialProcesses;
  }

  /** @brief Get the Geant4 run configuration
   ** @return Array with the 3 strings for the Geant4 run configuration
   **
   **/
  std::array<std::string, 3> GetG4RunConfig() { return fG4RunConfig; }

  /** @brief Set a new command which should be passsed to Geant4. The call will
   ** remove the current list of commands
   ** @param[in] Geant4 command string
   **/
  void SetG4Command(std::string command)
  {
    fG4Commands.clear();
    fG4Commands.push_back(command);
  }

  /** @brief Add a new command which should be passsed to Geant4 to the existing 
   ** list of commands
   ** @param[in] Geant4 command string
   **/
  void AddG4Command(std::string command) { fG4Commands.push_back(command); }

  /** @brief Set the maximum number of steps after which the transport is stopped
   ** list of commands
   ** @param[in] Number of steps
   **/
  void SetMaximumNumberOfSteps(Int_t numSteps) { fMaxNumSteps = numSteps; }

private:
  std::array<std::string, 3> fG4RunConfig {{"geomRoot", "QGSP_BERT_EMV+optical", "stepLimiter"}};

  std::vector<std::string> fG4Commands {"/process/optical/verbose 0",
                                        "/process/optical/defaults/cerenkov/setMaxPhotons 20",
                                        "/process/optical/defaults/cerenkov/setMaxBetaChange 0.1",
                                        "/process/optical/setTrackSecondariesFirst Cerenkov false"};

  Int_t fMaxNumSteps {10000000};

  ClassDef(CbmGeant4Settings, 3);
};

#endif /* CBMGEANT4SETTINGS_H */
