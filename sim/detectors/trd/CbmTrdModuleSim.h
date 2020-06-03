#ifndef CBMTRDMODULESIM_H
#define CBMTRDMODULESIM_H

#include "CbmTrdModuleAbstract.h"
#include <map>
#include "CbmTrdRawToDigiR.h"
#include "CbmTrdCheckUtil.h"

class TClonesArray;
class CbmTrdPoint;
class CbmTrdDigi;
class CbmMatch;
class CbmTrdDigitizer;
class CbmTrdRadiator;
/**
  * \brief Abstract class for module wise digitization and raw format producing
  **/
class CbmTrdModuleSim : public CbmTrdModuleAbstract
{
public:
  enum CbmTrdModuleSimDef{
    kTime           = 0,    ///< select Time based/Event by event simulations
    kNoise,                 ///< switch noise digits generation
    kWeightDistance         ///< use weighting distance for link generation
  };
  /**
   * \brief Default constructor.
   **/
  CbmTrdModuleSim();
  /**
   * \brief Constructor with placement
   **/
  CbmTrdModuleSim(Int_t mod, Int_t ly, Int_t rot);
  
  virtual ~CbmTrdModuleSim();
  
  virtual void     GetCounters(Int_t &nEl, Int_t &nLattice, Int_t &nOverThr) const {nEl=0; nLattice=0; nOverThr=0;} 
  virtual std::map<Int_t, std::pair<CbmTrdDigi*, CbmMatch*>>* 
                   GetDigiMap()         { return &fDigiMap;}
  virtual Int_t    GetEventId() const   { return fEventId; }
  virtual Int_t    GetInputId() const   { return fInputId; }
  virtual Int_t    GetPointId() const   { return fPointId; }
  /**
   * \brief Flush local digi buffer
   * \param time Time limit to dump digits. If 0 all (used also in EbyE mode) 
   * \return no of digits exported
   **/
  virtual Int_t  FlushBuffer(ULong64_t time=0) = 0;
  /**
   * \brief Steering routine for converting MC point to digits
   * \param[in] p MC point to be digitized 
   * \param[in] time Event time [ns]
   * \param[in] TR Request TR generation on top of dEdx  
   **/
  virtual Bool_t  MakeDigi(CbmTrdPoint *p, Double_t time, Bool_t TR=kFALSE) = 0;
  /**
   * \brief Steering routine for converting MC point to raw format
   **/
  virtual Bool_t  MakeRaw() = 0;

//  virtual void    SetAsicPar(CbmTrdParSetAsic *p=NULL)=0;
  void SetDigitizer(CbmTrdDigitizer* digitizer) { fDigitizer = digitizer; }
  virtual void    SetEventId(Int_t id)  { fEventId = id; }
  virtual void    SetInputId(Int_t id)  { fInputId = id; }
  virtual void    SetPointId(Int_t id)  { fPointId = id; }
  virtual void    SetRadiator(CbmTrdRadiator *radiator = NULL) = 0;
  virtual void    SetGamma(Double_t gamma = 0.) = 0;
  virtual void    SetPositionMC(Double_t pos[3])  { memcpy(fXYZ, pos, 3*sizeof(Double_t)); }
  virtual void    SetLinkId(Int_t input, Int_t event=-1, Int_t point=-1)  
                    { fInputId=input; fEventId=event; fPointId = point; }  
  virtual void    ResetCounters() {;}
  virtual void    SetMessageConverter(CbmTrdRawToDigiR *conv = NULL) = 0;
    virtual void    SetQA(CbmTrdCheckUtil *qa = NULL) = 0;
protected:
  // definitions of MC input
  Int_t           fPointId;     ///< MC point id being processed
  Int_t           fEventId;     ///< MC event id being processed
  Int_t           fInputId;     ///< MC input file number
  Double_t        fXYZ[3];      ///< MC position of the point in module coordinates
  CbmTrdDigitizer*    fDigitizer;   //! Pointer to digitizer
  
  // calibration objects
  CbmTrdRadiator* fRadiator;    ///< TR description for radiator

  std::map<Int_t, std::pair<CbmTrdDigi*, CbmMatch*>>              fDigiMap; ///< Temporary storage for complete digis for each CBM address.
  std::map<Int_t, std::vector<std::pair<CbmTrdDigi*, CbmMatch*>>> fBuffer;  ///< Local digits buffer as function of time for each TRD channel in the module

private:
  CbmTrdModuleSim(const CbmTrdModuleSim &ref);
  const CbmTrdModuleSim& operator=(const CbmTrdModuleSim &ref);



  ClassDef(CbmTrdModuleSim, 1)
};
#endif
