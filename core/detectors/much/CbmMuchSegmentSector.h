/* Copyright (C) 2012-2020 Petersburg Nuclear Physics Institute named by B.P.Konstantinov of National Research Centre "Kurchatov Institute", Gatchina
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Vikas Singhal, Evgeny Kryshen [committer], Florian Uhlig */

/** CbmMuchSegmentSector.h
 *@author Vikas Singhal <vikas@vecc.gov.in>
 *@since 07.03.16
 *@author Evgeny Kryshen <e.kryshen@gsi.de>
 *@since 05.02.12
 *@version 1.0
 *
 * class for making parameter file for MUCH digitizer
 * sector version
 */

#ifndef CbmMuchSegmentSector_H
#define CbmMuchSegmentSector_H 1

#include <FairTask.h>  // for FairTask, InitStatus

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Double_t, Bool_t
#include <TString.h>     // for TString
#include <typeinfo>      // for bad_cast

#include <fstream>  // for string, getline, char_traits, basic_istream
#include <map>      // for map
#include <vector>   // for vector

class CbmGeoMuchPar;
class CbmMuchLayerSide;
class CbmMuchModuleGemRadial;
class CbmMuchSectorRadial;
class TObjArray;

class CbmMuchSegmentSector : public FairTask {

public:
  /** Default constructor. */
  CbmMuchSegmentSector();

  /** Destructor. */
  virtual ~CbmMuchSegmentSector();

  /** Standard constructor.
     * @param inputFile     Name of the input parameters file
     * @param digiFileName  Name of the output file with segmentation parameters
     * for C++11 compatible mode char* changed to TString.
     */
  CbmMuchSegmentSector(TString inputFile, TString digiFileName, Int_t flag);

  /** Initialization. */
  virtual InitStatus Init();

  /** Performs the segmentation of all stations. */
  void SegmentMuch();

  void DebugSwitchOn() { fDebug = 1; }

private:
  CbmGeoMuchPar* fGeoPar;            // Geometry parameters container
  Int_t fNStations;                  // Number of stations
  Int_t fFlag;                       // falg for geometry
  TObjArray* fStations;              // Array of stations
  TString fInputFileName;            // Name of the file with input parameters
  TString fDigiFileName;             // Name of the file with segmentation parameters
  std::map<Int_t, Int_t> fNRegions;  // Map from a station index to a number of circled regions in the station
  std::map<Int_t, std::vector<Double_t>> fRadii;  // Map from a station index to a vector of circled regions radii
  std::map<Int_t, std::vector<Double_t>>
    fAngles;  // Map from a station index to a vector of single pad angles for given regions
  std::map<Int_t, std::vector<Double_t>>
    fSecLx;  // Map from a station index to a vector of sector widths for each region
  std::map<Int_t, std::vector<Double_t>>
    fSecLy;                                    // Map from a station index to a vector of sector lengths for each region
  std::map<Int_t, Int_t> fNChannels;           // Map from a station index to a number of channels per sector
  std::map<Int_t, std::vector<Int_t>> fNCols;  // Map from a station index to a vector of number of columns in a sector
  std::map<Int_t, std::vector<Int_t>> fNRows;  // Map from a station index to a vector of number of rows in a sector
  Int_t fDebug;                                // Debug info switch

  /** Get parameter containers **/
  virtual void SetParContainers();

  /**
     * Performs segmentation of the given layer side.
     * @param layerSide layer side to segment
     * @return number of segmented sectors
     */
  Int_t SegmentLayerSide(CbmMuchLayerSide* layerSide);

  /**
     * Performs segmentation of the given module.
     * @param module          Module to segment
     * @param useModuleDesign Whether module design is used
     */
  Int_t SegmentModule(CbmMuchModuleGemRadial* module, Bool_t useModuleDesign);

  /**
     * Performs segmentation of the given sector in the module.
     * @param module  Module which contains the given sector
     * @param sector  Sector to segment
     */
  void SegmentSector(CbmMuchModuleGemRadial* module, CbmMuchSectorRadial* sector);

  /** Reads input parameters for the segmentation. */
  void ReadInputFile();

  /** Draw segmentation */
  void DrawSegmentation();

  // -------------------- Methods for working with strings --------------------------
  void Trim(std::string& str)
  {
    std::string::size_type pos1 = str.find_first_not_of(' ');
    std::string::size_type pos2 = str.find_last_not_of(' ');
    str =
      str.substr(pos1 == std::string::npos ? 0 : pos1, pos2 == std::string::npos ? str.length() - 1 : pos2 - pos1 + 1);
  }

  Bool_t IsDummyLine(std::string& str)
  {
    Trim(str);
    return str[0] == '#' || str.length() == 0 || str[0] == '\0' || str[0] == '\n';
  }

  void OmitDummyLines(std::ifstream& infile, std::string& str)
  {
    getline(infile, str);
    while (IsDummyLine(str))
      getline(infile, str);
  }

  std::vector<std::string>& Split(const std::string& s, char delim, std::vector<std::string>& elems)
  {
    std::stringstream ss(s);
    std::string item;
    while (getline(ss, item, delim)) {
      if (item.length() != 0) elems.push_back(item);
    }
    return elems;
  }


  std::vector<std::string> Split(const std::string& s, char delim)
  {
    std::vector<std::string> elems;
    return Split(s, delim, elems);
  }

  template<class T>
  void StrToNum(std::string& str, T& number)
  {
    try {
      std::stringstream ss(str);
      if ((ss >> number).fail() || !(ss >> std::ws).eof()) throw std::bad_cast();
    }
    catch (const std::bad_cast& exc) {
      Fatal("", "Invalid cast.\n");
    }
  }
  // --------------------------------------------------------------------------------

  CbmMuchSegmentSector(const CbmMuchSegmentSector&);
  CbmMuchSegmentSector& operator=(const CbmMuchSegmentSector&);

  ClassDef(CbmMuchSegmentSector, 1)
};

#endif
