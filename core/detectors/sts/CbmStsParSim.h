/** @file CbmStsParSim.h
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 02.04.2020
 **/

#ifndef CBMSTSPARSIM_H
#define CBMSTSPARSIM_H 1

#include <Rtypes.h>             // for THashConsistencyHolder, ClassDef
#include <RtypesCore.h>         // for Bool_t, kTRUE, kFALSE

#include <FairParGenericSet.h>  // for FairParGenericSet

#include <string>               // for string

#include "CbmStsDefs.h"         // for CbmStsELoss, CbmStsELoss::kUrban

class FairParamList;

/** @class CbmStsParSim
 ** @brief Settings for STS simulation (digitizer)
 ** @author Volker Friese <v.friese@gsi.de>
 ** @date 02.04.2020
 **/
class CbmStsParSim : public FairParGenericSet {

  public:

    /** @brief Constructor **/
    CbmStsParSim(const char* name = "CbmStsParSim",
                 const char* title = "STS parameters",
                 const char* context = "Default");


    /** @brief Destructor **/
    virtual ~CbmStsParSim() {
    }


    /** @brief Reset all parameters **/
    virtual void clear();


    /** @brief Check whether cross-talk is applied
     ** @return If true, cross-talk between the readout channels is applied
     **/
    Bool_t CrossTalk() const { return fCrossTalk; }


    /** @brief Check whether diffusion is applied
     ** @return If true, diffusion in the sensors is applied
     **/
    Bool_t Diffusion() const { return fDiffusion; }


    /** @brief Energy loss model
     ** @return Energy loss model (see enum ECbmElossmModel)
     **/
    CbmStsELoss ELossModel() const { return fELossModel; }


    /** @brief Check whether event-by-event mode is applied
     ** @return If true, simulation will be / were done event-by-event
     **/
    Bool_t EventMode() const { return fEventMode; }


    /** @brief Reading parameters from ASCII. Abstract in base class.
     **
     ** An ASCII I/O is not implemented. The method throws an error.
     **/
    virtual Bool_t getParams(FairParamList* parList);


    /** @brief Check whether Lorentz shift is applied
     ** @return If true, Lorentz shift in the sensors is applied
     **/
    Bool_t LorentzShift() const { return fLorentzShift; }


    /** @brief Check whether inter-event noise is generated
     ** @return If true, noise will be / was generated
     **/
    Bool_t Noise() const { return fNoise; }


    /** @brief Process only primary tracks
     ** @return If true, secondary tracks will be ignored
     **/
    Bool_t OnlyPrimaries() const { return fOnlyPrimaries; }


    /** @brief Writing parameters to ASCII. Abstract in base class.
     **
     ** An ASCII I/O is not implemented. The method throws an error.
     **/
    virtual void putParams(FairParamList* parList);


    /** @brief Set event-by-event simulation mode
     ** @param choice If kTRUE, simulation will be performed event-by-event
     **
     ** By default, simulation are time-based.
     ** In event-by-event simulation, all analogue buffers will be processed
     ** and cleared after each event, such that there is no inteference of
     ** data from different events.
     **/
    void SetEventMode(Bool_t choice = kTRUE) { fEventMode = choice; }


    /** @brief Activate or de-activate inter-event noise
     ** @param choice If kTRUE, inter-event noise will be generated
     **
     ** By default, noise is generated.
     ** This setting will have no effect for event-by-event simulation.
     **/
    void SetGenerateNoise(Bool_t choice = kTRUE) { fNoise = choice; }


    /** @brief Process only primary tracks
     ** @param choice  If true, secondary tracks will be ignored
     **
     ** The option to process only primary tracks exists for detector
     ** studies. If set, the MC points from secondary tracks will
     ** be ignored.
     **/
    void SetOnlyPrimaries(Bool_t choice = kTRUE) { fOnlyPrimaries = choice; }


    /** Set physics processes
     ** @param eLossModel       Energy loss model
     ** @param useLorentzShift  If kTRUE, activate Lorentz shift
     ** @param useDiffusion     If kTRUE, activate diffusion
     ** @param useCrossTalk     If kTRUE, activate cross talk
     **
     ** Default is: all processes active; energy loss model is kELossUrban.
     **/
    void SetProcesses(CbmStsELoss eLossModel,
                      Bool_t useLorentzShift,
                      Bool_t useDiffusion,
                      Bool_t useCrossTalk) {
      fELossModel   = eLossModel;
      fLorentzShift = useLorentzShift;
      fDiffusion    = useDiffusion;
      fCrossTalk    = useCrossTalk;
    }


    /** @brief String output **/
    std::string ToString() const;


  private:

    Bool_t fEventMode = kFALSE;               ///< Event-by-event mode
    CbmStsELoss fELossModel = CbmStsELoss::kUrban; ///< Energy loss model
    Bool_t fLorentzShift = kTRUE;             ///< Apply Lorentz shift
    Bool_t fDiffusion = kTRUE;                ///< Apply diffusion
    Bool_t fCrossTalk = kTRUE;                ///< Apply cross-talk
    Bool_t fNoise = kTRUE;                    ///< Generate inter-event noise
    Bool_t fOnlyPrimaries = kFALSE;           ///< Process sonly primary tracks

    ClassDef(CbmStsParSim, 1);
};

#endif /* CBMSTSPARSIM_H */
