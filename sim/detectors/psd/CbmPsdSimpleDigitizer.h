/** CbmPsdSimpleDigitize.h
 *@author Sergey Morozov <morozovs@inr.ru>
 **
 ** The very simple digitizer produces digits of type CbmPsdDigi as sum of Edep
 **for all scintilators and simulates SiPM pixel statistics and electronics noise.
 **/


#ifndef CBMPSDSimpleDigitizer_H
#define CBMPSDSimpleDigitizer_H 1


#include "CbmDefs.h"
#include "CbmDigitize.h"
#include "CbmPsdDigi.h"

#include "TRandom3.h"

#define N_PSD_MODS 46
#define N_PSD_SECT 10

class TClonesArray;

class CbmPsdSimpleDigitizer : public CbmDigitize<CbmPsdDigi> {

public:
  /** Default constructor **/
  CbmPsdSimpleDigitizer();


  /** Destructor **/
  virtual ~CbmPsdSimpleDigitizer();

  ECbmModuleId GetSystemId() const { return ECbmModuleId::kPsd; }


  /** Virtual method Init **/
  virtual InitStatus Init();


  /** Virtual method Exec **/
  virtual void Exec(Option_t* opt);


  /** @brief End-of-run action **/
  virtual void Finish();


private:
  Int_t fNofEvents;
  Double_t fNofPoints;
  Double_t fNofDigis;
  Double_t fTimeTot;

  /** Input array of CbmPsdPoints **/
  TClonesArray* fPointArray;

  CbmPsdSimpleDigitizer(const CbmPsdSimpleDigitizer&);
  CbmPsdSimpleDigitizer operator=(const CbmPsdSimpleDigitizer&);

  ClassDef(CbmPsdSimpleDigitizer, 1);
};

#endif
