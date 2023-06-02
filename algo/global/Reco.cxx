/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#include "Reco.h"

#include <sstream>
#include <string>

#include <xpu/host.h>

#include "config/Yaml.h"
#include "log.hpp"
#include "util/TimingsFormat.h"

using namespace cbm::algo;

Reco::Reco() {}
Reco::~Reco() {}

void Reco::Validate(const Options& opts)
{
  if (!fs::exists(opts.ParamsDir())) {
    throw std::runtime_error("ParamsDir does not exist: " + opts.ParamsDir().string());
  }

  if (opts.HasDetector(Detector::TOF)) { throw std::runtime_error("TOF not implemented yet"); }
}

void Reco::Init(const Options& opts)
{
  if (fInitialized) throw std::runtime_error("Chain already initialized");

  Validate(opts);

  fContext.opts = opts;
  SetContext(&fContext);
  fUnpack.SetContext(&fContext);
  fStsHitFinder.SetContext(&fContext);

  xpu::device_prop props {xpu::device::active()};
  L_(info) << "Running CBM Reco on Device " << props.name();

  // Reco Params
  fs::path recoParamsPath = opts.ParamsDir() / "RecoParams.yaml";
  YAML::Node yaml         = YAML::LoadFile(recoParamsPath.string());

  fContext.recoParams     = config::Read<RecoParams>(yaml);

  for (auto x : Opts().ConfigKeys()) {
    YAML::Node node = YAML::Load(x);
    config::Update(fContext.recoParams, node);
  }
  L_(debug) << "RecoParams:\n" << config::Dump {}(fContext.recoParams);


  // STS Unpacker
  fs::path stsUnpackParamsPath    = opts.ParamsDir() / "StsReadout.yaml";
  yaml                            = YAML::LoadFile(stsUnpackParamsPath.string());
  sts::ReadoutSetup readoutConfig = config::Read<sts::ReadoutSetup>(yaml);
  sts::ReadoutMapping mapping(readoutConfig);
  fUnpack.Init(mapping);

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

  if (!Opts().HasStep(Step::Unpack)) { throw std::runtime_error("Unpacking not enabled."); }

  for (uint64_t comp = 0; comp < ts.num_components(); comp++) {
    xpu::t_add_bytes(ts.size_component(comp));
  }

  xpu::push_timer(fmt::format("TS {}", ts.index()));

  L_(info) << ">>> Processing TS " << ts.index();
  xpu::set<cbm::algo::Params>(Params());

  std::vector<CbmStsDigi> digis;

  if (Opts().HasStep(Step::Unpack)) {
    switch (Params().sts.unpackMode) {
      case RecoParams::UnpackMode::XPU:
        // digis = fUnpackXpu.Exec(ts);
        throw std::runtime_error("XPU unpacker currently not implemented");
        break;
      default:
      case RecoParams::UnpackMode::CPU: digis = fUnpack.Run(ts); break;
    }
  }
  if (Opts().HasStep(Step::LocalReco)) fStsHitFinder(digis);

  xpu::timings ts_times = xpu::pop_timer();

  PrintTimings(ts_times);
}

void Reco::Finalize()
{
  fStsHitFinder.Finalize();

  // Pop timer that was started in Init()
  xpu::timings t = xpu::pop_timer();
  if (Opts().CollectKernelTimes()) {
    L_(info) << MakeReportSubtimers("Run Summary", fTimesliceTimesAcc) << "\n" << MakeReportSummary("Total", t);
  }
  else {
    L_(info) << "Total Processing time (Wall): " << t.wall() << " ms";
  }
}

void Reco::PrintTimings(xpu::timings& timings)
{
  if (Opts().CollectKernelTimes()) {
    L_(info) << MakeReportSubtimers("TS timings", timings) << "\n" << MakeReportSummary("Total", timings);
    fTimesliceTimesAcc.merge(timings);
  }
  else {
    L_(info) << "TS Processing time (Wall): " << timings.wall() << " ms";
  }
}
