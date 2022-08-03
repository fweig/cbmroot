/* Copyright (C) 2022-2022 UGiessen/GSI, Giessen/Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

void create_rich_geo()
{
  RichGeoCreator* geoCreator = new RichGeoCreator();
  geoCreator->SetGeoName("rich_v22b");
  geoCreator->SetAddShieldingBox(false);
  geoCreator->SetVolumeColors(false);
  geoCreator->Create();
}
