#ifndef PAIRANALYSISVARMANAGER_H
#define PAIRANALYSISVARMANAGER_H
/* Copyright(c) 1998-2009, ALICE Experiment at CERN, All rights reserved. *
 * based on the ALICE-dielectron package                                  */

//#############################################################
//#                                                           #
//#         Class for management of available variables       #
//#                                                           #
//#  Authors:                                                 #
//#   Julian    Book,     Uni Ffm / Julian.Book@cern.ch       #
//#                                                           #
//#############################################################

#include <TNamed.h>
#include <TDatabasePDG.h>
#include <TPDGCode.h>

#include <TBits.h>
#include <TRandom3.h>
#include <TFormula.h>
#include <TVector2.h>
#include <TVector3.h>
#include <TMatrixFSym.h>

#include <FairRootManager.h>
#include <CbmRichElectronIdAnn.h>
#include <CbmCluster.h>
#include <CbmPixelHit.h>
#include <CbmTofHit.h>
#include <CbmRichHit.h>
#include <CbmTrdHit.h>
#include <CbmStsHit.h>
#include <CbmMvdHit.h>
#include <CbmMuchPixelHit.h>
#include <CbmTrdCluster.h>

#include <FairTrackParam.h>
#include <FairMCPoint.h>

#include <FairMCEventHeader.h>
#include <CbmVertex.h>
#include <CbmKFVertex.h>
#include <CbmGlobalTrack.h>
#include <CbmStsTrack.h>
#include <CbmMuchTrack.h>
#include <CbmTrdTrack.h>
#include <CbmRichRing.h>
#include <CbmMCTrack.h>
#include <CbmDefs.h>
#include <CbmTrackMatchNew.h>


//#include "L1Field.h"
//#include "CbmL1PFFitter.h"
//#include <CbmStsKFTrackFitter.h>

#include "PairAnalysisEvent.h"
#include "PairAnalysisPair.h"
#include "PairAnalysisPairLV.h"
#include "PairAnalysisTrack.h"
#include "PairAnalysisMC.h"
#include "PairAnalysisHelper.h"

#include "assert.h"

//________________________________________________________________
class PairAnalysisVarManager : public TNamed {

public:

  enum ValueTypes {
// Constant information
    kMEL=1,                  // pdg mass of electrons
    kMMU,                    // pdg mass of muons
    kMPI,                    // pdg mass of pions
    kMKA,                    // pdg mass of kaons
    kMPR,                    // pdg mass of protons
    kMK0,                    // pdg mass of neutral kaons
    kMLA,                    // pdg mass of lambdas
    kMPair,                  // pdg mass of pair
    kEbeam,                  // beam energy
    kConstMax,
// Hit specific variables
    kPosX=kConstMax,         // X position [cm]
    kPosY,                   // Y position [cm]
    kPosZ,                   // Z position [cm]
    kLinksMC,                // number of matched MC links
    kTrdLayer,               // plane/layer id
    kTrdPads,                // number of pads contributing to cluster/hit
    kTrdCols,                // number of pads columns contributing to cluster/hit
    kTrdRows,                // number of pads rows contributing to cluster/hit
    kEloss,                  // TRD energy loss dEdx+TR
//    kElossdEdx,              // TRD energy loss dEdx only
//    kElossTR,                // TRD energy loss TR only
    kNPhotons,               // RICH number of photons in this hit
    kPmtId,                  // RICH photomultiplier number
    kBeta,                   // TOF beta
    kTofPidDeltaBetaEL,      // delta of TOF beta to expected beta for electrons
    kTofPidDeltaBetaMU,      // delta of TOF beta to expected beta for muons
    kTofPidDeltaBetaPI,      // delta of TOF beta to expected beta for pions
    kTofPidDeltaBetaKA,      // delta of TOF beta to expected beta for kaons
    kTofPidDeltaBetaPR,      // delta of TOF beta to expected beta for protons
    kMassSq,                 // TOF mass squared
    kHitMax,
// Particle specific variables
    kPx = kHitMax,           // px
    kPy,                     // py
    kPz,                     // pz
    kPt,                     // transverse momentum
    kPtSq,                   // transverse momentum squared
    kP,                      // momentum
    kXv,                     // vertex position in x
    kYv,                     // vertex position in y
    kZv,                     // vertex position in z
    kOneOverPt,              // 1/pt
    kPhi,                    // phi angle
    kTheta,                  // theta polar angle
    kEta,                    // pseudo-rapidity
    kY,                      // rapidity
    kYlab,                   // rapidity lab
    kE,                      // energy
    kM,                      // mass
    kCharge,                 // charge
    kMt,                     // transverse mass sqrt(m^2+pt^2)
    kChi2NDFtoVtx,           // chi2/ndf impact parameter STS(+MVD) track to primary vertex in (sigmas)
    kImpactParXY,            // Impact parameter in XY plane
    kImpactParZ,             // Impact parameter in Z
    kInclAngle,              // inclination angle
    kParticleMax,
// Track specific variables
    // global track
    kTrackLength=kParticleMax, // Track length (cm)
    kTrackChi2NDF,           // chi2/ndf
    kPin,                    // first point momentum (GeV/c)
    kPtin,                   // first point transverse momentum (GeV/c)
    kPout,                   // last point momentum (GeV/c)
    kPtout,                  // last point transverse momentum (GeV/c)
    // trd track information
    kTrdSignal,              // TRD energy loss dEdx+TR (keV)
    kTrdPidWkn,              // PID value Wkn method
    kTrdPidANN,              // PID value Artificial Neural Network (ANN-method)
    kTrdPidLikeEL,           // PID value Likelihood method: electron
    kTrdPidLikePI,           // PID value Likelihood method: pion
    kTrdPidLikeKA,           // PID value Likelihood method: kaon
    kTrdPidLikePR,           // PID value Likelihood method: proton
    kTrdPidLikeMU,           // PID value Likelihood method: muon
    kTrdHits,                // number of TRD hits
    kTrdChi2NDF,             // chi2/ndf TRD
    kTrdPin,                 // first point TRD momentum (GeV/c)
    kTrdPtin,                // first point TRD transverse momentum (GeV/c)
    kTrdPhiin,               // first point TRD azimuthal angle (rad)
    kTrdThetain,             // first point TRD polar angle (rad)
    kTrdPout,                // last point TRD momentum (GeV/c)
    kTrdPtout,               // last point TRD transverse momentum (GeV/c)
    kTrdThetaCorr,           // correction factor for theta track angle
    kTrdPhiCorr,             // correction factor for phi track angle
    //    kTrdTrackLength,         // track length in cm of the trd tracklet
    // sts track information
    kMvdhasEntr,             // weather track enters first MVD station
    kMvdHits,                // number of MVD hits
    kMvdFirstHitPosZ,        // position of the first hit in the MVD (cm)
    kMvdFirstExtX,           // x-position of the extrapolated track at the first MVD station (cm)
    kMvdFirstExtY,           // y-position of the extrapolated track at the first MVD station (cm)
   //    kImpactParZ,             // Impact parameter of track at target z, in units of its error  
    kStsHits,                // number of STS hits
    kStsChi2NDF,             // chi2/ndf STS
    kStsPin,                 // first point STS momentum (GeV/c)
    kStsPtin,                // first point STS transverse momentum (GeV/c)
    kStsPout,                // last point STS momentum (GeV/c)
    kStsPtout,               // last point STS transverse momentum (GeV/c)
    kStsXv,                  // STS point: x-coordinate
    kStsYv,                  // STS point: y-coordinate
    kStsZv,                  // STS point: z-coordinate
    kStsFirstHitPosZ,        // position of the first hit in the STS (cm)
    // rich ring information
    kRichhasProj,            // weather rich ring has a projection onto the pmt plane
    kRichPidANN,             // PID value Artificial Neural Network (ANN-method)
    kRichHitsOnRing,         // number of RICH hits on the ring
    kRichHits,               // number of RICH hits (ANN input)
    kRichChi2NDF,            // chi2/ndf ring fit (ANN input)
    kRichRadius,             // ring radius
    kRichAxisA,              // major semi-axis (ANN input)
    kRichAxisB,              // minor semi-axis (ANN input)
    kRichCenterX,            // ring center in x
    kRichCenterY,            // ring center in y
    kRichDistance,           // distance between ring center and track (ANN input)
    kRichRadialPos,          // radial psoition = sqrt(x**2+abs(y-110)**2), (ANN input)
    kRichRadialAngle,        // radial angle (0||1||2)*pi +- atan( abs((+-100-y)/-x) ), (ANN input)
    kRichPhi,                // phi rotation angle of ellipse (ANN input)
    // tof track information
    kTofHits,                // number of TOF hits
    // much track information
    kMuchHits,               // number of MUCH hits
    kMuchHitsPixel,          // number of MUCH pixel hits
    kMuchChi2NDF,            // chi2/ndf MUCH
    // technical variables
    kRndmTrack,              // randomly created number (used to apply special selection cuts)
    kTrackMax,

// Pair specific variables
    kChi2NDF = kTrackMax,    // Chi^2/NDF
    kDecayLength,            // decay length p*t (cm)
    kR,                      // xy-distance to origin (cm)
    kOpeningAngle,           // opening angle
    kCosPointingAngle,       // cosine of the pointing angle
    kArmAlpha,               // Armenteros-Podolanski alpha
    kArmPt,                  // Armenteros-Podolanski pt
    // helicity picture: Z-axis is considered the direction of the mother's 3-momentum vector
    kThetaHE,                // theta in mother's rest frame in the helicity picture 
    kPhiHE,                  // phi in mother's rest frame in the helicity picture
    kThetaSqHE,              // squared value of kThetaHE
    kCos2PhiHE,              // Cosine of 2*phi in mother's rest frame in the helicity picture
    kCosTilPhiHE,            // Shifted phi depending on kThetaHE
    // Collins-Soper picture: Z-axis is considered the direction of the vectorial difference between 
    // the 3-mom vectors of target and projectile beams
    kThetaCS,                // theta in mother's rest frame in Collins-Soper picture
    kPhiCS,                  // phi in mother's rest frame in Collins-Soper picture
    kThetaSqCS,              // squared value of kThetaCS
    kCos2PhiCS,              // Cosine of 2*phi in mother's rest frame in the Collins-Soper picture
    kCosTilPhiCS,            // Shifted phi depending on kThetaCS
    kPsiPair,                // phi in mother's rest frame in Collins-Soper picture
    kPhivPair,               // angle between ee plane and the magnetic field (can be useful for conversion rejection)
	
    kLegDist,                // distance of the legs
    kLegDistXY,              // distance of the legs in XY
    kDeltaEta,               // Absolute value of Delta Eta for the legs
    kDeltaPhi,               // Absolute value of Delta Phi for the legs
    kLegsP,                  // sqrt of p_leg1*p_leg2
    kMerr,                   // error of mass calculation
    kDCA,                    // distance of closest approach TODO: not implemented yet
    kPairType,               // type of the pair, like like sign ++ unlikesign ...
    kMomAsymDau1,            // momentum fraction of daughter1
    kMomAsymDau2,            // momentum fraction of daughter2
    kPairEff,                // pair efficiency
    kOneOverPairEff,         // 1 / pair efficiency (correction factor)
    kOneOverPairEffSq,        // 1 / pair efficiency squared (correction factor)
    kRndmPair,               // radomly created number (used to apply special signal reduction cuts)
    kPairs,                  // number of Ev1PM pair candidates after all cuts
    kPairMax,                 //

  // Event specific variables
    kXvPrim=kPairMax,        /// prim vertex [cm]
    kYvPrim,                 /// prim vertex [cm]
    kZvPrim,                 /// prim vertex [cm]
    kVtxChi,                 /// chi2
    kVtxNDF,                 /// nof degrees of freedom 
    kXRes,                   // primary vertex x-resolution
    kYRes,                   // primary vertex y-resolution
    kZRes,                   // primary vertex z-resolution
    kMaxPt,                  // track with maximum pt

    kRndmRej,                // random rejection probability by the pair pre filter
    kNTrk,                   // number of tracks
    kTracks,                 // track after all cuts
    kNVtxContrib,            /// number of primary vertex contibutors

    kCentrality,             // event centrality fraction
    kNevents,                // event counter
    kRunNumber,              // run number
    kYbeam,                  // beam rapdity
    kMixingBin,              // event mixing pool number
    kTotalTRDHits,           // size of trd hit array
    kNMaxValues,              //

// MC information
    // Hit specific variables
    kPosXMC=kNMaxValues,       // X position [cm]
    kPosYMC,                   // Y position [cm]
    kPosZMC,                   // Z position [cm]
    kElossMC,                  // energy loss dEdx+TR
    kHitMaxMC,
    // Particle specific MC variables
    kPxMC=kHitMaxMC,           // px
    kPyMC,                     // py
    kPzMC,                     // pz
    kPtMC,                     // transverse momentum
    kPtSqMC,                   // transverse momentum squared
    kPMC,                      // momentum
    kXvMC,                     // vertex position in x
    kYvMC,                     // vertex position in y
    kZvMC,                     // vertex position in z
    kPhivMC,                   // vertex position in phi
    kThetavMC,                 // vertex position in theta
    kOneOverPtMC,              // 1/pt
    kPhiMC,                    // phi angle
    kThetaMC,                  // theta angle
    kEtaMC,                    // pseudo-rapidity
    kYMC,                      // rapidity
    kYlabMC,                   // rapidity lab
    kBetaGammaMC,              // beta gamma
    kEMC,                      // energy
    kEMotherMC,                // energy of the mother
    kMMC,                      // mass
    kChargeMC,                 // charge
    kPdgCode,                  // PDG code
    kPdgCodeMother,            // PDG code of the mother
    kPdgCodeGrandMother,       // PDG code of the grand mother
    kGeantId,                  // geant process id (see TMCProcess)
    kWeight,                   // weight NxBR
    kParticleMaxMC,

    // Track specific MC variables
    kTrdHitsMC=kParticleMaxMC, // number of TRD hits
    kMvdHitsMC,                // number of MVD hits
    kStsHitsMC,                // number of STS hits
    kTofHitsMC,                // number of TOF hits
    kMuchHitsMC,               // number of MUCH hits
    kRichHitsMC,               // number of RICH hits
    kTrdMCTracks,              // number of TRD MC Points in reconstructed track
    kRichMCPoints,              // number of TRD MC Points in reconstructed track
    kTrdTrueHits,              // number of true TRD hits in reconstructed track
    kTrdDistHits,              // number of distorted TRD hits in reconstructed track
    kTrdFakeHits,              // number of fake TRD hits in reconstructed track
    kTrdDistortion,            // level of distortion of reconstructed track [0,1]
    kStsTrueHits,              // number of true STS hits in reconstructed track
    kStsDistHits,              // number of distorted STS hits in reconstructed track
    kStsFakeHits,              // number of fake STS hits in reconstructed track
    kTrdisMC,                  // status bit for matching btw. glbl. and local MC track
    kMvdisMC,                  // status bit for matching btw. glbl. and local MC track
    kStsisMC,                  // status bit for matching btw. glbl. and local MC track
    kMuchisMC,                 // status bit for matching btw. glbl. and local MC track
    kRichisMC,                 // status bit for matching btw. glbl. and local MC track
    kTofisMC,                  // status bit for matching btw. glbl. and local MC track
    kTrackMaxMC,

    // Pair specific MC variables
    kOpeningAngleMC=kTrackMaxMC,// opening angle
    kCosPointingAngleMC,        // cosine of the pointing angle
    //    kPhivPairMC,                // angle between d1d2 plane and the magnetic field
    kPairMaxMC,

    // Event specific MCvariables
    kNTrkMC=kPairMaxMC,      // number of MC tracks
    kXvPrimMC,               /// MC vertex [cm]
    kYvPrimMC,               /// MC vertex [cm]
    kZvPrimMC,               /// MC vertex [cm]
    kStsMatches,             // number of matched STS tracks
    kTrdMatches,             // number of matched TRD tracks
    kVageMatches,            // number of MC tracks (STS) matched to multiple reconstr. track
    kTotalTRDHitsMC,         // size of trd MC point array
    kImpactParam,            // impact parameter from MC header
    kNPrimMC,                // primary particles from MC header
    kNMaxValuesMC

  };


  PairAnalysisVarManager();
  PairAnalysisVarManager(const char* name, const char* title);
  virtual ~PairAnalysisVarManager();

  static void InitFormulas();
  static void InitFitter();

  static void Fill(             const TObject* particle,                    Double_t * const values);
  static void FillVarMCParticle(const CbmMCTrack *p1, const CbmMCTrack *p2, Double_t * const values);
  static void FillSum(          const TObject* particle,                    Double_t * const values);

  static void CalculateHitTypes(const PairAnalysisTrack *track, ECbmModuleId idet,
				Int_t *trueH, Int_t *distH, Int_t *fakeH);

  // Setter
  static void SetFillMap(        TBits   *map)                   { fgFillMap=map;     }
  static void SetEvent(          PairAnalysisEvent * const ev);
  static void SetEventData(const Double_t data[PairAnalysisVarManager::kNMaxValuesMC]);
  static void SetValue(          ValueTypes   var, Double_t val) { fgData[var]  =val; }

  // Getter
  static PairAnalysisEvent* GetCurrentEvent()  { return fgEvent;       }
  static const CbmKFVertex* GetKFVertex()       { return fgKFVertex;    }
  static const char* GetValueName(Int_t i)      { return (i>=0&&i<kNMaxValuesMC)?fgkParticleNames[i][0]:""; }
  static const char* GetValueLabel(Int_t i)     { return (i>=0&&i<kNMaxValuesMC)?fgkParticleNames[i][1]:""; }
  static const char* GetValueUnit(Int_t i)      { return (i>=0&&i<kNMaxValuesMC)?fgkParticleNames[i][2]:""; }
  static Double_t* GetData()                    { return fgData;        }
  static Double_t GetValue(ValueTypes val)      { return fgData[val];   }
  static UInt_t GetValueType(const char* valname);
  static UInt_t GetValueTypeMC(UInt_t var);

  static UInt_t* GetArray(ValueTypes var0,
			  ValueTypes var1=kNMaxValuesMC, ValueTypes var2=kNMaxValuesMC, ValueTypes var3=kNMaxValuesMC,
			  ValueTypes var4=kNMaxValuesMC, ValueTypes var5=kNMaxValuesMC, ValueTypes var6=kNMaxValuesMC,
			  ValueTypes var7=kNMaxValuesMC, ValueTypes var8=kNMaxValuesMC, ValueTypes var9=kNMaxValuesMC);

  // data member
  static TFormula *fgFormula[kNMaxValuesMC];        // variable formulas

private:

  // data member
  static Double_t fgData[kNMaxValuesMC];                   //! data
  static const char* fgkParticleNames[kNMaxValuesMC][3];   // variable names
  //static const char* fgkParticleNamesMC[kNMaxValuesMC]; // MC variable names
  static PairAnalysisEvent *fgEvent;                      // current event pointer
  static CbmKFVertex       *fgKFVertex;                   // kf vertex                   @TODO: OBSOLETE/UNUSED?
  static CbmVertex         *fgVertexMC;                   // MC vertex
  //  static CbmStsKFTrackFitter*fgKFFitter;                   // kf fitter
  //  static CbmL1PFFitter      *fgL1Fitter;                   // L1 fitter
  static TBits              *fgFillMap;                    // map for filling variables
  static Int_t               fgCurrentRun;                 // current run number

  // fill functions
  static Bool_t Req(ValueTypes var) { return (fgFillMap ? fgFillMap->TestBitNumber(var) : kTRUE); }

  static void FillVarConstants(Double_t * const values);
  static void FillVarPairAnalysisEvent( const PairAnalysisEvent *event,  Double_t * const values);
  static void FillVarVertex(            const CbmVertex *vertex,         Double_t * const values);
  static void FillVarPairAnalysisTrack( const PairAnalysisTrack *track,  Double_t * const values);
  static void FillVarGlobalTrack(       const CbmGlobalTrack *track,     Double_t * const values);
  static void FillVarStsTrack(          const CbmStsTrack *track,        Double_t * const values);
  static void FillVarMuchTrack(         const CbmMuchTrack *track,       Double_t * const values);
  static void FillVarTrdTrack(          const CbmTrdTrack *track,        Double_t * const values);
  static void FillVarRichRing(          const CbmRichRing *track,        Double_t * const values);
  static void FillVarMCTrack(           const CbmMCTrack *particle,      Double_t * const values);
  static void FillVarPairAnalysisPair(  const PairAnalysisPair *pair,    Double_t * const values);
  static void FillVarMvdHit(            const CbmMvdHit *hit,            Double_t * const values);
  static void FillVarStsHit(            const CbmStsHit *hit,            Double_t * const values);
  static void FillVarMuchHit(           const CbmMuchPixelHit *hit,      Double_t * const values);
  static void FillVarTrdHit(            const CbmTrdHit *hit,            Double_t * const values);
  static void FillVarRichHit(           const CbmRichHit *hit,           Double_t * const values);
  static void FillVarTofHit(            const CbmTofHit *hit,            Double_t * const values);
  static void FillVarPixelHit(          const CbmPixelHit *hit,          Double_t * const values);
  static void FillVarTrdCluster(        const CbmTrdCluster *cluster,    Double_t * const values);
  static void FillVarMCPoint(           const FairMCPoint *hit,          Double_t * const values);
  static void FillSumVarMCPoint(        const FairMCPoint *hit,          Double_t * const values);
  static void FillVarMCHeader(          const FairMCEventHeader *header, Double_t * const values);

  //  static Double_t GetChiToVertex(       const CbmStsTrack *track, CbmVertex *vertex);

  //  static void FillVarKFParticle(const AliKFParticle *pair,   Double_t * const values);

  // setter
  static void ResetArrayData(Int_t to,   Double_t * const values);
  static void ResetArrayDataMC(Int_t to, Double_t * const values);

  PairAnalysisVarManager(const PairAnalysisVarManager &c);
  PairAnalysisVarManager &operator=(const PairAnalysisVarManager &c);

  ClassDef(PairAnalysisVarManager,1); // Variables management for event, pair, track, hit infos (static)
};


//Inline functions
inline void PairAnalysisVarManager::Fill(const TObject* object, Double_t * const values)
{
  //
  // Main function to fill all available variables according to the type
  //

  //Protect
  if (!object) return;

  if      (object->IsA() == PairAnalysisEvent::Class()) FillVarPairAnalysisEvent(static_cast<const PairAnalysisEvent*>(object), values);
  else if (object->IsA() == CbmVertex::Class())       FillVarVertex(         static_cast<const CbmVertex*>(object),      values);
  else if (object->IsA() == PairAnalysisTrack::Class()) FillVarPairAnalysisTrack(static_cast<const PairAnalysisTrack*>(object),values);
  else if (object->IsA() == CbmGlobalTrack::Class())  FillVarGlobalTrack(    static_cast<const CbmGlobalTrack*>(object), values);
  else if (object->IsA() == CbmStsTrack::Class())     FillVarStsTrack(       static_cast<const CbmStsTrack*>(object),    values);
  else if (object->IsA() == CbmMuchTrack::Class())    FillVarMuchTrack(      static_cast<const CbmMuchTrack*>(object),   values);
  else if (object->IsA() == CbmTrdTrack::Class())     FillVarTrdTrack(       static_cast<const CbmTrdTrack*>(object),    values);
  else if (object->IsA() == CbmRichRing::Class())     FillVarRichRing(       static_cast<const CbmRichRing*>(object),    values);
  else if (object->IsA() == CbmMCTrack::Class())      FillVarMCTrack(        static_cast<const CbmMCTrack*>(object),     values);
  else if (object->InheritsFrom(PairAnalysisPair::Class()))  FillVarPairAnalysisPair( static_cast<const PairAnalysisPair*>(object), values);
  else if (object->IsA() == CbmMvdHit::Class())       FillVarMvdHit(         static_cast<const CbmMvdHit*>(object),      values);
  else if (object->IsA() == CbmStsHit::Class())       FillVarStsHit(         static_cast<const CbmStsHit*>(object),      values);
  else if (object->IsA() == CbmMuchPixelHit::Class()) FillVarMuchHit(        static_cast<const CbmMuchPixelHit*>(object),values);
  else if (object->IsA() == CbmTrdHit::Class())       FillVarTrdHit(         static_cast<const CbmTrdHit*>(object),      values);
  else if (object->IsA() == CbmRichHit::Class())      FillVarRichHit(        static_cast<const CbmRichHit*>(object),     values);
  else if (object->IsA() == CbmTofHit::Class())       FillVarTofHit(         static_cast<const CbmTofHit*>(object),      values);
  else if (object->InheritsFrom(FairMCPoint::Class()))     FillVarMCPoint(   static_cast<const FairMCPoint*>(object),    values);
  else printf("PairAnalysisVarManager::Fill: Type %s is not supported by PairAnalysisVarManager! \n", object->ClassName());
}


inline void PairAnalysisVarManager::FillSum(const TObject* object, Double_t * const values)
{
  //
  // Main function to incremenebt available variables according to the type
  //

  //Protect
  if (!object) return;
  else if (object->InheritsFrom(FairMCPoint::Class()))     FillSumVarMCPoint(   static_cast<const FairMCPoint*>(object),    values);
  else printf("PairAnalysisVarManager::FillSum: Type %s is not supported by PairAnalysisVarManager! \n", object->ClassName());

}

inline void PairAnalysisVarManager::ResetArrayData(Int_t to, Double_t * const values)
{
  // Protect
  if (to >= kNMaxValues) return;
  // Reset
  for (Int_t i=kConstMax; i<to; ++i) {
    values[i] = 0.;
  }
  // reset values different from zero
  if(to>=kTrackMax && to>kParticleMax) {
    values[kTrdPidANN]  = -999.;
    values[kRichPidANN] = -999.;
  }
  if(to>=kHitMax && to>kConstMax) {
    values[kMassSq]     = -999.;
    values[kBeta]       = -999.;
    values[kTofPidDeltaBetaEL] = -999.;
    values[kTofPidDeltaBetaMU] = -999.;
    values[kTofPidDeltaBetaPI] = -999.;
    values[kTofPidDeltaBetaKA] = -999.;
    values[kTofPidDeltaBetaPR] = -999.;
  }
}


inline void PairAnalysisVarManager::ResetArrayDataMC(Int_t to, Double_t * const values)
{
  // Protect
  if (to >= kNMaxValuesMC) return;
  // Reset
  for (Int_t i=kNMaxValues; i<to; ++i) values[i] = 0.;
  // reset values different from zero
  //  /*
  values[kPdgCode]            = -99999.;
  values[kPdgCodeMother]      = -99999.;
  values[kPdgCodeGrandMother] = -99999.;
  //  */
  //valuesMC[kNumberOfDaughters]  = -999.;
  if(to>=kHitMaxMC && to>kNMaxValues) {
    values[kPosXMC]     = -999.;
    values[kPosYMC]     = -999.;
    values[kPosZMC]     = -999.;
    values[kElossMC]    = -999.;
  }
}

inline void PairAnalysisVarManager::FillVarPairAnalysisEvent(const PairAnalysisEvent *event, Double_t * const values)
{
  //
  // Fill event information available into an array
  //

  // Reset array
  ResetArrayData(  kNMaxValues,   values);
  ResetArrayDataMC(kNMaxValuesMC, values);

  // Protect
  if(!event) return;

  // Set
  values[kNTrk]         = event->GetNumberOfTracks();
  values[kStsMatches]   = event->GetNumberOfMatches(ECbmModuleId::kSts);
  values[kTrdMatches]   = event->GetNumberOfMatches(ECbmModuleId::kTrd);
  values[kVageMatches]  = event->GetNumberOfVageMatches();
  values[kTotalTRDHits] = event->GetNumberOfHits(ECbmModuleId::kTrd);
  const Double_t proMass = TDatabasePDG::Instance()->GetParticle(2212)->Mass();
  Double_t beta          = TMath::Sqrt(values[kEbeam]*values[kEbeam] - proMass*proMass) / (values[kEbeam]+proMass);
  values[kYbeam]        = TMath::ATanH( beta );
  //  Printf("beam rapidity new: %f",values[kYbeam]);
  values[kNTrkMC]         = event->GetNumberOfMCTracks();
  values[kTotalTRDHitsMC] = event->GetNumberOfPoints(ECbmModuleId::kTrd);


  // Set vertex
  FillVarVertex(event->GetPrimaryVertex(),values);

  // Set header information
  FillVarMCHeader(event->GetMCHeader(),values);
  

}

inline void PairAnalysisVarManager::FillVarMCHeader(const FairMCEventHeader *header, Double_t * const values)
{
  //
  // Fill MCheader information available into an array
  //

  // Protect
  if (!header) return;

  // Reset
  // ResetArrayData(kNMaxValues, values);
  if(fgVertexMC) fgVertexMC->Reset();

  // Set
  //  values[k]  = header->GetPhi(); // event plane angle [rad]

  // accessors via first FairMCEventHeader
  values[kXvPrimMC]     = header->GetX();
  values[kYvPrimMC]     = header->GetY();
  values[kZvPrimMC]     = header->GetZ();
  values[kImpactParam]  = header->GetB(); // [fm]
  values[kNPrimMC]      = header->GetNPrim();

  // Fill mc vertex data member
  TMatrixFSym mat(3);
  fgVertexMC->SetVertex(values[kXvPrimMC],values[kYvPrimMC],values[kZvPrimMC],
			-999., 1., values[kNPrimMC], mat);
}

inline void PairAnalysisVarManager::FillVarVertex(const CbmVertex *vertex, Double_t * const values)
{
  //
  // Fill vertex information available into an array
  //

  // Protect
  if (!vertex) return;

  // Reset
  // ResetArrayData(kNMaxValues, values);

  // Set
  values[kXvPrim]       = vertex->GetX();
  values[kYvPrim]       = vertex->GetY();
  values[kZvPrim]       = vertex->GetZ();
  values[kNVtxContrib]  = vertex->GetNTracks();
  values[kVtxChi]       = vertex->GetChi2();
  values[kVtxNDF]       = vertex->GetNDF();

}


inline void PairAnalysisVarManager::FillVarPairAnalysisTrack(const PairAnalysisTrack *track, Double_t * const values)
{
  //
  // Fill track information for the all track and its sub tracks
  //

  // Reset
  ResetArrayData(  kTrackMax,   values);
  ResetArrayDataMC(kTrackMaxMC, values);

  // Protect
  if(!track) return;

  // Set track specific variables
  Fill(track->GetGlobalTrack(), values);
  Fill(track->GetStsTrack(),    values);
  Fill(track->GetMuchTrack(),   values);
  Fill(track->GetTrdTrack(),    values);
  Fill(track->GetRichRing(),    values);

  values[kP]         = track->P();
  
  values[kRichDistance]    = 1.;//CbmRichUtil::GetRingTrackDistance(track->GetGlobalIndex());
  values[kRichPidANN]      = CbmRichElectronIdAnn::GetInstance().CalculateAnnValue(track->GetGlobalIndex(), values[kP]); // fgRichElIdA
  
  // acceptance defintions
  FairTrackParam *param=NULL;
  if( (param = track->GetRichProj()) ) {   // RICH
    values[kRichhasProj] = (TMath::Abs(param->GetX() + param->GetY()) > 0.);
  }
  if( (param = track->GetMvdEntrance()) ) {  // MVD
    values[kMvdFirstExtX] = param->GetX();
    values[kMvdFirstExtY] = param->GetY();
    Double_t innerLimit=0.5; //cm, TODO: no hardcoding
    Double_t outerLimit=2.5; //cm
    values[kMvdhasEntr] = ( (TMath::Abs(param->GetX()) > innerLimit && TMath::Abs(param->GetX()) < outerLimit && TMath::Abs(param->GetY()) < outerLimit)
			    ||
			    (TMath::Abs(param->GetY()) > innerLimit && TMath::Abs(param->GetY()) < outerLimit && TMath::Abs(param->GetX()) < outerLimit)
			  );
  }

  // mc
  Fill(track->GetMCTrack(),     values); // this contains particle infos as well

  if(track->GetTrackMatch(ECbmModuleId::kTrd)) {       // track match specific (accessors via CbmTrackMatchNew)
    values[kTrdMCTracks]    = track->GetTrackMatch(ECbmModuleId::kTrd)->GetNofLinks(); //number of different! mc tracks

    Int_t trueHits=0, distHits=0, fakeHits=0;
    CalculateHitTypes(track,ECbmModuleId::kTrd, &trueHits,&distHits,&fakeHits);

    values[kTrdTrueHits]    = trueHits;
    values[kTrdDistHits]    = distHits;
    values[kTrdFakeHits]    = fakeHits;
    //    values[kTrdDistortion]  = dist/links;
    /* values[kTrdTrueHits]    = tmtch->GetNofTrueHits(); //NOTE: changed defintion */
    /* values[kTrdFakeHits]    = tmtch->GetNofWrongHits(); //NOTE: changed definition */
  }
  if(track->GetTrackMatch(ECbmModuleId::kSts)) {
    Int_t trueHits=0, distHits=0, fakeHits=0;
    CalculateHitTypes(track,ECbmModuleId::kSts, &trueHits,&distHits,&fakeHits);

    values[kStsTrueHits]    = trueHits;
    values[kStsDistHits]    = distHits;
    values[kStsFakeHits]    = fakeHits;
  }
  if(track->GetTrackMatch(ECbmModuleId::kRich)) {
    values[kRichMCPoints]    = track->GetTrackMatch(ECbmModuleId::kRich)->GetNofLinks();
  }
  values[kStsisMC]   = track->TestBit( BIT(14+ToIntegralType(ECbmModuleId::kSts) ) );
  values[kMuchisMC]  = track->TestBit( BIT(14+ToIntegralType(ECbmModuleId::kMuch)) );
  values[kTrdisMC]   = track->TestBit( BIT(14+ToIntegralType(ECbmModuleId::kTrd) ) );
  values[kRichisMC]  = track->TestBit( BIT(14+ToIntegralType(ECbmModuleId::kRich)) );
  values[kMvdisMC]   = track->TestBit( BIT(14+ToIntegralType(ECbmModuleId::kMvd) ) );
  values[kTofisMC]   = track->TestBit( BIT(14+ToIntegralType(ECbmModuleId::kTof) ) );
  values[kWeight]    = track->GetWeight();

  // Reset
  ResetArrayData(  kParticleMax,   values);

  // Set DATA default (refitted sts track to primary vertex)
  values[kPx]        = track->Px();
  values[kPy]        = track->Py();
  values[kPz]        = track->Pz();
  values[kPt]        = track->Pt();
  values[kPtSq]      = track->Pt()*track->Pt();
  values[kP]         = track->P();

  values[kXv]        = track->Xv();
  values[kYv]        = track->Yv();
  values[kZv]        = track->Zv();

  values[kOneOverPt] = (track->Pt()>1.0e-3 ? track->OneOverPt() : 0.0);
  values[kPhi]       = (TMath::IsNaN(track->Phi()) ? -999. : TVector2::Phi_0_2pi(track->Phi()));
  values[kTheta]     = track->Theta();
  //  values[kEta]       = track->Eta();
  values[kY]         = track->Y() - values[kYbeam];
  values[kYlab]      = track->Y();
  values[kE]         = track->E();
  values[kM]         = track->M();
  values[kCharge]    = track->Charge();
  values[kMt]        = TMath::Sqrt(values[kMPair]*values[kMPair] + values[kPtSq]);
  //  values[kPdgCode]   = track->PdgCode();
  values[kChi2NDFtoVtx] = track->ChiToVertex();
  values[kImpactParXY]  = TMath::Sqrt( TMath::Power(TMath::Abs(values[kXv]-values[kXvPrim]),2) + 
				       TMath::Power(TMath::Abs(values[kYv]-values[kYvPrim]),2) );
  values[kImpactParZ]   = TMath::Abs(values[kZv]-values[kZvPrim]);

  // special
  ///  printf("track length %f \n",values[kTrackLength]);
  //  values[kTrackLength] = track->GetGlobalTrack()->GetLength(); // cm
  values[kInclAngle] = TMath::ASin(track->Pt()/track->P());
  Fill(track->GetTofHit(),      values);
  values[kTofHits]   = (track->GetTofHit() ? 1. : 0.);
  values[kRndmTrack] = gRandom->Rndm();

}

inline void PairAnalysisVarManager::FillVarGlobalTrack(const CbmGlobalTrack *track, Double_t * const values)
{
  //
  // Fill track information for the global track into array
  //

  // Protect
  if(!track) return;

  // Set
  values[kTrackChi2NDF]= (track->GetNDF()>0. ? track->GetChi2()/track->GetNDF() : -999.);
  values[kTrackLength] = track->GetLength(); // cm
  // accessors via first FairTrackParam
  TVector3 mom;
  track->GetParamFirst()->Momentum(mom);
  values[kPin]         = mom.Mag();
  values[kPtin]        = mom.Pt();
  track->GetParamLast()->Momentum(mom);
  values[kPout]        = mom.Mag();
  values[kPtout]       = mom.Pt();
  values[kCharge]      = (track->GetParamFirst()->GetQp()>0. ? +1. : -1. );
 
}

inline void PairAnalysisVarManager::FillVarRichRing(const CbmRichRing *track, Double_t * const values)
{
  //
  // Fill track information for the trd track into array
  //

  // Protect
  if(!track) return;

  // Set
  //Do Select is no longer supported use CbmRichElectronIdAnn::GetInstance().CalculateAnnValue(globalTrackIndex, momentum);
  //  values[kRichPidANN]      = -1; // fgRichElIdAnn->DoSelect(const_cast<CbmRichRing*>(track), values[kP]); // PID value ANN method
  values[kRichHitsOnRing]  = track->GetNofHitsOnRing();
  values[kRichHits]        = track->GetNofHits();
  values[kRichChi2NDF]     = (track->GetNDF()>0. ? track->GetChi2()/track->GetNDF() : -999.);
  values[kRichRadius]      = track->GetRadius();
  values[kRichAxisA]       = track->GetAaxis();
  values[kRichAxisB]       = track->GetBaxis();
  values[kRichCenterX]     = track->GetCenterX();
  values[kRichCenterY]     = track->GetCenterY();
	// CbmRichRing::GetDistance() method is no longer supported
	// If you wan to use cuts update code using CbmRichUtil::GetRingTrackDistance()
  //  values[kRichDistance]    = 1.;
  values[kRichRadialPos]   = track->GetRadialPosition();
  values[kRichRadialAngle] = track->GetRadialAngle();
  values[kRichPhi]         = track->GetPhi();

}

inline void PairAnalysisVarManager::FillVarTrdTrack(const CbmTrdTrack *track, Double_t * const values)
{
  //
  // Fill track information for the trd track into array
  //

  // Protect
  if(!track) return;

  // Calculate eloss
  TClonesArray *hits   = fgEvent->GetHits(ECbmModuleId::kTrd);
  if(hits &&  track->GetELoss()<1.e-8 /*&& Req(kTrdSignal)*/ ) {
    Double_t eloss   = 0;
    for (Int_t ihit=0; ihit < track->GetNofHits(); ihit++){
      Int_t idx = track->GetHitIndex(ihit);
      CbmTrdHit* hit = (CbmTrdHit*) hits->At(idx);
      if(hit) {
	eloss   += hit->GetELoss(); // dEdx + TR
      }
    }
    //   printf("track %p \t eloss %.3e \n",track,eloss);
    const_cast<CbmTrdTrack*>(track)->SetELoss(eloss); // NOTE: this is the sum
  }

  // Set
  values[kTrdSignal]      = track->GetELoss() * 1.e+6; //GeV->keV, NOTE: see corrections,normalisation below (angles,#hits)
  values[kTrdPidWkn]      = track->GetPidWkn(); // PID value Wkn method
  values[kTrdPidANN]      = track->GetPidANN(); // PID value ANN method
  // PID value Likelihood method
  values[kTrdPidLikeEL]   = track->GetPidLikeEL();
  values[kTrdPidLikePI]   = track->GetPidLikePI();
  values[kTrdPidLikeKA]   = track->GetPidLikeKA();
  values[kTrdPidLikePR]   = track->GetPidLikePR();
  values[kTrdPidLikeMU]   = track->GetPidLikeMU();
  // accessors via CbmTrack
  values[kTrdHits]        = track->GetNofHits();
  values[kTrdChi2NDF]     = (track->GetNDF()>0. ? track->GetChiSq()/track->GetNDF() : -999.);
  // accessors via first FairTrackParam
  TVector3 mom;
  track->GetParamFirst()->Momentum(mom);
  values[kTrdPin]         = mom.Mag();
  values[kTrdPtin]        = mom.Pt();
  values[kTrdThetain]     = mom.Theta();
  values[kTrdPhiin]       = mom.Phi();
  // correction factors
  values[kTrdThetaCorr]   = 1. / mom.CosTheta();
  values[kTrdPhiCorr]     = 1. / TMath::Cos(mom.Phi());
  // apply correction and normalisation
  values[kTrdSignal]      /= values[kTrdHits];// * values[kTrdThetaCorr] * values[kTrdPhiCorr]);

  track->GetParamLast()->Momentum(mom);
  values[kTrdPout]        = mom.Mag();
  values[kTrdPtout]       = mom.Pt();
  //  values[kTrdCharge]      = (track->GetParamFirst()->GetQp()>0. ? +1. : -1. );
  /* TVector3 pos1; */
  /* track->GetParamFirst()->Position(pos1); */
  /* TVector3 pos2; */
  /* track->GetParamLast()->Position(pos2); */
  //  values[kTrdTrackLength] =  (pos2!=pos1 ? (pos2-=pos1).Mag() : 1.);

 
}

inline void PairAnalysisVarManager::FillVarStsTrack(const CbmStsTrack *track, Double_t * const values)
{
  //
  // Fill track information for the Sts track into array
  //

  // Protect
  if(!track) return;

  // Calculate first hit position for sts and mvd
  Double_t minSts = 9999.;
  TClonesArray *hits   = fgEvent->GetHits(ECbmModuleId::kSts);
  if(hits /*&& Req(kStsFirstHitPosZ)*/ ) {
    for (Int_t ihit=0; ihit < track->GetNofStsHits(); ihit++){
      Int_t idx = track->GetStsHitIndex(ihit);
      CbmStsHit* hit = (CbmStsHit*) hits->At(idx);
      if(hit && minSts > hit->GetZ()) {
	minSts = hit->GetZ();
	//	Printf("hit %d idx %d position %.5f",ihit,idx,min);
      }
    }
  }
  Double_t minMvd = 9999.;
  hits   = fgEvent->GetHits(ECbmModuleId::kMvd);
  if(hits) {
    for (Int_t ihit=0; ihit < track->GetNofMvdHits(); ihit++){
      Int_t idx = track->GetMvdHitIndex(ihit);
      CbmMvdHit* hit = (CbmMvdHit*) hits->At(idx);
      if(hit && minMvd > hit->GetZ()) {
	minMvd = hit->GetZ();
      }
    }
  }

  // Set
  values[kMvdHits]        = track->GetNofMvdHits();
  //  values[kImpactParZ]     = track->GetB();  //Impact parameter of track at target z, in units of its error
  //  printf(" mom %f   impactparz %f \n",values[kPout],values[kImpactParZ]);
  // accessors via CbmTrack
  values[kStsHits]        = track->GetNofStsHits();
  values[kStsChi2NDF]     = (track->GetNDF()>0. ? track->GetChiSq()/track->GetNDF() : -999.);
  // accessors via first FairTrackParam
  TVector3 mom;
  track->GetParamFirst()->Momentum(mom);
  values[kStsPin]         = mom.Mag();
  values[kStsPtin]        = mom.Pt();
  track->GetParamFirst()->Position(mom);
  values[kStsXv]          = mom.X();
  values[kStsYv]          = mom.Y();
  values[kStsZv]          = mom.Z();
  track->GetParamLast()->Momentum(mom);
  values[kStsPout]        = mom.Mag();
  values[kStsPtout]       = mom.Pt();
  //  values[kStsCharge]      = (track->GetParamFirst()->GetQp()>0. ? +1. : -1. );

  values[kMvdFirstHitPosZ]= minMvd;
  values[kStsFirstHitPosZ]= minSts;


}

inline void PairAnalysisVarManager::FillVarMuchTrack(const CbmMuchTrack *track, Double_t * const values)
{
  //
  // Fill track information for the Much track into array
  //

  // Protect
  if(!track) return;

  // Calculate straw, (TODO:trigger) and pixel hits
  values[kMuchHitsPixel]  = 0.;
  for (Int_t ihit=0; ihit < track->GetNofHits(); ihit++){
//    Int_t idx = track->GetHitIndex(ihit);
      Int_t hitType = track->GetHitType(ihit);
      if      (hitType==ToIntegralType(ECbmDataType::kMuchPixelHit)) values[kMuchHitsPixel]++;
  }

  // Set
  // accessors via CbmTrack
  values[kMuchHits]        = track->GetNofHits();
  values[kMuchChi2NDF]     = (track->GetNDF()>0. ? track->GetChiSq()/track->GetNDF() : -999.);
}

inline void PairAnalysisVarManager::FillVarMCParticle(const CbmMCTrack *p1, const CbmMCTrack *p2, Double_t * const values)
{
  //
  // fill 2 track information starting from MC legs
  //

  // Reset
  ResetArrayDataMC(kPairMaxMC, values);

  // Protect
  if(!p1 || !p2) return;

  // Get the MC interface if available
  PairAnalysisMC *mc = PairAnalysisMC::Instance();
  if (!mc->HasMC()) return;

  // Set
  CbmMCTrack* mother=0x0;
  Int_t mLabel1 = p1->GetMotherId();
  Int_t mLabel2 = p2->GetMotherId();
  if(mLabel1==mLabel2) mother = mc->GetMCTrackFromMCEvent(mLabel1);
  
  PairAnalysisPair *pair = new PairAnalysisPairLV();
  pair->SetMCTracks(p1,p2);

  if(mother)
    FillVarMCTrack(mother,values);
  else {
    values[kPxMC]        = pair->Px();
    values[kPyMC]        = pair->Py();
    values[kPzMC]        = pair->Pz();
    values[kPtMC]        = pair->Pt();
    values[kPtSqMC]      = pair->Pt()*pair->Pt();
    values[kPMC]         = pair->P();

    values[kXvMC]        = pair->Xv();
    values[kYvMC]        = pair->Yv();
    values[kZvMC]        = pair->Zv();
    //TODO  values[kTMC]         = 0.;

    values[kOneOverPtMC] = (pair->Pt()>1.0e-3 ? pair->OneOverPt() : 0.0);
    values[kPhiMC]       = (TMath::IsNaN(pair->Phi()) ? -999. : TVector2::Phi_0_2pi(pair->Phi()));
    values[kThetaMC]     = pair->Theta();
    //    values[kEtaMC]       = pair->Eta();
    values[kYMC]         = pair->Y() - values[kYbeam];
    values[kYlabMC]      = pair->Y();
    values[kEMC]         = pair->E();
    values[kMMC]         = pair->M();
    values[kChargeMC]    = p1->GetCharge()*p2->GetCharge();

  }
  values[kOpeningAngleMC]     = pair->OpeningAngle();
  values[kCosPointingAngleMC] = fgVertexMC ? pair->GetCosPointingAngle(fgVertexMC) : -1;
  //  values[kPhivPairMC]     = pair->PhivPair(1.);

  // delete the surplus pair
  delete pair;
  
}

inline void PairAnalysisVarManager::FillVarMCTrack(const CbmMCTrack *particle, Double_t * const values) {
  //
  // fill particle information from MC leg
  //

  // Reset
  ResetArrayDataMC(kTrackMaxMC, values);

  // Protect
  if(!particle) return;

  // Get the MC interface if available
  PairAnalysisMC *mc = PairAnalysisMC::Instance();
  if (!mc->HasMC()) return;

  // Set
  CbmMCTrack* mother=0x0;
  Int_t mLabel1 = particle->GetMotherId();
  mother = mc->GetMCTrackFromMCEvent(mLabel1);

  values[kPdgCode]            = particle->GetPdgCode();
  values[kPdgCodeMother]      = (mother ? mother->GetPdgCode() : -99999. );
  values[kEMotherMC]          = (mother ? mother->GetEnergy()  : -99999. );
  CbmMCTrack* granni = 0x0;
  if(mother) granni = mc->GetMCTrackMother(mother);
  values[kPdgCodeGrandMother] = (granni ? granni->GetPdgCode() : -99999. );//mc->GetMotherPDG(mother);
  values[kGeantId]            = particle->GetGeantProcessId();

  values[kPxMC]        = particle->GetPx();
  values[kPyMC]        = particle->GetPy();
  values[kPzMC]        = particle->GetPz();
  values[kPtMC]        = particle->GetPt();
  values[kPtSqMC]      = particle->GetPt()*particle->GetPt();
  values[kPMC]         = particle->GetP();

  values[kXvMC]        = particle->GetStartX();
  values[kYvMC]        = particle->GetStartY();
  values[kZvMC]        = particle->GetStartZ();
  TVector3 vtx;
  particle->GetStartVertex(vtx);
  values[kPhivMC]      = vtx.Phi();
  values[kThetavMC]    = vtx.Theta();
  //TODO  values[kTMC]         = particle->GetStartT(); [ns]

  TLorentzVector mom;
  if(particle) particle->Get4Momentum(mom);
  values[kOneOverPtMC] = (particle->GetPt()>1.0e-3 ? 1./particle->GetPt() : 0.0);
  values[kPhiMC]       = (TMath::IsNaN(mom.Phi()) ? -999. : TVector2::Phi_0_2pi(mom.Phi()));
  values[kThetaMC]     = mom.Theta();
  //  values[kEtaMC]       = mom.Eta();
  values[kYMC]         = particle->GetRapidity() - values[kYbeam];;
  values[kYlabMC]      = particle->GetRapidity();
  Double_t pom  = particle->GetP()/particle->GetMass();
  Double_t beta  = pom / TMath::Sqrt(pom*pom+1.);
    //  Double_t gamma = 1./ TMath::Sqrt(1.-pom*pom);
  values[kBetaGammaMC] = 1./ TMath::Sqrt(1.-beta*beta);
  values[kEMC]         = particle->GetEnergy();
  values[kMMC]         = mom.M();//particle->GetMass();
  values[kChargeMC]    = particle->GetCharge();

  // detector info
  values[kRichHitsMC]  = particle->GetNPoints(ECbmModuleId::kRich);
  values[kTrdHitsMC]   = particle->GetNPoints(ECbmModuleId::kTrd);
  values[kMvdHitsMC]   = particle->GetNPoints(ECbmModuleId::kMvd);
  values[kStsHitsMC]   = particle->GetNPoints(ECbmModuleId::kSts);
  values[kTofHitsMC]   = particle->GetNPoints(ECbmModuleId::kTof);
  values[kMuchHitsMC]  = particle->GetNPoints(ECbmModuleId::kMuch);

}

inline void PairAnalysisVarManager::FillVarPairAnalysisPair(const PairAnalysisPair *pair, Double_t * const values)
{
  //
  // Fill pair information available for histogramming into an array
  //

  // Reset
  ResetArrayData(kPairMax, values);
  ResetArrayDataMC(kPairMaxMC, values);

  // Protect
  if(!pair) return;

  // set the beamenergy (needed by some variables)
  pair->SetBeamEnergy( values[kEbeam] );

  // first fill mc info to avoid kWeight beeing reset
  // TODO: check if it makes sence only for pairtypes of SE
  FillVarMCParticle(pair->GetFirstDaughter()->GetMCTrack(),
		    pair->GetSecondDaughter()->GetMCTrack(), values);

  // Set
  values[kPairType]  = pair->GetType();

  values[kPx]        = pair->Px();
  values[kPy]        = pair->Py();
  values[kPz]        = pair->Pz();
  values[kPt]        = pair->Pt();
  values[kPtSq]      = pair->Pt()*pair->Pt();
  values[kP]         = pair->P();

  values[kXv]        = pair->Xv();
  values[kYv]        = pair->Yv();
  values[kZv]        = pair->Zv();

  values[kOneOverPt] = (pair->Pt()>1.0e-3 ? pair->OneOverPt() : 0.0);
  values[kPhi]       = (TMath::IsNaN(pair->Phi()) ? -999. : TVector2::Phi_0_2pi(pair->Phi()));
  values[kTheta]     = pair->Theta();
  //  values[kEta]       = pair->Eta();
  values[kY]         = pair->Y() - values[kYbeam];
  values[kYlab]      = pair->Y();
  values[kE]         = pair->E();
  values[kM]         = pair->M();
  values[kCharge]    = pair->Charge();
  values[kMt]        = TMath::Sqrt(values[kMPair]*values[kMPair] + values[kPtSq]);

  ///TODO: check
  /* values[kPdgCode]=-1; */
  /* values[kPdgCodeMother]=-1; */
  /* values[kPdgCodeGrandMother]=-1; */
  values[kWeight]    = pair->GetWeight();

  Double_t thetaHE=0;
  Double_t phiHE=0;
  Double_t thetaCS=0;
  Double_t phiCS=0;
  if(Req(kThetaHE) || Req(kPhiHE) || Req(kThetaCS) || Req(kPhiCS)) {
    pair->GetThetaPhiCM(thetaHE,phiHE,thetaCS,phiCS);

    values[kThetaHE]      = thetaHE;
    values[kPhiHE]        = phiHE;
    values[kThetaSqHE]    = thetaHE * thetaHE;
    values[kCos2PhiHE]    = TMath::Cos(2.0*phiHE);
    values[kCosTilPhiHE]  = (thetaHE>0)?(TMath::Cos(phiHE-TMath::Pi()/4.)):(TMath::Cos(phiHE-3*TMath::Pi()/4.));
    values[kThetaCS]      = thetaCS;
    values[kPhiCS]        = phiCS;
    values[kThetaSqCS]    = thetaCS * thetaCS;
    values[kCos2PhiCS]    = TMath::Cos(2.0*phiCS);
    values[kCosTilPhiCS]  = (thetaCS>0)?(TMath::Cos(phiCS-TMath::Pi()/4.)):(TMath::Cos(phiCS-3*TMath::Pi()/4.));
  }

  values[kChi2NDF]          = pair->GetChi2()/pair->GetNdf();
  values[kDecayLength]      = pair->GetDecayLength();
  values[kR]                = pair->GetR();
  values[kOpeningAngle]     = pair->OpeningAngle();
  values[kCosPointingAngle] = fgEvent ? pair->GetCosPointingAngle(fgEvent->GetPrimaryVertex()) : -1;

  values[kLegDist]          = pair->DistanceDaughters();
  values[kLegDistXY]        = pair->DistanceDaughtersXY();
  //  values[kDeltaEta]     = pair->DeltaEta();
  //  values[kDeltaPhi]     = pair->DeltaPhi();
  values[kLegsP]            = TMath::Sqrt(pair->DaughtersP());

  // Armenteros-Podolanski quantities
  values[kArmAlpha]         = pair->GetArmAlpha();
  values[kArmPt]            = pair->GetArmPt();

  //if(Req(kPsiPair))  values[kPsiPair]      = fgEvent ? pair->PsiPair(fgEvent->GetMagneticField()) : -5;
  //if(Req(kPhivPair))  values[kPhivPair]      = pair->PhivPair(1.);

  // impact parameter
  Double_t d0z0[2]={-999., -999.};
  if(fgEvent) pair->GetDCA(fgEvent->GetPrimaryVertex(), d0z0);
  values[kImpactParXY]   = d0z0[0];
  values[kImpactParZ]    = d0z0[1];

  values[kRndmPair] = gRandom->Rndm();

}


inline void PairAnalysisVarManager::FillVarPixelHit(const CbmPixelHit *hit, Double_t * const values)
{
  //
  // Fill hit information for the rich hit into array
  //

  // Protect
  if(!hit) return;

  // accessors via CbmPixelHit
  values[kPosX]     = hit->GetX();
  values[kPosY]     = hit->GetY();
  // accessors via CbmHit
  values[kPosZ]     = hit->GetZ();
  // accessors via CbmMatch
  values[kLinksMC]  = (hit->GetMatch() ? hit->GetMatch()->GetNofLinks() : 0.);

}

inline void PairAnalysisVarManager::FillVarStsHit(const CbmStsHit *hit, Double_t * const values)
{
  //
  // Fill hit information for the sts hit into array
  //

  // Reset array
  ResetArrayData(  kHitMax,   values);

  // Protect
  if(!hit) return;

  // accessors via CbmPixelHit & CbmHit
  FillVarPixelHit(hit, values);

  // Set
  // ...

}

inline void PairAnalysisVarManager::FillVarMvdHit(const CbmMvdHit *hit, Double_t * const values)
{
  //
  // Fill hit information for the mvd hit into array
  //

  // Reset array
  ResetArrayData(  kHitMax,   values);

  // Protect
  if(!hit) return;

  // accessors via CbmPixelHit & CbmHit
  FillVarPixelHit(hit, values);

  // Set
  // TODO: add center of gravity?
  // ...

}

inline void PairAnalysisVarManager::FillVarMuchHit(const CbmMuchPixelHit *hit, Double_t * const values)
{
  //
  // Fill hit information for the much hit into array
  //

  // Reset array
  ResetArrayData(  kHitMax,   values);

  // Protect
  if(!hit) return;

  // accessors via CbmPixelHit & CbmHit
  FillVarPixelHit(hit, values);

  // Set
  // ...

}

inline void PairAnalysisVarManager::FillVarRichHit(const CbmRichHit *hit, Double_t * const values)
{
  //
  // Fill hit information for the rich hit into array
  //

  // Reset array
  ResetArrayData(  kHitMax,   values);

  // Protect
  if(!hit) return;

  // accessors via CbmPixelHit & CbmHit
  FillVarPixelHit(hit, values);

  // Set
    values[kNPhotons] = 1;//hit->GetNPhotons();
  values[kPmtId]    = hit->GetPmtId();


}

inline void PairAnalysisVarManager::FillVarTrdHit(const CbmTrdHit *hit, Double_t * const values)
{
  //
  // Fill hit information for the trd hit into array
  //

  // Reset array
  ResetArrayData(  kHitMax,   values);

  // Protect
  if(!hit) return;

  // accessors via CbmPixelHit & CbmHit
  FillVarPixelHit(hit, values);

  // accessors via CbmCluster & CbmTrdCluster
  TClonesArray *cluster  = fgEvent->GetCluster(ECbmModuleId::kTrd);
  if(cluster->GetEntriesFast()>0) {
    const CbmTrdCluster *cls = static_cast<const CbmTrdCluster*>( cluster->At(hit->GetRefId()) );
    FillVarTrdCluster(cls, values);
    //    if(cls) std::cout << (cls->ToString()).data();
  }

  // Set
  values[kTrdLayer]  = hit->GetPlaneId(); //layer id
  /// NOTE: use correction from first TRD track param
  values[kEloss]     = hit->GetELoss()     * 1.e+6; //GeV->keV, dEdx + TR
//  values[kElossdEdx] = hit->GetELossdEdX() * 1.e+6; //GeV->keV, dEdx
//  values[kElossTR]   = hit->GetELossTR()   * 1.e+6; //GeV->keV, TR
  //  Printf("eloss trd: %.3e (%.3e TR, %.3e dEdx)",hit->GetELoss(),hit->GetELossTR(),hit->GetELossdEdX());
}

inline void PairAnalysisVarManager::FillVarTofHit(const CbmTofHit *hit, Double_t * const values)
{
  //
  // Fill hit information for the tof hit into array
  //

  // Reset array
  ResetArrayData(  kHitMax,   values);

  // Protect
  if(!hit) return;

  // accessors via CbmPixelHit & CbmHit
  FillVarPixelHit(hit, values);

  // Set
  values[kBeta]    = values[kTrackLength]/100 / (hit->GetTime()*1e-9) / TMath::C();
  // PID value detla beta
  values[kTofPidDeltaBetaEL] = values[kBeta] - ( values[kP]/TMath::Sqrt(values[kMEL]*values[kMEL]+values[kP]*values[kP]) );
  values[kTofPidDeltaBetaMU] = values[kBeta] - ( values[kP]/TMath::Sqrt(values[kMMU]*values[kMMU]+values[kP]*values[kP]) );
  values[kTofPidDeltaBetaPI] = values[kBeta] - ( values[kP]/TMath::Sqrt(values[kMPI]*values[kMPI]+values[kP]*values[kP]) );
  values[kTofPidDeltaBetaKA] = values[kBeta] - ( values[kP]/TMath::Sqrt(values[kMKA]*values[kMKA]+values[kP]*values[kP]) );
  values[kTofPidDeltaBetaPR] = values[kBeta] - ( values[kP]/TMath::Sqrt(values[kMPR]*values[kMPR]+values[kP]*values[kP]) );

  values[kMassSq]    = values[kP]*values[kP] * (TMath::Power(1./values[kBeta],2) -1);

  //  Printf("track length: %f beta: %f",values[kTrackLength],values[kBeta]);
  //  Double_t mass2 = TMath::Power(momentum, 2.) * (TMath::Power(time/ trackLength, 2) - 1);
}

inline void PairAnalysisVarManager::FillVarTrdCluster(const CbmTrdCluster *cluster, Double_t * const values)
{
  //
  // Fill cluster information for the trd cluster into array
  //

  // Reset array
  //  ResetArrayDataMC(  kHitMaxMC,   values);

  // Protect
  if(!cluster) return;

  // accessors via CbmCluster
  values[kTrdPads]  = cluster->GetNofDigis();

  // Set
  values[kTrdCols]  = cluster->GetNCols();
  values[kTrdRows]  = cluster->GetNRows();

}

inline void PairAnalysisVarManager::FillVarMCPoint(const FairMCPoint *hit, Double_t * const values)
{
  //
  // Fill MC hit information
  //

  // Reset array
  ResetArrayDataMC(  kHitMaxMC,   values);

  // Protect
  if(!hit) return;

  // Set
  values[kPosXMC]     = hit->GetX();
  values[kPosYMC]     = hit->GetY();
  values[kPosZMC]     = hit->GetZ();
  values[kElossMC]    = hit->GetEnergyLoss() * 1.e+6; //GeV->keV, dEdx

}

inline void PairAnalysisVarManager::FillSumVarMCPoint(const FairMCPoint *hit, Double_t * const values)
{
  //
  // Sum upMC hit information
  //

  // DO NOT reset array

  // Protect
  if(!hit) return;

  // Set
  values[kPosXMC]     += hit->GetX();
  values[kPosYMC]     += hit->GetY();
  values[kPosZMC]     += hit->GetZ();
  values[kElossMC]    += hit->GetEnergyLoss() * 1.e+6; //GeV->keV, dEdx

}

inline void PairAnalysisVarManager::FillVarConstants(Double_t * const values)
{
  //
  // Fill constant information available into an array
  // make use of TPDGCode
  //

  // Set
  values[kMEL]          = TDatabasePDG::Instance()->GetParticle(kElectron) ->Mass();
  values[kMMU]          = TDatabasePDG::Instance()->GetParticle(kMuonMinus)->Mass();
  values[kMPI]          = TDatabasePDG::Instance()->GetParticle(kPiPlus)   ->Mass();
  values[kMKA]          = TDatabasePDG::Instance()->GetParticle(kKPlus)    ->Mass();
  values[kMPR]          = TDatabasePDG::Instance()->GetParticle(kProton)   ->Mass();
  values[kMK0]          = TDatabasePDG::Instance()->GetParticle(kK0Short)  ->Mass();
  values[kMLA]          = TDatabasePDG::Instance()->GetParticle(kLambda0)  ->Mass();
  values[kMPair]        = fgData[kMPair]; /// automaticaly filled in PairAnalysis::Process using PairAnalysis::fPdgMother
  values[kEbeam]        = fgData[kEbeam]; /// automaticaly filled in AnalysisTaskMultiPairAnalysis::Init
}

inline void PairAnalysisVarManager::SetEvent(PairAnalysisEvent * const ev)
{
  //
  // set event and vertex
  //

  // Set
  fgEvent = ev;

  // Reset
  if (fgKFVertex) delete fgKFVertex;
  fgKFVertex=0x0;
  if (fgVertexMC) fgVertexMC->Reset();
  else            fgVertexMC = new CbmVertex();

  // Set
  FillVarConstants(fgData);
  if (ev && ev->GetPrimaryVertex()) fgKFVertex=new CbmKFVertex(*ev->GetPrimaryVertex());
  Fill(fgEvent, fgData);
}

inline void PairAnalysisVarManager::SetEventData(const Double_t data[PairAnalysisVarManager::kNMaxValuesMC])
{
  /* for (Int_t i=0; i<kNMaxValuesMC;++i) fgData[i]=0.; */
  for (Int_t i=kPairMax; i<kNMaxValues;++i)     fgData[i]=data[i];
  for (Int_t i=kPairMaxMC; i<kNMaxValuesMC;++i) fgData[i]=data[i];
}


inline void PairAnalysisVarManager::CalculateHitTypes(const PairAnalysisTrack *track, ECbmModuleId idet,
						      Int_t *trueH, Int_t *distH, Int_t *fakeH) {

  CbmTrack    *trkl = track->GetTrack(idet);
  CbmRichRing *ring = track->GetRichRing();
  Int_t nhits = 0;
  switch(idet) {
  case ECbmModuleId::kMvd:  if(trkl) nhits = static_cast<CbmStsTrack*>(trkl)->GetNofMvdHits(); break;
  case ECbmModuleId::kSts:  if(trkl) nhits = static_cast<CbmStsTrack*>(trkl)->GetNofStsHits(); break;
  case ECbmModuleId::kMuch:
  case ECbmModuleId::kTrd:  if(trkl) nhits = trkl->GetNofHits();    break;
  case ECbmModuleId::kTof:  nhits = 1; /* one is maximum */         break;
  case ECbmModuleId::kRich: if(ring) nhits = ring->GetNofHits();    break;
  default:
    return;
  }

  CbmTrackMatchNew *tmtch = track->GetTrackMatch(idet);
  Int_t mctrk = (tmtch && tmtch->GetNofHits()>0 && tmtch->GetNofLinks()>0 ? tmtch->GetMatchedLink().GetIndex() : -1 );

    PairAnalysisMC *mc = PairAnalysisMC::Instance();
    if(mc) {

      /// Calculate true, distorted and fake hits
      TClonesArray *hits   = fgEvent->GetHits(idet);
      TClonesArray *pnts   = fgEvent->GetPoints(idet);

      Int_t links=0;
      Double_t dist=0.;
      *trueH=0;
      *distH=0;
      *fakeH=(mctrk>-1 ? 0 : nhits);
      if(hits && pnts && mctrk>-1) {
	for (Int_t ihit=0; ihit < nhits; ihit++){

	  CbmHit *hit=NULL;
	  Int_t idx      = -1;
	  switch(idet) {
	  case ECbmModuleId::kMvd:  idx = static_cast<CbmStsTrack*>(trkl)->GetMvdHitIndex(ihit); break;
	  case ECbmModuleId::kSts:  idx = static_cast<CbmStsTrack*>(trkl)->GetStsHitIndex(ihit); break;
	  case ECbmModuleId::kMuch:
	  case ECbmModuleId::kTrd:  idx = trkl->GetHitIndex(ihit);                               break;
	  case ECbmModuleId::kTof:  hit = track->GetTofHit();                                    break;
	  case ECbmModuleId::kRich: idx = ring->GetHit(ihit);                                    break;
	  default:
	    continue;
	  }

	  if(idet!=ECbmModuleId::kTof && idx>-1)  {
	    hit = dynamic_cast<CbmHit*>(hits->At(idx));
	  }

	  if(!hit)  { (*fakeH)++; continue; }
	  CbmMatch *mtch = hit->GetMatch();
	  if(!mtch)  { (*fakeH)++; continue; }

	  Bool_t btrueH=kTRUE;
	  Bool_t bfakeH=kTRUE;
	  Int_t nlinks=mtch->GetNofLinks();
	  links+=nlinks;
	  for (Int_t iLink = 0; iLink < nlinks; iLink++) {
	    //	if(nlinks!=1) { fakeH++; continue; }
	    FairMCPoint *pnt = static_cast<FairMCPoint*>( pnts->At(mtch->GetLink(iLink).GetIndex()) );
	    // hit defintion
	    if(!pnt) btrueH=kFALSE;
	    else {
	      Int_t lbl  = pnt->GetTrackID();
	      Int_t lblM = mc->GetMothersLabel(lbl);
	      Int_t lblG = mc->GetMothersLabel(lblM);
	      if(lbl!=mctrk && lblM!=mctrk && lblG!=mctrk) {
		btrueH=kFALSE; dist+=1.;
	      }
	      else                                         bfakeH=kFALSE;
	    }
	  }
	  // increase counters
	  if(btrueH) (*trueH)++;
	  if(bfakeH) (*fakeH)++;
	  if(!btrueH &&!bfakeH) (*distH)++;
	}
      }
      /* values[kTrdDistortion]  = dist/links; */
    }


}


inline UInt_t* PairAnalysisVarManager::GetArray(ValueTypes var0,
						 ValueTypes var1, ValueTypes var2, ValueTypes var3,
						 ValueTypes var4, ValueTypes var5, ValueTypes var6,
						 ValueTypes var7, ValueTypes var8, ValueTypes var9) {
  //
  // build var array for e.g. TFormula's, THnBase's
  //
  static UInt_t arr[10] = {0};
  arr[0]=static_cast<UInt_t>(var0);
  arr[1]=static_cast<UInt_t>(var1);
  arr[2]=static_cast<UInt_t>(var2);
  arr[3]=static_cast<UInt_t>(var3);
  arr[4]=static_cast<UInt_t>(var4);
  arr[5]=static_cast<UInt_t>(var5);
  arr[6]=static_cast<UInt_t>(var6);
  arr[7]=static_cast<UInt_t>(var7);
  arr[8]=static_cast<UInt_t>(var8);
  arr[9]=static_cast<UInt_t>(var9);
  return arr;

}

inline void PairAnalysisVarManager::InitFormulas() {
  if(fgFormula[1]) return;
  for(Int_t i=1; i<kNMaxValuesMC-1; ++i) {
    fgFormula[i] = new TFormula(fgkParticleNames[i][0],"[0]");
    fgFormula[i]->SetParameter(0, i);
    fgFormula[i]->SetParName(  0, fgkParticleNames[i][0] );
  }
}

inline void PairAnalysisVarManager::InitFitter() {
  /* if(!fgKFFitter) { */
  /*   fgKFFitter = new CbmStsKFTrackFitter(); */
  /*   fgKFFitter->Init(); */
  /* } */
  //  if(!fgL1Fitter) fgL1Fitter = new CbmL1PFFitter();
}

#endif

