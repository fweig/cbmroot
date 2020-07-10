#ifndef CBMTRDMODULERECT_H
#define CBMTRDMODULERECT_H

#include "CbmTrdModuleRec.h"
#include <list>
#include <map>
#include <vector>
#define NBINSCORRX 151  //! no of bins in the discretized correction LUT
#define NBINSCORRY 4    //! no of bins in the parametrization correction

class TGraphErrors;
class TF1;
/**
  * \brief Triangular pad module; Cluster finding and hit reconstruction algorithms
  **/
class CbmTrdModuleRecT : public CbmTrdModuleRec {
public:
  enum CbmTrdModuleRecTconfig {
    kVerbose = 0  // steer verbosity on/off
    ,
    kDraw  // steer graphic representation on/off
  };

  /** \brief Default constructor.*/
  CbmTrdModuleRecT();
  /** \brief Constructor with placement */
  CbmTrdModuleRecT(Int_t mod, Int_t ly = -1, Int_t rot = 0);
  virtual ~CbmTrdModuleRecT();


  /** \brief Add digi to local module **/
  virtual Bool_t AddDigi(const CbmTrdDigi* d, Int_t id);
  /** \brief Config task with the following settings
   * \param[in] v verbosity toggle
   * \param[in] d drawing toggle
   */
  virtual inline void Config(Bool_t v, Bool_t d);
  /** \brief Count RO channels (R or T) with data**/
  virtual Int_t GetOverThreshold() const;
  /** \brief Finalize hits (merge RC hits, etc)**/
  virtual Bool_t Finalize();
  /** \brief Finalize clusters **/
  virtual Int_t FindClusters();
  /** \brief Steering routine for building hits **/
  virtual Bool_t MakeHits();
  /** \brief Steering routine for converting cluster to hit **/
  virtual CbmTrdHit* MakeHit(Int_t cId,
                             const CbmTrdCluster* c,
                             std::vector<const CbmTrdDigi*>* digis);

protected:
private:
  CbmTrdModuleRecT(const CbmTrdModuleRecT& ref);
  const CbmTrdModuleRecT& operator=(const CbmTrdModuleRecT& ref);

  Bool_t CDRAW() const { return TESTBIT(fConfigMap, kDraw); }
  Bool_t CWRITE() const { return TESTBIT(fConfigMap, kVerbose); }
  Double_t GetXoffset(Int_t n0) const;
  Double_t GetYoffset(Int_t n0) const;
  /** \brief Load digis info into local data structures
   * \param[in] digis initial digis list shrinked for incomplete digis.
   * \param[in] vdgM list of merged digis
   * \param[in] vmask position of merged digis in the digis list
   * \param[in] t0 prompt time of cluster 
   * \param[out] cM relative position of maximum
   * \return no of signals loaded. if detected overflow negative number
   */
  Int_t LoadDigis(std::vector<const CbmTrdDigi*>* digis,
                  std::vector<CbmTrdDigi*>* vdgM,
                  std::vector<Bool_t>* vmask,
                  ULong64_t& t0,
                  Int_t& cM);
  /** \brief Merge R/T signals to digis if topological conditions in cluster are fulfilled
   * \param[in] digis initial digis list.
   * \param[out] vdgM list of merged digis
   * \param[out] vmask position of merged digis in the output digis list
   * \return If successful the digis are resized by removing the references to incomplete clusters
   */
  Bool_t MergeDigis(std::vector<const CbmTrdDigi*>* digis,
                    std::vector<CbmTrdDigi*>* vdgM,
                    std::vector<Bool_t>* vmask);

  UChar_t fConfigMap;  // task configuration settings
  ULong64_t fT0;       //! start time of event/time slice [clk]
  std::map<Int_t, std::list<CbmTrdCluster*>>
    fBuffer;                  //row-wise organized clusters
  std::vector<Double_t> vs;   //! working copy of signals from cluster
  std::vector<Double_t> vse;  //! working copy of signal errors from cluster
  std::vector<Char_t> vt;     //! working copy of signal relative timing
  std::vector<Double_t> vx;   //! working copy of signal relative positions
  std::vector<Double_t>
    vxe;  //! working copy of signal relative position errors

  static Float_t fgCorrXdx;  //! step of the discretized correction LUT
  static Float_t fgCorrXval[2][NBINSCORRX];  //! discretized correction LUT
  static Float_t fgCorrYval[NBINSCORRY][2];  //! discretized correction params
  static Double_t fgDT[3];                   //! FASP delay wrt signal
  static TGraphErrors* fgEdep;               //! data handler for cluster PRF
  static TF1* fgPRF;                         //! fitter for cluster PRF
  static TGraphErrors* fgT;                  //! data handler for cluster TRF

  ClassDef(
    CbmTrdModuleRecT,
    1)  // Triangular pad module; Cluster finding and hit reconstruction algorithms
};

void CbmTrdModuleRecT::Config(Bool_t v, Bool_t d) {
  if (v)
    SETBIT(fConfigMap, kVerbose);
  else
    CLRBIT(fConfigMap, kVerbose);
  printf("CbmTrdModuleRecT::Verbose[%c]\n", CWRITE() ? 'y' : 'n');
  if (d)
    SETBIT(fConfigMap, kDraw);
  else
    CLRBIT(fConfigMap, kDraw);
  printf("CbmTrdModuleRecT::Draw[%c]\n", CDRAW() ? 'y' : 'n');
}
#endif
