/* Copyright (C) 2018-2020 Horia Hulubei National Institute of Physics and Nuclear Engineering, Bucharest
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Alexandru Bercuci[committer] */

#ifndef CBMTRDMODULEREC2D_H
#define CBMTRDMODULEREC2D_H

#include "CbmTrdModuleRec.h"

#include <list>
#include <map>
#include <vector>
#define NBINSCORRX 50  //! no of bins in the discretized correction LUT
#define NBINSCORRY 4   //! no of bins in the parametrization correction
#define NANODE 9

using std::list;
using std::map;
using std::vector;
class TGraphErrors;
class CbmTrdDigiRec;
class TF1;
/** @class CbmTrdModuleRec2D
 ** @brief Cluster finding and hit reconstruction algorithms for the TRD(2D) module. 
 ** @author Alexandru Bercucic <abercuci@niham.nipne.ro>
 ** @since 01.02.2019
 ** @date 01.10.2021
 **
 ** Extend the TRD module reconstructor for the particular case of inner TRD-2D modules.
 ** The class is a collection of algorithms to :
 ** - identify time and spatially correlated digis and build clusters
 ** - identify the type of clusters and apply further merging/deconvolution
 ** - apply FEE (channel gain, baseline) and detector (energy gain, maps, etc) calibration
 ** - steer the calculation of hit 4D parameters (x, y, t, E)
 **/
class CbmTrdModuleRec2D : public CbmTrdModuleRec {
public:
  enum CbmTrdModuleRec2Dconfig
  {
    kVerbose = 0  // steer verbosity on/off
      ,
    kDraw  // steer graphic representation on/off
  };

  /** \brief Default constructor.*/
  CbmTrdModuleRec2D();
  /** \brief Constructor with placement */
  CbmTrdModuleRec2D(Int_t mod, Int_t ly = -1, Int_t rot = 0);
  virtual ~CbmTrdModuleRec2D();


  /** \brief Add digi to local module **/
  virtual Bool_t AddDigi(const CbmTrdDigi* d, Int_t id);
  /** \brief Config task with the following settings
   * \param[in] v verbosity toggle
   * \param[in] d drawing toggle
   */
  virtual inline void Config(Bool_t v, Bool_t d);
  virtual void DrawHit(CbmTrdHit*) const { ; }
  /** \brief Count RO channels (R or T) with data**/
  virtual Int_t GetOverThreshold() const;
  /** \brief Check hit quality (deconvolute pile-ups, etc)**/
  virtual Bool_t PreProcessHits();
  /** \brief Finalize hits (merge RC hits, etc)**/
  virtual Bool_t PostProcessHits();
  /** \brief Finalize clusters **/
  virtual Int_t FindClusters();
  /** \brief Steering routine for building hits **/
  virtual Bool_t MakeHits();
  /** \brief Steering routine for converting cluster to hit **/
  virtual CbmTrdHit* MakeHit(Int_t cId, const CbmTrdCluster* c, std::vector<const CbmTrdDigi*>* digis);
  /** \brief Load RAW digis into working array of RECO digis
   * \param[in] din list of RAW digis in increasing order of column no
   * \param[in] cid cluster index in the cluster array 
   * \return no of digis loaded
   */
  Int_t LoadDigis(vector<const CbmTrdDigi*>* din, Int_t cid);
  Int_t ProjectDigis(Int_t cid, Int_t cjd = -1);
  /** \brief Implement topologic cuts for hit merging
   * \return index of anode wire wrt to boundary or 0 if check fails 
   */
  Int_t CheckMerge(Int_t cid, Int_t cjd);
  /** \brief Algorithm for hit merging
   * \param[in] h hit to be modified by addition of hp.
   * \param[in] a0 anode hypothesis around boundary (see CheckMerge function).
   * \return TRUE if succesful.
   */
  Bool_t MergeHits(CbmTrdHit* h, Int_t a0);
  Bool_t BuildHit(CbmTrdHit* h);
  UShort_t GetHitMap() const { return vyM; }
  /** \brief x position correction based on LUT
   * \param[in] dx offset computed on charge sharing expressed in [pw]
   * \param[in] typ hit type central pad [0] or edge [1]
   * \param[in] cls correction class x wrt center [0] row-cross [1] row-cross biassed x [2]
   * \return correction expresed in [cm]
   */
  Double_t GetXcorr(Double_t dx, Int_t typ, Int_t cls = 0) const;
  /** \brief y position correction based on LUT
   * \param[in] dy offset computed on charge sharing expressed in [ph]
   * \param[in] cls correction class 
   * \return correction expresed in [cm]
   */
  Double_t GetYcorr(Double_t dy, Int_t cls = 0) const;
  /** \brief Shift graph representation to [-0.5, 0.5]
   * \param[in] dx offset wrt center pad [pw]
   */
  void RecenterXoffset(Double_t& dx);
  /** \brief Shift graph representation to [-0.5, 0.5]
   * \param[in] dy offset wrt center pad [ph]
   */
  void RecenterYoffset(Double_t& dy);
  /** \brief Hit classification wrt center pad
   * \return hit type : center hit [0]; side hit [1]
   */
  Int_t GetHitClass() const;
  /** \brief Hit classification wrt signal bias
   * \return hit type : center hit [0]; side hit [1]
   */
  Int_t GetHitRcClass(Int_t a0) const;

protected:
private:
  CbmTrdModuleRec2D(const CbmTrdModuleRec2D& ref);
  const CbmTrdModuleRec2D& operator=(const CbmTrdModuleRec2D& ref);

  Bool_t CDRAW() const { return TESTBIT(fConfigMap, kDraw); }
  Bool_t CWRITE() const { return TESTBIT(fConfigMap, kVerbose); }
  /** \brief Implement cuts for hit convolution definition
   * \param[in] h hit to be analysed.
   * \return TRUE if double cluster
   */
  Bool_t CheckConvolution(CbmTrdHit* h) const;
  /** \brief Algorithm for cluster spliting
   * \param[in] h hit to be analysed.
   * \return TRUE if succesful. The extra cluster is added to the end of the hits array 
   */
  Bool_t Deconvolute(CbmTrdHit* h);
  Double_t GetXoffset(Int_t n0 = 0) const;
  Double_t GetYoffset(Int_t n0 = 0) const;
  /** \brief Load digis info into local data structures
   * \param[in] digis initial digis list shrinked for incomplete digis.
   * \param[in] vdgM list of merged digis
   * \param[in] vmask position of merged digis in the digis list
   * \param[in] t0 prompt time of cluster 
   * \param[out] cM relative position of maximum
   * \return no of signals loaded. if detected overflow negative number
   */
  Int_t LoadDigis(std::vector<const CbmTrdDigi*>* digis, std::vector<CbmTrdDigi*>* vdgM, std::vector<Bool_t>* vmask,
                  ULong64_t& t0, Int_t& cM);
  Int_t LoadDigisRC(vector<const CbmTrdDigi*>* digis, const Int_t r0, const Int_t a0,
                    /*vector<CbmTrdDigi*> *vdgM, */ ULong64_t& t0, Int_t& cM);

  /** \brief Merge R/T signals to digis if topological conditions in cluster are fulfilled
   * \param[in] digis initial digis list.
   * \param[out] vdgM list of merged digis
   * \param[out] vmask position of merged digis in the output digis list
   * \return If successful the digis are resized by removing the references to incomplete clusters
   */
  Bool_t MergeDigis(std::vector<const CbmTrdDigi*>* digis, std::vector<CbmTrdDigi*>* vdgM, std::vector<Bool_t>* vmask);

  Bool_t HasLeftSgn() const { return TESTBIT(vyM, 3); }
  Bool_t HasOvf() const { return TESTBIT(vyM, 12); }
  Bool_t IsBiasX() const { return TESTBIT(vyM, 4); }
  Bool_t IsBiasXleft() const { return TESTBIT(vyM, 5); }
  Bool_t IsBiasXmid() const { return TESTBIT(vyM, 6); }
  Bool_t IsBiasXright() const { return TESTBIT(vyM, 7); }
  Bool_t IsBiasY() const { return TESTBIT(vyM, 8); }
  Bool_t IsBiasYleft() const { return TESTBIT(vyM, 9); }
  Bool_t IsLeftHit() const { return TESTBIT(vyM, 2); }
  Bool_t IsMaxTilt() const { return TESTBIT(vyM, 0); }
  Bool_t IsOpenLeft() const { return (viM % 2 && !IsMaxTilt()) || (!(viM % 2) && IsMaxTilt()); }
  inline Bool_t IsOpenRight() const;
  Bool_t IsSymmHit() const { return TESTBIT(vyM, 1); }
  void SetBiasX(Bool_t set = 1) { set ? SETBIT(vyM, 4) : CLRBIT(vyM, 4); }
  void SetBiasXleft(Bool_t set = 1) { set ? SETBIT(vyM, 5) : CLRBIT(vyM, 5); }
  void SetBiasXmid(Bool_t set = 1) { set ? SETBIT(vyM, 6) : CLRBIT(vyM, 6); }
  void SetBiasXright(Bool_t set = 1) { set ? SETBIT(vyM, 7) : CLRBIT(vyM, 7); }
  void SetBiasY(Bool_t set = 1) { set ? SETBIT(vyM, 8) : CLRBIT(vyM, 8); }
  void SetBiasYleft(Bool_t set = 1) { set ? SETBIT(vyM, 9) : CLRBIT(vyM, 9); }
  void SetBiasYmid(Bool_t set = 1) { set ? SETBIT(vyM, 10) : CLRBIT(vyM, 10); }
  void SetBiasYright(Bool_t set = 1) { set ? SETBIT(vyM, 11) : CLRBIT(vyM, 11); }
  void SetLeftSgn(Bool_t set = 1) { set ? SETBIT(vyM, 3) : CLRBIT(vyM, 3); }
  void SetLeftHit(Bool_t set = 1) { set ? SETBIT(vyM, 2) : CLRBIT(vyM, 2); }
  void SetSymmHit(Bool_t set = 1) { set ? SETBIT(vyM, 1) : CLRBIT(vyM, 1); }
  void SetMaxTilt(Bool_t set = 1) { set ? SETBIT(vyM, 0) : CLRBIT(vyM, 0); }
  void SetOvf(Bool_t set = 1) { set ? SETBIT(vyM, 12) : CLRBIT(vyM, 12); }

  UChar_t fConfigMap;                                  // task configuration settings
  ULong64_t fT0;                                       //! start time of event/time slice [clk]
  std::map<Int_t, std::list<CbmTrdCluster*>> fBuffer;  //row-wise organized clusters
  std::map<Int_t, vector<CbmTrdDigiRec*>> fDigis;      //!cluster-wise organized calibrated digi
  // working representation of a hit on which the reconstruction is performed
  ULong64_t vt0;              //! start time of current hit [clk]
  UChar_t vcM;                //! maximum col
  UChar_t vrM;                //! maximum row
  UChar_t viM;                //! index of maximum signal in the projection
  UShort_t vyM;               //! bit map for cluster topology classification
  std::vector<Double_t> vs;   //! working copy of signals from cluster
  std::vector<Double_t> vse;  //! working copy of signal errors from cluster
  std::vector<Char_t> vt;     //! working copy of signal relative timing
  std::vector<Double_t> vx;   //! working copy of signal relative positions
  std::vector<Double_t> vxe;  //! working copy of signal relative position errors

  static Float_t fgCorrXdx;                         //! step of the discretized correction LUT
  static Float_t fgCorrXval[3][NBINSCORRX];         //! discretized correction LUT
  static Float_t fgCorrYval[NBINSCORRY][2];         //! discretized correction params
  static Float_t fgCorrRcXval[2][NBINSCORRX];       //! discretized correction LUT
  static Float_t fgCorrRcXbiasXval[3][NBINSCORRX];  //! discretized correction LUT
  static Double_t fgDT[3];                          //! FASP delay wrt signal
  static TGraphErrors* fgEdep;                      //! data handler for cluster PRF
  static TF1* fgPRF;                                //! fitter for cluster PRF
  static TGraphErrors* fgT;                         //! data handler for cluster TRF

  ClassDef(CbmTrdModuleRec2D,
           2)  // Triangular pad module; Cluster finding and hit reconstruction algorithms
};

void CbmTrdModuleRec2D::Config(Bool_t v, Bool_t d)
{
  if (v) SETBIT(fConfigMap, kVerbose);
  else
    CLRBIT(fConfigMap, kVerbose);
  printf("CbmTrdModuleRec2D::Verbose[%c]\n", CWRITE() ? 'y' : 'n');
  if (d) SETBIT(fConfigMap, kDraw);
  else
    CLRBIT(fConfigMap, kDraw);
  printf("CbmTrdModuleRec2D::Draw[%c]\n", CDRAW() ? 'y' : 'n');
}

Bool_t CbmTrdModuleRec2D::IsOpenRight() const
{
  Int_t nR = vs.size() - 1 - viM;
  return (nR % 2 && IsMaxTilt()) || (!(nR % 2) && !IsMaxTilt());
}
#endif
