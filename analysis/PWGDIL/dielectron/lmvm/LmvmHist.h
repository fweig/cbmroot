/* Copyright (C) 2012-2021 UGiessen, JINR-LIT
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer], Elena Lebedeva, Florian Uhlig */

#ifndef LMVM_HIST_H
#define LMVM_HIST_H

#include "CbmHistManager.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TObject.h"

#include <string>
#include <vector>

#include "LmvmDef.h"


class LmvmHist {
public:
  LmvmHist();
  virtual ~LmvmHist() { ; }

  const static int fNofSrc = 5;
  const static std::vector<ELmvmSrc> fSrcs;
  const static std::vector<std::string> fSrcNames;
  const static std::vector<std::string> fSrcLatex;
  const static std::vector<int> fSrcColors;

  const static int fNofAnaSteps = 12;
  const static std::vector<ELmvmAnaStep> fAnaSteps;
  const static std::vector<std::string> fAnaStepNames;
  const static std::vector<std::string> fAnaStepLatex;
  const static std::vector<int> fAnaStepColors;

  static const int fNofSignals = 5;
  const static std::vector<std::string> fSignalNames;
  const static std::vector<ELmvmSignal> fSignals;

  const static int fNofBgPairSrc = 6;
  const static std::vector<std::string> fBgPairSrcNames;
  const static std::vector<std::string> fBgPairSrcLatex;

  std::vector<std::string> CombineNames(const std::string& name, const std::vector<std::string>& subNames);

  std::vector<std::string> CombineNames(const std::string& name, const std::vector<std::string>& subNames1,
                                        const std::vector<std::string>& subNames2);

  // Probably one can move these many methods to main CbmHistManager class
  void CreateH1(const std::string& name, const std::string& axisX, const std::string& axisY, double nBins, double min,
                double max);

  void CreateH2(const std::string& name, const std::string& axisX, const std::string& axisY, const std::string& axisZ,
                double nBinsX, double minX, double maxX, double nBinsY, double minY, double maxY);

  void CreateH1(const std::string& name, const std::vector<std::string>& subNames, const std::string& axisX,
                const std::string& axisY, double nBins, double min, double max);

  void CreateH2(const std::string& name, const std::vector<std::string>& subNames, const std::string& axisX,
                const std::string& axisY, const std::string& axisZ, double nBinsX, double minX, double maxX,
                double nBinsY, double minY, double maxY);

  void CreateH1(const std::string& name, const std::vector<std::string>& subNames1,
                const std::vector<std::string>& subNames2, const std::string& axisX, const std::string& axisY,
                double nBins, double min, double max);

  void CreateH2(const std::string& name, const std::vector<std::string>& subNames1,
                const std::vector<std::string>& subNames2, const std::string& axisX, const std::string& axisY,
                const std::string& axisZ, double nBinsX, double minX, double maxX, double nBinsY, double minY,
                double maxY);


  template<typename T>
  T* CreateHByClone(const std::string& name, const std::string& newName)
  {
    T* hNew = static_cast<T*>(fHM.GetObject(name)->Clone());
    hNew->SetNameTitle(newName.c_str(), newName.c_str());
    fHM.Add(newName, hNew);
    return hNew;
  }

  template<typename T>
  T* CreateHByClone(const std::string& name, const std::string& newName, ELmvmAnaStep step)
  {
    return CreateHByClone<T>(GetName(name, step), GetName(newName, step));
  }

  void FillH1(const std::string& name, double x, double w = 1.);
  void FillH2(const std::string& name, double x, double y, double w = 1.);
  void FillH1(const std::string& name, ELmvmAnaStep step, double x, double w = 1.);
  void FillH2(const std::string& name, ELmvmAnaStep step, double x, double y, double w = 1.);
  void FillH1(const std::string& name, ELmvmSrc src, double x, double wSignal);
  void FillH2(const std::string& name, ELmvmSrc src, double x, double y, double wSignal);
  void FillH1(const std::string& name, ELmvmSrc src, ELmvmAnaStep step, double x, double wSignal);
  void FillH2(const std::string& name, ELmvmSrc src, ELmvmAnaStep step, double x, double y, double wSignal);

  TNamed* GetObject(const std::string& name) { return fHM.GetObject(name); }

  TH1D* H1(const std::string& name) { return static_cast<TH1D*>(fHM.H1(name)); }
  TH2D* H2(const std::string& name) { return static_cast<TH2D*>(fHM.H2(name)); }
  TH1D* H1(const std::string& name, ELmvmAnaStep step) { return H1(GetName(name, step)); }
  TH2D* H2(const std::string& name, ELmvmAnaStep step) { return H2(GetName(name, step)); }
  TH1D* H1(const std::string& name, ELmvmSrc src) { return H1(GetName(name, src)); }
  TH2D* H2(const std::string& name, ELmvmSrc src) { return H2(GetName(name, src)); }
  TH1D* H1(const std::string& name, ELmvmSrc src, ELmvmAnaStep step) { return H1(GetName(name, src, step)); }
  TH2D* H2(const std::string& name, ELmvmSrc src, ELmvmAnaStep step) { return H2(GetName(name, src, step)); }

  TH1D* H1Clone(const std::string& name) { return static_cast<TH1D*>(H1(name)->Clone()); }
  TH2D* H2Clone(const std::string& name) { return static_cast<TH2D*>(H2(name)->Clone()); }
  TH1D* H1Clone(const std::string& name, ELmvmAnaStep step) { return static_cast<TH1D*>(H1(name, step)->Clone()); }
  TH2D* H2Clone(const std::string& name, ELmvmAnaStep step) { return static_cast<TH2D*>(H2(name, step)->Clone()); }
  TH1D* H1Clone(const std::string& name, ELmvmSrc src) { return static_cast<TH1D*>(H1(name, src)->Clone()); }
  TH2D* H2Clone(const std::string& name, ELmvmSrc src) { return static_cast<TH2D*>(H2(name, src)->Clone()); }
  TH1D* H1Clone(const std::string& name, ELmvmSrc src, ELmvmAnaStep step)
  {
    return static_cast<TH1D*>(H1(name, src, step)->Clone());
  }
  TH2D* H2Clone(const std::string& name, ELmvmSrc src, ELmvmAnaStep step)
  {
    return static_cast<TH2D*>(H2(name, src, step)->Clone());
  }

  std::string GetName(const std::string& name, ELmvmAnaStep step);
  std::string GetName(const std::string& name, ELmvmSrc src);
  std::string GetName(const std::string& name, ELmvmSrc src, ELmvmAnaStep step);


  void Rebin(const std::string& name, int nGroup);
  void Rebin(const std::string& name, const std::vector<std::string>& subNames, int nGroup);
  void Rebin(const std::string& name, const std::vector<std::string>& subNames1,
             const std::vector<std::string>& subNames2, int nGroup);

  void WriteToFile();

  void DrawEfficiency(TH1* h1, TH1* h2, double xPos, double yPos);

  static void DrawAnaStepOnPad(ELmvmAnaStep step);

  CbmHistManager fHM;

  ClassDef(LmvmHist, 1);
};

#endif
