#include "TTree.h"
#include "Riostream.h"
#include "TGrid.h"
#include "TFile.h"
#include "TSystem.h"
#include "TError.h"
#include "TLeaf.h"
#include "TFolder.h"

#include "TObjArray.h"
#include "TCollection.h"
#include "TList.h"
#include "TObjString.h"


void DisableBranches(TTree* tree);

void skimming_reco(TString inFile)
{

  // I/O files
  TString outFile = inFile;
  outFile.ReplaceAll(".root","skim.root");

  TTree::SetMaxTreeSize(90000000000);

  /// set root ignore level to supress:
  // Could not find the real data member 'PrimaryVertex' when constructing the branch 'PrimaryVertex'

  ///  gErrorIgnoreLevel = kPrint, kInfo, kWarning, kError, kBreak, kSysError, kFatal;
  gErrorIgnoreLevel = kFatal;


  //Get old file, old tree and set top branch address
  TFile *oldfile = new TFile(inFile.Data());
  TTree *oldtree = (TTree*)oldfile->Get("cbmsim");

  // disable all branches not needed
  DisableBranches(oldtree);

  //Create a new file (a skimmed clone of old tree)
  TFile *newfile = new TFile(outFile.Data(),"recreate");

  // get the tree and clone
  TTree *newtree = oldtree->CloneTree(-1,"fast");
  newtree->AutoSave();

  // get basic branch lists (needed by the FairFileSource)
  TList* list    = dynamic_cast <TList*>   (oldfile->Get("BranchList"));
  TFolder* added = dynamic_cast <TFolder*> (oldfile->Get("cbmout"));
  newfile->cd();
  if(list)  list->Write("BranchList",TObject::kSingleKey);
  if(added) added->Write();

  // close the file
  newfile->Close();
  delete newfile;

  delete oldfile;

}

void DisableBranches(TTree* tree)
{
  TObjArray* list = tree->GetListOfLeaves();
  TIter next((TCollection*)list);
  TLeaf* leaf;

  // list of branches not needed/wanted
  TObjArray branchesToDelete;
  branchesToDelete.SetOwner(kTRUE);
  // branchesToDelete.Add(new TObjString("Mvd"));
  branchesToDelete.Add(new TObjString("Digi")   );
  branchesToDelete.Add(new TObjString("Cluster"));

  TIter nit(&branchesToDelete);

  // default is to keep all bracnhes
  tree->SetBranchStatus("*",1);

  Bool_t on(kFALSE);

  while ( ( leaf = static_cast<TLeaf*>(next()) ) )
    {
      TString name(leaf->GetName());
      if ( (name.EndsWith("_") || name.EndsWith(".")) )
	{
	  //	  Printf("check: %s",leaf->GetName());
	  TObjString* str;
	  nit.Reset();
	  on=kTRUE;
	  while ( ( str = static_cast<TObjString*>(nit()) ) )
	    {
	      if ( name.Contains(str->String()) && name.EndsWith("_") )
		{
		  on = kFALSE;
		}
	    }
	}
      // switch of branch copy
      if ( !on )
	{
	  //	  Printf("remove: %s",leaf->GetName());
	  leaf->GetBranch()->SetStatus(0);
	}
    }

}
