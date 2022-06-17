/* Copyright (C) 2022 Johann Wolfgang Goethe-Universitaet Frankfurt, Frankfurt am Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jan de Cuveland [committer] */

#ifndef APP_CBMRECO_APPLICATION_H
#define APP_CBMRECO_APPLICATION_H

#include "CbmReco.h"

#include <memory>

#include "Monitor.hpp"
#include "ProgramOptions.h"
#include "log.hpp"

/** @class Application
 ** @brief Main class of the "cbmreco_fairrun" application
 ** @author Jan de Cuveland <cuveland@compeng.uni-frankfurt.de>
 ** @since 16 March 2022
 **
 ** This class implements a stand-alone command-line application.
 ** It instantiatates and configures a CbmReco object, which executes
 ** the CBM reconstruction steps using FairTasks and FairRunOnline.
 **/
class Application {
public:
  /** @brief Standard constructor, initialize the application **/
  explicit Application(ProgramOptions const& opt);

  /** @brief Copy constructor forbidden **/
  Application(const Application&) = delete;

  /** @brief Assignment operator forbidden **/
  void operator=(const Application&) = delete;

  /** @brief Destructor **/
  ~Application();

  /** @brief Run the application **/
  void Run();

private:
  std::unique_ptr<cbm::Monitor> fMonitor;  ///< The application's monitoring object
  ProgramOptions const& fOpt;              ///< Program options object
  std::unique_ptr<CbmReco> fCbmReco;       ///< CBM reconstruction steering class instance
};

#endif
