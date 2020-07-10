/**
 * \file CbmTextReportElement.cxx
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
#include "CbmTextReportElement.h"

#include <iomanip>  // for operator<<, setfill, setw
#include <sstream>  // for string, basic_ostream, stringstream
#include <string>   // for char_traits

using std::endl;
using std::left;
using std::right;
using std::setfill;
using std::setw;
using std::string;
using std::stringstream;
using std::vector;

CbmTextReportElement::CbmTextReportElement() : fColW(25) {}

CbmTextReportElement::~CbmTextReportElement() {}

string CbmTextReportElement::TableBegin(const string& caption,
                                        const vector<string>& colNames) const {
  stringstream ss;
  ss << caption << endl;
  ss << right;
  ss << setfill('_') << setw(colNames.size() * fColW) << "_" << endl;
  ss << setfill(' ');
  for (UInt_t i = 0; i < colNames.size(); i++) {
    ss << FormatCell(colNames[i]);  //setw(fColW) << colNames[i];
  }
  ss << endl;
  ss << setfill('_') << setw(colNames.size() * fColW) << "_" << endl;
  return ss.str();
}

string CbmTextReportElement::TableEnd() const {
  stringstream ss;
  ss << endl;
  return ss.str();
}

string CbmTextReportElement::TableEmptyRow(Int_t nofCols,
                                           const string& name) const {
  stringstream ss;
  ss << setfill('-') << left;
  ss << setw(nofCols * fColW) << name << endl;
  ss << setfill(' ') << left;
  return ss.str();
}

string CbmTextReportElement::TableRow(const vector<string>& row) const {
  stringstream ss;
  ss << right;
  for (UInt_t i = 0; i < row.size(); i++) {
    ss << FormatCell(row[i]);  //setw(fColW) << row[i];
  }
  ss << endl;
  return ss.str();
}

string CbmTextReportElement::Image(const string& /*title*/,
                                   const string& /*file*/) const {
  return "";
}

string CbmTextReportElement::DocumentBegin() const {
  stringstream ss;
  ss << "------------------------------------------------" << endl;
  return ss.str();
}

string CbmTextReportElement::DocumentEnd() const { return ""; }

string CbmTextReportElement::Title(Int_t /*size*/, const string& title) const {
  return title;
}

string CbmTextReportElement::FormatCell(const string& cell) const {
  if (static_cast<Int_t>(cell.size()) <= fColW) {
    stringstream ss;
    ss << setw(fColW) << cell;
    return ss.str();
  } else {
    string str = cell;
    str.resize(fColW - 3);
    str.insert(fColW - 3, "...");
    return str;
  }
}

ClassImp(CbmTextReportElement)
