/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   ContFactory.h
 * Author: tablyaz
 *
 * Created on October 5, 2017, 7:10 PM
 */

#ifndef CBM_BINNED_CONT_FACTORY_H
#define CBM_BINNED_CONT_FACTORY_H

#include "FairContFact.h"

class FairContainer;

class CbmBinnedContFactory : public FairContFact
{
public:
    CbmBinnedContFactory();
    FairParSet* createContainer(FairContainer*);
  
private:
    void setAllContainers();
    ClassDef(CbmBinnedContFactory, 1)
};

#endif /* CBM_BINNED_CONT_FACTORY_H */
