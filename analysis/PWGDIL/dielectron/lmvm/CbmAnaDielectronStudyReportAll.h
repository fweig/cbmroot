/** @file CbmAnaDielectronStudyReportAll.h
  * @copyright Copyright (C) 2012-2015 UGiessen, JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Semen Lebedev [orginator], Elena Lebedeva **/

#ifndef CBM_ANA_DIELECTRON_STUDY_REPORT_ALL
#define CBM_ANA_DIELECTRON_STUDY_REPORT_ALL

#include "CbmStudyReport.h"

/**
 * \class CbmAnaDielectronStudyReportAll
 * \brief Creates study report for LMVM analysis.
 *
 */
class CbmAnaDielectronStudyReportAll : public CbmStudyReport {
public:
  /**
    * \brief Constructor.
    */
  CbmAnaDielectronStudyReportAll();

  /**
    * \brief Destructor.
    */
  virtual ~CbmAnaDielectronStudyReportAll();

protected:
  /**
    * \brief Inherited from CbmLitStudyReport.
    */
  void Create();

  /**
   * \brief Inherited from CbmLitStudyReport.
   */
  void Draw();

  /**
   * \brief Draw Invariant mass of combinatorial BG.
   */
  void DrawBgMinv();

  /**
   * \brief Draw S/Bg ratio vs invariant mass.
   */
  void DrawSBgMinv();
};

#endif
