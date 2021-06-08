/* Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

#include "CbmFlesCanvasTools.h"

#include "TCanvas.h"
#include "TH1.h"

#include <iostream>

/**********************************************************************/
CanvasConfig::CanvasConfig()
  : fsName("")
  , fsTitle("")
  , fuNbPads(0)
  , fuNbPadsX(0)
  , fuNbPadsY(0)
  , fvbGridx()
  , fvbGridy()
  , fvbLogx()
  , fvbLogy()
  , fvbLogz()
  , fvvsObjName()
  , fvvsOptions()
{
}
CanvasConfig::CanvasConfig(std::string sName, std::string sTitle, uint32_t uNbPadsX, uint32_t uNbPadsY)
  : fsName(sName)
  , fsTitle(sTitle)
  , fuNbPads(uNbPadsX * uNbPadsY)
  , fuNbPadsX(uNbPadsX)
  , fuNbPadsY(uNbPadsY)
  , fvbGridx(fuNbPads, false)
  , fvbGridy(fuNbPads, false)
  , fvbLogx(fuNbPads, false)
  , fvbLogy(fuNbPads, false)
  , fvbLogz(fuNbPads, false)
  , fvvsObjName(fuNbPads)
  , fvvsOptions(fuNbPads)
{
}

CanvasConfig::~CanvasConfig()
{
  /// Clear potential leftovers
  fvbGridx.clear();
  fvbGridy.clear();
  fvbLogx.clear();
  fvbLogy.clear();
  fvbLogz.clear();
  for (uint32_t uObj = 0; uObj < fvvsObjName.size(); ++uObj) {
    fvvsObjName[uObj].clear();
    fvvsOptions[uObj].clear();
  }  // for( uint32_t uObj = 0; uObj < fvvsObjName.size(); ++uObj )
  fvvsObjName.clear();
  fvvsOptions.clear();
}

/// accessors
bool CanvasConfig::GetGridx(uint32_t uPadIdx) const
{
  /// Check first if in-boundary
  if (uPadIdx < fuNbPads) { return fvbGridx[uPadIdx]; }  // if( uPadIdx < fuNbPads )
  else {
    std::cerr << "CanvasConfig::GetGridx => Pad index out of bounds, returning false! " << uPadIdx << " VS " << fuNbPads
              << std::endl;
    return false;
  }  // else of if( uPadIdx < fuNbPads )
}
bool CanvasConfig::GetGridy(uint32_t uPadIdx) const
{
  /// Check first if in-boundary
  if (uPadIdx < fuNbPads) { return fvbGridy[uPadIdx]; }  // if( uPadIdx < fuNbPads )
  else {
    std::cerr << "CanvasConfig::GetGridy => Pad index out of bounds, returning false! " << uPadIdx << " VS " << fuNbPads
              << std::endl;
    return false;
  }  // else of if( uPadIdx < fuNbPads )
}
bool CanvasConfig::GetLogx(uint32_t uPadIdx) const
{
  /// Check first if in-boundary
  if (uPadIdx < fuNbPads) { return fvbLogx[uPadIdx]; }  // if( uPadIdx < fuNbPads )
  else {
    std::cerr << "CanvasConfig::GetLogx => Pad index out of bounds, returning false! " << uPadIdx << " VS " << fuNbPads
              << std::endl;
    return false;
  }  // else of if( uPadIdx < fuNbPads )
}
bool CanvasConfig::GetLogy(uint32_t uPadIdx) const
{
  /// Check first if in-boundary
  if (uPadIdx < fuNbPads) { return fvbLogy[uPadIdx]; }  // if( uPadIdx < fuNbPads )
  else {
    std::cerr << "CanvasConfig::GetLogy => Pad index out of bounds, returning false! " << uPadIdx << " VS " << fuNbPads
              << std::endl;
    return false;
  }  // else of if( uPadIdx < fuNbPads )
}
bool CanvasConfig::GetLogz(uint32_t uPadIdx) const
{
  /// Check first if in-boundary
  if (uPadIdx < fuNbPads) { return fvbLogz[uPadIdx]; }  // if( uPadIdx < fuNbPads )
  else {
    std::cerr << "CanvasConfig::GetLogz => Pad index out of bounds, returning false! " << uPadIdx << " VS " << fuNbPads
              << std::endl;
    return false;
  }  // else of if( uPadIdx < fuNbPads )
}
uint32_t CanvasConfig::GetNbObjsInPad(uint32_t uPadIdx) const
{
  /// Check first if in-boundary
  if (uPadIdx < fuNbPads) { return fvvsObjName[uPadIdx].size(); }  // if( uPadIdx < fuNbPads )
  else {
    std::cerr << "CanvasConfig::GetLogz => Pad index out of bounds, returning 0! " << uPadIdx << " VS " << fuNbPads
              << std::endl;
    return 0;
  }  // else of if( uPadIdx < fuNbPads )
}
std::string CanvasConfig::GetObjName(uint32_t uPadIdx, uint32_t uObjIdx) const
{
  /// Check first if in-boundary
  if (uPadIdx >= fuNbPads) {
    std::cerr << "CanvasConfig::GetObjName => Pad index out of bounds, "
                 "returning nullptr! "
              << uPadIdx << " VS " << fuNbPads << std::endl;
    return std::string("nullptr");
  }  // if( uPadIdx >= fuNbPads )

  /// Check if object is in vector boundary
  if (uObjIdx < GetNbObjsInPad(uPadIdx)) {
    return fvvsObjName[uPadIdx][uObjIdx];
  }  // if( uObjIdx < GetNbObjsInPad( uPadIdx ) )
  {
    std::cerr << "CanvasConfig::GetObjName => Object index out of bounds, "
                 "returning nullptr! "
              << "Pad " << uPadIdx << " " << uObjIdx << " VS " << GetNbObjsInPad(uPadIdx) << std::endl;
    return std::string("nullptr");
  }  // else of if( uObjIdx < GetNbObjsInPad( uPadIdx ) )
}
std::string CanvasConfig::GetOption(uint32_t uPadIdx, uint32_t uObjIdx) const
{
  /// Check first if in-boundary
  if (uPadIdx >= fuNbPads) {
    std::cerr << "CanvasConfig::GetObjName => Pad index out of bounds, "
                 "returning nullptr! "
              << uPadIdx << " VS " << fuNbPads << std::endl;
    return std::string("nullptr");
  }  // if( uPadIdx >= fuNbPads )

  /// Check if object is in vector boundary
  if (uObjIdx < GetNbObjsInPad(uPadIdx)) {
    return fvvsOptions[uPadIdx][uObjIdx];
  }  // if( uObjIdx < GetNbObjsInPad( uPadIdx ) )
  {
    std::cerr << "CanvasConfig::GetObjName => Object index out of bounds, "
                 "returning nullptr! "
              << "Pad " << uPadIdx << " " << uObjIdx << " VS " << GetNbObjsInPad(uPadIdx) << std::endl;
    return std::string("nullptr");
  }  // else of if( uObjIdx < GetNbObjsInPad( uPadIdx ) )
}

/// setters
void CanvasConfig::SetNbPadsX(uint32_t uNbColumns)
{
  if (fuNbPadsX != uNbColumns) {
    /// Print warning in case some pads were already defined
    if (0 < fuNbPads)
      std::cout << "CanvasConfig::SetNbPadsX => Warning: Number of pads "
                   "changed, stored configuration cleared!"
                << std::endl;

    fuNbPadsX = uNbColumns;

    /// Update total nb of pads
    fuNbPads = fuNbPadsX * fuNbPadsY;

    /// Re-initialize vectors for pad config
    ResizeFields();
  }  // if( fuNbPadsX != uNbColumns )
}
void CanvasConfig::SetNbPadsY(uint32_t uNbRows)
{
  if (fuNbPadsY != uNbRows) {
    /// Print warning in case some pads were already defined
    if (0 < fuNbPads)
      std::cout << "CanvasConfig::SetNbPadsY => Warning: Number of pads "
                   "changed, stored configuration cleared!"
                << std::endl;

    fuNbPadsY = uNbRows;

    /// Update total nb of pads
    fuNbPads = fuNbPadsX * fuNbPadsY;

    /// Re-initialize vectors for pad config
    ResizeFields();
  }  // if( fuNbPadsY != uNbRows )
}

bool CanvasConfig::SetConfig(uint32_t uPadIdx, bool bGridx, bool bGridy, bool bLogx, bool bLogy, bool bLogz,
                             std::vector<std::string> vsObjName, std::vector<std::string> vsOptions)
{
  if (uPadIdx < fuNbPads) {
    /// Assign PAD config values
    fvbGridx[uPadIdx]    = bGridx;
    fvbGridy[uPadIdx]    = bGridy;
    fvbLogx[uPadIdx]     = bLogx;
    fvbLogy[uPadIdx]     = bLogy;
    fvbLogz[uPadIdx]     = bLogz;
    fvvsObjName[uPadIdx] = vsObjName;
    fvvsOptions[uPadIdx] = vsOptions;
  }  // if( uPadIdx < fuNbPads )
  else {
    std::cerr << "CanvasConfig::SetConfig => Pad index out of bounds! " << uPadIdx << " VS " << fuNbPads << std::endl;
    return false;
  }  // else of if( uPadIdx < fuNbPads )

  return true;
}

void CanvasConfig::ResizeFields()
{
  /// First clear potential leftovers
  fvbGridx.clear();
  fvbGridy.clear();
  fvbLogx.clear();
  fvbLogy.clear();
  fvbLogz.clear();
  for (uint32_t uObj = 0; uObj < fvvsObjName.size(); ++uObj) {
    fvvsObjName[uObj].clear();
    fvvsOptions[uObj].clear();
  }  // for( uint32_t uObj = 0; uObj < fvvsObjName.size(); ++uObj )
  fvvsObjName.clear();
  fvvsOptions.clear();

  /// Resize vectors
  fvbGridx.resize(fuNbPads, false);
  fvbGridy.resize(fuNbPads, false);
  fvbLogx.resize(fuNbPads, false);
  fvbLogy.resize(fuNbPads, false);
  fvbLogz.resize(fuNbPads, false);
  fvvsObjName.resize(fuNbPads);
  fvvsOptions.resize(fuNbPads);
}
/**********************************************************************/

void GetNbPadsXY(TPad* pPad, uint32_t& uNbPadsX, uint32_t& uNbPadsY)
{
  uint32_t uNbPads = 1;  /// start from 1
  uNbPadsX         = 0;  /// start from 0
  uNbPadsY         = 0;  /// start from 0
  pPad->cd();            // set current (mother) pad

  /// Previous Canvas geom
  double_t dPrevX = -1.0;  // relative scale from 0 to 1, go over to avoid problem if not margin
  double_t dPrevY = 2.0;   // relative scale from 0 to 1, go over to avoid problem if not margin

  /// Loop until we run out of Pads
  while (pPad->GetPad(uNbPads)) {
    /// Go in pad to update gPad
    pPad->cd(uNbPads);

    /// Detect unknown columns: Indices scan from left to right => increasing values
    if (dPrevX < gPad->GetXlowNDC()) {
      uNbPadsX++;
      dPrevX = gPad->GetXlowNDC();
    }  // if( dPrevX < gPad->GetXlowNDC() )

    /// Detect unknown rows: Indices scan from top to bottom => decreasing values
    if (dPrevY > gPad->GetYlowNDC()) {
      uNbPadsY++;
      dPrevY = gPad->GetYlowNDC();
    }  // if( dPrevY > gPad->GetYlowNDC() )

    /// Increase pad Index/Counter
    uNbPads++;
  }  // while( pPad->GetPad( uNbPads ) );

  return;
}

/// Format of Can config is "NbPadX(U);NbPadY(U);ConfigPad1(s);....;ConfigPadXY(s)"
/// Format of Pad config is "GrixX(b),GridY(b),LogX(b),LogY(b),LogZ(b),(HistoName1,DrawOptions1),...,(HistoNameZ,DrawOptionsZ)"
std::string GenerateCanvasConfigString(TCanvas* pCanv)
{
  if (nullptr == pCanv) return std::string("");

  /// Add name to config
  std::string sConfig = pCanv->GetName();
  sConfig += ";";

  /// Add title to config string
  sConfig += pCanv->GetTitle();
  sConfig += ";";

  /// Prepare generation of Pads config
  std::string sPadsConfig = "";
  uint32_t uNbPads        = 1;  /// start from 1
  uint32_t uNbPadsX       = 0;  /// start from 0
  uint32_t uNbPadsY       = 0;  /// start from 0
  pCanv->cd();                  // set current (mother) pad

  /// Previous Canvas geom
  double_t dPrevX = -1.0;  // relative scale from 0 to 1, go over to avoid problem if not margin
  double_t dPrevY = 2.0;   // relative scale from 0 to 1, go over to avoid problem if not margin

  /// Loop until we run out of Pads
  while (pCanv->GetPad(uNbPads)) {
    /// Go in pad to update gPad
    pCanv->cd(uNbPads);

    /// Detect unknown columns: Indices scan from left to right => increasing values
    if (dPrevX < gPad->GetXlowNDC()) {
      uNbPadsX++;
      dPrevX = gPad->GetXlowNDC();
    }  // if( dPrevX < gPad->GetXlowNDC() )

    /// Detect unknown rows: Indices scan from top to bottom => decreasing values
    if (dPrevY > gPad->GetYlowNDC()) {
      uNbPadsY++;
      dPrevY = gPad->GetYlowNDC();
    }  // if( dPrevY > gPad->GetYlowNDC() )

    /// Get Pad properties
    /// GridX, GridY, LogX, LogY, LogZ
    sPadsConfig +=
      Form("%d,%d,%d,%d,%d", gPad->GetGridx(), gPad->GetGridy(), gPad->GetLogx(), gPad->GetLogy(), gPad->GetLogz());
    /// Histogram(s) name and draw option
    TObjLink* lnkHist = gPad->GetListOfPrimitives()->FirstLink();

    /// catch case of empty pad
    if (!lnkHist) sPadsConfig += ",(nullptr,nullptr)";

    while (lnkHist) {
      if (nullptr != dynamic_cast<TH1*>(lnkHist->GetObject()))
        sPadsConfig += Form(",(%s,%s)", lnkHist->GetObject()->GetName(), lnkHist->GetOption());
      lnkHist = lnkHist->Next();
    }  // else while( lnkHist ) of if( nullptr == lnkHist )

    /// Add closing semi-colon
    sPadsConfig += ";";

    /// Increase pad Index/Counter
    uNbPads++;
  }  // while( pCanv->GetPad( uNbPads ) )

  sConfig += Form("%u;%u;", uNbPadsX, uNbPadsY);
  sConfig += sPadsConfig;

  return sConfig;
};

/// Format of Can config is "NbPadX(U);NbPadY(U);ConfigPad1(s);....;ConfigPadXY(s)"
/// Format of Pad config is "GrixX(b),GridY(b),LogX(b),LogY(b),LogZ(b),(HistoName1,DrawOptions1),...,(HistoNameZ,DrawOptionsZ)"
CanvasConfig ExtractCanvasConfigFromString(std::string sFullConfig)
{
  /// Temp storage
  std::string sName  = "";
  std::string sTitle = "";
  uint32_t uNbPads   = 0;  /// start from 0
  uint32_t uNbPadsX  = 0;  /// start from 0
  uint32_t uNbPadsY  = 0;  /// start from 0
  bool bGridx        = false;
  bool bGridy        = false;
  bool bLogx         = false;
  bool bLogy         = false;
  bool bLogz         = false;
  std::vector<std::string> vsObjName;
  std::vector<std::string> vsOptions;

  /// Extract General config
  size_t charPosDel;

  /// Name
  charPosDel = sFullConfig.find(';');
  sName      = sFullConfig.substr(0, charPosDel);
  charPosDel++;
  std::string sNext = sFullConfig.substr(charPosDel);

  /// Title
  charPosDel = sNext.find(';');
  sTitle     = sNext.substr(0, charPosDel);
  charPosDel++;
  sNext = sNext.substr(charPosDel);

  /// Nb Pads X
  uNbPadsX = std::stoul(sNext, &charPosDel);
  charPosDel++;
  sNext = sNext.substr(charPosDel);

  /// Nb Pads Y
  uNbPadsY = std::stoul(sNext, &charPosDel);
  charPosDel++;
  sNext = sNext.substr(charPosDel);

  /// Total Nb pads to expect in config
  uNbPads = uNbPadsX * uNbPadsY;

  /// Create output object
  CanvasConfig conf(sName, sTitle, uNbPadsX, uNbPadsY);

  /// Loop on pads
  for (UInt_t uPadIdx = 0; uPadIdx < uNbPads; ++uPadIdx) {
    if (0 == sNext.size()) {
      std::cerr << "ExtractCanvasConfigFromString => Empty configuration string while " << uPadIdx << " over "
                << uNbPads << " pads remain! "
                << "last ones will have default config!" << std::endl;
    }  // if( 0 == sNext.size() )

    /// Extract Pad config
    /// Grid X
    bGridx = std::stoul(sNext, &charPosDel);
    charPosDel++;
    sNext = sNext.substr(charPosDel);

    /// Grid Y
    bGridy = std::stoul(sNext, &charPosDel);
    charPosDel++;
    sNext = sNext.substr(charPosDel);

    /// Log X
    bLogx = std::stoul(sNext, &charPosDel);
    charPosDel++;
    sNext = sNext.substr(charPosDel);

    /// Log Y
    bLogy = std::stoul(sNext, &charPosDel);
    charPosDel++;
    sNext = sNext.substr(charPosDel);

    /// Log Z
    bLogz = std::stoul(sNext, &charPosDel);
    charPosDel++;
    sNext = sNext.substr(charPosDel);

    /// Objects config string
    charPosDel        = sNext.find(';');
    std::string sObjs = sNext.substr(0, charPosDel);

    /// Objects in pad
    size_t charPosOpBrkt = sObjs.find('(');
    while (charPosOpBrkt != std::string::npos) {
      /// Remove all leftover parts before current object config
      sObjs = sObjs.substr(charPosOpBrkt);

      /// Find delimiters for each field
      charPosOpBrkt        = sObjs.find('(');
      size_t charPosComma  = sObjs.find(',');
      size_t charPosClBrkt = sObjs.find(')');

      charPosOpBrkt++;
      charPosComma++;
      charPosClBrkt++;

      /// Extract fields
      std::string sObjName = sObjs.substr(charPosOpBrkt, charPosComma - charPosOpBrkt - 1);
      std::string sObjOpt  = sObjs.substr(charPosComma, charPosClBrkt - charPosComma - 1);

      vsObjName.push_back(sObjName);
      vsOptions.push_back(sObjOpt);

      /// Remove extracted object and comma from config string
      sObjs         = sObjs.substr(charPosClBrkt);
      charPosOpBrkt = sObjs.find('(');
    }  // while( charPosOpBrkt < charPosDel )

    /// Prepare string for next pad if any
    charPosDel = sNext.find(';');
    charPosDel++;
    sNext = sNext.substr(charPosDel);

    /// Load Pad config
    conf.SetConfig(uPadIdx, bGridx, bGridy, bLogx, bLogy, bLogz, vsObjName, vsOptions);

    /// Clear objects vectors
    vsObjName.clear();
    vsOptions.clear();
  }  // for( UInt_t uPadIdx = 0; uPadIdx < uNbPads; ++uPadIdx )

  /// Return full canvas config
  return conf;
}
