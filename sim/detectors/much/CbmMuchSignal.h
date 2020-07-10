/**CbmMuchSignal.h
 * @author Vikas Singhal <vikas@vecc.gov.in>
 **@since 10/04/18
 **@version 2.0
 ** Removing fSignalShape and introducing fCharge and fModifiedCharge
 ** and fPileupTime
 **@since 10/08/16
 **@version 1.0
 ** Data class for analog MUCH information
 ** Data level: buffered in the CbmMuchReadoutBuffer
 **
 **
 **/


#ifndef CBMMUCHSIGNAL_H
#define CBMMUCHSIGNAL_H 1
#define SLOWSHAPERPEAK 75

#include "CbmMatch.h"
#include "TArrayD.h"
#include "TArrayI.h"
#include "TObject.h"


class TClonesArray;

//static const Int_t gkResponseBin	= 1;
static const Int_t fDeadTime = 400;  // Deadtime

class CbmMuchSignal : public TObject {
public:
  /** Default Constructor */
  CbmMuchSignal()
    : TObject()
    , fAddress(0)
    , fTimeStart(0)
    , fTimeStop(0)
    , fCharge(0)
    , fPileUpCount(0)
    , fMatch(nullptr) {}
  //CbmMuchSignal(): TObject(), fAddress(0), fTimeStart(0), fTimeStop(0), fCharge(0), fPileUpTime(0), fModifiedCharge(0), fPileUpCount(0),fMatch(nullptr) {}
  //WithSignalShape  CbmMuchSignal(): TObject(), fAddress(0), fTimeStart(0), fTimeStop(0), fSignalShape(0), fMatch(nullptr) {}

  /** Standard constructor
	** @param  fTimeStart     Start time [ns]
	** @param  fTimeStop      Stop time [ns] = Start Time + Dead Time
	** @param  fCharge        Charge in electrons as slowshaper output of XYTER 
	** @param  fModifiedCharge        Charge after PileUp and checks whetherslowshaper crossed 70-80ns time window. If yes then no change and fCharge will be used. If no then charge will be calculated as per slowshaper behavior of XYTER 
 	**
	**/
  CbmMuchSignal(UInt_t address, Long64_t starttime = 0)
    : TObject()
    , fAddress(address)
    , fTimeStart(starttime)
    , fTimeStop(fTimeStart + fDeadTime)
    , fCharge(0)
    , fPileUpCount(0)
    , fMatch(new CbmMatch()) {}
  /*	CbmMuchSignal(UInt_t address, ULong64_t starttime=0)
		: TObject(), fAddress(address), fTimeStart(starttime), fTimeStop(fTimeStart + fDeadTime), fCharge(0), fPileUpTime(0), fModifiedCharge(0), fPileUpCount(0), fMatch(new CbmMatch())
		{
		}

	CbmMuchSignal(UInt_t address, ULong64_t starttime=0)
	//	: TObject(), fAddress(address), fTimeStart(starttime), fTimeStop(fTimeStart + fDeadTime),  fSignalShape(0), fMatch(new CbmMatch())

		{
//			fMatch = new CbmMatch();
//			fTimeStop = fTimeStart + fDeadTime;
		}
	*/

  CbmMuchSignal(CbmMuchSignal* signal) = delete;
  CbmMuchSignal(const CbmMuchSignal&);
  CbmMuchSignal& operator=(const CbmMuchSignal&) = delete;

  /** Destructor **/
  virtual ~CbmMuchSignal() {
    if (fMatch) delete fMatch;
  }


  // Added below functions as CbmMuchSignal will be buffered in the CbmMuchReadoutBuffer.
  // Absolute start time for the CbmMuchSignal. If earlier MCPoint hits the same pad which was hit by later MCPoint and it is within deadtime then earlier MCPoint Time should be start time of this Signal
  Long_t GetTimeStart() const { return fTimeStart; }

  // Absolute stop time for this CbmMuchSignal. If later MCPoint hits on the same pad and it is within deadtime then later MCPoint Time + deadtime should be stop time of this Signal. It shows tha for how much time this pad is active.
  Long64_t GetTimeStop() const { return fTimeStop; }
  //ULong64_t GetPileUpTime() const	{return fPileUpTime; }
  UInt_t GetCharge() const { return fCharge; }
  //UInt_t GetModifiedCharge() const	{return fModifiedCharge; }
  UInt_t GetPileUpCount() const { return fPileUpCount; }
  //TArrayD GetSignalShape() const	{return fSignalShape; }
  CbmMatch* GetMatch() const { return fMatch; }
  Int_t GetAddress() const { return fAddress; }

  //Setters
  void SetTimeStart(Long_t starttime) { fTimeStart = starttime; }
  void SetTimeStop(Long_t stoptime) { fTimeStop = stoptime; }
  //	void SetSignalShape(TArrayD signalshape) {fSignalShape = signalshape; }

  //void SetPileUpTime(Double_t pileuptime) {fPileUpTime = pileuptime; }
  void SetCharge(UInt_t charge) { fCharge = charge; }
  //void SetModifiedCharge(UInt_t modifiedcharge) {fModifiedCharge = modifiedcharge; }
  void SetMatch(CbmMatch* match) { fMatch = match; }

  //void MakeSignalShape(UInt_t,TArrayD);
  //MergeSignal Function which will implement Signal PileUp
  void MergeSignal(CbmMuchSignal*);
  void AddNoise(UInt_t);

  //Int_t GetMaxCharge();
  Int_t GetCharge() { return fCharge; }  //Will return respective ADC value
  Long64_t GetTimeStamp() { return fTimeStart; }

private:
  UInt_t fAddress;      //Unique detector address
  Long64_t fTimeStart;  // Absolute start Time[ns]
  Long64_t fTimeStop;   //Absolut Time till then this digi is active
  //ULong64_t	fPileUpTime; 	//TimewhenPileuphappened
  UInt_t fCharge;  //Charge which will stored in the Digi
  //UInt_t		fModifiedCharge;//If PileUp then modify Charge
  UInt_t fPileUpCount = 0;  //Count for PileUp cases
                            //	TArrayD 	fSignalShape;	// Array of time bins
                            //Making Actual Electronics Signal Shape
  CbmMatch* fMatch;

  ClassDef(CbmMuchSignal, 2);
};
#endif
