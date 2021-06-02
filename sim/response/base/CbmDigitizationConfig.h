/** @file CbmDigitizationConfig.h
  * @copyright Copyright (C) 2021 National Research Nuclear University MEPhI (Moscow Engineering Physics Institute), Moscow
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Oleg Golosov [committer] **/

#pragma once

#include "CbmConfigBase.h"

class CbmDigitization;

class CbmDigitizationConfig : public CbmConfigBase<CbmDigitizationConfig, CbmDigitization> {

public:
  using TagSet_t = CbmConfigBase<CbmDigitizationConfig, CbmDigitization>::TagSet_t;

  static std::string GetModuleTag();
  static TagSet_t GetValidationTags();
  static bool LoadImpl(CbmDigitization& obj, const pt::ptree& moduleTree);
  static bool SetIO(CbmDigitization& obj, const pt::ptree& moduleTree);
  static bool SetDigitizationParameters(CbmDigitization& obj, const pt::ptree& moduleTree);
  static bool SetGeometry(CbmDigitization& obj, const pt::ptree& moduleTree);

  ClassDef(CbmDigitizationConfig, 1);
};

//#endif
