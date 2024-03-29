/* Copyright (C) 2019 GSI Helmholtzzentrum fuer Schwerionenforschung, Darmstadt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Florian Uhlig [committer] */

char HistText[256];

TFile* ParFile;
TFile* SimFile;
TFile* RecFile;

TH1D* H_distance;
TH2D* H_position;

TTree* cbmrec;

//TClonesArray *tracks;
//TClonesArray *points;
//TClonesArray *ReflPoints;
TClonesArray* hits;
TClonesArray* rings;
TClonesArray* ringmatch;
TClonesArray* richProj;

// ------------------------------------------------------------------- //
