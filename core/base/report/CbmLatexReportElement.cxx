/* Copyright (C) 2011-2020 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev, Andrey Lebedev [committer] */

/**
 * \file CbmLatexReportElement.cxx
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 */
#include "CbmLatexReportElement.h"

#include <sstream>  // for string, operator<<, basic_ostream, endl
#include <string>   // for operator+, char_traits, operator!=

using std::endl;
using std::string;
using std::stringstream;
using std::vector;

CbmLatexReportElement::CbmLatexReportElement() {}

CbmLatexReportElement::~CbmLatexReportElement() {}

string CbmLatexReportElement::TableBegin(const string& caption, const vector<string>& colNames) const
{
  string st = "\\begin{table}[h] \n";
  st += "\\centering";
  if (caption != "") st += "\\caption{" + caption + "} \n";
  st += "\\begin{tabular}{|";
  for (unsigned int i = 0; i < colNames.size(); i++) {
    st += "c|";
  }
  st += "} \\hline \n";
  // names start from the second column
  for (unsigned int i = 0; i < colNames.size(); i++) {
    st += (i < (colNames.size() - 1)) ? (colNames[i] + " & ") : (colNames[i]);
  }
  st += "\\\\ \\hline \n";
  return st;
}

string CbmLatexReportElement::TableEnd() const { return "\\end{tabular} \\end{table}"; }

string CbmLatexReportElement::TableEmptyRow(int nofCols, const string& name) const
{
  stringstream ss;
  ss << "\\multicolumn{" << nofCols << "}{|c|}{" << name << "} \\\\ \\hline" << endl;
  return ss.str();
}

string CbmLatexReportElement::TableRow(const vector<string>& row) const
{
  string st;
  for (unsigned int i = 0; i < row.size(); i++) {
    st += (i < (row.size() - 1)) ? (row[i] + " & ") : (row[i]);
  }
  st += " \\\\ \\hline \n";
  return st;
}

string CbmLatexReportElement::Image(const string& title, const string& file) const
{
  stringstream ss;

  ss << "\\begin{figure}[h]" << endl;
  ss << "\\centering" << endl;
  ss << "\\includegraphics[width=7cm]{" << file << ".eps}" << endl;
  ss << "\\caption{" << title << "}" << endl;
  ss << "\\end{figure}" << endl;
  return ss.str();
}

string CbmLatexReportElement::DocumentBegin() const
{
  string str = "\\documentclass[a4paper,14pt]{article}";
  str += "\\usepackage{amssymb}";
  str += "\\setlength\\oddsidemargin{-2cm}";
  str += "\\setlength\\evensidemargin{-2cm}";
  str += "\\setlength\\textwidth{17cm}";
  str += "\\setlength\\topmargin{0cm}";
  str += "\\usepackage[dvips]{graphicx}";
  str += "\\begin{document}";

  return str;
}

string CbmLatexReportElement::DocumentEnd() const { return "\\end{document}"; }

string CbmLatexReportElement::Title(int size, const string& title) const
{
  string st = "\\";
  for (int i = 0; i < size; i++)
    st += "sub";
  st += "title{" + title + "}";
  return st;
}

ClassImp(CbmLatexReportElement)
