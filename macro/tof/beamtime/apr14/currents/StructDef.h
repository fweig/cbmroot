
#include "Riostream.h"

typedef struct {
   Int_t    iTimeSec;
   Int_t    iTimeMilliSec;
   Double_t dVoltageNeg;
   Double_t dCurrentNeg;
   Int_t    iPowerNeg;
   Int_t    iStatusNeg;
   Double_t dVoltagePos;
   Double_t dCurrentPos;
   Int_t    iPowerPos;
   Int_t    iStatusPos;
} Rpc_HV;

void Rpc_HV::Clear()
{
   iTimeSec      = 0;
   iTimeMilliSec = 0;
   dVoltageNeg   = 0.0;
   dCurrentNeg   = 0.0;
   iPowerNeg     = -1;
   iStatusNeg    = -1;
   dVoltagePos   = 0.0;
   dCurrentPos   = 0.0;
   iPowerPos     = -1;
   iStatusPos    = -1;
};
void Rpc_HV::Rpc_HV( &Rpc_HV in )
{
   iTimeSec      = in.iTimeSec;
   iTimeMilliSec = in.iTimeMilliSec;
   dVoltageNeg   = in.dVoltageNeg;
   dCurrentNeg   = in.dCurrentNeg;
   iPowerNeg     = in.iPowerNeg;
   iStatusNeg    = in.iStatusNeg;
   dVoltagePos   = in.dVoltagePos;
   dCurrentPos   = in.dCurrentPos;
   iPowerPos     = in.iPowerPos;
   iStatusPos    = in.iStatusPos;
};

void Rpc_HV::Print()
{
   TString sOut = Form( " Time %09d s %03d ms V- = %5.2f I- = %5.2f P- = %1d S- = %1d V+ = %5.2f I+ = %5.2f P+ = %1d S+ = %1d",
                        iTimeSec, iTimeMilliSec, 
                        dVoltageNeg, dCurrentNeg, iPowerNeg, iStatusNeg,
                        dVoltagePos, dCurrentPos, iPowerPos, iStatusPos );
   cout<< sOut << endl;
};

typedef struct {
   Int_t    iTimeSec;
   Int_t    iTimeMilliSec;
   Double_t dVoltage;
   Double_t dCurrent;
   Int_t    iPower;
   Int_t    iStatus;
} Pmt_HV;

void Pmt_HV::Clear()
{
   iTimeSec      = 0;
   iTimeMilliSec = 0;
   dVoltage      = 0.0;
   dCurrent      = 0.0;
   iPower        = -1;
   iStatus       = -1;
};
void Pmt_HV::Rpc_HV( &Pmt_HV in )
{
   iTimeSec      = in.iTimeSec;
   iTimeMilliSec = in.iTimeMilliSec;
   dVoltage      = in.dVoltage;
   dCurrent      = in.dCurrent;
   iPower        = in.iPower;
   iStatus       = in.iStatus;
};

void Pmt_HV::Print()
{
   TString sOut = Form( " Time %09d s %03d ms V- = %5.2f I- = %5.2f P- = %1d S- = %1d",
                        iTimeSec, iTimeMilliSec, 
                        dVoltage, dCurrent, iPower, iStatus);
   cout<< sOut << endl;
};
