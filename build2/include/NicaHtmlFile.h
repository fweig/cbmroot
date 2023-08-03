/*
 * NicaHTMLFile.h
 *
 *  Created on: 8 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO_FEATURES_IO_HTML_FILE_H_
#define NICAFEMTO_FEATURES_IO_HTML_FILE_H_

#include <TObject.h>
#include <TString.h>

#include "NicaHtmlCore.h"
#include "NicaHtmlObject.h"

class NicaHtmlFile : public TObject {
 protected:
  TString fPath;
  TString fRelativePath;
  TString fDir;
  TString fBody;
  Bool_t fSaved;

 public:
  /**
   *
   * @param filename name of the file
   * @param mainFile tell if this is the "main file". Main file means that
   * all javascripts and css are copied to the "main file's" directory. If
   * mainFile is fale then code will not copy those fille but will try to use
   * relative path to "main file"
   */
  NicaHtmlFile(TString filename = "", Bool_t mainFile = kTRUE);
  void AddContent(const NicaHtmlObject& obj) {
    fBody = fBody + obj.ToString();
  };
  void AddStringContent(const TString str) { fBody = fBody + str; };
  virtual void Save();
  virtual ~NicaHtmlFile();
  ClassDef(NicaHtmlFile, 1)
};
#endif /* NICAFEMTO_FEATURES_IO_HTML_FILE_H_ */
