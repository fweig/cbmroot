/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#include "Reco.h"

#include <xpu/host.h>

#include "config/Yaml.h"
#include "util/TimingsFormat.h"

using namespace cbm::algo;

Reco::Reco() {}
Reco::~Reco() {}

void Reco::Init(const Options& opts)
{
  if (fInitialized) { throw std::runtime_error("Chain already initialized"); }

  fContext.opts = opts;
  SetContext(&fContext);
  fUnpack.SetContext(&fContext);
  fStsHitFinder.SetContext(&fContext);

  xpu::device_prop props {xpu::device::active()};
  LOG(info) << "Running CBM Reco on Device " << props.name();

  // Reco Params
  fs::path recoParamsPath = opts.ParamsDir() / "Reco.yaml";
  YAML::Node yaml         = YAML::LoadFile(recoParamsPath.string());
  fContext.recoParams     = config::Read<RecoParams>(yaml);

  // STS Unpacker
  // yaml = YAML::LoadFile(opts.ParamsDir() / "StsReadout.yaml");
  // sts::ReadoutPars readoutConfig = config::Read<sts::ReadoutPars>(yaml);
  StsReadoutConfig readoutConfig;
  fUnpack.Init(readoutConfig);

  // STS Hitfinder
  fs::path stsHitfinderParamsPath    = opts.ParamsDir() / "StsHitfinder.yaml";
  yaml                               = YAML::LoadFile(stsHitfinderParamsPath.string());
  sts::HitfinderPars hitFinderConfig = config::Read<sts::HitfinderPars>(yaml);
  hitFinderConfig.landauTable        = sts::LandauTable::FromFile(opts.ParamsDir() / "LandauWidthTable.txt");
  fStsHitFinder.SetParameters(hitFinderConfig);

  fInitialized = true;

  xpu::push_timer("Reco");
}

void Reco::Run(const fles::Timeslice& ts)
{
  if (!fInitialized) { throw std::runtime_error("Chain not initialized"); }

  for (uint64_t comp = 0; comp < ts.num_components(); comp++) {
    xpu::t_add_bytes(ts.size_component(comp));
  }

  xpu::push_timer(fmt::format("TS {}", ts.index()));

  LOG(info) << fair::Logger::startColor(fair::Logger::Color::fgRed) << ">>> Processing TS " << ts.index()
            << fair::Logger::endColor();

  xpu::set<cbm::algo::Params>(Params());

  std::vector<CbmStsDigi> digis;
  switch (Params().sts.unpackMode) {
    case RecoParams::UnpackMode::XPU:
      // digis = fUnpackXpu.Exec(ts);
      throw std::runtime_error("XPU unpacker currently not implemented");
      break;
    default:
    case RecoParams::UnpackMode::CPU: digis = fUnpack.Run(ts); break;
  }
  fStsHitFinder(digis);

  xpu::timings ts_times = xpu::pop_timer();

  PrintTimings(ts_times);
}

void Reco::Finalize()
{
  // Pop timer that was started in Init()
  xpu::timings t = xpu::pop_timer();
  if (Opts().CollectKernelTimes()) {
    LOG(info) << MakeReportSubtimers("Run Summary", fTimesliceTimesAcc) << "\n" << MakeReportSummary("Total", t);
  }
  else {
    LOG(info) << "Total Processing time (Wall): " << t.wall() << " ms";
  }
}

void Reco::PrintTimings(xpu::timings& timings)
{
  if (Opts().CollectKernelTimes()) {
    LOG(info) << MakeReportSubtimers("TS timings", timings) << "\n" << MakeReportSummary("Total", timings);
    fTimesliceTimesAcc.merge(timings);
  }
  else {
    LOG(info) << "TS Processing time (Wall): " << timings.wall() << " ms";
  }
}
