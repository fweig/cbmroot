

#include "CbmHaddBase.h"

#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TKey.h"
#include "TTree.h"

#include <sstream>

#include <glob.h>
#include <string.h>

#include "Riostream.h"

using namespace std;


ClassImp(CbmHaddBase);


vector<string> CbmHaddBase::GetFilesByPattern(const string& pattern)
{
  glob_t glob_result;
  glob(pattern.c_str(), GLOB_TILDE, NULL, &glob_result);
  vector<string> files;
  for (unsigned int i = 0; i < glob_result.gl_pathc; ++i) {
    files.push_back(string(glob_result.gl_pathv[i]));
  }
  globfree(&glob_result);
  return files;
}


vector<string> CbmHaddBase::GetGoodFiles(const string& pattern, Int_t fileSizeLimit, Int_t nofEvents)
{
  vector<string> files = GetFilesByPattern(pattern);
  cout << "pattern:" << pattern << endl;
  cout << "# of files by pattern:" << files.size() << endl;
  vector<string> goodFiles;
  for (size_t i = 0; i < files.size(); i++) {
    TFile* file = TFile::Open((files[i]).c_str(), "READ");
    if (file != NULL) cout << i << " open file" << endl;
    // TODO: Do we need to check also reco file?
    Bool_t isGoodFile = CheckFile(file, fileSizeLimit, nofEvents);
    if (file != NULL) file->Close();
    if (isGoodFile) goodFiles.push_back(files[i]);
  }
  cout << "GetGoodFiles all:" << files.size() << " good:" << goodFiles.size() << endl;
  return goodFiles;
}

Bool_t CbmHaddBase::CheckFileSize(TFile* file, Int_t fileSizeLimit)
{
  if (file == NULL) return false;
  if (file->GetEND() < fileSizeLimit) return false;

  return true;
}


Bool_t CbmHaddBase::CheckFile(TFile* file, Int_t fileSizeLimit, Int_t nofEvents)
{
  if (file == NULL) return false;
  if (file->GetEND() < fileSizeLimit) return false;
  TTree* tree = (TTree*) file->Get("cbmsim");
  if (tree == NULL) return false;
  Long64_t treeSizeAna = tree->GetEntriesFast();
  if (treeSizeAna == nofEvents) return true;
  return false;
}


void CbmHaddBase::AddFilesInDir(const string& dir, const string& fileTemplate, const string& addString, Int_t nofFiles,
                                Int_t fileSizeLimit, Int_t nofEvents)
{
  Int_t maxNofFiles   = 100;
  string fileNameAna  = dir + string("analysis") + fileTemplate;
  string fileNameReco = dir + string("reco") + fileTemplate;
  string fileNameQa   = dir + string("litqa") + fileTemplate;

  cout << "-I- " << dir << endl;
  int count              = 0;
  TList* fileList        = new TList();
  TList* tempTargetFiles = new TList();
  Int_t targetFileNum    = 0;
  for (int i = 1; i < nofFiles; i++) {
    stringstream ss;
    ss.fill('0');
    ss.width(5);
    ss << i << ".root";

    TFile* fileAna  = TFile::Open((fileNameAna + ss.str()).c_str(), "READ");
    TFile* fileReco = TFile::Open((fileNameReco + ss.str()).c_str(), "READ");

    Bool_t isGoodFile = CheckFile(fileAna, fileSizeLimit, nofEvents) && CheckFile(fileReco, fileSizeLimit, nofEvents);
    if (fileReco != NULL) fileReco->Close();
    if (isGoodFile) {
      if (addString == "analysis") {
        fileList->Add(fileAna);
        count++;
      }
      if (addString == "litqa") {
        TFile* fileQa       = TFile::Open((fileNameQa + ss.str()).c_str(), "READ");
        Bool_t isGoodQaFile = CheckFileSize(fileQa, fileSizeLimit);
        if (isGoodQaFile) {
          fileList->Add(fileQa);
          count++;
        }
        else {
          if (fileQa != NULL) fileQa->Close();
        }
        if (fileAna != NULL) fileAna->Close();
      }
    }
    else {
      if (fileAna != NULL) fileAna->Close();
      if (fileReco != NULL) fileReco->Close();
    }

    if (fileList->GetEntries() >= maxNofFiles || i == nofFiles - 1) {
      TFile* tf = CreateAndMergeTempTargetFile(dir, addString, targetFileNum, fileList);
      tempTargetFiles->Add(tf);
      CloseFilesFromList(fileList);
      fileList->RemoveAll();
      targetFileNum++;
    }
  }
  cout << endl << "-I- number of files to merge = " << count << endl << endl;

  string outputFile = fileNameAna + "all.root";
  if (addString == "litqa") outputFile = fileNameQa + "all.root";
  cout << "-I- OUTPUT: " << outputFile << endl;
  TFile* targetFile = TFile::Open(outputFile.c_str(), "RECREATE");
  MergeRootfile(targetFile, tempTargetFiles);
  CloseFilesFromList(tempTargetFiles);
  targetFile->Close();

  if (targetFile != NULL) delete targetFile;
  if (tempTargetFiles != NULL) delete tempTargetFiles;
  if (fileList != NULL) delete fileList;
}

TFile* CbmHaddBase::CreateAndMergeTempTargetFile(const string& dir, const string& addString, Int_t targetFileNum,
                                                 TList* fileList)
{
  cout << "-I- CreateAndMergeTempTargetFile no:" << targetFileNum << ", nofFiles:" << fileList->GetEntries() << endl;
  stringstream ss;
  ss << targetFileNum << ".root";
  TFile* targetFile = TFile::Open(string(dir + addString + ".temp.taget.file." + ss.str()).c_str(), "RECREATE");
  if (fileList->GetEntries() > 0) MergeRootfile(targetFile, fileList);
  return targetFile;
}


void CbmHaddBase::CloseFilesFromList(TList* fileList)
{
  int nFL = fileList->GetEntries();
  for (int iFL = 0; iFL < nFL; iFL++) {
    TFile* f = (TFile*) fileList->At(iFL);
    if (f != NULL) {
      f->Close();
      delete f;
    }
  }
}


void CbmHaddBase::MergeRootfile(TDirectory* target, TList* sourcelist)
{
  //  cout << "Target path: " << target->GetPath() << endl;
  TString path((char*) strstr(target->GetPath(), ":"));
  path.Remove(0, 2);

  TFile* first_source = (TFile*) sourcelist->First();
  first_source->cd(path);
  TDirectory* current_sourcedir = gDirectory;
  //gain time, do not add the objects in the list in memory
  Bool_t status = TH1::AddDirectoryStatus();
  TH1::AddDirectory(kFALSE);

  // loop over all keys in this directory
  //	TChain *globChain = 0;
  TIter nextkey(current_sourcedir->GetListOfKeys());
  TKey *key, *oldkey = 0;
  while ((key = (TKey*) nextkey())) {
    //keep only the highest cycle number for each key
    if (oldkey && !strcmp(oldkey->GetName(), key->GetName())) continue;

    // read object from first source file
    first_source->cd(path);
    TObject* obj = key->ReadObj();

    if (obj->IsA()->InheritsFrom(TH1::Class())) {
      // descendant of TH1 -> merge it

      //      cout << "Merging histogram " << obj->GetName() << endl;
      TH1* h1 = (TH1*) obj;

      // loop over all source files and add the content of the
      // correspondant histogram to the one pointed to by "h1"
      TFile* nextsource = (TFile*) sourcelist->After(first_source);
      while (nextsource) {
        // make sure we are at the correct directory level by cd'ing to path
        nextsource->cd(path);
        TKey* key2 = (TKey*) gDirectory->GetListOfKeys()->FindObject(h1->GetName());
        if (key2) {
          TH1* h2 = (TH1*) key2->ReadObj();
          h1->Add(h2);
          delete h2;
        }
        // cout << nextsource->GetPath() << endl;
        nextsource = (TFile*) sourcelist->After(nextsource);
      }
    }
    //      else if ( obj->IsA()->InheritsFrom( TTree::Class() ) ) {
    //
    //         // loop over all source files create a chain of Trees "globChain"
    //         const char* obj_name= obj->GetName();
    //
    //         globChain = new TChain(obj_name);
    //         globChain->Add(first_source->GetName());
    //         TFile *nextsource = (TFile*)sourcelist->After( first_source );
    //         //      const char* file_name = nextsource->GetName();
    //         // cout << "file name  " << file_name << endl;
    //         while ( nextsource ) {
    //
    //            globChain->Add(nextsource->GetName());
    //            nextsource = (TFile*)sourcelist->After( nextsource );
    //         }
    //
    //      } else if ( obj->IsA()->InheritsFrom( TDirectory::Class() ) ) {
    //         // it's a subdirectory
    //
    //         cout << "Found subdirectory " << obj->GetName() << endl;
    //
    //         // create a new subdir of same name and title in the target file
    //         target->cd();
    //         TDirectory *newdir = target->mkdir( obj->GetName(), obj->GetTitle() );
    //
    //         // newdir is now the starting point of another round of merging
    //         // newdir still knows its depth within the target file via
    //         // GetPath(), so we can still figure out where we are in the recursion
    //         MergeRootfile( newdir, sourcelist );
    //
    //      } else {
    //
    //         // object is of no type that we know or can handle
    //         cout << "Unknown object type, name: "
    //         << obj->GetName() << " title: " << obj->GetTitle() << endl;
    //      }

    // now write the merged histogram (which is "in" obj) to the target file
    // note that this will just store obj in the current directory level,
    // which is not persistent until the complete directory itself is stored
    // by "target->Write()" below
    if (obj) {
      target->cd();

      //!!if the object is a tree, it is stored in globChain...
      //         if(obj->IsA()->InheritsFrom( TTree::Class() ))
      //            globChain->Merge(target->GetFile(),0,"keep");
      //         else
      obj->Write(key->GetName());
    }

  }  // while ( ( TKey *key = (TKey*)nextkey() ) )

  // save modifications to target file
  target->SaveSelf(kTRUE);
  TH1::AddDirectory(status);
}
