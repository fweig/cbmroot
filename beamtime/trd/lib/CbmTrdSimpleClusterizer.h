#ifndef CBMTRDSIMPLECLUSTERIZER_H
#define CBMTRDSIMPLECLUSTERIZER_H


#include "CbmTrdQABase.h"
#include <utility>
class CbmTrdSimpleClusterizer : public CbmTrdQABase
{
 protected:
  CbmDigiManager* fDigiMan = nullptr;
  TClonesArray* fClusters;
  void FillAndSortProxyArray();
  void BubbleSort();


 public:
  std::vector<UInt_t>fProxyArray;
  CbmTrdSimpleClusterizer();
  CbmTrdSimpleClusterizer(const CbmTrdSimpleClusterizer&) = delete;
  CbmTrdSimpleClusterizer& operator=(const CbmTrdSimpleClusterizer&) = delete;
  //  ~CbmTrdSimpleClusterizer();
  InitStatus Init();
  void Exec(Option_t*);
  void CreateHistograms();
    
ClassDef(CbmTrdSimpleClusterizer,1);
};

#endif

