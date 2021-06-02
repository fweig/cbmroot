/** @file CbmHtmlReportElement.h
  * @copyright Copyright (C) 2011-2020 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Semen Lebedev, Andrey Lebedev [committer], Florian Uhlig **/

/**
 * \file CbmHtmlReportElement.h
 * \brief Implementation of CbmReportElement for HTML output.
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
#ifndef CBMHTMLREPORTELEMENT_H_
#define CBMHTMLREPORTELEMENT_H_

#include "CbmReportElement.h"  // for CbmReportElement

#include <Rtypes.h>  // for THashConsistencyHolder, ClassDef

#include <string>  // for string
#include <vector>  // for vector

/**
 * \class CbmHtmlReportElement
 * \brief Implementation of CbmReportElement for text output.
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
class CbmHtmlReportElement : public CbmReportElement {
public:
  /**
    * \brief Constructor.
    */
  CbmHtmlReportElement();

  /**
    * \brief Destructor.
    */
  virtual ~CbmHtmlReportElement();

  /**
    * \brief Inherited from CbmReportElement.
    */
  virtual std::string TableBegin(const std::string& caption, const std::vector<std::string>& colNames) const;

  /**
    * \brief Inherited from CbmReportElement.
    */
  virtual std::string TableEnd() const;

  /**
    * \brief Inherited from CbmReportElement.
    */
  virtual std::string TableEmptyRow(int nofCols, const std::string& name) const;

  /**
    * \brief Inherited from CbmReportElement.
    */
  virtual std::string TableRow(const std::vector<std::string>& row) const;

  /**
    * \brief Inherited from CbmReportElement.
    */
  virtual std::string Image(const std::string& title, const std::string& file) const;

  /**
    * \brief Inherited from CbmReportElement.
    */
  virtual std::string DocumentBegin() const;

  /**
    * \brief Inherited from CbmReportElement.
    */
  virtual std::string DocumentEnd() const;

  /**
    * \brief Inherited from CbmReportElement.
    */
  virtual std::string Title(int size, const std::string& title) const;

  ClassDef(CbmHtmlReportElement, 1)
};

#endif /* CBMHTMLREPORTELEMENT_H_ */
