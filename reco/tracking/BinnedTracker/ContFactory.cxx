/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "ContFactory.h"
#include "FairRuntimeDb.h"
#include "Settings.h"

#include <iostream>

using std::cout;
using std::endl;

ClassImp(CbmBinnedContFactory)

static CbmBinnedContFactory gCbmBinnedContFactory;
   
CbmBinnedContFactory::CbmBinnedContFactory()
{
   fName = "CbmBinnedContFactory";
   fTitle = "Factory for parameter containers in libBinnedTracker";
   setAllContainers();
   FairRuntimeDb::instance()->addContFactory(this);
}

void CbmBinnedContFactory::setAllContainers()
{
   FairContainer* p1 = new FairContainer("CbmBinnedSettings", "Binned tracker reconstruction parameters", "Default");
   p1->addContext("Default");
   containers->Add(p1);
}

FairParSet* CbmBinnedContFactory::createContainer(FairContainer* c)
{
   const char* name = c->GetName();
   cout << " -I container name " << name << endl;
   FairParSet* p = 0;
  
   if (0 == strcmp(name, "CbmBinnedSettings"))
      p = CbmBinnedSettings::Instance();
   
   return p;
}
