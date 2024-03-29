/* Copyright (C) 2017 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Maksym Zyzak [committer] */

#include "CbmL1Counters.h"

#include <cstring>
#include <iostream>

class L1EventEfficiencies {
public:
  L1EventEfficiencies()
    : names()
    , indices()
    , ratio_reco()
    , ratio_ghosts(0)
    , mc()
    , reco()
    , ghosts(0)
    , nEvents(0)
    , ratio_clone()
    , clone()
  {
    AddCounter("reconstructable", "Reconstructable tracks   ");
    AddCounter("reconstructed", "Reconstructed tracks     ");
  }

  ~L1EventEfficiencies() {};

  void AddCounter(std::string shortname, std::string name)
  {
    indices[shortname] = names.size();
    names.push_back(name);

    ratio_reco.AddCounter();
    mc.AddCounter();
    reco.AddCounter();
    ratio_clone.AddCounter();
    clone.AddCounter();
  }

  L1EventEfficiencies& operator+=(L1EventEfficiencies& a)
  {
    mc += a.mc;
    reco += a.reco;
    clone += a.clone;
    ghosts += a.ghosts;
    nEvents += a.nEvents;
    return *this;
  }

  void CalcEff()
  {
    ratio_reco = reco / mc;
    const double total =
      reco.counters[indices["reconstructable"]] + ghosts + clone.counters[indices["reconstructable"]];
    if (total > 0) ratio_ghosts = ghosts / total;
    else
      ratio_ghosts = -1;
    ratio_clone = clone / mc;
  }


  void Inc(bool isReco, int _nclones, std::string name)
  {
    const int index = indices[name];
    mc.counters[index]++;
    if (isReco) reco.counters[index]++;
    clone.counters[index] += _nclones;
  }

  void IncNEvents() { nEvents++; };

  void AddGhost(int i) { ghosts += i; }

  void PrintEff()
  {

    std::cout.setf(std::ios::fixed);
    std::cout.setf(std::ios::showpoint);
    std::cout.precision(3);
    std::cout << "Event category normalised to: "
              << " Eff  "
              << " / "
              << "Clones"
              << " / "
              << "All Reco"
              << " | "
              << "All MC" << std::endl;

    int NCounters = mc.GetNcounters();
    for (int iC = 0; iC < NCounters; iC++) {
      std::cout << names[iC] << "   : " << ratio_reco.counters[iC] << "  / "
                << ratio_clone.counters[iC]  // nCloneTracks/nMCTracks
                << "  / " << std::setw(8) << reco.counters[iC] / double(nEvents) << " | " << std::setw(8)
                << mc.counters[iC] / double(nEvents) << std::endl;
    }
    std::cout << "Ghost     probability       : " << ratio_ghosts << " | " << ghosts << std::endl;
  }

private:
  std::vector<std::string> names;      // names counters indexed by index of counter
  std::map<std::string, int> indices;  // indices of counters indexed by a counter shortname

  TL1TracksCatCounters<double> ratio_reco;
  double ratio_ghosts;

  TL1TracksCatCounters<int> mc;
  TL1TracksCatCounters<int> reco;
  int ghosts;
  int nEvents;

  TL1TracksCatCounters<double> ratio_clone;
  TL1TracksCatCounters<int> clone;
};
