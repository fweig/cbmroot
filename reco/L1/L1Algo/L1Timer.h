/* Copyright (C) 2012-2019 Frankfurt Institute for Advanced Studies, Goethe-Universitaet Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Igor Kulakov [committer], Maksym Zyzak */

#ifndef _L1Timer_H
#define _L1Timer_H

#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "L1Def.h"

class TimerInfo {
public:
  TimerInfo() : fName(""), fReal(0), fCpu(0) {};
  TimerInfo(const std::string& name) : fName(name), fReal(0), fCpu(0) {};

  TimerInfo& operator=(TStopwatch& sw)
  {
    fReal = sw.RealTime();
    fCpu  = sw.CpuTime();
    return (*this);
  };
  TimerInfo& operator+=(TStopwatch& sw)
  {
    fReal += sw.RealTime();
    fCpu += sw.CpuTime();
    return (*this);
  };
  void operator+=(const TimerInfo& t)
  {
    fReal += t.fReal;
    fCpu += t.fCpu;
  }
  TimerInfo operator/(const float f) const
  {
    TimerInfo r;
    r.fName = fName;
    r.fReal = fReal / f;
    r.fCpu  = fCpu / f;
    return r;
  }

  // void Print(){ std::cout << fReal << "/" << fCpu; };
  void PrintReal() { std::cout << fReal; };
  float Real() { return fReal; };
  std::string& Name() { return fName; };

private:
  std::string fName;
  float fReal, fCpu;
};

class L1CATFIterTimerInfo {
public:
  L1CATFIterTimerInfo() : fNameToI(), fTIs() {};
  void Add(std::string name)
  {
    fNameToI[name] = fTIs.size();
    fTIs.push_back(TimerInfo(name));
  };
  TimerInfo& operator[](std::string name) { return fTIs[fNameToI[name]]; };
  TimerInfo& operator[](int i) { return fTIs[i]; };
  void operator+=(L1CATFIterTimerInfo& t)
  {
    for (unsigned int i = 0; i < fTIs.size(); ++i)
      fTIs[i] += t[i];
  }
  L1CATFIterTimerInfo operator/(float f)
  {
    L1CATFIterTimerInfo r;
    r.fNameToI = fNameToI;
    r.fTIs.resize(fTIs.size());
    for (unsigned int i = 0; i < fTIs.size(); ++i) {
      r.fTIs[i] = fTIs[i] / f;
    }
    return r;
  }

  void PrintReal(int f = 0)
  {
    if (f) {
      PrintNames();
      std::cout << '\n';
    }
    fTIs[0].PrintReal();
    for (unsigned int i = 1; i < fTIs.size(); ++i) {
      std::cout << " | " << std::setw(fTIs[i].Name().size());
      fTIs[i].PrintReal();
    }
    if (f) std::cout << '\n';
  };
  void PrintNames()
  {
    std::cout << fTIs[0].Name();
    for (unsigned int i = 1; i < fTIs.size(); ++i) {
      std::cout << " | " << fTIs[i].Name();
    }
  };

private:
  std::map<std::string, int> fNameToI;
  std::vector<TimerInfo> fTIs;
};

class L1CATFTimerInfo {
public:
  L1CATFTimerInfo() : fTIIs(), fTIAll() {};
  void SetNIter(int n) { fTIIs.resize(n); };

  void Add(std::string name)
  {
    for (unsigned int i = 0; i < fTIIs.size(); ++i)
      fTIIs[i].Add(name);
    fTIAll.Add(name);
  };  // use after setniter
  L1CATFIterTimerInfo& GetTimerAll() { return fTIAll; };
  L1CATFIterTimerInfo& operator[](int i) { return fTIIs[i]; };
  void operator+=(L1CATFTimerInfo& t)
  {
    for (unsigned int i = 0; i < fTIIs.size(); ++i)
      fTIIs[i] += t[i];
    fTIAll += t.GetAllInfo();
  }
  L1CATFTimerInfo operator/(float f)
  {
    L1CATFTimerInfo r;
    r.fTIAll = fTIAll / f;
    r.SetNIter(fTIIs.size());
    for (unsigned int i = 0; i < fTIIs.size(); ++i) {
      r.fTIIs[i] = fTIIs[i] / f;
    }
    return r;
  }

  void Calc()
  {
    fTIAll = fTIIs[0];
    for (unsigned int i = 1; i < fTIIs.size(); ++i)
      fTIAll += fTIIs[i];
  }

  L1CATFIterTimerInfo& GetAllInfo() { return fTIAll; };
  void PrintReal()
  {
    std::cout.precision(1);
    std::cout.setf(std::ios::fixed);
    std::cout << " stage "
              << " : ";
    fTIAll.PrintNames();
    std::cout << '\n';
    for (unsigned int i = 0; i < fTIIs.size(); ++i) {
      std::cout << " iter " << i << " : ";
      fTIIs[i].PrintReal();
      std::cout << '\n';
    }
    std::cout << " all   "
              << " : ";
    fTIAll.PrintReal();
    std::cout << '\n';
  };

private:
  std::vector<L1CATFIterTimerInfo> fTIIs;
  L1CATFIterTimerInfo fTIAll;
};

#endif
