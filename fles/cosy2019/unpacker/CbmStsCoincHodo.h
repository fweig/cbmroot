/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *              GNU Lesser General Public Licence (LGPL) version 3,             *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
#ifndef CBMCHECKCOINCHODO_H
#define CBMCHECKCOINCHODO_H

#include "FairTask.h"

#include "TString.h"
#include "TH3.h"
#include "TClonesArray.h"

//class TClonesArray;
class TH1;
class TH2;
class TH3;

class CbmStsCoincHodo : public FairTask
{
  public:

    /** Default constructor **/
    CbmStsCoincHodo();

    CbmStsCoincHodo(const CbmStsCoincHodo&) = delete;
    CbmStsCoincHodo operator=(const CbmStsCoincHodo&) = delete;

    /** Constructor with parameters (Optional) **/
    //  CbmStsCoincHodo(Int_t verbose);


    /** Destructor **/
    ~CbmStsCoincHodo();


    /** Initiliazation of task at the beginning of a run **/
    virtual InitStatus Init();

    /** ReInitiliazation of task when the runID changes **/
    virtual InitStatus ReInit();


    /** Executed for each event. **/
    virtual void Exec(Option_t*);

    /** Finish task called at the end of the run **/
    virtual void Finish();

    inline void SetOutFilename( TString sNameIn ) { fOutFileName = sNameIn; }

    inline void SetTsStart( Int_t i ) { TsStart = i; }
    inline void SetTsStop( Int_t i ) { TsStop = i; }
    
  private:

    void CreateHistos();
    void WriteHistos();


    /** Input array from previous already existing data level **/
    TClonesArray* arrayClusters = nullptr;
    TClonesArray* arrayHits     = nullptr;
    
    /// Constants
   Double_t dPosZHodoA =  200.0;
   Double_t dPosZSts   =  229.0;
   Double_t dPosZHodoB =  264.0;
   Double_t dHodoDistZ = dPosZHodoB - dPosZHodoA;
   Double_t dStsDistZ  = dPosZSts   - dPosZHodoA;

   Double_t dMidStsHodoA = ( dPosZHodoA + dPosZSts ) / 2.0;
   Double_t dMidStsHodoB = ( dPosZHodoB + dPosZSts ) / 2.0;

   Int_t    iCoincLimitClk = 100;
   Double_t dClockCycle    =   3.125; // ns
   Double_t dCoincLimit    = iCoincLimitClk * dClockCycle;
 
    //
    Int_t fNbTs = 0;
    Int_t fNrOfStsDigis = 0; 

    //
    Int_t TsStart;
    Int_t TsStop;
    
    // Histos
    TH1 * phHitsStsTime;
    TH1 * phHitsHodoATime;
    TH1 * phHitsHodoBTime;

    TH2 * phHitsPositionHodoA;
    TH2 * phHitsPositionSts;
    TH2 * phHitsPositionHodoB;
    TH2 * phNbHitsCompHodo;
    TH2 * phNbHitsCompStsHodoA;
    TH2 * phNbHitsCompStsHodoB;
    
    TH2 * phHitsCoincCorrXX;
    TH2 * phHitsCoincCorrYY;
    TH2 * phHitsCoincCorrXY;
    TH2 * phHitsCoincCorrYX;
    TH2 * phHitsPositionCoincA;
    TH2 * phHitsPositionCoincB;
    TH2 * phHitsPositionDiff;
    TH1 * phHitsTimeDiff;
    TH1 * phHitsCoincDist;
    TH1 * phHitsCoincAngle;

    TH2 * phHitsSingleCoincCorrXX;
    TH2 * phHitsSingleCoincCorrYY;
    TH2 * phHitsSingleCoincCorrXY;
    TH2 * phHitsSingleCoincCorrYX;
    TH2 * phHitsSinglePositionCoincA;
    TH2 * phHitsSinglePositionCoincB;
    TH2 * phHitsSinglePositionDiff;
    TH1 * phHitsSingleTimeDiff;
    TH1 * phHitsSingleCoincDist;
    TH1 * phHitsSingleCoincAngle;

    TH2 * phHitsBestCoincCorrXX;
    TH2 * phHitsBestCoincCorrYY;
    TH2 * phHitsBestCoincCorrXY;
    TH2 * phHitsBestCoincCorrYX;
    TH2 * phHitsBestPositionCoincA;
    TH2 * phHitsBestPositionCoincB;
    TH2 * phHitsBestPositionDiff;
    TH1 * phHitsBestTimeDiff;
    TH1 * phHitsBestCoincDist;
    TH1 * phHitsBestCoincAngle;

    TH2 * phHitsPositionCoincExtr;
	
    TH2 * phHitsStsCoincCorrXX;
    TH2 * phHitsStsCoincCorrYY;
    TH2 * phHitsStsCoincCorrXY;
    TH2 * phHitsStsCoincCorrYX;
    TH2 * phHitsStsPositionCoincExtr;
    TH2 * phHitsStsPositionCoinc;
    TH1 * phHitsStsTimeDiff;
    TH2 * phHitsStsPositionDiff;
    TH2 * phHitsStsPositionDiffInv;
	
    TH2 * phHitsStsBestCoincCorrXX;
    TH2 * phHitsStsBestCoincCorrYY;
    TH2 * phHitsStsBestCoincCorrXY;
    TH2 * phHitsStsBestCoincCorrYX;
    TH2 * phHitsStsBestPositionCoincExtr;
    TH2 * phHitsStsBestPositionCoinc;
    TH2 * phHitsStsBestPositionShiftCoinc;
    TH1 * phHitsStsBestTimeDiff;
    TH2 * phHitsStsBestPositionDiff;                                        
    TH2 * phHitsStsBestPositionDiffInv;
    TH3 * phHitsStsBestDiff;                                        

    TH2 * phHitsStsEff;
    
    TString fOutFileName{"testCoincHodo.root"};

    ClassDef(CbmStsCoincHodo,1);
};

#endif
