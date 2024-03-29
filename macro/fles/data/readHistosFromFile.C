/* Copyright (C) 2015 Institut fuer Kernphysik, Westfaelische Wilhelms-Universitaet Muenster, Muenster
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Cyrano Bergmann [committer] */

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TKey.h"
#include "TString.h"
#include "TTree.h"

#include "Riostream.h"

void readHistosFromFile(TString filename = "test.root")
{
  TFile file = TFile(filename, "READ");
  if (file.IsOpen()) {
    TKey* key = NULL;
    TIter iter(file.GetListOfKeys());
    while ((key = dynamic_cast<TKey*>(iter())) != 0) {
      TObject* obj = key->ReadObj();
      if (obj->IsA()->InheritsFrom(TH1::Class())) {
        cout << "               ..." << obj->GetName() << endl;
        TH1* h1 = (TH1*) obj;
        // do what ever you like
      }
      else {

        // object is of no type that we know or can handle
        cout << "Unknown object type, name: " << obj->GetName() << " title: " << obj->GetTitle() << endl;
      }
    }
  }
  else {
    printf("ERROR:: no file %s found!\n", filename.Data());
  }
}
