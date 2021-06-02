/** @file Analyze_matching.h
  * @copyright Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Florian Uhlig [committer] **/

char HistText[256];

TFile* ParFile;
TFile* SimFile;
TFile* RecFile;

TH1F* H_Radius;
TH1F* H_aAxis;
TH1F* H_bAxis;
TH1F* H_boa;
TH1F* H_dR;
TH1F* H_nbHits;

TTree* cbmsim;
TTree* cbmrec;

TClonesArray* tracks;
TClonesArray* points;
TClonesArray* ReflPoints;
TClonesArray* hits;
TClonesArray* rings;
TClonesArray* ringmatch;

// ------------------------------------------------------------------- //
