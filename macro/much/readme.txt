************ MUCH analysis software /cbmroot/analysis/much

------ CbmAnaMuonCandidate

Object: muon track candidate with some important parameters, 
which is created with CbmAnaDimuonAnalysis task.

-------------- Sets of parameters --------------

------ Parameters for invariant mass calculation
SetMomentum - reconstructed momentum of the particle
SetSign     - charge of particle
    
------ Parameters for selection of best track candidates
-- selection of long tracks:
SetNStsHits  - number of STS hits
SetNMuchHits - number of MUCH hits
SetNTrdHits  - number of TRD hits
SetNTofHits  - number of TOF hits

-- selection of primary tracks:
SetChiToVertex - chi2/NDF of primary vertex 
    
-- selection of good quality tracks:
SetChiMuch   - chi2/NDF in MUCH
SetChiSts    - chi2/NDF in STS
SetChiTrd    - chi2/NDF in TRD          // is not used in present analysis 
SetChiGlobal - chi2/NDF of global track // is not used in present analysis

-- selection of tracks using TOF particle identification:
SetTofM - mass of particle from TOF
    
-- set of MC particle ID from STS reconstructed track (WARNING: use only for feasibility study)
SetTrueMu - MC true muon in STS
SetStsPdg - MC PDG from reconstructed track in STS
    
Corresponding gets return the set values.

------ CbmAnaDimuonAnalysis

Task: sort the reconstructed global tracks and create muon track candidates with important parameters 
(see CbmAnaMuonCandidate).

With UseCuts(kTRUE) you can select the best muon track candidates.

Cut sets for selecton of the best muon track candidate:

SetChi2StsCut    - maximum value for chi2/NDF in STS  (default 2)
SetChi2MuchCut   - maximum value for chi2/NDF in MUCH (default 3)
SetChi2VertexCut - maximum value for chi2/NDF in primary vertex (default 3)
  
SetNofMuchCut - minimum number of hits in MUCH (default 11)
SetNofStsCut  - minimum number of hits in STS  (default  7)
SetNofTrdCut  - minimum number of hits in TRD  (default  1)
  
SetSigmaTofCut - sigma value for polynomial fit of TOF mass distribution (default 2, possible 2, 3, 4)

Example:

  CbmAnaDimuonAnalysis* ana = new CbmAnaDimuonAnalysis("");  
  ana->SetChi2MuChCut(3.); 
  ana->SetChi2StsCut(2.);  
  ana->SetChi2VertexCut(3.);  
  ana->SetNofMuchCut(11);
  ana->SetNofStsCut(7);
  ana->SetNofTrdCut(1);  
  ana->SetSigmaTofCut(2);
  ana->UseCuts(kTRUE);


Additional output:

1. YPtM.root - 3D histogram of reconstructed muon pairs

2. YPt_histo.root - YPt spectra for pluto, accepted and reconstructed muon pairs using MC PID
(only for pluto(+urqmd) if CbmAnaDimuonAnalysis* ana = new CbmAnaDimuonAnalysis("pluto.root");)

3. eff_histo.root - efficiency plots of accepted and reconstructed muon pairs using MC PID
(only for pluto(+urqmd) if CbmAnaDimuonAnalysis* ana = new CbmAnaDimuonAnalysis("pluto.root");)

4. sup_histo.root - P distributions of reconstructed single muon track candidates for background suppression study 
(only for urqmd if CbmAnaDimuonAnalysis* ana = new CbmAnaDimuonAnalysis("");)

Macro add_histo.C combines these histograms.

Macro draw_histo.C produces canvas with results after run add_histo.C for background suppression
and for efficiency histograms after (for example):

hadd eff_histo.root sis100_muon_lmvm/8gev/omega/*/eff_histo.root

************ Run batch jobs /cbmroot/macro/much

1. Run full chain (transport, digitization, reconstruction, analysis) for background
and all signals: 

sbatch run_batch_jobs.sh  your_cbmroot_dir

2. If you would like to simulate background only, run: 

sbatch batch_run_bg.sh your_cbmroot_dir

3. If you would like to simulate background and omega, run: 

sbatch batch_run_bg.sh  your_cbmroot_dir
sbatch batch_run_sgn.sh your_cbmroot_dir 0

4. At the end you will have transport files, files after reconstruction and analysis files. 
The files after digitization will be deleted due to memory save reason.
If you need these files, comment in batch_run_bg.sh or batch_run_sgn.sh:

#rm *raw*.root

