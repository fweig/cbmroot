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

#include <CbmCluster.h>
#include <CbmDefs.h>
#include <CbmGlobalTrack.h>
#include <CbmKFVertex.h>
#include <CbmMCTrack.h>
#include <CbmMuchPixelHit.h>
#include <CbmMuchTrack.h>
#include <CbmMvdHit.h>
#include <CbmPixelHit.h>
#include <CbmRichElectronIdAnn.h>
#include <CbmRichHit.h>
#include <CbmRichRing.h>
#include <CbmStsHit.h>
#include <CbmStsTrack.h>
#include <CbmTofHit.h>
#include <CbmTrackMatchNew.h>
#include <CbmTrdCluster.h>
#include <CbmTrdHit.h>
#include <CbmTrdTrack.h>
#include <CbmVertex.h>

#include <FairMCEventHeader.h>
#include <FairEventHeader.h>
#include <FairMCPoint.h>
#include <FairRootManager.h>
#include <FairTrackParam.h>

#include <TBits.h>
#include <TDatabasePDG.h>
#include <TFormula.h>
#include <TMatrixFSym.h>
#include <TNamed.h>
#include <TPDGCode.h>
#include <TRandom3.h>
#include <TVector2.h>
#include <TVector3.h>


//#include "L1Field.h"
//#include "CbmL1PFFitter.h"
//#include <CbmStsKFTrackFitter.h>

#include "PairAnalysisEvent.h"
#include "PairAnalysisHelper.h"
#include "PairAnalysisMC.h"
#include "PairAnalysisPair.h"
#include "PairAnalysisPairLV.h"
#include "PairAnalysisTrack.h"
#include "assert.h"

//________________________________________________________________
class PairAnalysisVarManager : public TNamed {

public:
  enum ValueTypes
  {
    // Constant information
    kMEL = 1,         // pdg mass of electrons
    kMMU,             // pdg mass of muons
    kMPI,             // pdg mass of pions
    kMKA,             // pdg mass of kaons
    kMPR,             // pdg mass of protons
    kMK0,             // pdg mass of neutral kaons
    kMLA,             // pdg mass of lambdas
    kMPair,           // pdg mass of pair
    kEbeam,           // beam energy
    kThermalScaling,  // scaling for uniform mass distributions
    kConstMax,
    // Hit specific variables
    kPosX = kConstMax,  // X position [cm]
    kPosY,              // Y position [cm]
    kPosZ,              // Z position [cm]
    kLinksMC,           // number of matched MC links
    kTrdLayer,          // plane/layer id
    kTrdPads,           // number of pads contributing to cluster/hit
    kTrdCols,           // number of pads columns contributing to cluster/hit
    kTrdRows,           // number of pads rows contributing to cluster/hit
    kEloss,             // TRD energy loss dEdx+TR
    //    kElossdEdx,              // TRD energy loss dEdx only
    //    kElossTR,                // TRD energy loss TR only
    kNPhotons,           // RICH number of photons in this hit
    kPmtId,              // RICH photomultiplier number
    kBeta,               // TOF beta
    kTofPidDeltaBetaEL,  // delta of TOF beta to expected beta for electrons
    kTofPidDeltaBetaMU,  // delta of TOF beta to expected beta for muons
    kTofPidDeltaBetaPI,  // delta of TOF beta to expected beta for pions
    kTofPidDeltaBetaKA,  // delta of TOF beta to expected beta for kaons
    kTofPidDeltaBetaPR,  // delta of TOF beta to expected beta for protons
    kMassSq,             // TOF mass squared
    kHitMax,
    // Particle specific variables
    kPx = kHitMax,  // px
    kPy,            // py
    kPz,            // pz
    kPt,            // transverse momentum
    kPtSq,          // transverse momentum squared
    kP,             // momentum
    kXv,            // vertex position in x
    kYv,            // vertex position in y
    kZv,            // vertex position in z
    kOneOverPt,     // 1/pt
    kPhi,           // phi angle
    kTheta,         // theta polar angle
    kEta,           // pseudo-rapidity
    kY,             // rapidity
    kYlab,          // rapidity lab
    kE,             // energy
    kM,             // mass
    kCharge,        // charge
    kMt,            // transverse mass sqrt(m^2+pt^2)
    kChi2NDFtoVtx,  // chi2/ndf impact parameter STS(+MVD) track to primary vertex in (sigmas)
    kImpactParXY,   // Impact parameter in XY plane
    kImpactParZ,    // Impact parameter in Z
    kInclAngle,     // inclination angle
    kParticleMax,
    // Track specific variables
    // global track
    kTrackLength = kParticleMax,  // Track length (cm)
    kTrackChi2NDF,                // chi2/ndf
    kPin,                         // first point momentum (GeV/c)
    kPtin,                        // first point transverse momentum (GeV/c)
    kPout,                        // last point momentum (GeV/c)
    kPtout,                       // last point transverse momentum (GeV/c)
    // trd track information
    kTrdSignal,     // TRD energy loss dEdx+TR (keV)
    kTrdPidWkn,     // PID value Wkn method
    kTrdPidANN,     // PID value Artificial Neural Network (ANN-method)
    kTrdPidLikeEL,  // PID value Likelihood method: electron
    kTrdPidLikePI,  // PID value Likelihood method: pion
    kTrdPidLikeKA,  // PID value Likelihood method: kaon
    kTrdPidLikePR,  // PID value Likelihood method: proton
    kTrdPidLikeMU,  // PID value Likelihood method: muon
    kTrdHits,       // number of TRD hits
    kTrdChi2NDF,    // chi2/ndf TRD
    kTrdPin,        // first point TRD momentum (GeV/c)
    kTrdPtin,       // first point TRD transverse momentum (GeV/c)
    kTrdPhiin,      // first point TRD azimuthal angle (rad)
    kTrdThetain,    // first point TRD polar angle (rad)
    kTrdPout,       // last point TRD momentum (GeV/c)
    kTrdPtout,      // last point TRD transverse momentum (GeV/c)
    kTrdThetaCorr,  // correction factor for theta track angle
    kTrdPhiCorr,    // correction factor for phi track angle
    //    kTrdTrackLength,         // track length in cm of the trd tracklet
    // sts track information
    kMvdhasEntr,                 // weather track enters first MVD station
    kMvdHits,                    // number of MVD hits
    kMvdHitClosest,              // Distance to the closest hit in the firstreconstructed MVD station
    kStsHitClosest,              // Distance to the closest hit in the firstreconstructed STS station
    kMvdHitClosestOpeningAngle,  // opening angle calculated between the reconstructed track and the track associated to the closest hit in the first mvd station
    kStsHitClosestOpeningAngle,  // opening angle calculated between the reconstructed track and the track associated to the closest hit in the first sts station
    kMvdHitClosestMom,           // sqrt( p_rec * p_closest_mvd)
    kStsHitClosestMom,           // sqrt( p_rec * p_closest_sts)
    kMvdFirstHitPosZ,            // position of the first hit in the MVD (cm)
    kMvdFirstExtX,               // x-position of the extrapolated track at the first MVD station (cm)
    kMvdFirstExtY,               // y-position of the extrapolated track at the first MVD station (cm)
    //    kImpactParZ,             // Impact parameter of track at target z, in units of its error
    kStsHits,          // number of STS hits
    kStsMvdHits,       // number of STS hits
    kStsChi2NDF,       // chi2/ndf STS
    kStsPin,           // first point STS momentum (GeV/c)
    kStsPtin,          // first point STS transverse momentum (GeV/c)
    kStsPout,          // last point STS momentum (GeV/c)
    kStsPtout,         // last point STS transverse momentum (GeV/c)
    kStsXv,            // STS point: x-coordinate
    kStsYv,            // STS point: y-coordinate
    kStsZv,            // STS point: z-coordinate
    kStsFirstHitPosZ,  // position of the first hit in the STS (cm)
    // rich ring information
    kRichhasProj,      // weather rich ring has a projection onto the pmt plane
    kRichPidANN,       // PID value Artificial Neural Network (ANN-method)
    kRichHitsOnRing,   // number of RICH hits on the ring
    kRichHits,         // number of RICH hits (ANN input)
    kRichChi2NDF,      // chi2/ndf ring fit (ANN input)
    kRichRadius,       // ring radius
    kRichAxisA,        // major semi-axis (ANN input)
    kRichAxisB,        // minor semi-axis (ANN input)
    kRichCenterX,      // ring center in x
    kRichCenterY,      // ring center in y
    kRichDistance,     // distance between ring center and track (ANN input)
    kRichRadialPos,    // radial psoition = sqrt(x**2+abs(y-110)**2), (ANN input)
    kRichRadialAngle,  // radial angle (0||1||2)*pi +- atan( abs((+-100-y)/-x) ), (ANN input)
    kRichPhi,          // phi rotation angle of ellipse (ANN input)
    // tof track information
    kTofHits,  // number of TOF hits
    // much track information
    kMuchHits,       // number of MUCH hits
    kMuchHitsPixel,  // number of MUCH pixel hits
    kMuchChi2NDF,    // chi2/ndf MUCH
    // technical variables
    kRndmTrack,  // randomly created number (used to apply special selection cuts)
    kPRes,       // momentum resolution
    kTrackMax,

    // Pair specific variables
    kChi2NDF = kTrackMax,  // Chi^2/NDF
    kDecayLength,          // decay length p*t (cm)
    kR,                    // xy-distance to origin (cm)
    kOpeningAngle,         // opening angle
    kCosPointingAngle,     // cosine of the pointing angle
    kArmAlpha,             // Armenteros-Podolanski alpha
    kArmPt,                // Armenteros-Podolanski pt
    // helicity picture: Z-axis is considered the direction of the mother's 3-momentum vector
    kThetaHE,      // theta in mother's rest frame in the helicity picture
    kPhiHE,        // phi in mother's rest frame in the helicity picture
    kThetaSqHE,    // squared value of kThetaHE
    kCos2PhiHE,    // Cosine of 2*phi in mother's rest frame in the helicity picture
    kCosTilPhiHE,  // Shifted phi depending on kThetaHE
    // Collins-Soper picture: Z-axis is considered the direction of the vectorial difference between
    // the 3-mom vectors of target and projectile beams
    kThetaCS,                   // theta in mother's rest frame in Collins-Soper picture
    kPhiCS,                     // phi in mother's rest frame in Collins-Soper picture
    kThetaSqCS,                 // squared value of kThetaCS
    kCos2PhiCS,                 // Cosine of 2*phi in mother's rest frame in the Collins-Soper picture
    kCosTilPhiCS,               // Shifted phi depending on kThetaCS
    kPsiPair,                   // phi in mother's rest frame in Collins-Soper picture
    kStsMvdFirstDaughter,       // number of STS and MVD hits of the first daughter particle
    kStsMvdSecondDaughter,      // number of STS and MVD hits
    kStsMvdTrdFirstDaughter,    // number of STS and MVD hitsof the first daughter particle
    kStsMvdTrdSecondDaughter,   // number of STS and MVD hits
    kStsMvdRichFirstDaughter,   // number of STS and MVD hitsof the first daughter particle
    kStsMvdRichSecondDaughter,  // number of STS and MVD hits
    kStsFirstDaughter,          // number of STS hits of the first daughter particle
    kStsSecondDaughter,         // number of STS hits of the second daughter particle
    kMvdFirstDaughter,          // number of MVD hits of the first daughter particle
    kMvdSecondDaughter,         // number of MVD hits of the second daughter particle
    kTrdFirstDaughter,          // number of TRD hits of the first daughter particle
    kTrdSecondDaughter,         // number of TRD hits of the second daughter particle
    kRichFirstDaughter,         // number of RICH hits of the first daughter particle
    kRichSecondDaughter,        // number of RICH hits of the second daughter particle
    kStsHitDist,                // distance to the closest hit in the first sts station
    kMvdHitDist,                // distance to the closest hit in the first mvd station
    kPhivPair,  // angle between ee plane and the magnetic field (can be useful for conversion rejection)

    kLegDist,           // distance of the legs
    kLegDistXY,         // distance of the legs in XY
    kDeltaEta,          // Absolute value of Delta Eta for the legs
    kDeltaPhi,          // Absolute value of Delta Phi for the legs
    kLegsP,             // sqrt of p_leg1*p_leg2
    kMerr,              // error of mass calculation
    kDCA,               // distance of closest approach TODO: not implemented yet
    kPairType,          // type of the pair, like like sign ++ unlikesign ...
    kMomAsymDau1,       // momentum fraction of daughter1
    kMomAsymDau2,       // momentum fraction of daughter2
    kPairEff,           // pair efficiency
    kOneOverPairEff,    // 1 / pair efficiency (correction factor)
    kOneOverPairEffSq,  // 1 / pair efficiency squared (correction factor)
    kRndmPair,          // radomly created number (used to apply special signal reduction cuts)
    kPairs,             // number of Ev1PM pair candidates after all cuts
    kPairMax,           //

    // Event specific variables
    kXvPrim = kPairMax,  /// prim vertex [cm]
    kYvPrim,             /// prim vertex [cm]
    kZvPrim,             /// prim vertex [cm]
    kVtxChi,             /// chi2
    kVtxNDF,             /// nof degrees of freedom
    kXRes,               // primary vertex x-resolution
    kYRes,               // primary vertex y-resolution
    kZRes,               // primary vertex z-resolution
    kMaxPt,              // track with maximum pt

    kRndmRej,      // random rejection probability by the pair pre filter
    kNTrk,         // number of tracks
    kTracks,       // track after all cuts
    kNVtxContrib,  /// number of primary vertex contibutors

    kCentrality,    // event centrality fraction
    kNevents,       // event counter
    kEvStartTime,   // reconstructed event start time
    kRunNumber,     // run number
    kYbeam,         // beam rapdity
    kMixingBin,     // event mixing pool number
    kTotalTRDHits,  // size of trd hit array
    kNMaxValues,    //

    // MC information
    // Hit specific variables
    kPosXMC = kNMaxValues,  // X position [cm]
    kPosYMC,                // Y position [cm]
    kPosZMC,                // Z position [cm]
    kElossMC,               // energy loss dEdx+TR
    kHitMaxMC,
    // Particle specific MC variables
    kPxMC = kHitMaxMC,    // px
    kPyMC,                // py
    kPzMC,                // pz
    kPtMC,                // transverse momentum
    kPtSqMC,              // transverse momentum squared
    kPMC,                 // momentum
    kXvMC,                // vertex position in x
    kYvMC,                // vertex position in y
    kRvMC,                // vertex position as (x^2 + y^2)
    kZvMC,                // vertex position in z
    kPhivMC,              // vertex position in phi
    kThetavMC,            // vertex position in theta
    kOneOverPtMC,         // 1/pt
    kPhiMC,               // phi angle
    kThetaMC,             // theta angle
    kEtaMC,               // pseudo-rapidity
    kYMC,                 // rapidity
    kYlabMC,              // rapidity lab
    kBetaGammaMC,         // beta gamma
    kEMC,                 // energy
    kEMotherMC,           // energy of the mother
    kMMC,                 // mass
    kChargeMC,            // charge
    kPdgCode,             // PDG code
    kPdgCodeMother,       // PDG code of the mother
    kPdgCodeGrandMother,  // PDG code of the grand mother
    kGeantId,             // geant process id (see TMCProcess)
    kWeight,              // weight NxBR
    kParticleMaxMC,

    // Track specific MC variables
    kTrdHitsMC = kParticleMaxMC,  // number of TRD hits
    kMvdHitsMC,                   // number of MVD hits
    kStsHitsMC,                   // number of STS hits
    kStsMvdHitsMC,                // number of STS hits
    kTofHitsMC,                   // number of TOF hits
    kMuchHitsMC,                  // number of MUCH hits
    kRichHitsMC,                  // number of RICH hits
    kTrdMCTracks,                 // number of TRD MC Points in reconstructed track
    kRichMCPoints,                // number of TRD MC Points in reconstructed track
    kTrdTrueHits,                 // number of true TRD hits in reconstructed track
    kTrdDistHits,                 // number of distorted TRD hits in reconstructed track
    kTrdFakeHits,                 // number of fake TRD hits in reconstructed track
    kTrdDistortion,               // level of distortion of reconstructed track [0,1]
    kStsTrueHits,                 // number of true STS hits in reconstructed track
    kStsDistHits,                 // number of distorted STS hits in reconstructed track
    kStsFakeHits,                 // number of fake STS hits in reconstructed track
    kTrdisMC,                     // status bit for matching btw. glbl. and local MC track
    kMvdisMC,                     // status bit for matching btw. glbl. and local MC track
    kStsisMC,                     // status bit for matching btw. glbl. and local MC track
    kMuchisMC,                    // status bit for matching btw. glbl. and local MC track
    kRichisMC,                    // status bit for matching btw. glbl. and local MC track
    kTofisMC,                     // status bit for matching btw. glbl. and local MC track
    kTrackMaxMC,

    // Pair specific MC variables
    kOpeningAngleMC = kTrackMaxMC,  // opening angle
    kCosPointingAngleMC,            // cosine of the pointing angle
    kStsMvdFirstDaughterMC,         // number of STS and MVD hits
    kStsMvdSecondDaughterMC,        // number of STS and MVD hits
    //    kPhivPairMC,                // angle between d1d2 plane and the magnetic field
    kPairMaxMC,

    // Event specific MCvariables
    kNTrkMC = kPairMaxMC,  // number of MC tracks
    kXvPrimMC,             /// MC vertex [cm]
    kYvPrimMC,             /// MC vertex [cm]
    kZvPrimMC,             /// MC vertex [cm]
    kStsMatches,           // number of matched STS tracks
    kTrdMatches,           // number of matched TRD tracks
    kVageMatches,          // number of MC tracks (STS) matched to multiple reconstr. track
    kTotalTRDHitsMC,       // size of trd MC point array
    kImpactParam,          // impact parameter from MC header
    kNPrimMC,              // primary particles from MC header
    kNMaxValuesMC
  };


  PairAnalysisVarManager();
  PairAnalysisVarManager(const char* name, const char* title);
  virtual ~PairAnalysisVarManager();

  static void InitFormulas();
  static void InitFitter();

  static void Fill(const TObject* particle, Double_t* const values);
  static void FillVarMCParticle(const CbmMCTrack* p1, const CbmMCTrack* p2, Double_t* const values);
  static void FillSum(const TObject* particle, Double_t* const values);

  static void CalculateHitTypes(const PairAnalysisTrack* track, ECbmModuleId idet, Int_t* trueH, Int_t* distH,
                                Int_t* fakeH);

  // Setter
  static void SetFillMap(TBits* map) { fgFillMap = map; }
  static void SetEvent(PairAnalysisEvent* const ev);
  static void SetEventData(const Double_t data[PairAnalysisVarManager::kNMaxValuesMC]);
  static void SetValue(ValueTypes var, Double_t val) { fgData[var] = val; }

  // Getter
  static PairAnalysisEvent* GetCurrentEvent() { return fgEvent; }
  static const CbmKFVertex* GetKFVertex() { return fgKFVertex; }
  static const char* GetValueName(Int_t i) { return (i >= 0 && i < kNMaxValuesMC) ? fgkParticleNames[i][0] : ""; }
  static const char* GetValueLabel(Int_t i) { return (i >= 0 && i < kNMaxValuesMC) ? fgkParticleNames[i][1] : ""; }
  static const char* GetValueUnit(Int_t i) { return (i >= 0 && i < kNMaxValuesMC) ? fgkParticleNames[i][2] : ""; }
  static Double_t* GetData() { return fgData; }
  static Double_t GetValue(ValueTypes val) { return fgData[val]; }
  static UInt_t GetValueType(const char* valname);
  static UInt_t GetValueTypeMC(UInt_t var);

  static UInt_t* GetArray(ValueTypes var0, ValueTypes var1 = kNMaxValuesMC, ValueTypes var2 = kNMaxValuesMC,
                          ValueTypes var3 = kNMaxValuesMC, ValueTypes var4 = kNMaxValuesMC,
                          ValueTypes var5 = kNMaxValuesMC, ValueTypes var6 = kNMaxValuesMC,
                          ValueTypes var7 = kNMaxValuesMC, ValueTypes var8 = kNMaxValuesMC,
                          ValueTypes var9 = kNMaxValuesMC);

  // data member
  static TFormula* fgFormula[kNMaxValuesMC];  // variable formulas

private:
  // data member
  static Double_t fgData[kNMaxValuesMC];                  //! data
  static const char* fgkParticleNames[kNMaxValuesMC][3];  // variable names
  //static const char* fgkParticleNamesMC[kNMaxValuesMC]; // MC variable names
  static PairAnalysisEvent* fgEvent;  // current event pointer
  static CbmKFVertex* fgKFVertex;     // kf vertex                   @TODO: OBSOLETE/UNUSED?
  static CbmVertex* fgVertexMC;       // MC vertex
  //  static CbmStsKFTrackFitter*fgKFFitter;                   // kf fitter
  //  static CbmL1PFFitter      *fgL1Fitter;                   // L1 fitter
  static TBits* fgFillMap;    // map for filling variables
  static Int_t fgCurrentRun;  // current run number

  // fill functions
  static Bool_t Req(ValueTypes var) { return (fgFillMap ? fgFillMap->TestBitNumber(var) : kTRUE); }

  static void FillVarConstants(Double_t* const values);
  static void FillVarPairAnalysisEvent(const PairAnalysisEvent* event, Double_t* const values);
  static void FillVarVertex(const CbmVertex* vertex, Double_t* const values);
  static void FillVarPairAnalysisTrack(const PairAnalysisTrack* track, Double_t* const values);
  static void FillVarGlobalTrack(const CbmGlobalTrack* track, Double_t* const values);
  static void FillVarStsTrack(const CbmStsTrack* track, Double_t* const values);
  static void FillVarMuchTrack(const CbmMuchTrack* track, Double_t* const values);
  static void FillVarTrdTrack(const CbmTrdTrack* track, Double_t* const values);
  static void FillVarRichRing(const CbmRichRing* track, Double_t* const values);
  static void FillVarMCTrack(const CbmMCTrack* particle, Double_t* const values);
  static void FillVarPairAnalysisPair(const PairAnalysisPair* pair, Double_t* const values);
  static void FillVarMvdHit(const CbmMvdHit* hit, Double_t* const values);
  static void FillVarStsHit(const CbmStsHit* hit, Double_t* const values);
  static void FillVarMuchHit(const CbmMuchPixelHit* hit, Double_t* const values);
  static void FillVarTrdHit(const CbmTrdHit* hit, Double_t* const values);
  static void FillVarRichHit(const CbmRichHit* hit, Double_t* const values);
  static void FillVarTofHit(const CbmTofHit* hit, Double_t* const values);
  static void FillVarPixelHit(const CbmPixelHit* hit, Double_t* const values);
  static void FillVarTrdCluster(const CbmTrdCluster* cluster, Double_t* const values);
  static void FillVarMCPoint(const FairMCPoint* hit, Double_t* const values);
  static void FillSumVarMCPoint(const FairMCPoint* hit, Double_t* const values);
  static void FillVarMCHeader(const FairMCEventHeader* header, Double_t* const values);

  //  static Double_t GetChiToVertex(       const CbmStsTrack *track, CbmVertex *vertex);

  //  static void FillVarKFParticle(const AliKFParticle *pair,   Double_t * const values);

  // setter
  static void ResetArrayData(Int_t to, Double_t* const values);
  static void ResetArrayDataMC(Int_t to, Double_t* const values);

  PairAnalysisVarManager(const PairAnalysisVarManager& c);
  PairAnalysisVarManager& operator=(const PairAnalysisVarManager& c);

  ClassDef(PairAnalysisVarManager,
           1);  // Variables management for event, pair, track, hit infos (static)
};


//Inline functions
inline void PairAnalysisVarManager::Fill(const TObject* object, Double_t* const values)
{
  //
  // Main function to fill all available variables according to the type
  //

  //Protect
  if (!object) return;

  if (object->IsA() == PairAnalysisEvent::Class())
    FillVarPairAnalysisEvent(static_cast<const PairAnalysisEvent*>(object), values);
  else if (object->IsA() == CbmVertex::Class())
    FillVarVertex(static_cast<const CbmVertex*>(object), values);
  else if (object->IsA() == PairAnalysisTrack::Class())
    FillVarPairAnalysisTrack(static_cast<const PairAnalysisTrack*>(object), values);
  else if (object->IsA() == CbmGlobalTrack::Class())
    FillVarGlobalTrack(static_cast<const CbmGlobalTrack*>(object), values);
  else if (object->IsA() == CbmStsTrack::Class())
    FillVarStsTrack(static_cast<const CbmStsTrack*>(object), values);
  else if (object->IsA() == CbmMuchTrack::Class())
    FillVarMuchTrack(static_cast<const CbmMuchTrack*>(object), values);
  else if (object->IsA() == CbmTrdTrack::Class())
    FillVarTrdTrack(static_cast<const CbmTrdTrack*>(object), values);
  else if (object->IsA() == CbmRichRing::Class())
    FillVarRichRing(static_cast<const CbmRichRing*>(object), values);
  else if (object->IsA() == CbmMCTrack::Class())
    FillVarMCTrack(static_cast<const CbmMCTrack*>(object), values);
  else if (object->InheritsFrom(PairAnalysisPair::Class()))
    FillVarPairAnalysisPair(static_cast<const PairAnalysisPair*>(object), values);
  else if (object->IsA() == CbmMvdHit::Class())
    FillVarMvdHit(static_cast<const CbmMvdHit*>(object), values);
  else if (object->IsA() == CbmStsHit::Class())
    FillVarStsHit(static_cast<const CbmStsHit*>(object), values);
  else if (object->IsA() == CbmMuchPixelHit::Class())
    FillVarMuchHit(static_cast<const CbmMuchPixelHit*>(object), values);
  else if (object->IsA() == CbmTrdHit::Class())
    FillVarTrdHit(static_cast<const CbmTrdHit*>(object), values);
  else if (object->IsA() == CbmRichHit::Class())
    FillVarRichHit(static_cast<const CbmRichHit*>(object), values);
  else if (object->IsA() == CbmTofHit::Class())
    FillVarTofHit(static_cast<const CbmTofHit*>(object), values);
  else if (object->InheritsFrom(FairMCPoint::Class()))
    FillVarMCPoint(static_cast<const FairMCPoint*>(object), values);
  else
    printf("PairAnalysisVarManager::Fill: Type %s is not supported by "
           "PairAnalysisVarManager! \n",
           object->ClassName());
}


inline void PairAnalysisVarManager::FillSum(const TObject* object, Double_t* const values)
{
  //
  // Main function to incremenebt available variables according to the type
  //

  //Protect
  if (!object) return;
  else if (object->InheritsFrom(FairMCPoint::Class()))
    FillSumVarMCPoint(static_cast<const FairMCPoint*>(object), values);
  else
    printf("PairAnalysisVarManager::FillSum: Type %s is not supported by "
           "PairAnalysisVarManager! \n",
           object->ClassName());
}

inline void PairAnalysisVarManager::ResetArrayData(Int_t to, Double_t* const values)
{
  // Protect
  if (to >= kNMaxValues) return;
  // Reset
  for (Int_t i = kConstMax; i < to; ++i) {
    values[i] = 0.;
  }
  // reset values different from zero
  if (to >= kTrackMax && to > kParticleMax) {
    values[kTrdPidANN]  = -999.;
    values[kRichPidANN] = -999.;
  }
  if (to >= kHitMax && to > kConstMax) {
    values[kMassSq]            = -999.;
    values[kBeta]              = -999.;
    values[kTofPidDeltaBetaEL] = -999.;
    values[kTofPidDeltaBetaMU] = -999.;
    values[kTofPidDeltaBetaPI] = -999.;
    values[kTofPidDeltaBetaKA] = -999.;
    values[kTofPidDeltaBetaPR] = -999.;
  }
}


inline void PairAnalysisVarManager::ResetArrayDataMC(Int_t to, Double_t* const values)
{
  // Protect
  if (to >= kNMaxValuesMC) return;
  // Reset
  for (Int_t i = kNMaxValues; i < to; ++i)
    values[i] = 0.;
  // reset values different from zero
  //  /*
  values[kPdgCode]            = -99999.;
  values[kPdgCodeMother]      = -99999.;
  values[kPdgCodeGrandMother] = -99999.;
  //  */
  //valuesMC[kNumberOfDaughters]  = -999.;
  if (to >= kHitMaxMC && to > kNMaxValues) {
    values[kPosXMC]  = -999.;
    values[kPosYMC]  = -999.;
    values[kPosZMC]  = -999.;
    values[kElossMC] = -999.;
  }
}

inline void PairAnalysisVarManager::FillVarPairAnalysisEvent(const PairAnalysisEvent* event, Double_t* const values)
{
  //
  // Fill event information available into an array
  //

  // Reset array
  ResetArrayData(kNMaxValues, values);
  ResetArrayDataMC(kNMaxValuesMC, values);

  // Protect
  if (!event) return;

  // Set
  values[kNTrk]          = event->GetNumberOfTracks();
  values[kStsMatches]    = event->GetNumberOfMatches(ECbmModuleId::kSts);
  values[kTrdMatches]    = event->GetNumberOfMatches(ECbmModuleId::kTrd);
  values[kVageMatches]   = event->GetNumberOfVageMatches();
  values[kTotalTRDHits]  = event->GetNumberOfHits(ECbmModuleId::kTrd);
  const Double_t proMass = TDatabasePDG::Instance()->GetParticle(2212)->Mass();
  Double_t beta  = TMath::Sqrt(values[kEbeam] * values[kEbeam] - proMass * proMass) / (values[kEbeam] + proMass);
  values[kYbeam] = TMath::ATanH(beta);
  //  Printf("beam rapidity new: %f",values[kYbeam]);
  values[kNTrkMC]         = event->GetNumberOfMCTracks();
  values[kTotalTRDHitsMC] = event->GetNumberOfPoints(ECbmModuleId::kTrd);


  // Set vertex
  FillVarVertex(event->GetPrimaryVertex(), values);

  // Set header information
  FillVarMCHeader(event->GetMCHeader(), values);
  values[kEvStartTime]=event->GetEvStartTime();
  std::cout<< "Event Time: " << values[kEvStartTime] <<std::endl;

}

inline void PairAnalysisVarManager::FillVarMCHeader(const FairMCEventHeader* header, Double_t* const values)
{
  //
  // Fill MCheader information available into an array
  //

  // Protect
  if (!header) return;

  // Reset
  // ResetArrayData(kNMaxValues, values);
  if (fgVertexMC) fgVertexMC->Reset();

  // Set
  //  values[k]  = header->GetPhi(); // event plane angle [rad]

  // accessors via first FairMCEventHeader
  values[kXvPrimMC]    = header->GetX();
  values[kYvPrimMC]    = header->GetY();
  values[kZvPrimMC]    = header->GetZ();
  values[kImpactParam] = header->GetB();  // [fm]
  values[kNPrimMC]     = header->GetNPrim();

  // Fill mc vertex data member
  TMatrixFSym mat(3);
  fgVertexMC->SetVertex(values[kXvPrimMC], values[kYvPrimMC], values[kZvPrimMC], -999., 1., values[kNPrimMC], mat);
}

inline void PairAnalysisVarManager::FillVarVertex(const CbmVertex* vertex, Double_t* const values)
{
  //
  // Fill vertex information available into an array
  //

  // Protect
  if (!vertex) return;

  // Reset
  // ResetArrayData(kNMaxValues, values);

  // Set
  values[kXvPrim]      = vertex->GetX();
  values[kYvPrim]      = vertex->GetY();
  values[kZvPrim]      = vertex->GetZ();
  values[kNVtxContrib] = vertex->GetNTracks();
  values[kVtxChi]      = vertex->GetChi2();
  values[kVtxNDF]      = vertex->GetNDF();
}


inline void PairAnalysisVarManager::FillVarPairAnalysisTrack(const PairAnalysisTrack* track, Double_t* const values)
{
  //
  // Fill track information for the all track and its sub tracks
  //

  // Reset
  ResetArrayData(kTrackMax, values);
  ResetArrayDataMC(kTrackMaxMC, values);

  // Protect
  if (!track) return;

  // Set track specific variables
  Fill(track->GetGlobalTrack(), values);
  Fill(track->GetStsTrack(), values);
  Fill(track->GetMuchTrack(), values);
  Fill(track->GetTrdTrack(), values);
  Fill(track->GetRichRing(), values);

  values[kP] = track->P();

  // Calculate first hit position for sts and mvd
  Double_t minSts        = 9999.;
  Int_t compIndex        = 0;
  Double_t xref          = 0.;
  Double_t yref          = 0.;
  Double_t zref          = 0.;
  TClonesArray* hits     = fgEvent->GetHits(ECbmModuleId::kSts);
  TObjArray* PaPaTracks  = fgEvent->GetTracks();
  CbmStsTrack* ststrack1 = (CbmStsTrack*) track->GetStsTrack();
  if (hits /*&& Req(kStsFirstHitPosZ)*/) {
    for (Int_t ihit = 0; ihit < ststrack1->GetNofStsHits(); ihit++) {
      Int_t idx = ststrack1->GetStsHitIndex(ihit);
      if (idx > hits->GetEntriesFast()) continue;
      CbmStsHit* hit = (CbmStsHit*) hits->At(idx);
      if (hit && minSts > hit->GetZ()) {
        minSts = hit->GetZ();
        xref   = hit->GetX();
        yref   = hit->GetY();
        zref   = hit->GetZ();
      }
    }
  }
  Double_t minStsClosest = 9999.;
  if (PaPaTracks && hits) {
    for (Int_t iTracks = 0; iTracks < PaPaTracks->GetEntriesFast(); iTracks++) {
      PairAnalysisTrack* ptrk = (PairAnalysisTrack*) PaPaTracks->At(iTracks);
      if (!ptrk) continue;
      CbmStsTrack* ststrack2 = (CbmStsTrack*) ptrk->GetStsTrack();
      for (Int_t ihit = 0; ihit < ststrack2->GetNofStsHits(); ihit++) {
        Int_t idx = ststrack2->GetStsHitIndex(ihit);
        if (idx > hits->GetEntriesFast() || idx < 0) continue;
        CbmStsHit* hit = (CbmStsHit*) hits->At(idx);
        Double_t xdiff = hit->GetX() - xref;
        Double_t ydiff = hit->GetY() - yref;
        Double_t zdiff = hit->GetZ() - zref;
        Double_t dist  = TMath::Sqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
        if (hit && dist < minStsClosest && dist > 0.) {
          minStsClosest = dist;
          compIndex     = iTracks;
        }
      }
    }
    PairAnalysisTrack* ptrk     = (PairAnalysisTrack*) PaPaTracks->At(compIndex);
    PairAnalysisTrack* reftrack = new PairAnalysisTrack(*track);
    if (ptrk && reftrack) {
      PairAnalysisPair* pair = new PairAnalysisPairLV();
      if (pair) {
        if (reftrack->PdgCode() < 1.e8 && ptrk->PdgCode() < 1.e8)
          pair->SetTracks(reftrack, reftrack->PdgCode(), ptrk, ptrk->PdgCode());
        values[kStsHitClosestOpeningAngle] = pair->OpeningAngle();
        values[kStsHitClosestMom]          = TMath::Sqrt(ptrk->P() * reftrack->P());
        delete pair;
      }
      delete reftrack;
    }
  }

  Double_t minMvd        = 9999.;
  xref                   = 0.;
  yref                   = 0.;
  zref                   = 0.;
  hits                   = fgEvent->GetHits(ECbmModuleId::kMvd);
  CbmStsTrack* mvdtrack1 = (CbmStsTrack*) track->GetStsTrack();
  if (hits) {
    for (Int_t ihit = 0; ihit < mvdtrack1->GetNofMvdHits(); ihit++) {
      Int_t idx = mvdtrack1->GetMvdHitIndex(ihit);
      if (idx > hits->GetEntriesFast()) continue;
      CbmMvdHit* hit = (CbmMvdHit*) hits->At(idx);
      if (hit && minMvd > hit->GetZ()) {
        minMvd = hit->GetZ();
        xref   = hit->GetX();
        yref   = hit->GetY();
        zref   = hit->GetZ();
      }
    }
  }
  Double_t minMvdClosest = 9999.;
  if (PaPaTracks && hits) {
    for (Int_t iTracks = 0; iTracks < PaPaTracks->GetEntriesFast(); iTracks++) {
      PairAnalysisTrack* ptrk = (PairAnalysisTrack*) PaPaTracks->At(iTracks);
      if (!ptrk) continue;
      CbmStsTrack* mvdtrack2 = (CbmStsTrack*) ptrk->GetStsTrack();
      for (Int_t ihit = 0; ihit < mvdtrack2->GetNofMvdHits(); ihit++) {
        Int_t idx = mvdtrack2->GetMvdHitIndex(ihit);
        if (idx > hits->GetEntriesFast()) continue;
        CbmMvdHit* hit = (CbmMvdHit*) hits->At(idx);
        Double_t xdiff = hit->GetX() - xref;
        Double_t ydiff = hit->GetY() - yref;
        Double_t zdiff = hit->GetZ() - zref;
        Double_t dist  = TMath::Sqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
        if (hit && dist < minMvdClosest && dist > 0.) {
          minMvdClosest = dist;
          compIndex     = iTracks;
        }
      }
    }
    PairAnalysisTrack* ptrk     = (PairAnalysisTrack*) PaPaTracks->At(compIndex);
    PairAnalysisTrack* reftrack = new PairAnalysisTrack(*track);
    if (ptrk && reftrack) {
      PairAnalysisPair* pair = new PairAnalysisPairLV();
      if (pair) {
        if (reftrack->PdgCode() < 1.e8 && ptrk->PdgCode() < 1.e8)
          pair->SetTracks(reftrack, reftrack->PdgCode(), ptrk, ptrk->PdgCode());
        values[kMvdHitClosestOpeningAngle] = pair->OpeningAngle();
        values[kMvdHitClosestMom]          = TMath::Sqrt(ptrk->P() * reftrack->P());
        delete pair;
      }
      delete reftrack;
    }
  }

  values[kStsHitClosest] = minStsClosest;
  values[kMvdHitClosest] = minMvdClosest;


  values[kRichDistance] = 1.;  //CbmRichUtil::GetRingTrackDistance(track->GetGlobalIndex());
  values[kRichPidANN] =
    CbmRichElectronIdAnn::GetInstance().CalculateAnnValue(track->GetGlobalIndex(), values[kP]);  // fgRichElIdA

  // acceptance defintions
  FairTrackParam* param = NULL;
  if ((param = track->GetRichProj())) {  // RICH
    values[kRichhasProj] = (TMath::Abs(param->GetX() + param->GetY()) > 0.);
  }
  if ((param = track->GetMvdEntrance())) {  // MVD
    values[kMvdFirstExtX] = param->GetX();
    values[kMvdFirstExtY] = param->GetY();
    Double_t innerLimit   = 0.5;  //cm, TODO: no hardcoding
    Double_t outerLimit   = 2.5;  //cm
    values[kMvdhasEntr]   = ((TMath::Abs(param->GetX()) > innerLimit && TMath::Abs(param->GetX()) < outerLimit
                            && TMath::Abs(param->GetY()) < outerLimit)
                           || (TMath::Abs(param->GetY()) > innerLimit && TMath::Abs(param->GetY()) < outerLimit
                               && TMath::Abs(param->GetX()) < outerLimit));
  }

  // mc
  Fill(track->GetMCTrack(), values);  // this contains particle infos as well

  if (track->GetMCTrack()) {
    values[kPRes] = TMath::Abs(values[kP] - track->GetMCTrack()->GetP()) / track->GetMCTrack()->GetP();
  }

  if (track->GetTrackMatch(ECbmModuleId::kTrd)) {  // track match specific (accessors via CbmTrackMatchNew)
    values[kTrdMCTracks] = track->GetTrackMatch(ECbmModuleId::kTrd)->GetNofLinks();  //number of different! mc tracks

    Int_t trueHits = 0, distHits = 0, fakeHits = 0;
    CalculateHitTypes(track, ECbmModuleId::kTrd, &trueHits, &distHits, &fakeHits);

    values[kTrdTrueHits] = trueHits;
    values[kTrdDistHits] = distHits;
    values[kTrdFakeHits] = fakeHits;
    //    values[kTrdDistortion]  = dist/links;
    /* values[kTrdTrueHits]    = tmtch->GetNofTrueHits(); //NOTE: changed defintion */
    /* values[kTrdFakeHits]    = tmtch->GetNofWrongHits(); //NOTE: changed definition */
  }
  if (track->GetTrackMatch(ECbmModuleId::kSts)) {
    Int_t trueHits = 0, distHits = 0, fakeHits = 0;
    CalculateHitTypes(track, ECbmModuleId::kSts, &trueHits, &distHits, &fakeHits);

    values[kStsTrueHits] = trueHits;
    values[kStsDistHits] = distHits;
    values[kStsFakeHits] = fakeHits;
  }
  if (track->GetTrackMatch(ECbmModuleId::kRich)) {
    values[kRichMCPoints] = track->GetTrackMatch(ECbmModuleId::kRich)->GetNofLinks();
  }
  values[kStsisMC]  = track->TestBit(BIT(14 + ToIntegralType(ECbmModuleId::kSts)));
  values[kMuchisMC] = track->TestBit(BIT(14 + ToIntegralType(ECbmModuleId::kMuch)));
  values[kTrdisMC]  = track->TestBit(BIT(14 + ToIntegralType(ECbmModuleId::kTrd)));
  values[kRichisMC] = track->TestBit(BIT(14 + ToIntegralType(ECbmModuleId::kRich)));
  values[kMvdisMC]  = track->TestBit(BIT(14 + ToIntegralType(ECbmModuleId::kMvd)));
  values[kTofisMC]  = track->TestBit(BIT(14 + ToIntegralType(ECbmModuleId::kTof)));
  values[kWeight]   = track->GetWeight();

  // Reset
  ResetArrayData(kParticleMax, values);

  // Set DATA default (refitted sts track to primary vertex)
  values[kPx]   = track->Px();
  values[kPy]   = track->Py();
  values[kPz]   = track->Pz();
  values[kPt]   = track->Pt();
  values[kPtSq] = track->Pt() * track->Pt();
  values[kP]    = track->P();

  values[kXv] = track->Xv();
  values[kYv] = track->Yv();
  values[kZv] = track->Zv();

  values[kOneOverPt] = (track->Pt() > 1.0e-3 ? track->OneOverPt() : 0.0);
  values[kPhi]       = (TMath::IsNaN(track->Phi()) ? -999. : TVector2::Phi_0_2pi(track->Phi()));
  values[kTheta]     = track->Theta();
  //  values[kEta]       = track->Eta();
  values[kY]            = track->Y() - values[kYbeam];
  values[kYlab]         = track->Y();
  values[kE]            = track->E();
  values[kM]            = track->M();
  values[kCharge]       = track->Charge();
  values[kMt]           = TMath::Sqrt(values[kMPair] * values[kMPair] + values[kPtSq]);
  values[kPdgCode]      = track->PdgCode();
  values[kChi2NDFtoVtx] = track->ChiToVertex();
  values[kImpactParXY]  = TMath::Sqrt(TMath::Power(TMath::Abs(values[kXv] - values[kXvPrim]), 2)
                                     + TMath::Power(TMath::Abs(values[kYv] - values[kYvPrim]), 2));
  values[kImpactParZ]   = TMath::Abs(values[kZv] - values[kZvPrim]);

  // special
  ///  printf("track length %f \n",values[kTrackLength]);
  //  values[kTrackLength] = track->GetGlobalTrack()->GetLength(); // cm
  values[kInclAngle] = TMath::ASin(track->Pt() / track->P());
  Fill(track->GetTofHit(), values);
  values[kTofHits]   = (track->GetTofHit() ? 1. : 0.);
  values[kRndmTrack] = gRandom->Rndm();
}

inline void PairAnalysisVarManager::FillVarGlobalTrack(const CbmGlobalTrack* track, Double_t* const values)
{
  //
  // Fill track information for the global track into array
  //

  // Protect
  if (!track) return;

  // Set
  values[kTrackChi2NDF] = (track->GetNDF() > 0. ? track->GetChi2() / track->GetNDF() : -999.);
  values[kTrackLength]  = track->GetLength();  // cm
  // accessors via first FairTrackParam
  TVector3 mom;
  track->GetParamFirst()->Momentum(mom);
  values[kPin]  = mom.Mag();
  values[kPtin] = mom.Pt();
  track->GetParamLast()->Momentum(mom);
  values[kPout]   = mom.Mag();
  values[kPtout]  = mom.Pt();
  values[kCharge] = (track->GetParamFirst()->GetQp() > 0. ? +1. : -1.);
}

inline void PairAnalysisVarManager::FillVarRichRing(const CbmRichRing* track, Double_t* const values)
{
  //
  // Fill track information for the trd track into array
  //

  // Protect
  if (!track) return;

  // Set
  //Do Select is no longer supported use CbmRichElectronIdAnn::GetInstance().CalculateAnnValue(globalTrackIndex, momentum);
  //  values[kRichPidANN]      = -1; // fgRichElIdAnn->DoSelect(const_cast<CbmRichRing*>(track), values[kP]); // PID value ANN method
  values[kRichHitsOnRing] = track->GetNofHitsOnRing();
  values[kRichHits]       = track->GetNofHits();
  values[kRichChi2NDF]    = (track->GetNDF() > 0. ? track->GetChi2() / track->GetNDF() : -999.);
  values[kRichRadius]     = track->GetRadius();
  values[kRichAxisA]      = track->GetAaxis();
  values[kRichAxisB]      = track->GetBaxis();
  values[kRichCenterX]    = track->GetCenterX();
  values[kRichCenterY]    = track->GetCenterY();
  // CbmRichRing::GetDistance() method is no longer supported
  // If you wan to use cuts update code using CbmRichUtil::GetRingTrackDistance()
  //  values[kRichDistance]    = 1.;
  values[kRichRadialPos]   = track->GetRadialPosition();
  values[kRichRadialAngle] = track->GetRadialAngle();
  values[kRichPhi]         = track->GetPhi();
}

inline void PairAnalysisVarManager::FillVarTrdTrack(const CbmTrdTrack* track, Double_t* const values)
{
  //
  // Fill track information for the trd track into array
  //

  // Protect
  if (!track) return;

  // Calculate eloss
  TClonesArray* hits = fgEvent->GetHits(ECbmModuleId::kTrd);
  if (hits && track->GetELoss() < 1.e-8 /*&& Req(kTrdSignal)*/) {
    Double_t eloss = 0;
    for (Int_t ihit = 0; ihit < track->GetNofHits(); ihit++) {
      Int_t idx      = track->GetHitIndex(ihit);
      CbmTrdHit* hit = (CbmTrdHit*) hits->At(idx);
      if (hit) {
        eloss += hit->GetELoss();  // dEdx + TR
      }
    }
    //   printf("track %p \t eloss %.3e \n",track,eloss);
    const_cast<CbmTrdTrack*>(track)->SetELoss(eloss);  // NOTE: this is the sum
  }

  // Set
  values[kTrdSignal] = track->GetELoss() * 1.e+6;  //GeV->keV, NOTE: see corrections,normalisation below (angles,#hits)
  values[kTrdPidWkn] = track->GetPidWkn();         // PID value Wkn method
  values[kTrdPidANN] = track->GetPidANN();         // PID value ANN method
  // PID value Likelihood method
  values[kTrdPidLikeEL] = track->GetPidLikeEL();
  values[kTrdPidLikePI] = track->GetPidLikePI();
  values[kTrdPidLikeKA] = track->GetPidLikeKA();
  values[kTrdPidLikePR] = track->GetPidLikePR();
  values[kTrdPidLikeMU] = track->GetPidLikeMU();
  // accessors via CbmTrack
  values[kTrdHits]    = track->GetNofHits();
  values[kTrdChi2NDF] = (track->GetNDF() > 0. ? track->GetChiSq() / track->GetNDF() : -999.);
  // accessors via first FairTrackParam
  TVector3 mom;
  track->GetParamFirst()->Momentum(mom);
  values[kTrdPin]     = mom.Mag();
  values[kTrdPtin]    = mom.Pt();
  values[kTrdThetain] = mom.Theta();
  values[kTrdPhiin]   = mom.Phi();
  // correction factors
  values[kTrdThetaCorr] = 1. / mom.CosTheta();
  values[kTrdPhiCorr]   = 1. / TMath::Cos(mom.Phi());
  // apply correction and normalisation
  values[kTrdSignal] /= values[kTrdHits];  // * values[kTrdThetaCorr] * values[kTrdPhiCorr]);

  track->GetParamLast()->Momentum(mom);
  values[kTrdPout]  = mom.Mag();
  values[kTrdPtout] = mom.Pt();
  //  values[kTrdCharge]      = (track->GetParamFirst()->GetQp()>0. ? +1. : -1. );
  /* TVector3 pos1; */
  /* track->GetParamFirst()->Position(pos1); */
  /* TVector3 pos2; */
  /* track->GetParamLast()->Position(pos2); */
  //  values[kTrdTrackLength] =  (pos2!=pos1 ? (pos2-=pos1).Mag() : 1.);
}

inline void PairAnalysisVarManager::FillVarStsTrack(const CbmStsTrack* track, Double_t* const values)
{
  //
  // Fill track information for the Sts track into array
  //

  // Protect
  if (!track) return;

  // Calculate first hit position for sts and mvd
  Double_t minSts    = 9999.;
  TClonesArray* hits = fgEvent->GetHits(ECbmModuleId::kSts);
  if (hits /*&& Req(kStsFirstHitPosZ)*/) {
    for (Int_t ihit = 0; ihit < track->GetNofStsHits(); ihit++) {
      Int_t idx      = track->GetStsHitIndex(ihit);
      CbmStsHit* hit = (CbmStsHit*) hits->At(idx);
      if (hit && minSts > hit->GetZ()) {
        minSts = hit->GetZ();
        //	Printf("hit %d idx %d position %.5f",ihit,idx,min);
      }
    }
  }
  Double_t minMvd = 9999.;
  hits            = fgEvent->GetHits(ECbmModuleId::kMvd);
  if (hits) {
    for (Int_t ihit = 0; ihit < track->GetNofMvdHits(); ihit++) {
      Int_t idx      = track->GetMvdHitIndex(ihit);
      CbmMvdHit* hit = (CbmMvdHit*) hits->At(idx);
      if (hit && minMvd > hit->GetZ()) { minMvd = hit->GetZ(); }
    }
  }

  // Set
  values[kMvdHits] = track->GetNofMvdHits();
  //  values[kImpactParZ]     = track->GetB();  //Impact parameter of track at target z, in units of its error
  //  printf(" mom %f   impactparz %f \n",values[kPout],values[kImpactParZ]);
  // accessors via CbmTrack
  values[kStsHits]    = track->GetNofStsHits();
  values[kStsMvdHits] = track->GetNofStsHits() + track->GetNofMvdHits();
  values[kStsChi2NDF] = (track->GetNDF() > 0. ? track->GetChiSq() / track->GetNDF() : -999.);
  // accessors via first FairTrackParam
  TVector3 mom;
  track->GetParamFirst()->Momentum(mom);
  values[kStsPin]  = mom.Mag();
  values[kStsPtin] = mom.Pt();
  track->GetParamFirst()->Position(mom);
  values[kStsXv] = mom.X();
  values[kStsYv] = mom.Y();
  values[kStsZv] = mom.Z();
  track->GetParamLast()->Momentum(mom);
  values[kStsPout]  = mom.Mag();
  values[kStsPtout] = mom.Pt();
  //  values[kStsCharge]      = (track->GetParamFirst()->GetQp()>0. ? +1. : -1. );

  values[kMvdFirstHitPosZ] = minMvd;
  values[kStsFirstHitPosZ] = minSts;
}

inline void PairAnalysisVarManager::FillVarMuchTrack(const CbmMuchTrack* track, Double_t* const values)
{
  //
  // Fill track information for the Much track into array
  //

  // Protect
  if (!track) return;

  // Calculate straw, (TODO:trigger) and pixel hits
  values[kMuchHitsPixel] = 0.;
  for (Int_t ihit = 0; ihit < track->GetNofHits(); ihit++) {
    //    Int_t idx = track->GetHitIndex(ihit);
    Int_t hitType = track->GetHitType(ihit);
    if (hitType == ToIntegralType(ECbmDataType::kMuchPixelHit)) values[kMuchHitsPixel]++;
  }

  // Set
  // accessors via CbmTrack
  values[kMuchHits]    = track->GetNofHits();
  values[kMuchChi2NDF] = (track->GetNDF() > 0. ? track->GetChiSq() / track->GetNDF() : -999.);
}

inline void PairAnalysisVarManager::FillVarMCParticle(const CbmMCTrack* p1, const CbmMCTrack* p2,
                                                      Double_t* const values)
{
  //
  // fill 2 track information starting from MC legs
  //

  // Reset
  ResetArrayDataMC(kPairMaxMC, values);

  // Protect
  if (!p1 || !p2) return;

  // Get the MC interface if available
  PairAnalysisMC* mc = PairAnalysisMC::Instance();
  if (!mc->HasMC()) return;

  // Set
  CbmMCTrack* mother = 0x0;
  //  CbmMCTrack* smother = 0x0;
  Int_t mLabel1 = p1->GetMotherId();
  Int_t mLabel2 = p2->GetMotherId();
  if (mLabel1 == mLabel2) mother = mc->GetMCTrackFromMCEvent(mLabel1);
  /* mother  = mc->GetMCTrackFromMCEvent(mLabel1); */
  /* smother = mc->GetMCTrackFromMCEvent(mLabel2); */

  PairAnalysisPair* pair = new PairAnalysisPairLV();
  pair->SetMCTracks(p1, p2);

  if (mother) {
    FillVarMCTrack(mother, values);
    values[kStsMvdFirstDaughterMC]  = p1->GetNPoints(ECbmModuleId::kSts) + p1->GetNPoints(ECbmModuleId::kMvd);
    values[kStsMvdSecondDaughterMC] = p2->GetNPoints(ECbmModuleId::kSts) + p2->GetNPoints(ECbmModuleId::kMvd);
    //    FillVarMCTrack(smother, values);
  }
  else {
    values[kPxMC]   = pair->Px();
    values[kPyMC]   = pair->Py();
    values[kPzMC]   = pair->Pz();
    values[kPtMC]   = pair->Pt();
    values[kPtSqMC] = pair->Pt() * pair->Pt();
    values[kPMC]    = pair->P();

    values[kXvMC] = pair->Xv();
    values[kYvMC] = pair->Yv();
    if (mother)
      if (mother->GetPdgCode() == 22)
        std::cout << pair->Xv() << "   " << pair->Xv() * pair->Xv() << "   "
                  << TMath::Sqrt(pair->Xv() * pair->Xv() + pair->Yv() * pair->Yv()) << "   " << pair->Zv() << std::endl;
    values[kRvMC] = TMath::Sqrt(pair->Xv() * pair->Xv() + pair->Yv() * pair->Yv());
    values[kZvMC] = pair->Zv();
    //TODO  values[kTMC]         = 0.;

    values[kOneOverPtMC] = (pair->Pt() > 1.0e-3 ? pair->OneOverPt() : 0.0);
    values[kPhiMC]       = (TMath::IsNaN(pair->Phi()) ? -999. : TVector2::Phi_0_2pi(pair->Phi()));
    values[kThetaMC]     = pair->Theta();
    //    values[kEtaMC]       = pair->Eta();
    values[kYMC]      = pair->Y() - values[kYbeam];
    values[kYlabMC]   = pair->Y();
    values[kEMC]      = pair->E();
    values[kMMC]      = pair->M();
    values[kChargeMC] = p1->GetCharge() * p2->GetCharge();
  }
  values[kOpeningAngleMC]     = pair->OpeningAngle();
  values[kCosPointingAngleMC] = fgVertexMC ? pair->GetCosPointingAngle(fgVertexMC) : -1;
  //  values[kPhivPairMC]     = pair->PhivPair(1.);


  // delete the surplus pair
  delete pair;
}

inline void PairAnalysisVarManager::FillVarMCTrack(const CbmMCTrack* particle, Double_t* const values)
{
  //
  // fill particle information from MC leg
  //

  // Reset
  ResetArrayDataMC(kTrackMaxMC, values);

  // Protect
  if (!particle) return;

  // Get the MC interface if available
  PairAnalysisMC* mc = PairAnalysisMC::Instance();
  if (!mc->HasMC()) return;

  // Set
  CbmMCTrack* mother = 0x0;
  Int_t mLabel1      = particle->GetMotherId();
  mother             = mc->GetMCTrackFromMCEvent(mLabel1);

  values[kPdgCode]       = particle->GetPdgCode();
  values[kPdgCodeMother] = (mother ? mother->GetPdgCode() : -99999.);

  if (mother) {
    //    if(particle->GetPdgCode() == 11 || particle->GetPdgCode() == -11)  std::cout<<  particle->GetPdgCode()  <<"    " <<  mother->GetPdgCode() <<"   "<< particle->GetGeantProcessId() <<std::endl;
    if (mother->GetPdgCode() == 22) values[kPdgCodeMother] = 1;
    if (mother->GetPdgCode() == 111) values[kPdgCodeMother] = 3;
    if (mother->GetPdgCode() == 211 || mother->GetPdgCode() == -211) values[kPdgCodeMother] = 5;
    if (mother->GetPdgCode() == 2212) values[kPdgCodeMother] = 7;
    if (mother->GetPdgCode() == 99009011 || mother->GetPdgCode() == 99009911) values[kPdgCodeMother] = 9;
  }


  values[kEMotherMC] = (mother ? mother->GetEnergy() : -99999.);
  CbmMCTrack* granni = 0x0;
  if (mother) granni = mc->GetMCTrackMother(mother);
  values[kPdgCodeGrandMother] = (granni ? granni->GetPdgCode() : -99999.);  //mc->GetMotherPDG(mother);
  values[kGeantId]            = particle->GetGeantProcessId();

  values[kPxMC]   = particle->GetPx();
  values[kPyMC]   = particle->GetPy();
  values[kPzMC]   = particle->GetPz();
  values[kPtMC]   = particle->GetPt();
  values[kPtSqMC] = particle->GetPt() * particle->GetPt();
  values[kPMC]    = particle->GetP();

  values[kXvMC] = particle->GetStartX();
  values[kYvMC] = particle->GetStartY();
  values[kZvMC] = particle->GetStartZ();
  //  if(mother) if(mother->GetPdgCode() == 22)    std::cout<<particle->GetStartX()<<"   "<<particle->GetStartX()*particle->GetStartX()<<"   "<< TMath::Sqrt( particle->GetStartX()*particle->GetStartX() + particle->GetStartY()*particle->GetStartY() ) <<"   " << particle->GetStartZ()<<std::endl;
  values[kRvMC] =
    TMath::Sqrt(particle->GetStartX() * particle->GetStartX() + particle->GetStartY() * particle->GetStartY());

  TVector3 vtx;
  particle->GetStartVertex(vtx);
  values[kPhivMC]   = vtx.Phi();
  values[kThetavMC] = vtx.Theta();
  //TODO  values[kTMC]         = particle->GetStartT(); [ns]

  TLorentzVector mom;
  if (particle) particle->Get4Momentum(mom);
  values[kOneOverPtMC] = (particle->GetPt() > 1.0e-3 ? 1. / particle->GetPt() : 0.0);
  values[kPhiMC]       = (TMath::IsNaN(mom.Phi()) ? -999. : TVector2::Phi_0_2pi(mom.Phi()));
  values[kThetaMC]     = mom.Theta();
  //  values[kEtaMC]       = mom.Eta();
  values[kYMC] = particle->GetRapidity() - values[kYbeam];
  ;
  values[kYlabMC] = particle->GetRapidity();
  Double_t pom    = particle->GetP() / particle->GetMass();
  Double_t beta   = pom / TMath::Sqrt(pom * pom + 1.);
  //  Double_t gamma = 1./ TMath::Sqrt(1.-pom*pom);
  values[kBetaGammaMC] = 1. / TMath::Sqrt(1. - beta * beta);
  values[kEMC]         = particle->GetEnergy();
  values[kMMC]         = mom.M();  //particle->GetMass();
  values[kChargeMC]    = particle->GetCharge();

  // detector info
  values[kRichHitsMC]   = particle->GetNPoints(ECbmModuleId::kRich);
  values[kTrdHitsMC]    = particle->GetNPoints(ECbmModuleId::kTrd);
  values[kMvdHitsMC]    = particle->GetNPoints(ECbmModuleId::kMvd);
  values[kStsHitsMC]    = particle->GetNPoints(ECbmModuleId::kSts);
  values[kStsMvdHitsMC] = particle->GetNPoints(ECbmModuleId::kSts) + particle->GetNPoints(ECbmModuleId::kMvd);
  values[kTofHitsMC]    = particle->GetNPoints(ECbmModuleId::kTof);
  values[kMuchHitsMC]   = particle->GetNPoints(ECbmModuleId::kMuch);
}

inline void PairAnalysisVarManager::FillVarPairAnalysisPair(const PairAnalysisPair* pair, Double_t* const values)
{
  //
  // Fill pair information available for histogramming into an array
  //

  // Reset
  ResetArrayData(kPairMax, values);
  ResetArrayDataMC(kPairMaxMC, values);

  // Protect
  if (!pair) return;

  // set the beamenergy (needed by some variables)
  pair->SetBeamEnergy(values[kEbeam]);

  // first fill mc info to avoid kWeight beeing reset
  // TODO: check if it makes sence only for pairtypes of SE
  FillVarMCParticle(pair->GetFirstDaughter()->GetMCTrack(), pair->GetSecondDaughter()->GetMCTrack(), values);

  // Set
  values[kPairType] = pair->GetType();

  values[kPx]   = pair->Px();
  values[kPy]   = pair->Py();
  values[kPz]   = pair->Pz();
  values[kPt]   = pair->Pt();
  values[kPtSq] = pair->Pt() * pair->Pt();
  values[kP]    = pair->P();

  values[kXv] = pair->Xv();
  values[kYv] = pair->Yv();
  values[kZv] = pair->Zv();

  values[kOneOverPt] = (pair->Pt() > 1.0e-3 ? pair->OneOverPt() : 0.0);
  values[kPhi]       = (TMath::IsNaN(pair->Phi()) ? -999. : TVector2::Phi_0_2pi(pair->Phi()));
  values[kTheta]     = pair->Theta();
  //  values[kEta]       = pair->Eta();
  values[kY]      = pair->Y() - values[kYbeam];
  values[kYlab]   = pair->Y();
  values[kE]      = pair->E();
  values[kM]      = pair->M();
  values[kCharge] = pair->Charge();
  values[kMt]     = TMath::Sqrt(values[kMPair] * values[kMPair] + values[kPtSq]);

  ///TODO: check
  /* values[kPdgCode]=-1; */
  /* values[kPdgCodeMother]=-1; */
  /* values[kPdgCodeGrandMother]=-1; */
  values[kWeight] = pair->GetWeight();

  if (pair->GetFirstDaughter()->GetMCTrack()) {
    PairAnalysisMC* mc = PairAnalysisMC::Instance();
    if (!mc->HasMC()) return;

    // Set
    CbmMCTrack* mother = 0x0;
    Int_t mLabel1      = pair->GetFirstDaughter()->GetMCTrack()->GetMotherId();
    mother             = mc->GetMCTrackFromMCEvent(mLabel1);
    Int_t motherCode   = (mother ? mother->GetPdgCode() : -99999.);
    Int_t secondMother = -99999.;
    Int_t mLabel2      = -1;
    if (pair->GetSecondDaughter()->GetMCTrack()) {
      CbmMCTrack* sm = 0x0;
      mLabel2        = pair->GetSecondDaughter()->GetMCTrack()->GetMotherId();
      sm             = mc->GetMCTrackFromMCEvent(mLabel2);
      secondMother   = (sm ? sm->GetPdgCode() : -99999.);
    }

    if (values[kThermalScaling] == 3) {
      Double_t mass[170] = {
        0.0195, 0.0395, 0.0595, 0.0795, 0.0995, 0.1195, 0.1395, 0.1595, 0.1795, 0.1995, 0.2195, 0.2395, 0.2595, 0.2795,
        0.2995, 0.3195, 0.3395, 0.3595, 0.3795, 0.3995, 0.4195, 0.4395, 0.4595, 0.4795, 0.4995, 0.5195, 0.5395, 0.5595,
        0.5795, 0.5995, 0.6195, 0.6395, 0.6595, 0.6795, 0.6995, 0.7195, 0.7395, 0.7595, 0.7795, 0.7995, 0.8195, 0.8395,
        0.8595, 0.8795, 0.8995, 0.9195, 0.9395, 0.9595, 0.9795, 0.9995, 1.0195, 1.0395, 1.0595, 1.0795, 1.0995, 1.1195,
        1.1395, 1.1595, 1.1795, 1.1995, 1.2195, 1.2395, 1.2595, 1.2795, 1.2995, 1.3195, 1.3395, 1.3595, 1.3795, 1.3995,
        1.4195, 1.4395, 1.4595, 1.4795, 1.4995, 1.5195, 1.5395, 1.5595, 1.5795, 1.5995, 1.6195, 1.6395, 1.6595, 1.6795,
        1.6995, 1.7195, 1.7395, 1.7595, 1.7795, 1.7995, 1.8195, 1.8395, 1.8595, 1.8795, 1.8995, 1.9195, 1.9395, 1.9595,
        1.9795, 1.9995, 2.0195, 2.0395, 2.0595, 2.0795, 2.0995, 2.1195, 2.1395, 2.1595, 2.1795, 2.1995, 2.2195, 2.2395,
        2.2595, 2.2795, 2.2995, 2.3195, 2.3395, 2.3595, 2.3795, 2.3995, 2.4195, 2.4395, 2.4595, 2.4795, 2.4995, 2.5195,
        2.5395, 2.5595, 2.5795, 2.5995, 2.6195, 2.6395, 2.6595, 2.6795, 2.6995, 2.7195, 2.7395, 2.7595, 2.7795, 2.7995,
        2.8195, 2.8395, 2.8595, 2.8795, 2.8995, 2.9195, 2.9395, 2.9595, 2.9795, 2.9995, 3.0195, 3.0395, 3.0595, 3.0795,
        3.0995, 3.1195, 3.1395, 3.1595, 3.1795, 3.1995, 3.2195, 3.2395, 3.2595, 3.2795, 3.2995, 3.3195, 3.3395, 3.3595,
        3.3795, 3.3995};

      if (motherCode == 99009011 || secondMother == 99009011) {
        //inmed 12AGeV
        Double_t scale[170] = {
          41.706,     18.918,     11.465,     8.4388,     5.9176,     4.9025,     3.8087,     3.0387,     2.5856,
          2.1142,     1.7603,     1.5327,     1.28,       1.1579,     1.0367,     0.89355,    0.81317,    0.71582,
          0.65863,    0.59678,    0.53702,    0.45378,    0.41238,    0.37502,    0.33593,    0.28791,    0.26352,
          0.23939,    0.21167,    0.19479,    0.19204,    0.17492,    0.15811,    0.15479,    0.14935,    0.13803,
          0.1354,     0.11993,    0.1046,     0.08226,    0.073183,   0.055433,   0.043467,   0.033975,   0.028025,
          0.021504,   0.016863,   0.014108,   0.01094,    0.0088095,  0.007324,   0.0057162,  0.0046817,  0.0037459,
          0.0030017,  0.0024459,  0.0020671,  0.0016089,  0.0013754,  0.0011223,  0.00096256, 0.00081647, 0.00072656,
          0.00060776, 0.00051243, 0.00045705, 0.00039636, 0.00036259, 0.00033248, 0.0002953,  0.00027328, 0.00023776,
          0.00022163, 0.00019852, 0.000186,   0.00016846, 0.00015469, 0.00014169, 0.00013343, 0.00011594, 0.00010722,
          0.00010205, 9.1907e-05, 8.3718e-05, 7.5457e-05, 6.7192e-05, 6.2202e-05, 5.7372e-05, 4.8314e-05, 4.5502e-05,
          4.1334e-05, 3.7429e-05, 3.2131e-05, 3.0103e-05, 2.6125e-05, 2.3601e-05, 2.1167e-05, 1.94e-05,   1.7025e-05,
          1.5496e-05, 1.3704e-05, 1.1866e-05, 1.1135e-05, 9.8842e-06, 8.9101e-06, 7.9225e-06, 7.0706e-06, 6.3536e-06,
          5.3786e-06, 4.7179e-06, 4.2128e-06, 4.0015e-06, 3.4118e-06, 3.1864e-06, 2.734e-06,  2.3844e-06, 2.173e-06,
          1.8774e-06, 1.6468e-06, 1.501e-06,  1.3597e-06, 1.2113e-06, 1.0384e-06, 9.4105e-07, 8.4223e-07, 7.434e-07,
          6.5049e-07, 5.8824e-07, 5.3603e-07, 4.6756e-07, 4.1173e-07, 3.5872e-07, 3.2764e-07, 2.9889e-07, 2.5989e-07,
          2.219e-07,  1.9468e-07, 1.816e-07,  1.5707e-07, 1.3565e-07, 1.2619e-07, 1.0919e-07, 1.0071e-07, 8.4632e-08,
          7.6459e-08, 6.829e-08,  6.2046e-08, 5.5335e-08, 4.5937e-08, 4.2426e-08, 3.567e-08,  3.4051e-08, 2.9627e-08,
          2.5249e-08, 2.2767e-08, 2.1054e-08, 1.7873e-08, 1.574e-08,  1.3713e-08, 1.23e-08,   1.1045e-08, 9.5536e-09,
          8.5859e-09, 7.7217e-09, 6.9958e-09, 6.0992e-09, 5.3453e-09, 4.7659e-09, 4.3313e-09, 3.6575e-09};
        TSpline3* weight = new TSpline3("inmedwghts", mass, scale, 170);
        Double_t corrw   = weight->Eval(values[PairAnalysisVarManager::kM]);
        values[kWeight] *= corrw;
        delete weight;
      }
      if (motherCode == 99009111 || secondMother == 99009111) {
        Double_t scale[170] = {
          39.496,     17.961,     11.024,     8.2093,     5.8331,     4.8995,     3.8612,     3.1258,     2.7006,
          2.2465,     1.908,      1.699,      1.4435,     1.3253,     1.2059,     1.049,      0.96753,    0.86685,
          0.81407,    0.75959,    0.70663,    0.61951,    0.58586,    0.55534,    0.51902,    0.46377,    0.4415,
          0.41412,    0.37414,    0.34883,    0.34494,    0.31141,    0.2762,     0.26331,    0.24693,    0.22286,
          0.21697,    0.1972,     0.1841,     0.16097,    0.16352,    0.14345,    0.13096,    0.11911,    0.11399,
          0.10111,    0.0913,     0.08764,    0.077745,   0.071417,   0.067561,   0.05987,    0.055543,   0.050193,
          0.045244,   0.04128,    0.03898,    0.03365,    0.031622,   0.028217,   0.026215,   0.023919,   0.022648,
          0.019915,   0.017524,   0.016145,   0.014357,   0.013362,   0.012368,   0.011036,   0.010198,   0.0088275,
          0.0081762,  0.0072697,  0.00675,    0.0060424,  0.0054788,  0.0049588,  0.0046174,  0.0039685,  0.00363,
          0.0034204,  0.0030534,  0.0027606,  0.0024723,  0.0021893,  0.0020174,  0.0018545,  0.0015584,  0.0014661,
          0.0013315,  0.0012065,  0.0010375,  0.00097456, 0.00084865, 0.00076982, 0.00069371, 0.00063931, 0.00056442,
          0.00051712, 0.00046054, 0.00040174, 0.00037996, 0.00034009, 0.00030921, 0.00027738, 0.00024981, 0.00022659,
          0.00019366, 0.00017153, 0.00015469, 0.00014841, 0.00012783, 0.00012061, 0.00010456, 9.2145e-05, 8.4856e-05,
          7.4087e-05, 6.5675e-05, 6.0496e-05, 5.5386e-05, 4.9865e-05, 4.3202e-05, 3.9571e-05, 3.5821e-05, 3.201e-05,
          2.8322e-05, 2.5886e-05, 2.384e-05,  2.1016e-05, 1.8703e-05, 1.6467e-05, 1.5199e-05, 1.4011e-05, 1.2311e-05,
          1.0621e-05, 9.4155e-06, 8.874e-06,  7.7548e-06, 6.7662e-06, 6.3589e-06, 5.5585e-06, 5.1791e-06, 4.3965e-06,
          4.012e-06,  3.6195e-06, 3.3215e-06, 2.9918e-06, 2.5084e-06, 2.3397e-06, 1.9865e-06, 1.915e-06,  1.6826e-06,
          1.448e-06,  1.3183e-06, 1.231e-06,  1.0551e-06, 9.3811e-07, 8.2511e-07, 7.4714e-07, 6.7735e-07, 5.9142e-07,
          5.3654e-07, 4.8709e-07, 4.4543e-07, 3.9199e-07, 3.4674e-07, 3.1203e-07, 2.862e-07,  2.4391e-07};
        TSpline3* weight = new TSpline3("inmedwghts", mass, scale, 170);
        Double_t corrw   = weight->Eval(values[PairAnalysisVarManager::kM]);
        values[kWeight] *= corrw;
        delete weight;
      }
    }

    //inmed 3.42AGeV
    if (values[kThermalScaling] == 7) {
      if (motherCode == 99009011 || secondMother == 99009011) {
        Double_t mass[125] = {
          0.0195, 0.0395, 0.0595, 0.0795, 0.0995, 0.1195, 0.1395, 0.1595, 0.1795, 0.1995, 0.2195, 0.2395, 0.2595,
          0.2795, 0.2995, 0.3195, 0.3395, 0.3595, 0.3795, 0.3995, 0.4195, 0.4395, 0.4595, 0.4795, 0.4995, 0.5195,
          0.5395, 0.5595, 0.5795, 0.5995, 0.6195, 0.6395, 0.6595, 0.6795, 0.6995, 0.7195, 0.7395, 0.7595, 0.7795,
          0.7995, 0.8195, 0.8395, 0.8595, 0.8795, 0.8995, 0.9195, 0.9395, 0.9595, 0.9795, 0.9995, 1.0195, 1.0395,
          1.0595, 1.0795, 1.0995, 1.1195, 1.1395, 1.1595, 1.1795, 1.1995, 1.2195, 1.2395, 1.2595, 1.2795, 1.2995,
          1.3195, 1.3395, 1.3595, 1.3795, 1.3995, 1.4195, 1.4395, 1.4595, 1.4795, 1.4995, 1.5195, 1.5395, 1.5595,
          1.5795, 1.5995, 1.6195, 1.6395, 1.6595, 1.6795, 1.6995, 1.7195, 1.7395, 1.7595, 1.7795, 1.7995, 1.8195,
          1.8395, 1.8595, 1.8795, 1.8995, 1.9195, 1.9395, 1.9595, 1.9795, 1.9995, 2.0195, 2.0395, 2.0595, 2.0795,
          2.0995, 2.1195, 2.1395, 2.1595, 2.1795, 2.1995, 2.2195, 2.2395, 2.2595, 2.2795, 2.2995, 2.3195, 2.3395,
          2.3595, 2.3795, 2.3995, 2.4195, 2.4395, 2.4595, 2.4795, 2.4995};

        Double_t scale[125] = {
          28.6773,     13.4566,     8.3913,      5.74418,     4.17493,     3.14912,     2.43708,     1.92407,
          1.54338,     1.25305,     1.02766,     0.850101,    0.713646,    0.605398,    0.516448,    0.445862,
          0.385488,    0.333449,    0.288725,    0.24875,     0.213922,    0.183566,    0.157146,    0.134313,
          0.1147,      0.0980171,   0.0839555,   0.0724097,   0.0630874,   0.0554402,   0.0492184,   0.0442134,
          0.0401273,   0.0367131,   0.0336863,   0.0308175,   0.0278289,   0.0244174,   0.0206308,   0.016819,
          0.013354,    0.0104392,   0.00810048,  0.00626932,  0.0048523,   0.00376027,  0.00291833,  0.00226873,
          0.00176674,  0.00137874,  0.001079,    0.000847372, 0.000668582, 0.000530747, 0.000424646, 0.000342751,
          0.000278383, 0.000228662, 0.000190229, 0.000159555, 0.00013539,  0.000115883, 0.000100173, 8.7451e-05,
          7.6779e-05,  6.78659e-05, 6.0253e-05,  5.37112e-05, 4.80505e-05, 4.30558e-05, 3.86565e-05, 3.47273e-05,
          3.11767e-05, 2.79639e-05, 2.50662e-05, 2.24603e-05, 2.01029e-05, 1.79612e-05, 1.60183e-05, 1.42617e-05,
          1.26788e-05, 1.1252e-05,  9.96625e-06, 8.81064e-06, 7.7753e-06,  6.85058e-06, 6.02588e-06, 5.29153e-06,
          4.63923e-06, 4.06136e-06, 3.55071e-06, 3.1002e-06,  2.70338e-06, 2.35454e-06, 2.04847e-06, 1.78043e-06,
          1.54601e-06, 1.34124e-06, 1.16263e-06, 1.00705e-06, 8.71694e-07, 7.54053e-07, 6.51895e-07, 5.63264e-07,
          4.86443e-07, 4.19912e-07, 3.6233e-07,  3.12522e-07, 2.69465e-07, 2.32265e-07, 2.00144e-07, 1.72419e-07,
          1.48498e-07, 1.27867e-07, 1.10079e-07, 9.47489e-08, 8.15401e-08, 7.01617e-08, 6.03625e-08, 5.19253e-08,
          4.46624e-08, 3.84113e-08, 3.3032e-08,  2.84035e-08, 2.44235e-08,
        };
        TSpline3* weight = new TSpline3("inmedwghts", mass, scale, 125);
        Double_t corrw   = weight->Eval(values[PairAnalysisVarManager::kM]);
        values[kWeight] *= corrw;
        delete weight;
      }
    }  //end thermal 7
  }    //end pair->GetMC ...


  if (pair->GetFirstDaughter() && pair->GetFirstDaughter()->GetStsTrack()) {
    values[kStsMvdFirstDaughter] = pair->GetFirstDaughter()->GetStsTrack()->GetNofStsHits()
                                   + pair->GetFirstDaughter()->GetStsTrack()->GetNofMvdHits();
  }
  if (pair->GetSecondDaughter() && pair->GetSecondDaughter()->GetStsTrack())
    values[kStsMvdSecondDaughter] = pair->GetSecondDaughter()->GetStsTrack()->GetNofStsHits()
                                    + pair->GetSecondDaughter()->GetStsTrack()->GetNofMvdHits();


  if (pair->GetFirstDaughter() && pair->GetFirstDaughter()->GetStsTrack()) {
    values[kStsMvdFirstDaughter] = pair->GetFirstDaughter()->GetStsTrack()->GetNofStsHits()
                                   + pair->GetFirstDaughter()->GetStsTrack()->GetNofMvdHits();
    values[kStsFirstDaughter] = pair->GetFirstDaughter()->GetStsTrack()->GetNofStsHits();
    values[kMvdFirstDaughter] = pair->GetFirstDaughter()->GetStsTrack()->GetNofMvdHits();

    if (pair->GetFirstDaughter()->GetTrdTrack()) {
      values[kStsMvdTrdFirstDaughter] = pair->GetFirstDaughter()->GetStsTrack()->GetNofStsHits()
                                        + pair->GetFirstDaughter()->GetStsTrack()->GetNofMvdHits()
                                        + pair->GetFirstDaughter()->GetTrdTrack()->GetNofHits();
      values[kTrdFirstDaughter] = pair->GetFirstDaughter()->GetTrdTrack()->GetNofHits();
    }
    else {
      values[kStsMvdTrdFirstDaughter] = 0;
      values[kTrdFirstDaughter]       = 0;
    }

    if (pair->GetFirstDaughter()->GetRichRing()) {
      values[kStsMvdRichFirstDaughter] = pair->GetFirstDaughter()->GetStsTrack()->GetNofStsHits()
                                         + pair->GetFirstDaughter()->GetStsTrack()->GetNofMvdHits() + 1;
      values[kRichFirstDaughter] = pair->GetFirstDaughter()->GetRichRing()->GetNofHits();
    }
    else {
      values[kStsMvdRichFirstDaughter] = 0;
      values[kRichFirstDaughter]       = 0;
    }
  }
  if (pair->GetSecondDaughter() && pair->GetSecondDaughter()->GetStsTrack()) {
    values[kStsMvdSecondDaughter] = pair->GetSecondDaughter()->GetStsTrack()->GetNofStsHits()
                                    + pair->GetSecondDaughter()->GetStsTrack()->GetNofMvdHits();


    values[kStsSecondDaughter] = pair->GetSecondDaughter()->GetStsTrack()->GetNofStsHits();
    values[kMvdSecondDaughter] = pair->GetSecondDaughter()->GetStsTrack()->GetNofMvdHits();

    if (pair->GetSecondDaughter()->GetTrdTrack()) {
      values[kStsMvdTrdSecondDaughter] = pair->GetSecondDaughter()->GetStsTrack()->GetNofStsHits()
                                         + pair->GetSecondDaughter()->GetStsTrack()->GetNofMvdHits()
                                         + pair->GetSecondDaughter()->GetTrdTrack()->GetNofHits();
      values[kTrdSecondDaughter] = pair->GetSecondDaughter()->GetTrdTrack()->GetNofHits();
    }
    else {
      values[kStsMvdTrdSecondDaughter] = 0;
      values[kTrdSecondDaughter]       = 0;
    }

    if (pair->GetSecondDaughter()->GetRichRing()) {
      values[kStsMvdRichSecondDaughter] = pair->GetSecondDaughter()->GetStsTrack()->GetNofStsHits()
                                          + pair->GetSecondDaughter()->GetStsTrack()->GetNofMvdHits() + 1;
      values[kRichSecondDaughter] = pair->GetSecondDaughter()->GetRichRing()->GetNofHits();
    }
    else {
      values[kStsMvdRichSecondDaughter] = 0;
      values[kRichSecondDaughter]       = 0;
    }
  }


  if (pair->GetFirstDaughter() && pair->GetFirstDaughter()->GetStsTrack() && pair->GetSecondDaughter()
      && pair->GetSecondDaughter()->GetStsTrack()) {

    TClonesArray* hits = fgEvent->GetHits(ECbmModuleId::kSts);
    if (hits && pair->GetFirstDaughter()->GetStsTrack()->GetNofStsHits() > 0
        && pair->GetSecondDaughter()->GetStsTrack()->GetNofStsHits() > 0) {

      CbmStsHit* hitx  = NULL;
      CbmStsHit* hity  = NULL;
      Double_t minStsA = 9999.;
      if (hits) {
        for (Int_t ihit = 0; ihit < pair->GetFirstDaughter()->GetStsTrack()->GetNofStsHits(); ihit++) {
          Int_t idx      = pair->GetFirstDaughter()->GetStsTrack()->GetStsHitIndex(ihit);
          CbmStsHit* hit = (CbmStsHit*) hits->At(idx);
          if (hit && minStsA > hit->GetZ()) {
            hitx    = hit;
            minStsA = hit->GetZ();
          }
        }
      }
      Double_t minStsB = 9999.;
      if (hits) {
        for (Int_t ihit = 0; ihit < pair->GetSecondDaughter()->GetStsTrack()->GetNofStsHits(); ihit++) {
          Int_t idx      = pair->GetSecondDaughter()->GetStsTrack()->GetStsHitIndex(ihit);
          CbmStsHit* hit = (CbmStsHit*) hits->At(idx);
          if (hit && minStsB > hit->GetZ()) {
            hity    = hit;
            minStsB = hit->GetZ();
          }
        }
      }
      if (hitx && hity && minStsA < 9999 && minStsB < 9999) {
        Double_t xdiff      = hitx->GetX() - hity->GetX();
        Double_t ydiff      = hitx->GetY() - hity->GetY();
        Double_t zdiff      = hitx->GetZ() - hity->GetZ();
        Double_t dist       = TMath::Sqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
        values[kStsHitDist] = dist;
      }
    }
    else {
      values[kStsHitDist] = -1;
    }


    hits = fgEvent->GetHits(ECbmModuleId::kMvd);
    if (hits && pair->GetFirstDaughter()->GetStsTrack()->GetNofMvdHits() > 0
        && pair->GetSecondDaughter()->GetStsTrack()->GetNofMvdHits() > 0) {
      CbmMvdHit* hitx  = NULL;
      CbmMvdHit* hity  = NULL;
      Double_t minMvdA = 9999.;
      if (hits) {
        for (Int_t ihit = 0; ihit < pair->GetFirstDaughter()->GetStsTrack()->GetNofMvdHits(); ihit++) {
          Int_t idx      = pair->GetFirstDaughter()->GetStsTrack()->GetMvdHitIndex(ihit);
          CbmMvdHit* hit = (CbmMvdHit*) hits->At(idx);
          if (hit && minMvdA > hit->GetZ()) {
            hitx    = hit;
            minMvdA = hit->GetZ();
          }
        }
      }
      Double_t minMvdB = 9999.;
      if (hits) {
        for (Int_t ihit = 0; ihit < pair->GetSecondDaughter()->GetStsTrack()->GetNofMvdHits(); ihit++) {
          Int_t idx      = pair->GetSecondDaughter()->GetStsTrack()->GetMvdHitIndex(ihit);
          CbmMvdHit* hit = (CbmMvdHit*) hits->At(idx);
          if (hit && minMvdB > hit->GetZ()) {
            hity    = hit;
            minMvdB = hit->GetZ();
          }
        }
      }
      if (hitx && hity && minMvdA < 9999 && minMvdB < 9999) {
        Double_t xdiff      = hitx->GetX() - hity->GetX();
        Double_t ydiff      = hitx->GetY() - hity->GetY();
        Double_t zdiff      = hitx->GetZ() - hity->GetZ();
        Double_t dist       = TMath::Sqrt(xdiff * xdiff + ydiff * ydiff + zdiff * zdiff);
        values[kMvdHitDist] = dist;
      }
    }
    else {
      values[kMvdHitDist] = -1;
    }
  }


  Double_t thetaHE = 0;
  Double_t phiHE   = 0;
  Double_t thetaCS = 0;
  Double_t phiCS   = 0;
  if (Req(kThetaHE) || Req(kPhiHE) || Req(kThetaCS) || Req(kPhiCS)) {
    pair->GetThetaPhiCM(thetaHE, phiHE, thetaCS, phiCS);

    values[kThetaHE]   = thetaHE;
    values[kPhiHE]     = phiHE;
    values[kThetaSqHE] = thetaHE * thetaHE;
    values[kCos2PhiHE] = TMath::Cos(2.0 * phiHE);
    values[kCosTilPhiHE] =
      (thetaHE > 0) ? (TMath::Cos(phiHE - TMath::Pi() / 4.)) : (TMath::Cos(phiHE - 3 * TMath::Pi() / 4.));
    values[kThetaCS]   = thetaCS;
    values[kPhiCS]     = phiCS;
    values[kThetaSqCS] = thetaCS * thetaCS;
    values[kCos2PhiCS] = TMath::Cos(2.0 * phiCS);
    values[kCosTilPhiCS] =
      (thetaCS > 0) ? (TMath::Cos(phiCS - TMath::Pi() / 4.)) : (TMath::Cos(phiCS - 3 * TMath::Pi() / 4.));
  }

  values[kChi2NDF]          = pair->GetChi2() / pair->GetNdf();
  values[kDecayLength]      = pair->GetDecayLength();
  values[kR]                = pair->GetR();
  values[kOpeningAngle]     = pair->OpeningAngle();
  values[kCosPointingAngle] = fgEvent ? pair->GetCosPointingAngle(fgEvent->GetPrimaryVertex()) : -1;

  values[kLegDist]   = pair->DistanceDaughters();
  values[kLegDistXY] = pair->DistanceDaughtersXY();
  //  values[kDeltaEta]     = pair->DeltaEta();
  //  values[kDeltaPhi]     = pair->DeltaPhi();
  values[kLegsP] = TMath::Sqrt(pair->DaughtersP());

  // Armenteros-Podolanski quantities
  values[kArmAlpha] = pair->GetArmAlpha();
  values[kArmPt]    = pair->GetArmPt();

  //if(Req(kPsiPair))  values[kPsiPair]      = fgEvent ? pair->PsiPair(fgEvent->GetMagneticField()) : -5;
  //if(Req(kPhivPair))  values[kPhivPair]      = pair->PhivPair(1.);

  // impact parameter
  Double_t d0z0[2] = {-999., -999.};
  if (fgEvent) pair->GetDCA(fgEvent->GetPrimaryVertex(), d0z0);
  values[kImpactParXY] = d0z0[0];
  values[kImpactParZ]  = d0z0[1];

  values[kRndmPair] = gRandom->Rndm();
}


inline void PairAnalysisVarManager::FillVarPixelHit(const CbmPixelHit* hit, Double_t* const values)
{
  //
  // Fill hit information for the rich hit into array
  //

  // Protect
  if (!hit) return;

  // accessors via CbmPixelHit
  values[kPosX] = hit->GetX();
  values[kPosY] = hit->GetY();
  // accessors via CbmHit
  values[kPosZ] = hit->GetZ();
  // accessors via CbmMatch
  values[kLinksMC] = (hit->GetMatch() ? hit->GetMatch()->GetNofLinks() : 0.);
}

inline void PairAnalysisVarManager::FillVarStsHit(const CbmStsHit* hit, Double_t* const values)
{
  //
  // Fill hit information for the sts hit into array
  //

  // Reset array
  ResetArrayData(kHitMax, values);

  // Protect
  if (!hit) return;

  // accessors via CbmPixelHit & CbmHit
  FillVarPixelHit(hit, values);

  // Set
  // ...
}

inline void PairAnalysisVarManager::FillVarMvdHit(const CbmMvdHit* hit, Double_t* const values)
{
  //
  // Fill hit information for the mvd hit into array
  //

  // Reset array
  ResetArrayData(kHitMax, values);

  // Protect
  if (!hit) return;

  // accessors via CbmPixelHit & CbmHit
  FillVarPixelHit(hit, values);

  // Set
  // TODO: add center of gravity?
  // ...
}

inline void PairAnalysisVarManager::FillVarMuchHit(const CbmMuchPixelHit* hit, Double_t* const values)
{
  //
  // Fill hit information for the much hit into array
  //

  // Reset array
  ResetArrayData(kHitMax, values);

  // Protect
  if (!hit) return;

  // accessors via CbmPixelHit & CbmHit
  FillVarPixelHit(hit, values);

  // Set
  // ...
}

inline void PairAnalysisVarManager::FillVarRichHit(const CbmRichHit* hit, Double_t* const values)
{
  //
  // Fill hit information for the rich hit into array
  //

  // Reset array
  ResetArrayData(kHitMax, values);

  // Protect
  if (!hit) return;

  // accessors via CbmPixelHit & CbmHit
  FillVarPixelHit(hit, values);

  // Set
  values[kNPhotons] = 1;  //hit->GetNPhotons();
  values[kPmtId]    = hit->GetPmtId();
}

inline void PairAnalysisVarManager::FillVarTrdHit(const CbmTrdHit* hit, Double_t* const values)
{
  //
  // Fill hit information for the trd hit into array
  //

  // Reset array
  ResetArrayData(kHitMax, values);

  // Protect
  if (!hit) return;

  // accessors via CbmPixelHit & CbmHit
  FillVarPixelHit(hit, values);

  // accessors via CbmCluster & CbmTrdCluster
  TClonesArray* cluster = fgEvent->GetCluster(ECbmModuleId::kTrd);
  if (cluster->GetEntriesFast() > 0) {
    const CbmTrdCluster* cls = static_cast<const CbmTrdCluster*>(cluster->At(hit->GetRefId()));
    FillVarTrdCluster(cls, values);
    //    if(cls) std::cout << (cls->ToString()).data();
  }

  // Set
  values[kTrdLayer] = hit->GetPlaneId();  //layer id
  /// NOTE: use correction from first TRD track param
  values[kEloss] = hit->GetELoss() * 1.e+6;  //GeV->keV, dEdx + TR
  //  values[kElossdEdx] = hit->GetELossdEdX() * 1.e+6; //GeV->keV, dEdx
  //  values[kElossTR]   = hit->GetELossTR()   * 1.e+6; //GeV->keV, TR
  //  Printf("eloss trd: %.3e (%.3e TR, %.3e dEdx)",hit->GetELoss(),hit->GetELossTR(),hit->GetELossdEdX());
}

inline void PairAnalysisVarManager::FillVarTofHit(const CbmTofHit* hit, Double_t* const values)
{
  //
  // Fill hit information for the tof hit into array
  //

  // Reset array
  ResetArrayData(kHitMax, values);

  // Protect
  if (!hit) return;

  // accessors via CbmPixelHit & CbmHit
  FillVarPixelHit(hit, values);

  // Set
  values[kBeta] = values[kTrackLength] / 100 / ((hit->GetTime()- values[kEvStartTime]) * 1e-9) / TMath::C();
  // PID value detla beta
  values[kTofPidDeltaBetaEL] =
    values[kBeta] - (values[kP] / TMath::Sqrt(values[kMEL] * values[kMEL] + values[kP] * values[kP]));
  values[kTofPidDeltaBetaMU] =
    values[kBeta] - (values[kP] / TMath::Sqrt(values[kMMU] * values[kMMU] + values[kP] * values[kP]));
  values[kTofPidDeltaBetaPI] =
    values[kBeta] - (values[kP] / TMath::Sqrt(values[kMPI] * values[kMPI] + values[kP] * values[kP]));
  values[kTofPidDeltaBetaKA] =
    values[kBeta] - (values[kP] / TMath::Sqrt(values[kMKA] * values[kMKA] + values[kP] * values[kP]));
  values[kTofPidDeltaBetaPR] =
    values[kBeta] - (values[kP] / TMath::Sqrt(values[kMPR] * values[kMPR] + values[kP] * values[kP]));

  values[kMassSq] = values[kP] * values[kP] * (TMath::Power(1. / values[kBeta], 2) - 1);

  //  Printf("track length: %f beta: %f",values[kTrackLength],values[kBeta]);
  //  Double_t mass2 = TMath::Power(momentum, 2.) * (TMath::Power(time/ trackLength, 2) - 1);
}

inline void PairAnalysisVarManager::FillVarTrdCluster(const CbmTrdCluster* cluster, Double_t* const values)
{
  //
  // Fill cluster information for the trd cluster into array
  //

  // Reset array
  //  ResetArrayDataMC(  kHitMaxMC,   values);

  // Protect
  if (!cluster) return;

  // accessors via CbmCluster
  values[kTrdPads] = cluster->GetNofDigis();

  // Set
  values[kTrdCols] = cluster->GetNCols();
  values[kTrdRows] = cluster->GetNRows();
}

inline void PairAnalysisVarManager::FillVarMCPoint(const FairMCPoint* hit, Double_t* const values)
{
  //
  // Fill MC hit information
  //

  // Reset array
  ResetArrayDataMC(kHitMaxMC, values);

  // Protect
  if (!hit) return;

  // Set
  values[kPosXMC]  = hit->GetX();
  values[kPosYMC]  = hit->GetY();
  values[kPosZMC]  = hit->GetZ();
  values[kElossMC] = hit->GetEnergyLoss() * 1.e+6;  //GeV->keV, dEdx
}

inline void PairAnalysisVarManager::FillSumVarMCPoint(const FairMCPoint* hit, Double_t* const values)
{
  //
  // Sum upMC hit information
  //

  // DO NOT reset array

  // Protect
  if (!hit) return;

  // Set
  values[kPosXMC] += hit->GetX();
  values[kPosYMC] += hit->GetY();
  values[kPosZMC] += hit->GetZ();
  values[kElossMC] += hit->GetEnergyLoss() * 1.e+6;  //GeV->keV, dEdx
}

inline void PairAnalysisVarManager::FillVarConstants(Double_t* const values)
{
  //
  // Fill constant information available into an array
  // make use of TPDGCode
  //

  // Set
  values[kMEL]   = TDatabasePDG::Instance()->GetParticle(kElectron)->Mass();
  values[kMMU]   = TDatabasePDG::Instance()->GetParticle(kMuonMinus)->Mass();
  values[kMPI]   = TDatabasePDG::Instance()->GetParticle(kPiPlus)->Mass();
  values[kMKA]   = TDatabasePDG::Instance()->GetParticle(kKPlus)->Mass();
  values[kMPR]   = TDatabasePDG::Instance()->GetParticle(kProton)->Mass();
  values[kMK0]   = TDatabasePDG::Instance()->GetParticle(kK0Short)->Mass();
  values[kMLA]   = TDatabasePDG::Instance()->GetParticle(kLambda0)->Mass();
  values[kMPair] = fgData[kMPair];  /// automaticaly filled in PairAnalysis::Process using PairAnalysis::fPdgMother
  values[kEbeam] = fgData[kEbeam];  /// automaticaly filled in AnalysisTaskMultiPairAnalysis::Init
}

inline void PairAnalysisVarManager::SetEvent(PairAnalysisEvent* const ev)
{
  //
  // set event and vertex
  //

  // Set
  fgEvent = ev;

  // Reset
  if (fgKFVertex) delete fgKFVertex;
  fgKFVertex = 0x0;
  if (fgVertexMC) fgVertexMC->Reset();
  else
    fgVertexMC = new CbmVertex();

  // Set
  FillVarConstants(fgData);
  if (ev && ev->GetPrimaryVertex()) fgKFVertex = new CbmKFVertex(*ev->GetPrimaryVertex());
  Fill(fgEvent, fgData);
}

inline void PairAnalysisVarManager::SetEventData(const Double_t data[PairAnalysisVarManager::kNMaxValuesMC])
{
  /* for (Int_t i=0; i<kNMaxValuesMC;++i) fgData[i]=0.; */
  for (Int_t i = kPairMax; i < kNMaxValues; ++i)
    fgData[i] = data[i];
  for (Int_t i = kPairMaxMC; i < kNMaxValuesMC; ++i)
    fgData[i] = data[i];
}


inline void PairAnalysisVarManager::CalculateHitTypes(const PairAnalysisTrack* track, ECbmModuleId idet, Int_t* trueH,
                                                      Int_t* distH, Int_t* fakeH)
{

  CbmTrack* trkl    = track->GetTrack(idet);
  CbmRichRing* ring = track->GetRichRing();
  Int_t nhits       = 0;
  switch (idet) {
    case ECbmModuleId::kMvd:
      if (trkl) nhits = static_cast<CbmStsTrack*>(trkl)->GetNofMvdHits();
      break;
    case ECbmModuleId::kSts:
      if (trkl) nhits = static_cast<CbmStsTrack*>(trkl)->GetNofStsHits();
      break;
    case ECbmModuleId::kMuch:
    case ECbmModuleId::kTrd:
      if (trkl) nhits = trkl->GetNofHits();
      break;
    case ECbmModuleId::kTof:
      nhits = 1; /* one is maximum */
      break;
    case ECbmModuleId::kRich:
      if (ring) nhits = ring->GetNofHits();
      break;
    default: return;
  }

  CbmTrackMatchNew* tmtch = track->GetTrackMatch(idet);
  Int_t mctrk =
    (tmtch && tmtch->GetNofHits() > 0 && tmtch->GetNofLinks() > 0 ? tmtch->GetMatchedLink().GetIndex() : -1);

  PairAnalysisMC* mc = PairAnalysisMC::Instance();
  if (mc) {

    /// Calculate true, distorted and fake hits
    TClonesArray* hits = fgEvent->GetHits(idet);
    TClonesArray* pnts = fgEvent->GetPoints(idet);

    Int_t links   = 0;
    Double_t dist = 0.;
    *trueH        = 0;
    *distH        = 0;
    *fakeH        = (mctrk > -1 ? 0 : nhits);
    if (hits && pnts && mctrk > -1) {
      for (Int_t ihit = 0; ihit < nhits; ihit++) {

        CbmHit* hit = NULL;
        Int_t idx   = -1;
        switch (idet) {
          case ECbmModuleId::kMvd: idx = static_cast<CbmStsTrack*>(trkl)->GetMvdHitIndex(ihit); break;
          case ECbmModuleId::kSts: idx = static_cast<CbmStsTrack*>(trkl)->GetStsHitIndex(ihit); break;
          case ECbmModuleId::kMuch:
          case ECbmModuleId::kTrd: idx = trkl->GetHitIndex(ihit); break;
          case ECbmModuleId::kTof: hit = track->GetTofHit(); break;
          case ECbmModuleId::kRich: idx = ring->GetHit(ihit); break;
          default: continue;
        }

        if (idet != ECbmModuleId::kTof && idx > -1) { hit = dynamic_cast<CbmHit*>(hits->At(idx)); }

        if (!hit) {
          (*fakeH)++;
          continue;
        }
        CbmMatch* mtch = hit->GetMatch();
        if (!mtch) {
          (*fakeH)++;
          continue;
        }

        Bool_t btrueH = kTRUE;
        Bool_t bfakeH = kTRUE;
        Int_t nlinks  = mtch->GetNofLinks();
        links += nlinks;
        for (Int_t iLink = 0; iLink < nlinks; iLink++) {
          //	if(nlinks!=1) { fakeH++; continue; }
          FairMCPoint* pnt = static_cast<FairMCPoint*>(pnts->At(mtch->GetLink(iLink).GetIndex()));
          // hit defintion
          if (!pnt) btrueH = kFALSE;
          else {
            Int_t lbl  = pnt->GetTrackID();
            Int_t lblM = mc->GetMothersLabel(lbl);
            Int_t lblG = mc->GetMothersLabel(lblM);
            if (lbl != mctrk && lblM != mctrk && lblG != mctrk) {
              btrueH = kFALSE;
              dist += 1.;
            }
            else
              bfakeH = kFALSE;
          }
        }
        // increase counters
        if (btrueH) (*trueH)++;
        if (bfakeH) (*fakeH)++;
        if (!btrueH && !bfakeH) (*distH)++;
      }
    }
    /* values[kTrdDistortion]  = dist/links; */
  }
}


inline UInt_t* PairAnalysisVarManager::GetArray(ValueTypes var0, ValueTypes var1, ValueTypes var2, ValueTypes var3,
                                                ValueTypes var4, ValueTypes var5, ValueTypes var6, ValueTypes var7,
                                                ValueTypes var8, ValueTypes var9)
{
  //
  // build var array for e.g. TFormula's, THnBase's
  //
  static UInt_t arr[10] = {0};
  arr[0]                = static_cast<UInt_t>(var0);
  arr[1]                = static_cast<UInt_t>(var1);
  arr[2]                = static_cast<UInt_t>(var2);
  arr[3]                = static_cast<UInt_t>(var3);
  arr[4]                = static_cast<UInt_t>(var4);
  arr[5]                = static_cast<UInt_t>(var5);
  arr[6]                = static_cast<UInt_t>(var6);
  arr[7]                = static_cast<UInt_t>(var7);
  arr[8]                = static_cast<UInt_t>(var8);
  arr[9]                = static_cast<UInt_t>(var9);
  return arr;
}

inline void PairAnalysisVarManager::InitFormulas()
{
  if (fgFormula[1]) return;
  for (Int_t i = 1; i < kNMaxValuesMC - 1; ++i) {
    fgFormula[i] = new TFormula(fgkParticleNames[i][0], "[0]");
    fgFormula[i]->SetParameter(0, i);
    fgFormula[i]->SetParName(0, fgkParticleNames[i][0]);
  }
}

inline void PairAnalysisVarManager::InitFitter()
{
  /* if(!fgKFFitter) { */
  /*   fgKFFitter = new CbmStsKFTrackFitter(); */
  /*   fgKFFitter->Init(); */
  /* } */
  //  if(!fgL1Fitter) fgL1Fitter = new CbmL1PFFitter();
}

#endif
