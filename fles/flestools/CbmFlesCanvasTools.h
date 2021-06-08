/* Copyright (C) 2019 Facility for Antiproton and Ion Research in Europe, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Pierre-Alain Loizeau [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                      CbmFlesCanvasTools                           -----
// -----               Created 22.10.2019 by P.-A. Loizeau                 -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#ifndef CbmFlesCanvasTools_H
#define CbmFlesCanvasTools_H

#include <string>
#include <vector>

class TPad;
class TCanvas;

class CanvasConfig {
public:
  CanvasConfig();
  CanvasConfig(std::string sName, std::string sTitle, uint32_t uNbPadsX, uint32_t uNbPadsY);

  ~CanvasConfig();

  /// accessors
  inline std::string GetName() const { return fsName; }
  inline std::string GetTitle() const { return fsTitle; }
  inline uint32_t GetNbPads() const { return fuNbPads; }
  inline uint32_t GetNbPadsX() const { return fuNbPadsX; }
  inline uint32_t GetNbPadsY() const { return fuNbPadsY; }
  bool GetGridx(uint32_t uPadIdx) const;
  bool GetGridy(uint32_t uPadIdx) const;
  bool GetLogx(uint32_t uPadIdx) const;
  bool GetLogy(uint32_t uPadIdx) const;
  bool GetLogz(uint32_t uPadIdx) const;
  uint32_t GetNbObjsInPad(uint32_t uPadIdx) const;
  std::string GetObjName(uint32_t uPadIdx, uint32_t uObjIdx) const;
  std::string GetOption(uint32_t uPadIdx, uint32_t uObjIdx) const;

  /// setters
  void SetNbPadsX(uint32_t uNbColumns);
  void SetNbPadsY(uint32_t uNbRows);

  bool SetConfig(uint32_t uPadIdx, bool bGridx, bool bGridy, bool bLogx, bool bLogy, bool bLogz,
                 std::vector<std::string> vsObjName, std::vector<std::string> vsOptions);

private:
  std::string fsName;
  std::string fsTitle;
  uint32_t fuNbPads;
  uint32_t fuNbPadsX;
  uint32_t fuNbPadsY;

  std::vector<bool> fvbGridx;
  std::vector<bool> fvbGridy;
  std::vector<bool> fvbLogx;
  std::vector<bool> fvbLogy;
  std::vector<bool> fvbLogz;
  std::vector<std::vector<std::string>> fvvsObjName;
  std::vector<std::vector<std::string>> fvvsOptions;

  void ResizeFields();
};

/**********************************************************************/
void GetNbPadsXY(TPad* pPad, uint32_t& uNbPadsX, uint32_t& uNbPadsY);

/// Format of Can config is "Name;NbPadX(U);NbPadY(U);ConfigPad1(s);....;ConfigPadXY(s)"
/// Format of Pad config is "GrixX(b),GridY(b),LogX(b),LogY(b),LogZ(b),(HistoName1,DrawOptions1),...,(HistoNameZ,DrawOptionsZ)"
/// Creation
std::string GenerateCanvasConfigString(TCanvas* pCanv);
/// Extraction
CanvasConfig ExtractCanvasConfigFromString(std::string sFullConfig);
/**********************************************************************/

#endif  // CbmFlesCanvasTools_H
