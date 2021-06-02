/** @file CbmTransportConfig.h
  * @copyright Copyright (C) 2021 National Research Nuclear University MEPhI (Moscow Engineering Physics Institute), Moscow
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Oleg Golosov [committer] **/

#pragma once

#include "CbmConfigBase.h"

class CbmTransport;
class CbmSetup;

class CbmTransportConfig : public CbmConfigBase<CbmTransportConfig, CbmTransport> {

public:
  using TagSet_t = CbmConfigBase<CbmTransportConfig, CbmTransport>::TagSet_t;

  static std::string GetModuleTag();
  static TagSet_t GetValidationTags();
  static TagSet_t GetAcceptedGenerators();
  static bool LoadImpl(CbmTransport& obj, const pt::ptree& moduleTree);
  static bool SetIO(CbmTransport& obj, const pt::ptree& moduleTree);
  static bool SetTarget(CbmTransport& obj, const pt::ptree& moduleTree);
  static bool SetBeamProfile(CbmTransport& obj, const pt::ptree& moduleTree);
  static bool SetTransportParameters(CbmTransport& obj, const pt::ptree& moduleTree);
  static bool SetGeometry(CbmTransport& obj, const pt::ptree& moduleTree);
  static bool SetGeometry(CbmSetup* setup, const pt::ptree& moduleTree);
  static bool SetStackFilter(CbmTransport& obj, const pt::ptree& moduleTree);

  ClassDef(CbmTransportConfig, 1);
};
