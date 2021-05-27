/** @file CbmTrdFASP.h
  * @copyright Copyright (C) 2018-2020 Horia Hulubei National Institute of Physics and Nuclear Engineering, Bucharest
  * @license SPDX-License-Identifier: GPL-3.0-only
  * @authors Alexandru Bercuci [orginator] **/

#ifndef CBMTRDFASP_H
#define CBMTRDFASP_H

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Float_t, ULong64_t, UInt_t, Bool_t
#include <TObject.h>     // for TObject
#include <tuple>         // for tuple

#include <utility>  // for pair
#include <vector>   // for vector

class CbmMatch;
class CbmTrdDigi;
class CbmTrdParFaspChannel;
class TCanvas;
class TGraph;
class TLine;

#define FASP_WINDOW 200
#define SHAPER_LUT 80
#define NGRAPH 160

/** \brief FASP channel simulator **/
class CbmTrdFASP : public TObject {
public:
  /** \brief Constructor of FASP simulator
   * \param[in] uslice length of microslice [ns]
   */
  CbmTrdFASP(UInt_t uslice = 1000);
  virtual ~CbmTrdFASP();
  /** \brief Finalize currently stored data*/
  virtual void Clear(Option_t* opt = "");
  /** \brief Graphical representation of FASP analog/digital response to currently stored data*/
  virtual void Draw(Option_t* opt = "");
  virtual ULong64_t GetEndTime() const { return fStartTime + fProcTime; }
  virtual ULong64_t GetStartTime() const { return fStartTime; }
  /** \brief Return the baseline value in ADC ch*/
  static Float_t GetBaselineCorr() { return 4095. * fgBaseline / fgOutGain; }
  /** \brief Check if there is enough time elapsed from fStartTime to run simulator*/
  virtual Bool_t Go(ULong64_t time);
  /** \brief [Re]Start processing of one channel
   * \param[in] col current column
   * \param[in] par FASP channel parametrization
   */
  virtual void Init(Int_t col, CbmTrdParFaspChannel* par);
  /** \brief Convert physics information in digi to the raw format
   * \param[in] digi list of digits for the current channel
   * \param[in] col column of current digis
   * \param[in] row row of current digis
   */
  virtual void PhysToRaw(std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>* digi, Int_t col = 0, Int_t row = 0);
  /** \brief Print-out FASP analog/digital response to currently stored data*/
  virtual void Print(Option_t* opt = "") const;
  /** \brief Set column for the current channel
   * \param[in] col column number in the current row. Default -1 for the first data
   */
  void SetCol(Int_t col) { fCol = col; }
  /** \brief Set linear-gate minimum length
   * \param[in] nclk number of clock cycles at current clock frequency
   */
  static void SetLGminLength(Int_t nclk) { fgNclkLG = nclk; }
  /** \brief Set FASP trigger mode 
   * \param[in] nb Enable trigger for neighbor channels [default = on]
   */
  static void SetNeighbourTrigger(Bool_t nb = kTRUE) { fgNeighbour = nb; }
  /** \brief Set threshold for the neighbour channel. CADENCE value 
   * \param[in] thr Threshold value [V] - default 100 mV
   */
  static void SetNeighbourThr(Float_t thr = 0.1) { fgNeighbourThr = thr; }
  /** \brief  Set threshold for the current channel. CADENCE value
   * \param[in] thr Threshold value [V] - default 200 mV
   */
  static void SetShaperThr(Float_t thr = 0.2) { fgShaperThr = thr; }
  /** \brief  Set limit in time for processing digis. */
  void SetProcTime(ULong64_t t = 0);
  /** \brief Set buffer time offset [ns]*/
  void SetStartTime(ULong64_t t) { fStartTime = t; }

protected:
  /** \brief Retrive linear interpolation of CADENCE for signal
   * \param[in] charge charge on channel
   */
  void GetShaperSignal(Double_t charge);
  /** \brief Make convolution of shaper1 superposition and theoretic shaping model (see fgkShaperPar)*/
  Double_t MakeOut(Int_t t);
  /** \brief Calculate output FASP signal and CS timming for the signal array stored in fShaper 
   * \return no. of raw digi found
   */
  Int_t ProcessShaper(Char_t typ = 'T');
  /** \brief Read digi array for single channel processing */
  void ScanDigi(std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>* digi, Int_t col, Int_t row);
  /** \brief Read digi array for neighbour trigger processing */
  void ScanDigiNE(std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>* digi, Int_t col, Int_t row);
  /** \brief Write processed digi to output array */
  void WriteDigi();

  ULong64_t fStartTime;  ///< time offset [ns] for the current simulation
  UInt_t fProcTime;      ///< time window [ns] for actual digi processing (excluded fgkBufferKeep)
  Int_t fCol;            ///< current column
  Int_t fRow;            ///< current row
  Int_t fAsicId;         ///< identifier of FASP in module
  Int_t fNphys[2];       ///< number of physical digi in the current [0] and next [1] shaper
  Int_t fNraw;           ///< number of raw digi for the tilt channel
  std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>* fDigi;  ///< link to digi vector to be transformed

  // analog support
  std::vector<bool> fHitThPrev;      ///< previous channel hit threshold
  std::vector<Float_t> fShaper;      ///< current channel shaper analog
  std::vector<Float_t> fShaperNext;  ///< next channel shaper analog
  std::vector<std::tuple<UInt_t, UInt_t, UInt_t, Bool_t>>
    fDigiProc;                   ///< proccessed info <hit_time[ns], CS_time[ns], OUT[ADC], trigger>
  Float_t fSignal[FASP_WINDOW];  ///< temporary array to store shaper analog signal for current charge interpolation

  // FASP channel characteristics
  CbmTrdParFaspChannel* fPar;  ///< current FASP channel parametrization
  Int_t fTimeLG;               ///< Linear gate time length [5*ns]
  Int_t fTimeFT;               ///< Chip Select time legth [5*ns]
  Int_t fTimeDY;               ///< Time decay from FT [5*ns]
  Float_t fFT;                 ///< Flat Top value [V]

  // draw support
  Int_t fGraphId;                                  ///< current graph to be filled if draw support is compiled
  std::vector<Float_t> fOut;                       ///< analog output for the current channel
  std::vector<std::pair<Int_t, Int_t>> fGraphMap;  ///<  map of ASIC id and output FASP signals
  TGraph* fGraph[NGRAPH];                          ///< graph representations of analog FASP response
  TGraph* fGraphShp[NGRAPH];                       ///< graph representations of FASP shaper
  TGraph* fGraphPhys[NGRAPH];                      ///< graph representations of physics digi
  TLine* fGthr;                                    ///< graph representation of various thresholds
  TCanvas* fMonitor;                               ///< monitor canvas when drawing

  // CADENCE parameters
  static const Int_t fgkNDB = 53;                       ///< DB shaper size
  static const Float_t fgkCharge[fgkNDB];               ///< DB input charge discretization
  static const Float_t fgkShaper[fgkNDB][FASP_WINDOW];  ///< DB shaper signals for each input charge discretization
  static const Float_t fgkShaperPar[4];                 ///< shaper parameters
  static const Float_t fgkShaperLUT[SHAPER_LUT];        ///< shaper LUT
  static const Float_t fgkDecayLUT[SHAPER_LUT];         ///< forced discharged of FASP LUT

  // FASP configuration parameters
  static const Int_t fgkNclkFT;   ///< length of flat top in FASP clocks
  static Int_t fgNclkLG;          ///< length of linear-gate command in FASP clocks
  static Bool_t fgNeighbour;      ///< Neighbour enable flag
  static Float_t fgNeighbourThr;  ///< neighbour threshold [V] for fgNeighbour=kTRUE
  static Float_t fgShaperThr;     ///< shaper threshold [V]
  static Float_t fgBaseline;      ///< FASP baseline [V]
  static Float_t fgOutGain;       ///< FASP -> ADC gain [V/4095 ADC]

  // FASP simulator configuration
  static const Int_t fgkBufferKeep;  ///< length of buffer time in 5ns which is kept between cycles

  ClassDef(CbmTrdFASP, 1)  // FASP ASIC simulator
};

#endif
