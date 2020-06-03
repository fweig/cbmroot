/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   HitReader.h
 * Author: tablyaz
 *
 * Created on August 18, 2017, 11:23 AM
 */

#ifndef HITREADER_H
#define HITREADER_H

#include "CbmPixelHit.h"
#include <vector>
#include <map>
#include <string>
#include "TClonesArray.h"
#include "Settings.h"

class CbmBinnedStation;

class CbmBinnedHitReader
{
public:
    static CbmBinnedHitReader* Instance();
    static CbmBinnedHitReader* Instance(const char* name);
    static void AddReader(const char* name);
    static void SetSettings(CbmBinnedSettings* v) { fSettings = v; }
    
public:
    CbmBinnedHitReader();
    CbmBinnedHitReader(const CbmBinnedHitReader&) = delete;
    CbmBinnedHitReader& operator=(const CbmBinnedHitReader&) = delete;
    virtual ~CbmBinnedHitReader();
    virtual void AddStation(CbmBinnedStation* station) { fStations.push_back(station); }
    virtual void Read() = 0;
    virtual void Handle() {}// Called optionally and is not implemented by the most of readers.
    virtual void Finish() {}// Called optionally and is not implemented by the most of readers.
    
protected:
    std::vector<CbmBinnedStation*> fStations;
    TClonesArray* fHitArray;
    
protected:
    static std::map<std::string, CbmBinnedHitReader*> fReaders;
    static CbmBinnedSettings* fSettings;
};

#endif /* HITREADER_H */

