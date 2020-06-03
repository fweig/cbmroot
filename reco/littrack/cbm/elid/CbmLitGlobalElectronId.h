/**
 * \file CbmLitReconstructionQa.h
 * \author Semen Lebedev <s.lebedev@gsi.de>
 * \date 2011
 **/

#ifndef CBMLITGLOBALELECTRONID_H_
#define CBMLITGLOBALELECTRONID_H_

#include "TObject.h"

class TClonesArray;
class CbmGlobalTrack;

class CbmLitGlobalElectronId: public TObject
{
private:
   /**
    * \brief Constructor.
    */
	CbmLitGlobalElectronId();

   /**
	* \brief Initialize TClonesArrays.
	*/
	void Init();


public:
	/**
	 * Return Instance of CbmLitGlobalElectronId.
	 */
	static CbmLitGlobalElectronId& GetInstance() {
		static CbmLitGlobalElectronId fInstance;
		return fInstance;
	}

	/**
	 * \brief Destructor.
	 */
	virtual ~CbmLitGlobalElectronId();

   /**
    * \brief Identify electron in RICH detector.
    * \param[in] globalTrackIndex Index of global track.
    * \param[in] momentum Momentum of track.
    * \return true if track is identified as electron otherwise return false.
    */
	Bool_t IsRichElectron(
			Int_t globalTrackIndex,
			Double_t momentum);

   /**
    * \brief Identify electron in RICH detector.
    * \param[in] globalTrackIndex Index of global track.
    * \param[in] momentum Momentum of track.
    * \return true if track is identified as electron otherwise return false.
    */
	Bool_t IsTrdElectron(
			Int_t globalTrackindex,
			Double_t momentum);

   /**
    * \brief Identify electron in RICH detector.
    * \param[in] globalTrackIndex Index of global track.
    * \param[in] momentum Momentum of track.
    * \return true if track is identified as electron otherwise return false.
    */
	Bool_t IsTofElectron(
			Int_t globalTrackIndex,
			Double_t momentum);

   /**
    * \brief Identify electron in RICH detector.
    * \param[in] globalTrackIndex Index of global track.
    * \param[in] momentum Momentum of track.
    * \return true if track is identified as electron otherwise return false.
    */
/*
	Bool_t IsElectron(
	      Int_t globalTrackIndex,
	      Double_t momentum);
*/

	/**
	* \brief Return ANN value for electron Identification in the RICH detector.
	* \param[in] globalTrackIndex Index of global track.
	* \param[in] momentum Momentum of track.
	* \return RICH ANN value.
	*/
	Double_t GetRichAnn(
			Int_t globalTrackIndex,
			Double_t momentum);

   /**
	* \brief Return ANN value for electron Identification in the TRD detector.
	* \param[in] globalTrackIndex Index of global track.
	* \param[in] momentum Momentum of track.
	* \return TRD ANN value.
	*/
	Double_t GetTrdAnn(
			Int_t globalTrackindex,
			Double_t momentum);

	/**
	 * \brief Set cut on TRD ANN output value.
	 */
	void SetTrdAnnCut(Double_t par){fTrdAnnCut = par;}

	/**
	 * \brief Set to true if you want to use ANN method for the RICH detector.
	 */
	void SetRichUseAnn(Bool_t par){fRichUseAnn = par;}

	/**
	 * \brief Set cut on RICH ANN output value.
	 */
	void SetRichAnnCut(Double_t par){fRichAnnCut = par;}

	/**
	 * \brief Set RICH electron ID cuts when ANN is not used.
	 */
	void SetRichCuts(
			Double_t meanA,
			Double_t rmsA,
			Double_t meanB,
			Double_t rmsB,
			Double_t rmsCoeff,
			Double_t rtDistCut)
	{
		fRichMeanA = meanA;
		fRichMeanB = meanB;
		fRichRmsA = rmsA;
		fRichRmsB = rmsB;
		fRichRmsCoeff = rmsCoeff;
		fRichDistCut = rtDistCut;
	}

	/**
	 * \brief Return RICH ANN cut.
	 */
	Double_t GetRichAnnCut() {return fRichAnnCut;}

	/**
	 * \brief Return TRD ANN cut.
	 */
	Double_t GetTrdAnnCut() { return fTrdAnnCut;}

private:
    Double_t fRichAnnCut;
    Bool_t fRichUseAnn;
    Double_t fRichMeanA;
    Double_t fRichMeanB;
    Double_t fRichRmsA;
    Double_t fRichRmsB;
    Double_t fRichRmsCoeff;
    Double_t fRichDistCut;
    Double_t fTrdAnnCut;

    TClonesArray* fGlobalTracks;
    TClonesArray* fRichRings;
    TClonesArray* fTrdTracks;
    TClonesArray* fTofHits;

    CbmLitGlobalElectronId(const CbmLitGlobalElectronId&);
    CbmLitGlobalElectronId& operator=(const CbmLitGlobalElectronId&);
    
    ClassDef(CbmLitGlobalElectronId, 1);
};

#endif /* CBMLITGLOBALELECTRONID_H_ */
