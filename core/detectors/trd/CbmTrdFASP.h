/* Copyright (C) 2018-2020 Horia Hulubei National Institute of Physics and Nuclear Engineering, Bucharest
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci [committer] */

#ifndef CBMTRDFASP_H
#define CBMTRDFASP_H

#include "CbmTrdParFasp.h"

#include <Rtypes.h>      // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>  // for Int_t, Float_t, ULong64_t, UInt_t, Bool_t
#include <TObject.h>     // for TObject

#include <map>      // for map
#include <tuple>         // for tuple
#include <utility>  // for pair

class CbmMatch;
class CbmTrdDigi;
class CbmTrdParFaspChannel;
class TCanvas;
class TGraph;
class TLine;

#define FASP_WINDOW 200
#define SHAPER_LUT 80
#define NGRAPH (NFASPMOD * NFASPCH + 1)

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
  /** \brief [Re]Initialize one of the two FASP channels 
   * \param[in] id FASP-CHANNEL identifier
   * \param[in] par FASP-CHANNEL parametrization
   * \param[in] asicId FASP id on the module (as in the DAQ mapping) to be used on drawing
   * \param[in] chId FASP-CHANNEL id on the module (as in the DAQ mapping) to be used on drawing
   */
  virtual void InitChannel(int id, const CbmTrdParFaspChannel* par, int asicId = -1, int chId = -1);
  /** \brief Convert physics information in digi to the raw format
   * \param[in] digi list of digits for the current pad
   */
  virtual void PhysToRaw(std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>* digi);
  /** \brief Print-out FASP analog/digital response to currently stored data*/
  virtual void Print(Option_t* opt = "") const;
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
  int AddGraph(char typ = 'T');
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
  /** \brief Read digi array for one pair T/R defined by the pad column
    \param[in] digi the set of digi and their MC info if avilable
    */
  void ScanDigi(std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>* digi);
  /** \brief Read digi array for neighbour trigger processing */
  void ScanDigiNE(std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>* digi);
  /** \brief Write processed digi to output array */
  void WriteDigi();

  ULong64_t fStartTime = 0;   ///< time offset [ns] for the current simulation
  UInt_t fProcTime     = 0;   ///< time window [ns] for actual digi processing (excluded fgkBufferKeep)
  int fPad             = -1;  ///< current pad as defined by CbmTrdModuleAbstract::GetPadAddress()
  Int_t fNphys[2];       ///< number of physical digi in the current [0] and next [1] shaper
  Int_t fNraw                                           = 0;        ///< number of raw digi for the tilt channel
  std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>* fDigi = nullptr;  ///< link to digi vector to be transformed

  // analog support
  std::vector<bool> fHitThPrev     = {0};   ///< previous channel hit threshold
  std::vector<Float_t> fShaper     = {0.};  ///< current channel shaper analog
  std::vector<Float_t> fShaperNext = {0.};  ///< next channel shaper analog
  std::vector<std::tuple<UInt_t, UInt_t, UInt_t, Bool_t>> fDigiProc =
    {};  ///< proccessed info wrt fStartTime <hit_time[ns], CS_time[ns], OUT[ADC], trigger>
  Float_t fSignal[FASP_WINDOW] = {
    0.};  ///< temporary array to store shaper analog signal for current charge interpolation

  // FASP channel characteristics
  const CbmTrdParFaspChannel* fPar[2] = {nullptr};  ///< current FASP ASIC parametrization
  Int_t fTimeLG                       = -1;         ///< Linear gate time length [5*ns]
  Int_t fTimeFT                       = -1;         ///< Chip Select time legth [5*ns]
  Int_t fTimeDY                       = -1;         ///< Time decay from FT [5*ns]
  Float_t fFT                         = 0.;         ///< Flat Top value [V]

  // FASP graphic support
  int fNgraph               = 1;         ///< No of graphs generated
  int fAsicId[2]            = {-1, -1};  ///< identifier of FASP(s) in module
  int fChId[2]              = {-1, -1};  ///< FASP channels being processed
  std::vector<Float_t> fOut = {0.};      ///< analog output for the current channel
  std::map<int, std::array<int, NFASPCH>> fGraphMap =
    {};                                    ///<  map of ASIC_id and (ch_id, output_id of FASP signals graphs) pairs
  TGraph* fGraph[NGRAPH]     = {nullptr};  ///< graph representations of analog FASP response
  TGraph* fGraphShp[NGRAPH]  = {nullptr};  ///< graph representations of FASP shaper
  TGraph* fGraphPhys[NGRAPH] = {nullptr};  ///< graph representations of physics digi
  TLine* fGthr               = nullptr;    ///< graph representation of various thresholds
  TCanvas* fMonitor          = nullptr;    ///< monitor canvas when drawing

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
