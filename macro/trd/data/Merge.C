/* Copyright (C) 2001-2016 Institut fuer Kernphysik, Westfaelische Wilhelms-Universitaet Muenster, Muenster
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Cyrano Bergmann [committer] */

//macro to add histogram files
//NOTE: This macro is kept for back compatibility only.
//Use instead the executable $ROOTSYS/bin/hadd
//
//This macro will add histograms from a list of root files and write them
//to a target root file. The target file is newly created and must not be
//identical to one of the source files.
//
//Author: Sven A. Schmidt, sven.schmidt@cern.ch
//Date:   13.2.2001

//This code is based on the hadd.C example by Rene Brun and Dirk Geppert,
//which had a problem with directories more than one level deep.
//(see macro hadd_old.C for this previous implementation).
//
//The macro from Sven has been enhanced by
//   Anne-Sylvie Nicollerat <Anne-Sylvie.Nicollerat@cern.ch>
// to automatically add Trees (via a chain of trees).
//
//To use this macro, modify the file names in function hadd.
//
//NB: This macro is provided as a tutorial.
//    Use $ROOTSYS/bin/hadd to merge many histogram files


#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TKey.h"
#include "TTree.h"

#include <string.h>

#include "Riostream.h"

TList* FileList;
TFile* Target;

void MergeHistos(TDirectory* target, TList* sourcelist);

void Merge(TString outfilename = "result.root")
{
  // in an interactive ROOT session, edit the file names
  // Target and FileList, then
  // root > .L hadd.C
  // root > hadd()
  Target = TFile::Open(outfilename, "RECREATE");
  TString filename;
  FileList = new TList();
  for (Int_t iurqmd = 0; iurqmd < 500; iurqmd++) {
    //if (iurqmd < 100) { //with 0100 pi0
    //if (iurqmd >= 100) { //with 0000 pi0
    /* 
       if (
       iurqmd != 31 &&
       iurqmd != 34 &&
       iurqmd != 35 &&
       iurqmd != 39 &&
       iurqmd != 43 &&
       iurqmd != 50 &&
       iurqmd != 61 &&
       iurqmd != 78 &&
       iurqmd != 83 &&
       iurqmd != 86 &&
       iurqmd != 93 &&
       iurqmd != 169 &&
       iurqmd != 173 &&
       iurqmd != 179 &&
       iurqmd != 185 &&
       iurqmd != 186 &&
       iurqmd != 189 &&
       iurqmd != 191 &&
       iurqmd != 192 &&
       iurqmd != 193 &&
       iurqmd != 195
       ){ //with 1000 pi0
    */
    /*if (
      iurqmd == 31 ||
      iurqmd == 34 ||
      iurqmd == 39 ||
      iurqmd == 43 ||
      iurqmd == 50 ||
      iurqmd == 61 ||
      iurqmd == 78 ||
      iurqmd == 83 ||
      iurqmd == 86 ||
      iurqmd == 93 ||
      iurqmd == 185 ||
      iurqmd == 186 ||
      iurqmd == 191 ||
      iurqmd == 192 ||
      iurqmd == 193
      ){ //without pi0
    */
    //if (iurqmd == 00)
    {
      //filename.Form("test.pa.trd_v12f.smearing.%04i.100events.root",iurqmd);
      filename.Form("sis100_electron_test.hd_qa.%04i.root", iurqmd);
      //cout << "       " << filename;
      TFile test = TFile(filename, "READ");
      if (test.IsOpen()) {
        cout << "                         file " << filename << ": added to TList" << endl;
        FileList->Add(TFile::Open(filename));
        test.Close();
      }
      else {
        //cout << ": not found!" << endl;
      }
    }
  }

  MergeHistos(Target, FileList);
}
void MergeHistos(TDirectory* target, TList* sourcelist)
{


  cout << "Target path: " << target->GetPath() << endl;
  TString path((char*) strstr(target->GetPath(), ":"));
  path.Remove(0, 2);
  TFile* first_source = (TFile*) sourcelist->First();

  first_source->cd(path);

  TDirectory* current_sourcedir = gDirectory;
  //gain time, do not add the objects in the list in memory
  Bool_t status = TH1::AddDirectoryStatus();
  TH1::AddDirectory(kFALSE);

  // loop over all keys in this directory
  TChain* globChain = 0;
  TIter nextkey(current_sourcedir->GetListOfKeys());
  TKey *key, *oldkey = 0;
  cout << "Merging histograms ..." << endl;
  while ((key = (TKey*) nextkey())) {

    //keep only the highest cycle number for each key
    if (oldkey && !strcmp(oldkey->GetName(), key->GetName())) continue;

    // read object from first source file
    first_source->cd(path);
    TObject* obj = key->ReadObj();

    if (obj->IsA()->InheritsFrom(TH1::Class())) {
      // descendant of TH1 -> merge it

      cout << "               ..." << obj->GetName() << endl;
      TH1* h1 = (TH1*) obj;

      // loop over all source files and add the content of the
      // correspondant histogram to the one pointed to by "h1"
      TFile* nextsource = (TFile*) sourcelist->After(first_source);
      while (nextsource) {
        //cout << "     File: " << nextsource->GetName() << endl;
        // make sure we are at the correct directory level by cd'ing to path
        nextsource->cd(path);
        TKey* key2 = (TKey*) gDirectory->GetListOfKeys()->FindObject(h1->GetName());
        if (key2) {
          TH1* h2 = (TH1*) key2->ReadObj();
          h1->Add(h2);
          delete h2;
        }
        else {
          // damaged file
        }

        nextsource = (TFile*) sourcelist->After(nextsource);
      }
    }
    else if (obj->IsA()->InheritsFrom(TTree::Class())) {
      /*
      // loop over all source files create a chain of Trees "globChain"
      const char* obj_name= obj->GetName();

      globChain = new TChain(obj_name);
      globChain->Add(first_source->GetName());
      TFile *nextsource = (TFile*)sourcelist->After( first_source );
      //      const char* file_name = nextsource->GetName();
      // cout << "file name  " << file_name << endl;
      while ( nextsource ) {

      globChain->Add(nextsource->GetName());
      nextsource = (TFile*)sourcelist->After( nextsource );
      }
      */
    }
    else if (obj->IsA()->InheritsFrom(TDirectory::Class())) {
      // it's a subdirectory

      cout << "Found subdirectory " << obj->GetName() << endl;

      // create a new subdir of same name and title in the target file
      target->cd();
      TDirectory* newdir = target->mkdir(obj->GetName(), obj->GetTitle());

      // newdir is now the starting point of another round of merging
      // newdir still knows its depth within the target file via
      // GetPath(), so we can still figure out where we are in the recursion
      MergeHistos(newdir, sourcelist);
    }
    else {

      // object is of no type that we know or can handle
      cout << "Unknown object type, name: " << obj->GetName() << " title: " << obj->GetTitle() << endl;
    }

    // now write the merged histogram (which is "in" obj) to the target file
    // note that this will just store obj in the current directory level,
    // which is not persistent until the complete directory itself is stored
    // by "target->Write()" below
    if (obj) {
      target->cd();

      //!!if the object is a tree, it is stored in globChain...
      if (obj->IsA()->InheritsFrom(TTree::Class()))
        //globChain->Merge(target->GetFile(),0,"keep");
        cout << " skiped TTree " << endl;
      else
        obj->Write(key->GetName());
    }

  }  // while ( ( TKey *key = (TKey*)nextkey() ) )


  // save modifications to target file
  target->SaveSelf(kTRUE);
  TH1::AddDirectory(status);
}
