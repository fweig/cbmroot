/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov, Sergei Zharko [committer] */

/// \file   CbmQaTable.h
/// \brief  Definition of CbmQaTable class
/// \author Sergei Zharko
/// \data   24.03.2022
///
/// Provides a ROOT Class to handle a numeric table
///

#ifndef CbmQaTable_h
#define CbmQaTable_h 1
#include "TROOT.h"
#include "TH2D.h"

#include <fstream>
#include <string>

// TODO: We need a method, which sets and gets cell values! (S.Zharko)

class CbmQaTable: public TH2D {
public:
  //
  // CONSTRUCTORS AND DESTRUCTORS
  //
  /// Default constructor 
  CbmQaTable(): TH2D() {}
  /// Constructor from number of rows and columns
  CbmQaTable(const char* name, const char* title, Int_t nRows, Int_t nCols);
  /// Destructor
  virtual ~CbmQaTable();

  /// Dumps table content into a string
  std::string ToString() const;
  /// Dumps table content into a text file. File open mode is also controllable, for example, use 
  /// mode = std::ios_base::app to append the table into an existing file
  void ToTextFile(const std::string& fileName, std::ios_base::openmode mode = std::ios_base::out) const;

  //
  // GETTERS
  //
  /// Gets cell content. Please mind, that the signature and result of this function is different to TH2D::GetBinContent
  Double_t GetCell(Int_t iRow, Int_t iCol) const;
  /// Gets cell error. Please mind, that the signature and result of this function is different to TH2D::GetBinError
  Double_t GetCellError(Int_t iRow, Int_t iCol) const;
  /// Sets number of rows
  Int_t GetNrows() const { return fNrows; }
  /// Sets number of columns
  Int_t GetNcols() const { return fNcols; }
  /// Sets cell content and error. Please mind, that the signature and result of this function
  /// is different to TH2D::SetBinContent and TH2D::SetBinError
  void SetCell(Int_t iRow, Int_t iCol, Double_t content, Double_t error = 0.);
  /// Sets the names of table columns
  void SetNamesOfCols(const std::vector<std::string>& names);
  /// Sets the names of table rows
  void SetNamesOfRows(const std::vector<std::string>& names);
  /// Sets size of the text
  void SetTextSize(Float_t size = 0.03);


  /// Dumps table content into a stream
  friend std::ostream& operator<<(std::ostream& out, const CbmQaTable& aTable);

private:
  // Forbid methods for direct access to the bins
  using TH2D::GetBinContent;
  using TH2D::GetBinError;
  using TH2D::SetBinContent;
  using TH2D::SetBinError;


  // Structural fields
  Int_t fNcols {0};  ///< number of columns in a table
  Int_t fNrows {0};  ///< number of rows in a table
 
  // Some hard-coded constants
  static constexpr Float_t kDefaultTextSize  {0.03};  ///< default size of text
  static constexpr Style_t kDefaultFontStyle {62};    ///< default text style

  static constexpr Int_t kDefaultSetwPar   {12};  ///< default size of entry in std::setw() for columns
  static constexpr Int_t kRowTitlesSetwPar {30};  ///< size of entry in std::setw() for row titles
  static constexpr Int_t kValuesPrecision  {3};   ///< precision of output parameters
  // TODO: Apply this precision and other options to graphical verion of the table

  ClassDef(CbmQaTable, 1);
};

#endif // CbmQaTable_h
