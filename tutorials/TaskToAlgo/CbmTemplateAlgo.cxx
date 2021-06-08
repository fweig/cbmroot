/* Copyright (C) 2019-2020 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

// -----------------------------------------------------------------------------
// -----                                                                   -----
// -----                     CbmTemplateAlgo                    -----
// -----                                                                   -----
// -----------------------------------------------------------------------------

#include "CbmTemplateAlgo.h"

#include <Logger.h>

// -------------------------------------------------------------------------
CbmTemplateAlgo::CbmTemplateAlgo() : CbmAlgo() {}

CbmTemplateAlgo::~CbmTemplateAlgo()
{
  /// Clear buffers
}

// -------------------------------------------------------------------------
Bool_t CbmTemplateAlgo::Init()
{
  LOG(info) << "Initializing tutorial template algo";

  return kTRUE;
}
void CbmTemplateAlgo::Reset() { LOG(info) << "In Reset function of tutorial template algo"; }

void CbmTemplateAlgo::Finish() { LOG(info) << "In Finish function of tutorial template algo"; }

// -------------------------------------------------------------------------
Bool_t CbmTemplateAlgo::InitContainers()
{
  LOG(info) << "Init parameter containers for CbmTemplateAlgo";
  Bool_t initOK = ReInitContainers();

  return initOK;
}
Bool_t CbmTemplateAlgo::ReInitContainers()
{
  LOG(info) << "**********************************************";
  LOG(info) << "ReInit parameter containers for CbmTemplateAlgo";

  // here the parameters are initialized from the TList which was
  // filled by the wrapper task or the wrapper device
  //   fTemplatePar = (CbmTemplatePar*)fParCList->FindObject("CbmTemplatePar");
  //if( nullptr == fUnpackPar )
  //   return kFALSE;

  Bool_t initOK = InitParameters();

  return initOK;
}

TList* CbmTemplateAlgo::GetParList()
{
  if (nullptr == fParCList) { fParCList = new TList(); }

  // Here the parameter container is created and passed to the
  // calling wrapper task or wrapper device. The caller will
  // change the pointer such that it points to the correctly
  // initialized parameter container. The initialization is done
  // by the framework

  //  fTemplatePar = new CbmTemplatePar("CbmTemplatePar");
  //  fParCList->Add(fUnpackPar);

  return fParCList;
}
Bool_t CbmTemplateAlgo::InitParameters()
{
  LOG(info) << "In InitParameters function of tutorial template algo";
  return kTRUE;
}
// -------------------------------------------------------------------------

std::vector<CbmStsHit> ProcessInputData(const std::vector<CbmStsPoint>&)
{
  std::vector<CbmStsHit> outputVect {};
  return outputVect;
}

// -------------------------------------------------------------------------
