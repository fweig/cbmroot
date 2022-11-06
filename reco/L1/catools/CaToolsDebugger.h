/* Copyright (C) 2022 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Sergey Gorbunov[committer] */

/// \file   CaToolsDebugger.h
/// \brief  Tracking Debugger class (header)
/// \since  03.10.2022
/// \author S.Gorbunov

#ifndef CaToolsDebugger_h
#define CaToolsDebugger_h 1

#include <iostream>
#include <vector>

class TFile;
class TNtuple;

namespace ca
{
  namespace tools
  {
    /// Class ca::tools::Debugger helps to debug CA tracking
    ///
    class Debugger {
    public:
      // *****************************************
      // **     Constructors and destructor     **
      // *****************************************


      /// Default constructor
      Debugger(const char* fileName = "CAdebug.root");

      /// Destructor
      ~Debugger();

      /// Copy constructor
      Debugger(const Debugger& other) = delete;

      /// Move constructor
      Debugger(Debugger&& other) = delete;

      /// Copy assignment operator
      Debugger& operator=(const Debugger& other) = delete;

      /// Move assignment operator
      Debugger& operator=(Debugger&& other) = delete;

      /// Instance
      static Debugger& Instance();

      /// Write ntuples to the file
      void Write();

      /// Set new ntuple
      void AddNtuple(const char* name, const char* varlist);

      /// Add an entry to ntuple
      void FillNtuple(const char* name, float v[]);

      /// Add an entry to ntuple
      template<typename... Targs>
      void FillNtuple(const char* name, Targs... args)
      {
        constexpr std::size_t n = sizeof...(Targs);
        if (n <= 0) return;
        float v[n];
        FillFloatArray(v, args...);
        FillNtuple(name, v);
      }

      /// Get ntuple index
      int GetNtupleIndex(const char* name);

    private:
      template<typename T, typename... Targs>
      void FillFloatArray(float* v, T val, Targs... args)
      {
        v[0] = (float) val;
        if (sizeof...(args) > 0) { FillFloatArray(v + 1, args...); }
      }

      template<typename T>
      void FillFloatArray(float* v, T last)
      {
        v[0] = (float) last;
      }

    private:
      bool fIsWritten {0};
      TFile* fFile {nullptr};
      std::vector<TNtuple*> fNtuples;
    };

  }  // namespace tools
}  // namespace ca

#endif  // CaToolsDebugger_h
