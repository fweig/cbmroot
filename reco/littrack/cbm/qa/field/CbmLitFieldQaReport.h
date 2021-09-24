/* Copyright (C) 2011-2018 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev [committer] */

/**
 * \file CbmLitFieldQaReport.h
 * \brief Creates field QA report.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2011
 */
#ifndef CBMLITFIELDQAREPORT_H_
#define CBMLITFIELDQAREPORT_H_

#include "CbmSimulationReport.h"

#include "TSystem.h"

#include <string>

/**
 * \class CbmLitFieldQaReport
 * \brief Creates field QA report.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2011
 */
class CbmLitFieldQaReport : public CbmSimulationReport {
public:
  /**
     * \brief Constructor.
     */
  CbmLitFieldQaReport();

  /**
     * \brief Destructor.
     */
  virtual ~CbmLitFieldQaReport();

private:
  /**
     * \brief Inherited from CbmSimulationReport.
     */
  virtual void Create();

  /**
     * \brief Inherited from CbmSimulationReport.
     */
  virtual void Draw();

  /**
     * \brief Draw field map components for each slice.
     */
  void DrawFieldSlices();

  /**
     * \brief Draw field map components for RICH PMT plane.
     */
  void DrawFieldRichPmtPlane();


  /**
     * \brief Draw field map components along Z coordinate.
     */
  void DrawFieldAlongZ();

  ClassDef(CbmLitFieldQaReport, 1)
};

#endif /* CBMLITFIELDQAREPORT_H_ */
