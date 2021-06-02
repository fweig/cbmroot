/** @file CbmRichDigiMapManager.h
  * @copyright Copyright (C) 2015-2020 GSI/JINR-LIT
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors slebedev, Andrey Lebedev [committer], Florian Uhlig, Semen Lebedev **/

/*
 * CbmRichDigiMap.h
 *
 *  Created on: Dec 17, 2015
 *      Author: slebedev
 */

#ifndef RICH_DETECTOR_CBMRICHDIGIMAPMANAGER_H_
#define RICH_DETECTOR_CBMRICHDIGIMAPMANAGER_H_

#include <RtypesCore.h>  // for Int_t
#include <TString.h>     // for TString

#include <iostream>  // for string
#include <map>       // for map
#include <vector>    // for vector

class CbmRichPixelData;
class CbmRichPmtData;

class CbmRichDigiMapManager {
private:
  CbmRichDigiMapManager();

public:
  /**
	 * Return Instance of CbmRichGeoManager.
	 */
  static CbmRichDigiMapManager& GetInstance()
  {
    static CbmRichDigiMapManager fInstance;
    return fInstance;
  }

  /*
	 * \brief Return digi address by path to node.
	 */
  Int_t GetPixelAddressByPath(const std::string& path);

  /*
	 * \brief Return CbmRichDataPixel by digi address.
	 */
  CbmRichPixelData* GetPixelDataByAddress(Int_t address);

  /*
	 * \brief Return the addresses of the direct neighbour pixels.
	 * C++11 efficient way to return vector
	 */
  std::vector<Int_t> GetDirectNeighbourPixels(Int_t address);

  /*
	 * \brief Return the addresses of the diagonal neighbour pixels.
	 * C++11 efficient way to return vector
	 */
  std::vector<Int_t> GetDiagonalNeighbourPixels(Int_t address);

  /*
	 * \brief Return random address. Needed for noise digi.
	 */
  Int_t GetRandomPixelAddress();


  /*
     * \brief Return addresses of all pixels
     */
  std::vector<Int_t> GetPixelAddresses();

  /*
     * \brief Return ids for all pmts
     */
  std::vector<Int_t> GetPmtIds();

  /*
     * \brief Return CbmRichDataPmt by id.
     */
  CbmRichPmtData* GetPmtDataById(Int_t id);

public:
  virtual ~CbmRichDigiMapManager();

private:
  std::map<std::string, Int_t> fPixelPathToAddressMap;
  std::map<Int_t, CbmRichPixelData*> fPixelAddressToDataMap;
  std::vector<Int_t> fPixelAddresses;  // vector of all  pixel addresses

  std::map<std::string, Int_t> fPmtPathToIdMap;
  std::map<Int_t, CbmRichPmtData*> fPmtIdToDataMap;
  std::vector<Int_t> fPmtIds;

  int getDetectorSetup(TString const nodePath);

  /*
	 * \brief Initialize maps.
	 */
  void Init();

  /**
	* \brief Copy constructor.
	*/
  CbmRichDigiMapManager(const CbmRichDigiMapManager&);

  /**
	* \brief Assignment operator.
	*/
  CbmRichDigiMapManager& operator=(const CbmRichDigiMapManager&);
};

#endif /* RICH_DETECTOR_CBMRICHDIGIMAPMANAGER_H_ */
