/* Copyright (C) 2008-2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Andrey Lebedev, David Emschermann, Volker Friese, Mohammad Al-Turany [committer], Florian Uhlig */

void eventDisplay(TString dataset = "data/test")
{
  TString inFile  = dataset + ".tra.root";
  TString parFile = dataset + ".par.root";
  TString outFile = dataset + ".display.root";

  // -----   Reconstruction run   -------------------------------------------
  FairRunAna* fRun = new FairRunAna();

  FairFileSource* inputSource = new FairFileSource(inFile.Data());
  fRun->SetSource(inputSource);
  FairRootFileSink* outputSink = new FairRootFileSink(outFile);
  fRun->SetSink(outputSink);

  FairRuntimeDb* rtdb          = fRun->GetRuntimeDb();
  FairParRootFileIo* parInput1 = new FairParRootFileIo();
  parInput1->open(parFile.Data());
  rtdb->setFirstInput(parInput1);

  FairEventManager* fMan = new FairEventManager();
  FairMCTracks* Track    = new FairMCTracks("Monte-Carlo Tracks");

  FairMCPointDraw* MvdPoint      = new FairMCPointDraw("MvdPoint", kBlack, kFullSquare);
  FairMCPointDraw* StsPoint      = new FairMCPointDraw("StsPoint", kBlue, kFullSquare);
  FairMCPointDraw* RichPoint     = new FairMCPointDraw("RichPoint", kOrange, kFullSquare);
  FairMCPointDraw* RefPlanePoint = new FairMCPointDraw("RefPlanePoint", kPink, kFullSquare);
  FairMCPointDraw* TrdPoint      = new FairMCPointDraw("MuchPoint", kYellow, kFullSquare);
  FairMCPointDraw* MuchPoint     = new FairMCPointDraw("TrdPoint", kCyan, kFullSquare);
  FairMCPointDraw* TofPoint      = new FairMCPointDraw("TofPoint", kRed, kFullSquare);
  FairMCPointDraw* EcalPoint     = new FairMCPointDraw("EcalPoint", kYellow, kFullSquare);

  fMan->AddTask(Track);

  fMan->AddTask(MvdPoint);
  fMan->AddTask(StsPoint);
  fMan->AddTask(RichPoint);
  fMan->AddTask(RefPlanePoint);
  fMan->AddTask(MuchPoint);
  fMan->AddTask(TrdPoint);
  fMan->AddTask(TofPoint);
  fMan->AddTask(EcalPoint);


  //  fMan->Init(1,4,10000);
  fMan->Init(1, 5, 10000);  // make STS visible by default
  //  fMan->Init(1,6,10000);  // make MVD visible by default
  //  fMan->Init(1,7,10000);  // make MVD visible by default
}
