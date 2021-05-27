/**
 * \file CbmTextReportElement.h
 * \brief Implementation of CbmLitReportElement for text output.
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
#ifndef CBMTEXTREPORTELEMENT_H_
#define CBMTEXTREPORTELEMENT_H_

#include "CbmReportElement.h"  // for CbmReportElement

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t

#include <string>  // for string
#include <vector>  // for vector

/**
 * \class CbmTextReportElement
 * \brief Implementation of CbmLitReportElement for text output.
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
class CbmTextReportElement : public CbmReportElement {
public:
  /**
    * \brief Constructor.
    */
  CbmTextReportElement();

  /**
    * \brief Destructor.
    */
  virtual ~CbmTextReportElement();

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
  virtual std::string TableEmptyRow(Int_t nofCols, const std::string& name) const;

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
  virtual std::string Title(Int_t size, const std::string& title) const;

private:
  std::string FormatCell(const std::string& cell) const;

  Int_t fColW;  // column width

  ClassDef(CbmTextReportElement, 1)
};

#endif /* CBMTEXTREPORTELEMENT_H_ */
