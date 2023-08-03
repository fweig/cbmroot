/*
 * NicaHtmlTable.h
 *
 *  Created on: 8 paź 2020
 *      Author: Daniel Wielanek
 *		E-mail: daniel.wielanek@gmail.com
 *		Warsaw University of Technology, Faculty of Physics
 */
#ifndef NICAFEMTO_FEATURES_IO_HTML_TABLE_H_
#define NICAFEMTO_FEATURES_IO_HTML_TABLE_H_
#include "NicaHtmlObject.h"

class NicaHtmlTable : public NicaHtmlObject {
 public:
  NicaHtmlTable();
  NicaHtmlTable(TString id, TString className, TString style);
  TString ToString() const;
  virtual void AddContent(const NicaHtmlObject& obj);
  virtual ~NicaHtmlTable();
  ClassDef(NicaHtmlTable, 1)
};

class NicaHtmlRow : public NicaHtmlObject {
 public:
  NicaHtmlRow();
  NicaHtmlRow(TString id, TString className, TString style);
  virtual void AddContent(const NicaHtmlObject& obj);
  TString ToString() const;
  virtual ~NicaHtmlRow();
  ClassDef(NicaHtmlRow, 1)
};

class NicaHtmlCell : public NicaHtmlObject {
 protected:
  Int_t fColSpan;
  Int_t fRowSpan;

 public:
  NicaHtmlCell();
  NicaHtmlCell(TString id, TString className, TString style);
  NicaHtmlCell(TString content);
  void SetColSpan(Int_t span) { fColSpan = span; };
  void SetRowSpan(Int_t span) { fRowSpan = span; };
  virtual void AddContent(const NicaHtmlObject& obj);
  TString ToString() const;
  virtual ~NicaHtmlCell();
  ClassDef(NicaHtmlCell, 1)
};

class NicaHtmlCellCol : public NicaHtmlCell {
 public:
  NicaHtmlCellCol();
  NicaHtmlCellCol(TString name, Int_t colspan);
  virtual ~NicaHtmlCellCol();
  ClassDef(NicaHtmlCellCol, 1)
};

class NicaHtmlCellRow : public NicaHtmlCell {
 public:
  NicaHtmlCellRow();
  NicaHtmlCellRow(TString name, Int_t colspan);
  virtual ~NicaHtmlCellRow();
  ClassDef(NicaHtmlCellRow, 1)
};

#endif /* NICAFEMTO_FEATURES_IO_HTML_TABLE_H_ */
