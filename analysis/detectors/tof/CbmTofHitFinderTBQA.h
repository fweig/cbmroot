/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   CbmTofHitFinderTBQA.h
 * Author: tablyaz
 *
 * Created on January 16, 2017, 3:32 PM
 */

#ifndef CBMTOFHITFINDERTBQA_H
#define CBMTOFHITFINDERTBQA_H

#include "FairTask.h"
#include "CbmMCDataArray.h"
#include "TClonesArray.h"
#include "CbmTimeSlice.h"
#include "CbmMCEventList.h"

class CbmTofHitFinderTBQA : public FairTask
{
public:
    CbmTofHitFinderTBQA();
    CbmTofHitFinderTBQA(const CbmTofHitFinderTBQA&) = delete;
    CbmTofHitFinderTBQA& operator=(const CbmTofHitFinderTBQA&) = delete;

    InitStatus Init();
    void Exec(Option_t* option);
    void Finish();
    void SetIsEvByEv(bool v) { isEvByEv = v; }

private:
    bool isEvByEv;
    TClonesArray* fTofHits;
    TClonesArray* fTofDigiMatchs;
    TClonesArray* fTofDigis;
    TClonesArray* fTofDigiPointMatchs;
    CbmMCDataArray* fTofMCPoints;
    CbmMCDataArray* fMCTracks;
    CbmTimeSlice* fTimeSlice;
    CbmMCEventList* fEventList;

    ClassDef(CbmTofHitFinderTBQA, 1)
};

#endif /* CBMTOFHITFINDERTBQA_H */

