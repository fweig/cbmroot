/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             * 
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *  
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
/**
 * ParameterMQServer.cxx
 *
 * @since 2015-10-26
 * @author M. Al-Turany, A. Rybalchenko
 */

#include "ParameterMQServer.h"

#include "CbmMQDefs.h"

#include "FairMQLogger.h"
#include "FairMQProgOptions.h"
#include "FairParAsciiFileIo.h"
#include "FairParGenericSet.h"
#include "FairParRootFileIo.h"
#include "FairRuntimeDb.h"

#include "Rtypes.h"
#include "TGeoManager.h"
#include "TList.h"
#include "TMessage.h"
#include "TObjString.h"
#include "TSystem.h"

using namespace std;

ParameterMQServer::ParameterMQServer()
  : fRtdb(FairRuntimeDb::instance())
  , fFirstInputName("first_input.root")
  , fFirstInputType("ROOT")
  , fSecondInputName("")
  , fSecondInputType("ROOT")
  , fOutputName("")
  , fOutputType("ROOT")
  , fChannelName("data")
{
}

void ParameterMQServer::InitTask()
{
  string loadLibs = fConfig->GetValue<string>("libs-to-load");
  if (loadLibs.length() > 0) {
    LOG(info) << "There are libraries to load.";
    if (loadLibs.find(";") != std::string::npos) {
      LOG(info) << "There are several libraries to load";
      istringstream f(loadLibs);
      string s;
      while (getline(f, s, ';')) {
        LOG(info) << "Load library " << s;
        gSystem->Load(s.c_str());
      }
    }
    else {
      LOG(info) << "Load library " << loadLibs;
      gSystem->Load(loadLibs.c_str());
    }
  }
  else {
    LOG(info) << "There are no libraries to load.";
  }

  fFirstInputName  = fConfig->GetValue<string>("first-input-name");
  fFirstInputType  = fConfig->GetValue<string>("first-input-type");
  fSecondInputName = fConfig->GetValue<string>("second-input-name");
  fSecondInputType = fConfig->GetValue<string>("second-input-type");
  fOutputName      = fConfig->GetValue<string>("output-name");
  fOutputType      = fConfig->GetValue<string>("output-type");
  fChannelName     = fConfig->GetValue<string>("channel-name");

  if (fRtdb != 0) {
    // Set first input
    if (fFirstInputType == "ROOT") {
      FairParRootFileIo* par1R = new FairParRootFileIo();
      par1R->open(fFirstInputName.data(), "UPDATE");
      fRtdb->setFirstInput(par1R);
    }
    else if (fFirstInputType == "ASCII") {
      FairParAsciiFileIo* par1A = new FairParAsciiFileIo();
      if (fFirstInputName.find(";") != std::string::npos) {
        LOG(info) << "File list found!";
        TList* parFileList = new TList();
        TObjString* parFile(NULL);
        istringstream f(fFirstInputName);
        string s;
        while (getline(f, s, ';')) {
          LOG(info) << "File: " << s;
          parFile = new TObjString(s.c_str());
          parFileList->Add(parFile);
          par1A->open(parFileList, "in");
        }
      }
      else {
        LOG(info) << "Single input file found!";
        par1A->open(fFirstInputName.data(), "in");
      }
      fRtdb->setFirstInput(par1A);
    }

    // Set second input
    if (fSecondInputName != "") {
      if (fSecondInputType == "ROOT") {
        FairParRootFileIo* par2R = new FairParRootFileIo();
        par2R->open(fSecondInputName.data(), "UPDATE");
        fRtdb->setSecondInput(par2R);
      }
      else if (fSecondInputType == "ASCII") {
        FairParAsciiFileIo* par2A = new FairParAsciiFileIo();
        if (fSecondInputName.find(";") != std::string::npos) {
          LOG(info) << "File list found!";
          TList* parFileList = new TList();
          TObjString* parFile(NULL);
          istringstream f(fSecondInputName);
          string s;
          while (getline(f, s, ';')) {
            LOG(info) << "File: " << s;
            parFile = new TObjString(s.c_str());
            parFileList->Add(parFile);
            par2A->open(parFileList, "in");
          }
        }
        else {
          LOG(info) << "Single input file found!";
          par2A->open(fFirstInputName.data(), "in");
        }
        fRtdb->setSecondInput(par2A);
      }
    }

    // Set output
    if (fOutputName != "") {
      if (fOutputType == "ROOT") {
        FairParRootFileIo* parOut = new FairParRootFileIo(kTRUE);
        parOut->open(fOutputName.data());
        fRtdb->setOutput(parOut);
      }

      fRtdb->saveOutput();
    }
  }
  fRtdb->print();
}

void ParameterMQServer::Run()
{
  string parameterName   = "";
  FairParGenericSet* par = nullptr;

  while (cbm::mq::CheckCurrentState(this, cbm::mq::State::Running)) {
    FairMQMessagePtr req(NewMessage());

    if (Receive(req, fChannelName, 0) > 0) {
      string reqStr(static_cast<char*>(req->GetData()), req->GetSize());
      LOG(info) << "Received parameter request from client: \"" << reqStr << "\"";

      size_t pos              = reqStr.rfind(",");
      string newParameterName = reqStr.substr(0, pos);
      int runId               = stoi(reqStr.substr(pos + 1));
      LOG(info) << "Parameter name: " << newParameterName;
      LOG(info) << "Run ID: " << runId;

      LOG(info) << "Retrieving parameter...";
      // Check if the parameter name has changed to avoid getting same container repeatedly
      if (newParameterName != parameterName) {
        parameterName = newParameterName;
        par           = static_cast<FairParGenericSet*>(fRtdb->getContainer(parameterName.c_str()));
      }
      LOG(info) << "Retrieving parameter...Done";

      if (-1 != runId) { fRtdb->initContainers(runId); }

      LOG(info) << "Sending following parameter to the client:";
      if (par) {
        par->print();

        TMessage* tmsg = new TMessage(kMESS_OBJECT);
        tmsg->WriteObject(par);

        FairMQMessagePtr rep(NewMessage(
          tmsg->Buffer(), tmsg->BufferSize(),
          [](void* /*data*/, void* object) { delete static_cast<TMessage*>(object); }, tmsg));

        if (Send(rep, fChannelName, 0) < 0) {
          LOG(error) << "failed sending reply";
          break;
        }
      }
      else {
        LOG(error) << "Parameter uninitialized!";
        // Send an empty message back to keep the REQ/REP cycle
        FairMQMessagePtr rep(NewMessage());
        if (Send(rep, fChannelName, 0) < 0) {
          LOG(error) << "failed sending reply";
          break;
        }
      }
    }
  }
}

ParameterMQServer::~ParameterMQServer()
{
  if (gGeoManager) {
    gGeoManager->GetListOfVolumes()->Delete();
    gGeoManager->GetListOfShapes()->Delete();
  }
  delete fRtdb;
}
