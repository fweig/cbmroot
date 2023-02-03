/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CbmQaTable.cxx
/// \brief  Implementation of CbmQaTable class
/// \author Sergei Zharko
/// \data   24.03.2022
///
/// Provides a ROOT Class to handle a numeric table
///

#include "CbmQaTable.h"

#include "TAxis.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

// TODO: Insert class info, try __PRETTY_FUNCTION__ and inline function for its modification (S.Zharko)

//------------------------------------------------------------------------------------------------------------------------
//
CbmQaTable::CbmQaTable(const char* name, const char* title, Int_t nRows, Int_t nCols)
  : TH2D(name, title, nCols, 0., static_cast<Double_t>(nCols), nRows, 0., static_cast<Double_t>(nRows))
  , fNcols(nCols)
  , fNrows(nRows)
{
  //
  // Setup default style of the table
  TH2D::SetStats(kFALSE);
  TH2D::SetOption("textX+");
  TH2D::GetXaxis()->SetTickLength(0.);
  TH2D::GetYaxis()->SetTickLength(0.);
  TH2D::GetXaxis()->SetLabelFont(kDefaultFontStyle);
  TH2D::GetYaxis()->SetLabelFont(kDefaultFontStyle);

  // Define basic names of rows and columns
  for (Int_t iRow = 1; iRow <= fNrows; ++iRow) {
    //TH2D::GetYaxis()->SetBinLabel(fNrows - iRow + 1, TString::Format("row %d", iRow).Data());
    TH2D::GetYaxis()->SetBinLabel(fNrows - iRow + 1, "");
  }
  for (Int_t iCol = 1; iCol <= fNcols; ++iCol) {
    TH2D::GetXaxis()->SetBinLabel(iCol, TString::Format("col %d", iCol).Data());
  }
}
//
//------------------------------------------------------------------------------------------------------------------------
//
CbmQaTable::~CbmQaTable() {}
//
//------------------------------------------------------------------------------------------------------------------------
//
Double_t CbmQaTable::GetCell(Int_t iRow, Int_t iCol) const { return TH2D::GetBinContent(iCol + 1, fNrows - iRow); }
//
//------------------------------------------------------------------------------------------------------------------------
//
Double_t CbmQaTable::GetCellError(Int_t iRow, Int_t iCol) const { return TH2D::GetBinError(iCol + 1, fNrows - iRow); }
//
//------------------------------------------------------------------------------------------------------------------------
//
void CbmQaTable::SetCell(Int_t iRow, Int_t iCol, Double_t content, Double_t error)
{
  TH2D::SetBinContent(iCol + 1, fNrows - iRow, content);
  TH2D::SetBinError(iCol + 1, fNrows - iRow, error);
}
//
//------------------------------------------------------------------------------------------------------------
//
void CbmQaTable::SetNamesOfCols(const std::vector<std::string>& names)
{
  Int_t nEntries = (fNcols > static_cast<Int_t>(names.size())) ? static_cast<Int_t>(names.size()) : fNcols;
  // TODO: Possibly, we need a restriction on the names size (S.Zharko)
  for (Int_t iCol = 1; iCol <= nEntries; ++iCol) {
    TH2D::GetXaxis()->SetBinLabel(iCol, names[iCol - 1].c_str());
  }
}
//
//------------------------------------------------------------------------------------------------------------------------
//
void CbmQaTable::SetNamesOfRows(const std::vector<std::string>& names)
{
  Int_t nEntries = (fNrows > static_cast<Int_t>(names.size())) ? static_cast<Int_t>(names.size()) : fNrows;
  // TODO: Possibly, we need a restriction on the names size (S.Zharko)
  for (Int_t iRow = 1; iRow <= nEntries; ++iRow) {
    TH2D::GetYaxis()->SetBinLabel(fNrows - iRow + 1, names[iRow - 1].c_str());
  }
}
//
//------------------------------------------------------------------------------------------------------------------------
//
std::string CbmQaTable::ToString(int prec) const
{
  std::stringstream aStream;
  aStream.setf(std::ios::fixed);
  aStream.setf(std::ios::showpoint);
  aStream.setf(std::ios::left);
  aStream.precision(prec);

  aStream << "Table: " << GetTitle() << '\n';
  aStream << std::setw(fColWidth) << std::setfill(' ') << ' ' << ' ';

  for (Int_t iCol = 0; iCol < fNcols; ++iCol) {
    std::string entry = std::string(this->GetXaxis()->GetBinLabel(iCol + 1));
    if (Int_t(entry.size()) + 3 > fColWidth) { entry = entry.substr(0, fColWidth - 3) + "..."; }
    aStream << std::setw(fColWidth) << std::setfill(' ') << entry << ' ';
  }
  aStream << '\n';

  for (Int_t iRow = 0; iRow < fNrows; ++iRow) {
    // Print row title
    std::string entry = std::string(GetYaxis()->GetBinLabel(fNrows - iRow));
    if (static_cast<Int_t>(entry.size()) > fColWidth) { entry = entry.substr(0, fColWidth - 3) + "..."; }
    aStream << std::setw(fColWidth) << std::setfill(' ') << entry << ' ';

    for (Int_t iCol = 0; iCol < fNcols; ++iCol) {
      aStream << std::setw(fColWidth) << std::setfill(' ') << GetCell(iRow, iCol) << ' ';
    }
    aStream << '\n';
  }
  return aStream.str();
}
//
//------------------------------------------------------------------------------------------------------------------------
//
void CbmQaTable::ToTextFile(const std::string& fileName, std::ios_base::openmode mode) const
{
  std::ofstream fileOut(fileName, mode);
  fileOut << (*this);
  fileOut.close();
}
//
//------------------------------------------------------------------------------------------------------------------------
//
void CbmQaTable::SetTextSize(Float_t size)
{
  TH2D::SetMarkerSize(size / kDefaultTextSize);
  TH2D::GetXaxis()->SetLabelSize(size);
  TH2D::GetYaxis()->SetLabelSize(size);
}

//
//------------------------------------------------------------------------------------------------------------------------
//
std::ostream& operator<<(std::ostream& out, const CbmQaTable& aTable)
{
  out << aTable.ToString(3);
  return out;
}
//
//------------------------------------------------------------------------------------------------------------------------
//
