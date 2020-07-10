/**
 * \file CbmReportElement.h
 * \brief Abstract class for basic report elements (headers, tables, images etc.).
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
#ifndef CBMREPORTELEMENT_H_
#define CBMREPORTELEMENT_H_

#include <Rtypes.h>   // for THashConsistencyHolder, ClassDef
#include <TObject.h>  // for TObject

#include <string>  // for string
#include <vector>  // for vector


/**
 * \class CbmReportElement
 *
 * \brief Abstract class for basic report elements (headers, tables, images etc.).
 *
 * Each concrete implementation for report elements has to implement
 * this interface (e.g. Latex, text, HTML). Report has to be written
 * using functionality of this interface class in order to be able
 * to automatically produce reports in different representations
 * (e.g. Latex, text, HTML).
 *
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 *
 */
class CbmReportElement : public TObject {
public:
  /**
    * \brief Constructor.
    */
  CbmReportElement() {}

  /**
    * \brief Destructor.
    */
  virtual ~CbmReportElement() {}

  /**
    * \brief Return string with table open tag.
    * \param[in] caption Table caption.
    * \param[in] colNames Names of the columns in table.
    * \return String with table open tag.
    */
  virtual std::string
  TableBegin(const std::string& caption,
             const std::vector<std::string>& colNames) const = 0;

  /**
    * \brief Return string with table close tag.
    * \return String with table close tag.
    */
  virtual std::string TableEnd() const = 0;

  /**
    * \brief Return string with table row which spans over all columns.
    * \param[in] nofCols number of columns in table.
    * \param[in] name Name of the row.
    * \return string with table row which spans over all columns.
    */
  virtual std::string TableEmptyRow(int nofCols,
                                    const std::string& name) const = 0;

  /**
    * \brief Return string with table row tags.
    * \param[in] data Array of strings with data for each cell in a row.
    * \return string with table row tags.
    */
  virtual std::string TableRow(const std::vector<std::string>& row) const = 0;

  /**
    * \brief Return string with image tags.
    * \param[in] title Title of the image.
    * \param[in] file Name of the image file.
    * \return string with image tags.
    */
  virtual std::string Image(const std::string& title,
                            const std::string& file) const = 0;

  /**
    * \brief Return string with open tags for document.
    * \return string with open tags for document.
    */
  virtual std::string DocumentBegin() const = 0;

  /**
    * \brief Return string with close tags of the document.
    * \return string with close tags of the document.
    */
  virtual std::string DocumentEnd() const = 0;

  /**
    * \brief Return string with title.
    * \param[in] size Size of the title. [0-5]. 0 is the largest size.
    * \param[in] title Title string.
    * \return string with subtitle.
    */
  virtual std::string Title(int size, const std::string& title) const = 0;

  ClassDef(CbmReportElement, 1)
};

#endif /* CBMREPORTELEMENT_H_ */
