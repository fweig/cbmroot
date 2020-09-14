//---------------------------------------------------
//
// ANN training of muon track candidates
//
// CbmAnaDimuonAnalysis has to be used with UseCuts(kFALSE)
//
// Anna Senger a.senger@gsi.de
//
//---------------------------------------------------

void ANN(Int_t energy=8, Int_t NofFiles=1000,
	 TString dir="../../much/data/",
	 TString setup="sis100_muon_lmvm",
	 Int_t ntrain=300, Int_t neurons=16)
{
 gStyle->SetCanvasColor(10);
 gStyle->SetFrameFillColor(10);
 gStyle->SetHistLineWidth(4);
 gStyle->SetPadColor(10);
 gStyle->SetStatColor(10);
 gStyle->SetPalette(55);
 gStyle->SetPaintTextFormat("2.1f");

 Double_t Chi2Vertex, Chi2STS, Chi2MUCH, Chi2TRD;
 Int_t NofSTS, NofMUCH, NofTRD;
 Double_t P, M;

 TTree *Signal = new TTree("Signal","mu");
 Signal->Branch("P", &P, "P/D", 20000000);
 Signal->Branch("M", &M, "M/D", 20000000);
 Signal->Branch("Chi2Vertex",&Chi2Vertex,"Chi2Vertex/D",20000000);
 Signal->Branch("Chi2STS",   &Chi2STS,   "Chi2STS/D",   20000000);
 Signal->Branch("Chi2MUCH",  &Chi2MUCH,  "Chi2MUCH/D",  20000000);
 Signal->Branch("Chi2TRD",  &Chi2TRD,  "Chi2TRD/D",  20000000);
 Signal->Branch("NofSTS", &NofSTS, "NofSTS/I", 20000000);
 Signal->Branch("NofMUCH",&NofMUCH,"NofMUCH/I",20000000);
 Signal->Branch("NofTRD", &NofTRD, "NofTRD/I", 20000000);

 TTree *Bg = new TTree("Background","bg");
 Bg->Branch("P", &P, "P/D", 20000000);
 Bg->Branch("M", &M, "M/D", 20000000);
 Bg->Branch("Chi2Vertex",&Chi2Vertex,"Chi2Vertex/D",20000000);
 Bg->Branch("Chi2STS",   &Chi2STS,   "Chi2STS/D",   20000000);
 Bg->Branch("Chi2MUCH",  &Chi2MUCH,  "Chi2MUCH/D",  20000000);
 Bg->Branch("Chi2TRD",  &Chi2TRD,  "Chi2TRD/D",  20000000);
 Bg->Branch("NofSTS", &NofSTS, "NofSTS/I", 20000000);
 Bg->Branch("NofMUCH",&NofMUCH,"NofMUCH/I",20000000);
 Bg->Branch("NofTRD", &NofTRD, "NofTRD/I", 20000000);

 TString name;

 TClonesArray *MuPlus = new TClonesArray("CbmAnaMuonCandidate");
 TClonesArray *MuMinus = new TClonesArray("CbmAnaMuonCandidate");
 TTree *InputTree;

 Int_t MUCHhits= 9;
 Int_t STShits = 4;
 Int_t TRDhits = 1;
 Int_t TOFhits = 1;

 Double_t MUCHchi2= 5.;
 Double_t STSchi2 = 5.; 
 Double_t Vchi2   = 5.;
 Double_t Mcut    = 5.;
 Double_t Pcut    = 20.;

//--------------------------------------------------------
 for(int i=0; i<2; i++){
    for(int k=1; k<NofFiles+1; k++)
    {
      if(i==0)name.Form("%s/%s/%dgev/omega/%d/muons.ana.root",dir.Data(),setup.Data(),energy,k);
     else name.Form("%s/%s/%dgev/centr/%d/muons.ana.root",dir.Data(),setup.Data(),energy,k);
    
     TFile *f = new TFile(name);
     if (f->IsZombie() || f->GetNkeys() < 1 || f->TestBit(TFile::kRecovered)){f->Close();continue;}
     
     if(k%100 == 0)cout<<"Input File "<<k<<" : "<<f->GetName()<<endl;
     
     InputTree = (TTree*)f->Get("cbmsim");  
     InputTree->SetBranchAddress("MuPlus",  &MuPlus); 
     InputTree->SetBranchAddress("MuMinus", &MuMinus); 
       
     //----------------------------------------------------------- 
 
     for(int iEvent=0; iEvent<InputTree->GetEntries(); iEvent++)
       {	
	 InputTree->GetEntry(iEvent);

	 int NofPlus  = MuPlus->GetEntriesFast();
	 int NofMinus = MuMinus->GetEntriesFast();
	 for (int iPart=0; iPart<NofPlus; iPart++){
	   Bool_t trigger = kFALSE;
	   
	   CbmAnaMuonCandidate  *mu_pl = (CbmAnaMuonCandidate*)MuPlus->At(iPart);
	   
	   if((i==0 && mu_pl->GetTrueMu()==1) || i==1)trigger=kTRUE;
	   if(!trigger)continue;

	   TLorentzVector *P_pl = mu_pl->GetMomentum();
	   M = mu_pl->GetTofM();
	   P = P_pl->P();	   
	   
	   if(mu_pl->GetNStsHits() < STShits)continue;
	   if(mu_pl->GetNMuchHits()< MUCHhits)continue;
	   if(mu_pl->GetNTofHits() < 1)continue;
	   /*
	   if(M > Mcut || M < -10)continue;
	   if(P > Pcut)continue;
	   
	   if(mu_pl->GetChiToVertex() > Vchi2 || mu_pl->GetChiToVertex() < 0)continue;
	   if(mu_pl->GetChiSts() > STSchi2    || mu_pl->GetChiSts() < 0)     continue;
	   if(mu_pl->GetChiMuch()> MUCHchi2   || mu_pl->GetChiMuch() < 0)    continue;
	   if(mu_pl->GetNTofHits() < 1) continue;
	   if(mu_pl->GetNMuchHits()< 1)continue;
	   if(M > Mcut)continue;
	   */
	   Chi2Vertex = mu_pl->GetChiToVertex();
	   Chi2STS    = mu_pl->GetChiSts();
	   Chi2MUCH   = mu_pl->GetChiMuch();
	   Chi2TRD    = mu_pl->GetChiTrd();
	   
	   NofSTS  = mu_pl->GetNStsHits();
	   NofMUCH = mu_pl->GetNMuchHits();
	   NofTRD  = mu_pl->GetNTrdHits();
	   if(i==0)Signal->Fill(); else Bg->Fill(); 

	 }
	 for (int jPart=0; jPart<NofMinus; jPart++){
	   Bool_t trigger = kFALSE;
	   
	   CbmAnaMuonCandidate  *mu_mn = (CbmAnaMuonCandidate*)MuMinus->At(jPart);
	   
	   if((i==0 && mu_mn->GetTrueMu()==1) || i==1)trigger=kTRUE;
	   if(!trigger)continue;

	   TLorentzVector *P_mn = mu_mn->GetMomentum();
	   M = mu_mn->GetTofM();
	   P = P_mn->P();
	  
	   if(mu_mn->GetNStsHits() < STShits)continue;
	   if(mu_mn->GetNMuchHits()< MUCHhits)continue;
	   if(mu_mn->GetNTofHits() < 1)continue;
	   
	   /*
	   if(M > Mcut || M < -10)continue;
	   if(P > Pcut)continue;
	  
	   if(mu_mn->GetChiToVertex() > Vchi2 || mu_mn->GetChiToVertex() < 0)continue;
	   if(mu_mn->GetChiSts() > STSchi2    || mu_mn->GetChiSts() < 0 )    continue;
	   if(mu_mn->GetChiMuch()> MUCHchi2   || mu_mn->GetChiMuch() < 0)    continue;
	   if(mu_mn->GetNTofHits() < 1)continue;
	   if(mu_mn->GetNMuchHits()< 1)continue;
	   if(M > Mcut)continue;
	   */
	   Chi2Vertex = mu_mn->GetChiToVertex();
	   Chi2STS    = mu_mn->GetChiSts();
	   Chi2MUCH   = mu_mn->GetChiMuch();
	   Chi2TRD    = mu_mn->GetChiTrd();
	   
	   NofSTS  = mu_mn->GetNStsHits();
	   NofMUCH = mu_mn->GetNMuchHits();
	   NofTRD  = mu_mn->GetNTrdHits();
	   if(i==0)Signal->Fill(); else Bg->Fill(); 
	 }
	}
      f->Close(); 
    }}
 
   Int_t type;
   
   TTree *simu = new TTree("MonteCarlo", "Filtered Monte Carlo Events");
   simu->Branch("P", &P, "P/D", 20000000);
   simu->Branch("M", &M, "M/D", 20000000);
   simu->Branch("Chi2Vertex",&Chi2Vertex,"Chi2Vertex/D", 20000000);
   simu->Branch("Chi2STS",   &Chi2STS,   "Chi2STS/D", 20000000);
   simu->Branch("Chi2MUCH",  &Chi2MUCH,  "Chi2MUCH/D", 20000000);
   simu->Branch("Chi2TRD",  &Chi2TRD,  "Chi2TRD/D", 20000000);
   simu->Branch("NofSTS", &NofSTS, "NofSTS/I", 20000000);
   simu->Branch("NofMUCH",&NofMUCH,"NofMUCH/I", 20000000);
   simu->Branch("NofTRD", &NofTRD, "NofTRD/I", 20000000);
   simu->Branch("type",   &type,   "type/I", 20000000);

   type=1; // 1 = signal, 0 = background
   Int_t i;
   for (i = 0; i < Signal->GetEntries(); i++) {
      Signal->GetEntry(i);
      simu->Fill();
   }

   type = 0;
   for (i = 0; i < Bg->GetEntries(); i++) {
      Bg->GetEntry(i);
      simu->Fill();
   }
   
   name.Form("@P,@M,@Chi2Vertex,@Chi2STS,@Chi2MUCH,@Chi2TRD,@NofSTS,@NofMUCH,@NofTRD:%d:type",neurons);
   TMultiLayerPerceptron *mlp = new TMultiLayerPerceptron(name.Data(),simu,"Entry$%2");
   
   mlp->Train(ntrain, "text,graph,update=10");  
   name.Form("muid_ann_%d_%s_weights.txt",neurons,setup.Data());
   mlp->DumpWeights(name);
   
   TCanvas* mlpa_canvas = new TCanvas("mlpa_canvas","Network analysis");
   mlpa_canvas->Divide(2,2);
   TMLPAnalyzer ana(mlp);
   ana.GatherInformations();
   ana.CheckNetwork();
   mlpa_canvas->cd(1);
   ana.DrawDInputs();
   mlpa_canvas->cd(2);
   mlp->Draw();
   mlpa_canvas->cd(3);
   ana.DrawNetwork(0,"type==1","type==0");
   mlpa_canvas->cd(4);

   TH1F *bg = new TH1F("bgh", "NN output", 50, -.5, 1.5);
   TH1F *sig = new TH1F("sigh", "NN output", 50, -.5, 1.5);
   bg->SetDirectory(0);
   sig->SetDirectory(0);
   
   Double_t params[9];
  
   for (i = 0; i < Bg->GetEntries(); i++) {
      Bg->GetEntry(i);
      params[0] = P;
      params[1] = M;
      params[2] = Chi2Vertex;
      params[3] = Chi2STS;
      params[4] = Chi2MUCH;
      params[5] = Chi2TRD;
      
      params[6] = (Double_t)NofSTS;
      params[7] = (Double_t)NofMUCH;
      params[8] = (Double_t)NofTRD;
          
      bg->Fill(mlp->Evaluate(0, params));
   }
  
   for (i = 0; i < Signal->GetEntries(); i++) {
      Signal->GetEntry(i);
      params[0] = P;
      params[1] = M;
      params[2] = Chi2Vertex;
      params[3] = Chi2STS;
      params[4] = Chi2MUCH;
      params[5] = Chi2TRD;
      
      params[6] = (Double_t)NofSTS;
      params[7] = (Double_t)NofMUCH;
      params[8] = (Double_t)NofTRD;
      
      sig->Fill(mlp->Evaluate(0,params));
   }   
   
   bg->SetLineColor(kBlue);
   bg->SetFillStyle(3008);   bg->SetFillColor(kBlue);
   sig->SetLineColor(kRed);
   sig->SetFillStyle(3003); sig->SetFillColor(kRed);
   bg->SetStats(0);
   sig->SetStats(0);
   bg->Draw();
   sig->Draw("same");
   TLegend *legend = new TLegend(.75, .80, .95, .95);
   legend->AddEntry(bg, "Background");
   legend->AddEntry(sig, "Signal (#mu)");
   legend->Draw();
   mlpa_canvas->cd(0);
   
   name.Form("ann_%d_%s_canvas.root",neurons,setup.Data());
   TFile *FFF = new TFile(name,"recreate");
   mlpa_canvas->Write();
   FFF->Close();   
}



