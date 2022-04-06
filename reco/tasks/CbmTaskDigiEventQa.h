/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Volker Friese [committer] */

#ifndef CBMTASKDIGIEVENTQA_H
#define CBMTASKDIGIEVENTQA_H 1

#include "CbmDefs.h"
#include "CbmDigiEvent.h"

#include <FairTask.h>

#include <vector>

//#include "EventBuilder.h"

class CbmDigiManager;
class CbmRecoConfig;
class TH1F;

/** @class CbmTaskDigiEventQa
 ** @brief QA task class for digi events produced by the event builder
 ** @author Volker Friese <v.friese@gsi.de>
 ** @since 15.03.2022
 **
 ** Currently implemented functionality: histogram the STS digi time within each event.
 ** To be expanded for other detector systems and probably more QA figures.
 ** The histograms are published to the THttpServer.
 **/
class CbmTaskDigiEventQa : public FairTask {

public:
  /** @brief Constructor **/
  CbmTaskDigiEventQa();


  /** @brief Copy constructor (disabled) **/
  CbmTaskDigiEventQa(const CbmTaskDigiEventQa&) = delete;


  /** @brief Destructor **/
  virtual ~CbmTaskDigiEventQa();


  /** @brief Configuration
   ** @param config  Reconstruction configuration
   **
   ** Histograms are created with limits adjusted to the windows use by the event builder.
   **/
  void Config(const CbmRecoConfig& config);


  /** @brief Task execution **/
  virtual void Exec(Option_t* opt);


  /** @brief Finish timeslice **/
  virtual void Finish();


  /** @brief Assignment operator (disabled) **/
  CbmTaskDigiEventQa& operator=(const CbmTaskDigiEventQa&) = delete;


private:  // methods
  /** @brief Task initialisation **/
  virtual InitStatus Init();


private:                                               // members
  const std::vector<CbmDigiEvent>* fEvents = nullptr;  //! Input data (events)
  size_t fNumTs                            = 0;        ///< Number of processed timeslices
  size_t fNumEvents                        = 0;        ///< Number of analysed events
  size_t fNumDigis                         = 0;        ///< Number of analysed digis
  double fExecTime                         = 0.;       ///< Execution time [s]
  TH1F* fHistDigiTimeSts                   = nullptr;  //!


  ClassDef(CbmTaskDigiEventQa, 1);
};

#endif /* CBMTASKDIGIEVENTQA_H */
