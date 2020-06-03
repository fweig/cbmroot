#include "CbmMediaList.h"

void CbmMediaList::AddEntry(TString path, TString medium) 
{
  fMatList.push_back(std::make_pair(path,medium));
}

ClassImp(CbmMediaList)


