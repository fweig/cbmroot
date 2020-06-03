/**
 * \file CbmLatexReportElement.h
 * \brief Implementation of CbmReportElement for Latex output.
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
#ifndef CBMLATEXREPORTELEMENT_H_
#define CBMLATEXREPORTELEMENT_H_

#include <Rtypes.h>            // for THashConsistencyHolder, ClassDef

#include <string>              // for string
#include <vector>              // for vector

#include "CbmReportElement.h"  // for CbmReportElement

/**
 * \class CbmLatexReportElement
 * \brief Implementation of CbmLitReportElement for Latex output.
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
class CbmLatexReportElement: public CbmReportElement
{
public:
   /**
    * \brief Constructor.
    */
   CbmLatexReportElement();

   /**
    * \brief Destructor.
    */
   virtual ~CbmLatexReportElement();

   /**
    * \brief Inherited from CbmReportElement.
    */
   virtual std::string TableBegin(
         const std::string& caption,
         const std::vector<std::string>& colNames) const;

   /**
    * \brief Inherited from CbmReportElement.
    */
   virtual std::string TableEnd() const;

   /**
    * \brief Inherited from CbmReportElement.
    */
   virtual std::string TableEmptyRow(
         int nofCols,
         const std::string& name) const;

   /**
    * \brief Inherited from CbmReportElement.
    */
   virtual std::string TableRow(
         const std::vector<std::string>& row) const;

   /**
    * \brief Inherited from CbmReportElement.
    */
   virtual std::string Image(
         const std::string& title,
         const std::string& file) const;

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
   virtual std::string Title(
         int size,
         const std::string& title) const;

   ClassDef(CbmLatexReportElement, 1)
};

#endif /* CBMLATEXREPORTELEMENT_H_ */
