#ifndef PAIRANALYSISMETADATA_H
#define PAIRANALYSISMETADATA_H
///////////////////////////////////////////////////////////////////////////////////////////
//                                                                                       //
// Meta data container:                                                                  //
// information that can be used for plotting, qa, trending, ....                         //
//                                                                                       //
//   Julian Book   <Julian.Book@cern.ch>                                                 //
//                                                                                       //
///////////////////////////////////////////////////////////////////////////////////////////

#include <Rtypes.h>

#include <TNamed.h>
// #include <TCollection.h>
#include <TList.h>
/* #include <TParamater<Int_t>.h> */
/* #include <TParamater<Double_t>.h> */

#include <TList.h> //new

class TString;
class TList;
class TParmameter;
// class TVectorT<double>;

class PairAnalysisMetaData : public TNamed {
public:

  PairAnalysisMetaData();
  PairAnalysisMetaData(const char* name, const char* title);
  virtual ~PairAnalysisMetaData();

  void Init();

  // setter
  void FillMeta(const char *name, Double_t val);
  void FillMeta(const char *name, Int_t    val);
  void FillMeta(const char *name, const char *val);

  void GetMeta(const char *name, Int_t *val);
  void GetMeta(const char *name, Double_t *val);

  void SetMetaData(TList &list, Bool_t setOwner=kTRUE);
  void ResetMetaData()                 { fMetaList.Clear(); }
  const TList* GetMetaData() const { return &fMetaList; }

  TObject* FindObject(const char *name) const { return fMetaList.FindObject(name); }

  void DrawSame(TString opt="msb");

private:

  TList fMetaList;             //-> list of parameters

  PairAnalysisMetaData(const PairAnalysisMetaData &hist);
  PairAnalysisMetaData& operator = (const PairAnalysisMetaData &hist);

  ClassDef(PairAnalysisMetaData,1) // Meta Data Manager
};
#endif
