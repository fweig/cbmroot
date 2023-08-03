/*
 * NicaHTMLDiv.h
 *
 *  Created on: 8 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO_FEATURES_IO_HTML_NICAHTMLDIV_H_
#define NICAFEMTO_FEATURES_IO_HTML_NICAHTMLDIV_H_

#include "NicaHtmlObject.h"
class NicaHtmlDiv : public NicaHtmlObject {
 public:
  NicaHtmlDiv();
  NicaHtmlDiv(TString id, TString className, TString style);
  TString ToString() const;
  virtual ~NicaHtmlDiv();
  NicaHtmlDiv(const NicaHtmlDiv& other) = default;
  NicaHtmlDiv& operator=(const NicaHtmlDiv& other) = default;
  ClassDef(NicaHtmlDiv, 1)
};
#endif /* NICAFEMTO_FEATURES_IO_HTML_NICAHTMLDIV_H_ */
