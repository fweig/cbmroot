************ Macros in /cbmroot/macro/analysis/much

Please, change the paths for your cbmroot directory and for data files !

------ Optimization.C
Optimization(Int_t energy=8, Int_t NofFiles=5000, Int_t type=0, TString version="v19a", TString dir="/lustre/cbm/prod/mc/OCT19/sis100_muon_lmvm")

type=0:  invariant mass calculation background+omega for particular set of cuts. 
Use the tracks after CbmAnaDimuonAnalysis with UseCuts(kFALSE) (without any cuts).

You can change the values of cuts in macro:

 Int_t MUCHhits = 11;
 Int_t STShits = 7;
 Int_t TRDhits = 1;
 Int_t TOFhits = 1;

 Double_t MUCHchi2 = 3.;
 Double_t STSchi2 = 2.; 
 Double_t Vchi2 = 3.;
 
 Int_t sigmaTOFCut = 2;
 
Output are root and ps files with invariant mass distribution background+omega, 
omega-to-background ratio and omega efficiency, and with list of used cuts.

type=1: invariant mass calculation background+omega for particular MUCH configuration. 
Use the tracks after CbmAnaDimuonAnalysis with UseCuts(kTRUE).

Output are root and ps files with invariant mass distribution background+omega, 
omega-to-background ratio and omega efficiency for MUCH configuration under investigation.

------ InvariantMassSpectra.C

Invariant mass calculation.

InvariantMassSpectra_woMC(Int_t energy=8, Int_t NofFiles=1000, Int_t NofSignals=1, TString dir="")
Input for macro: beam energy value, number of files, number of signals and directory with files after CbmAnaDimuonAnalysis task. 
If NofSignals=1 - only omega will be used.
In order to use all signals (low-mass vector mesons and thermal muons) set NofSignals=7 (or 8 with J/psi).

Output is root file with:
1. Invariant mass distributions: for all signals+background, for backgroundand full infariant mass spectra;
2. Pt(Y) function for signals and signal-to-background ratio, if MC information is available;
3. YPtM 3D histograms for signals and background for correction procedure.
The normalization for signals+background is from HSD predictions.

------ InvariantMassSpectra_SE.C

The invariant mass of background using Super Event technique.

------ InvariantMassSpectra_mix.C

The invariant mass of background using Mix Event technique.
For run use batch_run_macro.sh:

sbatch batch_run_macro.sh

------ Add_histo.C

Sum of histograms after run InvariantMassSpectra_mix.C

------ TrackParameters.C

Produce histograms with parameters of muon track candidates.
Use the tracks after CbmAnaDimuonAnalysis with UseCuts(kFALSE) (without any cuts).
Additional input - name of directory with files after analysis task.

------ Pluto_analysis.C

Produce histograms with parameters of PLUTO generated particles.

------ Correction.C

Example of YPt distribution correction for reconstructed omega.
Macro uses 3-D histograms of YPtM for PLUTO generated omega (after Pluto_analysis.C macro)
and for reconstructed omega and background (after InvariantMassSpectra_withMC.C or InvariantMassSpectra_woMC.C macro).
Outputs - corrected YPt spectrum of reconstructed omega and all invariant mass histograms
which were used for omega supstruction and correction.
