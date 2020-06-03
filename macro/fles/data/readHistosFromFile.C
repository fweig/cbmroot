
#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TTree.h"
#include "TKey.h"
#include "Riostream.h"
#include "TString.h"

void readHistosFromFile(TString filename = "test.root"){
  TFile file = TFile(filename,"READ");
  if (file.IsOpen()){
    TKey* key=NULL;
    TIter iter(file.GetListOfKeys());
     while ( ( key=dynamic_cast<TKey*>(iter()) ) !=0 ) {
      TObject *obj = key->ReadObj();
      if ( obj->IsA()->InheritsFrom( TH1::Class() ) ) {
	cout << "               ..." << obj->GetName() << endl;
	TH1 *h1 = (TH1*)obj;
	// do what ever you like
      } else {

	// object is of no type that we know or can handle
	cout << "Unknown object type, name: "
	     << obj->GetName() << " title: " << obj->GetTitle() << endl;
      }
    }
  } else {
    printf("ERROR:: no file %s found!\n",filename.Data());
  }
}
