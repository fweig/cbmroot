/* Copyright (C) 2018-2021 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer], Alexandru Bercuci */

#include "CbmTrdParSetGas.h"

#include "CbmTrdParModGas.h"  // for CbmTrdParModGas

#include <FairParamList.h>  // for FairParamList
#include <Logger.h>         // for Logger, LOG

#include <TArrayI.h>            // for TArrayI
#include <TDirectory.h>         // for TDirectory (ptr only), gDirectory
#include <TFile.h>              // for TFile, gFile
#include <TGenericClassInfo.h>  // for TGenericClassInfo
#include <TH2.h>                // for TH2F
#include <TObjArray.h>          // for TObjArray
#include <TObjString.h>         // for TObjString
#include <TString.h>            // for Form, TString
#include <TSystem.h>            // for TSystem, gSystem

#include <map>      // for map, map<>::iterator, operator!=, __m...
#include <utility>  // for pair

CbmTrdParSetGas::CbmTrdParSetGas(const char* name, const char* title, const char* context)
  : CbmTrdParSet(name, title, context)
{
}


//_______________________________________________________________________________
Bool_t CbmTrdParSetGas::getParams(FairParamList* l)
{
  if (!l) return kFALSE;
  if (!l->fill("NrOfModules", &fNrOfModules)) return kFALSE;
  Text_t gasMix[100];
  if (!l->fill("Gas", gasMix, 100)) return kFALSE;
  Text_t repo[100], pid[100];
  if (!l->fill("RepoDrift", repo, 100)) return kFALSE;
  if (!l->fill("RepoPid", pid, 100)) return kFALSE;

  TFile* oldFile     = gFile;
  TDirectory* oldDir = gDirectory;

  TDirectory* cwd(gDirectory);

  TFile* infile = TFile::Open(Form("%s/%s", gSystem->Getenv("VMCWORKDIR"), repo));
  if (!infile->IsOpen()) {
    LOG(error) << "Missing TRD DriftMap Repository : " << repo;
    gFile      = oldFile;
    gDirectory = oldDir;
    return kFALSE;
  }
  else {
    LOG(debug) << "TRD DriftMap Repository : " << gFile->GetName();
    LOG(debug) << "TRD PID Repository : " << pid;
  }

  TArrayI moduleId(fNrOfModules);
  if (!l->fill("ModuleIdArray", &moduleId)) return kFALSE;

  TString sgas(gasMix);
  TObjArray* so = sgas.Tokenize("_");
  Int_t pgas    = ((TObjString*) (*so)[1])->String().Atoi();

  TArrayI value(4);
  for (Int_t i = 0; i < fNrOfModules; i++) {
    if (!l->fill(Form("%d", moduleId[i]), &value)) return kFALSE;
    fModuleMap[moduleId[i]] = new CbmTrdParModGas(
      Form("Module/%d/Ua/%d/Ud/%d/Gas/%s", moduleId[i], value[0], value[1], ((TObjString*) (*so)[0])->GetName()));
    ((CbmTrdParModGas*) fModuleMap[moduleId[i]])
      ->SetDriftMap(GetDriftMap(((TObjString*) (*so)[0])->GetName(), value[0], value[1]), cwd);
    ((CbmTrdParModGas*) fModuleMap[moduleId[i]])->SetNobleGas(1.e-2 * pgas);
    ((CbmTrdParModGas*) fModuleMap[moduleId[i]])->SetDetType(value[2]);
    ((CbmTrdParModGas*) fModuleMap[moduleId[i]])->SetPidType(value[3]);
    ((CbmTrdParModGas*) fModuleMap[moduleId[i]])->SetFileName(pid);

    //if (fair::Logger::Logging(fair::Severity::debug) fModuleMap[moduleId[i]]->Print();
  }
  so->Delete();
  delete so;

  infile->Close();
  delete infile;

  gFile      = oldFile;
  gDirectory = oldDir;

  return kTRUE;
}

//_____________________________________________________________________
void CbmTrdParSetGas::putParams(FairParamList* l)
{
  if (!l) return;
  LOG(info) << GetName() << "::putParams(FairParamList*)";

  TArrayI moduleId(fNrOfModules);
  Int_t idx(0);
  for (std::map<Int_t, CbmTrdParMod*>::iterator imod = fModuleMap.begin(); imod != fModuleMap.end(); imod++) {
    moduleId[idx++] = imod->first;
  }
  CbmTrdParModGas* mod = (CbmTrdParModGas*) fModuleMap[moduleId[0]];


  l->add("RepoDrift", "parameters/trd/CbmTrdDriftMap.root");
  l->add("RepoPid", "parameters/trd/Likelihood_Xenon_85_GSI_12.root");
  l->add("Gas", Form("Xe_%d", Int_t(1.e2 * mod->GetNobleGas())));
  l->add("NrOfModules", fNrOfModules);
  l->add("ModuleIdArray", moduleId);

  TArrayI values(4);
  for (Int_t i = 0; i < fNrOfModules; i++) {
    mod       = (CbmTrdParModGas*) fModuleMap[moduleId[i]];
    values[0] = mod->GetUanode();
    values[1] = mod->GetUdrift();
    values[2] = mod->GetDetType();
    values[3] = mod->GetPidType();
    l->add(Form("%d", moduleId[i]), values);
  }
}

//_______________________________________________________________________________
TH2F* CbmTrdParSetGas::GetDriftMap(const Char_t* g, const Int_t ua, const Int_t ud)
{
  TString smap = Form("%s_%4d_%3d", g, ua, ud);
  TH2F* hm     = gFile->Get<TH2F>(smap.Data());
  if (hm) return hm;

  LOG(debug) << GetName() << "::GetDriftMap() : Interpolate drift map for " << g << " Ua[" << ua << "]"
             << " Ud[" << ud << "]";

  return gFile->Get<TH2F>(Form("%s_1500_300", g));
}

ClassImp(CbmTrdParSetGas)
