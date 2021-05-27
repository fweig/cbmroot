/** @file CbmStsSimModule.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 16.03.2020
 **/

#ifndef CBMSTSSIMMODULE_H
#define CBMSTSSIMMODULE_H 1


#include "CbmStsCluster.h"
#include "CbmStsDigi.h"
#include "CbmStsParModule.h"
#include "CbmStsSignal.h"

#include "TF1.h"
#include "TRandom.h"

#include <map>
#include <set>
#include <vector>

class TClonesArray;
class CbmStsPhysics;
class CbmStsDigitize;
class CbmStsElement;
class CbmStsParAsic;


/** @class CbmStsSimModule
 ** @brief Class for the simulation of a readout unit in the CBM-STS.
 ** @author V.Friese <v.friese@gsi.de>
 ** @data 16.03.2020
 **
 ** The module is the read-out unit in the CBM STS. It consists of one
 ** sensor or two or more daisy-chained sensors (CbmStsSensor), the analogue
 ** cable and the read-out electronics.
 **
 ** The module receives and stores the analogue signals from the sensor(s)
 ** in a buffer. It takes care of interference of signals in one and the
 ** same channel (two signals arriving within a given dead time).
 ** The module digitises the analogue signals and sends them to the
 ** CbmDaq when appropriate.
 **/
class CbmStsSimModule : public TObject {

public:
  /** @brief Standard constructor
     ** @param setupModule  Pointer to module element in the STS setup
     **/
  CbmStsSimModule(CbmStsElement* setupModule = nullptr, const CbmStsParModule* modulePar = nullptr,
                  CbmStsDigitize* digitizer = nullptr);


  /** @brief Copy constructor (disabled) **/
  CbmStsSimModule(const CbmStsSimModule&) = delete;


  /** @brief Assignment operator (disabled) **/
  CbmStsSimModule& operator=(const CbmStsSimModule&) = delete;


  /** @brief Destructor **/
  virtual ~CbmStsSimModule();


  /** Add an analogue signal to the buffer
     **
     ** @param channel        channel number
     ** @param time           time of signal [ns]
     ** @param charge         analogue charge [e]
     ** @param index          index of CbmStsPoint in TClonesArray
     ** @param entry          MC entry (event number)
     ** @param file           MC input file number
     **
     ** The signal will be added to the buffer. Interference with
     ** previous signals within the same channels is checked and the
     ** proper action is executed.
     **/
  void AddSignal(UShort_t channel, Double_t time, Double_t charge, Int_t index = 0, Int_t entry = 0, Int_t file = 0);


  /** Get status of the analogue buffer
     **
     ** @paramOut nofSignals Number of signals in buffer (active channels)
     ** @paramOut timeFirst  Time of first signal in buffer [ns]
     ** @paramOut timeLast   Time of last signal in buffer [ns]
     **/
  void BufferStatus(Int_t& nofSignals, Double_t& timeFirst, Double_t& timeLast);


  /** Convert charge to ADC channel.
     ** @param charge  analogUE charge [e]
     ** @return  ADC channel number
     **
     ** This must be the inverse of AdcToCharge
     **/
  //Int_t ChargeToAdc(Double_t charge, UShort_t channel);


  /** @brief Get the address from the module name (static)
     ** @param name Name of module
     ** @value Unique element address
     **/
  static Int_t GetAddressFromName(TString name);


  /** @brief Digitizer task
     ** @param Pointer to digitizer task
     **/
  CbmStsDigitize* GetDigitizer() const { return fDigitizer; }


  /** @brief Number of electronic channels
     ** @value Number of ADC channels
     **/
  UShort_t GetNofChannels() const { return fParams->GetNofChannels(); };


  /** Initialise the analogue buffer
     ** The analogue buffer contains a std::multiset for each channel, to be
     ** filled with CbmStsSignal objects. Without this method, a channel
     ** multiset would be instantiated at run time when the first signal
     ** for this channel arrives. Depending on the occupancy of this channel,
     ** this may happen only after several hundreds of events. Consequently,
     ** the memory consumption will increase for the first events until
     ** each channel was activated at least once. This behaviour mimics a
     ** memory leak and makes it harder to detect a real one in other parts
     ** of the code. This is avoided by instantiating each channel multiset
     ** at initialisation time.
     **/
  void InitAnalogBuffer();


  /** Check whether module parameters are set
     ** @value kTRUE if parameters are set
     **/
  Bool_t IsSet() const { return fIsSet; }


  /** Digitise signals in the analogue buffer
     ** @param time  readout time [ns]
     ** @return Number of created digis
     **
     ** All signals with time less than the readout time minus a
     ** safety margin (taking into account dead time and time resolution)
     ** will be digitised and removed from the buffer.
     **/
  Int_t ProcessAnalogBuffer(Double_t readoutTime);


  void SetParams(const CbmStsParModule& params) { fParams = &params; }

  /** Get vector of individual ASIC parameters of this module
     **/
  /* Not used
    std::vector<CbmStsDigitizeParameters>& GetParameters() {
      return fAsicParameterVector;
    }
     */


  /** Get parameters of the ASIC corresponding to the module channel number
     ** @param moduleChannel  module channel number
     **/
  /*
    const CbmStsParAsic& GetAsicParameters(UShort_t moduleChannel) const {
      return fParams->GetAsicPar(moduleChannel);
    }
     */


  /** @brief Generate noise
     ** @param t1  Start time [ns]
     ** @param t2  Stop time [n2]
     **
     ** This method will generate noise digis in the time interval [t1, t2]
     ** according to Rice's formula. The number of noise digis in this
     ** interval is sampled from a Poissonian with mean calculated from
     ** the single-channel noise rate, the number of channels and the
     ** length of the time interval. The noise hits are randomly distributed
     ** to the channels. The time of each noise digi is sampled from a flat
     ** distribution, its charge from a Gaussian with sigma = noise,
     ** truncated at threshold.
     **/
  Int_t GenerateNoise(Double_t t1, Double_t t2);


  /** String output **/
  std::string ToString() const;


private:
  Bool_t fIsSet                  = kFALSE;   ///< ? Parameters are set
  CbmStsElement* fElement        = nullptr;  //! Element in geometry setup
  CbmStsDigitize* fDigitizer     = nullptr;  //! Digitizer
  const CbmStsParModule* fParams = nullptr;  //! Module parameters

  /** Buffer for analog signals, key is channel number.
     ** Because signals do not, in general, arrive time-sorted,
     ** the buffer must hold a (multi)set of signals for each channel
     ** (multiset allowing for different signals at the same time).
     ** Sorting in the multiset is with the less operator of CbmStsSignal,
     ** which compares the time of the signals.
     **/
  typedef std::multiset<CbmStsSignal*, CbmStsSignal::Before> sigset;
  std::map<UShort_t, sigset> fAnalogBuffer;


  /** Digitise an analogue charge signal
     ** @param channel Channel number
     ** @param signal  Pointer to signal object
     **/
  void Digitize(UShort_t channel, CbmStsSignal* signal);


  ClassDef(CbmStsSimModule, 1);
};

#endif /* CBMSTSSIMMODULE_H */
