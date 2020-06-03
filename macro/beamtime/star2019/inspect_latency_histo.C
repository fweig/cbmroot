void inspect_latency_histo(Double_t dLat=0, TString hFile="data/HistosMonitorTofSync.root"){
  using namespace std; 

  TFile *fHistFile = TFile::Open(hFile,"READ");
  TString hname;
  TH1* h1;
  TH1* h1res;
  TH1* h1ok;
  TH1* h1Nok;
  TH1* h1CtsMin;
  TH1* h1LatMin;
  TH1* h23;
  TH1* h24;
  TH2* h2;
  const Int_t NGdpb=12;
  const Int_t NGbTx=6;
  Int_t iInit=0;
  Int_t NMissingGbtx=0;
  Bool_t bSectorFW[NGdpb]={NGdpb*kFALSE};
  Int_t MGet4[40]={12,11,39,8,19,36,29,5,28,4,18,22,20,23,0,6,25,24,34,38,16,35,17,21,10,15,9,14,32,13,33,37,30,1,31,26,7,3,27,2};
  //  
  if(1) {
    hname=Form("hPatternMissmatch");
    h2=(TH2 *)fHistFile->FindObjectAny(hname);
    if(NULL == h2) {
      cout << " Histo " << hname << " not found " << endl;  
      return;
    }
    //h2->Draw("colz"); //for debugging
    //fHistFile->Close();
    // open result file
    hFile="data/LatencyEvalHistos.root";
    TFile *fResultFile=new TFile(hFile.Data(),"UPDATE");
    if( !fResultFile->IsOpen() )
      fResultFile=new TFile(hFile.Data(),"RECREATE");

    Int_t NBad=0;
    gSystem->Exec(Form("rm /home/cbm/starsoft/dpbcontrols/MASK/*"));
    gSystem->Exec(Form("rm /home/cbm/starsoft/dpbcontrols/datainmask_g*"));

    for (Int_t iGdpb=0; iGdpb<NGdpb; iGdpb++){
      
      h1 = h2->ProjectionX("_px",iGdpb+1,iGdpb+1);  // get gdpb line
      
      // get histograms
      hname=Form("hLatency_%d",iGdpb);
      h1res=(TH1 *)fResultFile->FindObjectAny(hname);
      if (h1res == NULL )
	h1res=new TH1D(hname.Data(),hname.Data(),240,0,240.);
     
      hname=Form("hOK_%d",iGdpb);
      h1ok=(TH1 *)fResultFile->FindObjectAny(hname);
      if (h1ok == NULL )
	h1ok=new TH1D(hname.Data(),hname.Data(),240,0,240.);

      hname=Form("hNOK_%d",iGdpb);
      h1Nok=(TH1 *)fResultFile->FindObjectAny(hname);
      if (h1Nok == NULL )
	h1Nok=new TH1D(hname.Data(),hname.Data(),240,0,240.);      

      hname=Form("hCountsMinimum_%d",iGdpb);
      h1CtsMin=(TH1 *)fResultFile->FindObjectAny(hname);
      if (h1CtsMin == NULL )
	h1CtsMin=new TH1D(hname.Data(),hname.Data(),240,0,240.);
      
      hname=Form("hLatencyMinimum_%d",iGdpb);
      h1LatMin=(TH1 *)fResultFile->FindObjectAny(hname);
      if (h1LatMin == NULL )
	h1LatMin=new TH1D(hname.Data(),hname.Data(),240,0,240.);
      
      Int_t iSec=iGdpb+13;
      gSystem->Exec(Form("echo b=C0S00_gdpb0%d?etc/etof/ipbus_etof%d.xml > /home/cbm/starsoft/dpbcontrols/latency%d.gdpb ",iSec,iSec,iSec));
      
      Int_t Nbad=0;
      // fill histograms 
      for (Int_t iGbtx=0; iGbtx<NGbTx; iGbtx++) {
	Int_t iStart=iGbtx*40;
	for (Int_t iGet4=iStart; iGet4<iStart+32; iGet4++) {
	  //cout << " Inspect " << iGet4 << " " << iGdpb << endl; 
	  //cout << " Got " << h1->GetBinContent(iGet4+1)  << " entries " << endl;
	  if( h1->GetBinContent(iGet4+1) == 0) {
	    h1ok->Fill(iGet4);
	    Double_t dNok=h1Nok->GetBinContent(iGet4+1);
	    Double_t dNext=TMath::Max(1.,dNok);
	    //if(h1ok->GetBinContent(iGet4+1) == dNext) {
	    if(dNok == 0) {
	      h1res->SetBinContent(iGet4+1,dLat);
	      h1Nok->SetBinContent(iGet4+1,1);
	      h1CtsMin->SetBinContent(iGet4+1,0);
	      h1LatMin->SetBinContent(iGet4+1,dLat);
	      //gSystem->Exec(Form("echo SetLatencyGet4 %d,%d >> /home/cbm/starsoft/dpbcontrols/latency%d.gdpb ",iGet4,int(dLat),iSec));
	    } 
	    
	  } else {  // Get4 has Pattern Mismatches 
	    h1ok->SetBinContent(iGet4+1,0);
	    h1Nok->SetBinContent(iGet4+1,0);
	    gSystem->Exec(Form("/home/cbm/starsoft/dpbcontrols/write_mask_get4.sh %d %d %d ",iGdpb,iGbtx,iGet4-iGbtx*40));
	    if( h1CtsMin->GetBinContent(iGet4+1) == 0) {
	      h1CtsMin->SetBinContent(iGet4+1, h1->GetBinContent(iGet4+1)); // initialize
	      h1LatMin->SetBinContent(iGet4+1,dLat);
	      cout << "Init Latency for " << iSec << " " << iGet4 << " to " << dLat 
		   << " with " << h1->GetBinContent(iGet4+1) << " counts " << endl; 
	    } else {
	      if(  h1->GetBinContent(iGet4+1) < h1CtsMin->GetBinContent(iGet4+1) ) {
		h1CtsMin->SetBinContent(iGet4+1, h1->GetBinContent(iGet4+1)); // update
		h1LatMin->SetBinContent(iGet4+1,dLat);
		cout << "Upd  Latency for " << iSec << " " << iGet4 << " to " << dLat
		     << " with " << h1->GetBinContent(iGet4+1) << " counts " << endl; 
	      }
	    }
	    if( dLat ==  h1LatMin->GetBinContent(iGet4+1) ) {
	      h1CtsMin->SetBinContent(iGet4+1, h1->GetBinContent(iGet4+1));
	      cout << "Upd  LatencyCounts for " << iSec << " " << iGet4 << " to " << dLat
		   << " with " << h1->GetBinContent(iGet4+1) << " counts " << endl; 
	    }	      
	  }
	  if( h1Nok->GetBinContent(iGet4+1) == 0 ) Nbad++;
	}
	
	Double_t dLatN=dLat+1;
	if(dLatN == 15.) dLatN=0;	
	gSystem->Exec(Form("echo SetLatencyGbtx %d,0x%x%x%x%x%x%x%x%x,0x%x%x%x%x%x%x%x%x,0x%x%x%x%x%x%x%x%x,0x%x%x%x%x%x%x%x%x,0x%x%x%x%x%x%x%x%x >> /home/cbm/starsoft/dpbcontrols/latency%d.gdpb ",iGbtx,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[39]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[39]+1) :( uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[38]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[38]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[37]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[37]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[36]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[36]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[35]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[35]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[34]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[34]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[33]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[33]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[32]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[32]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[31]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[31]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[30]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[30]+1) : (uint)dLatN,
   			   h1Nok->GetBinContent(iGbtx*40+MGet4[29]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[29]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[28]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[28]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[27]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[27]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[26]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[26]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[25]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[25]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[24]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[24]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[23]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[23]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[22]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[22]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[21]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[21]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[20]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[20]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[19]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[19]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[18]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[18]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[17]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[17]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[16]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[16]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[15]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[15]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[14]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[14]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[13]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[13]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[12]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[12]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[11]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[11]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[10]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[10]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[9]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[9]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[8]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[8]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[7]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[7]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[6]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[6]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[5]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[5]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[4]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[4]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[3]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[3]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[2]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[2]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[1]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[1]+1) : (uint)dLatN,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[0]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[0]+1) : (uint)dLatN,iSec));
       
	// write temporary gdpb - file
       
      }
      gSystem->Exec(Form("echo quit >> /home/cbm/starsoft/dpbcontrols/latency%d.gdpb ",iSec));

      // write best values
      gSystem->Exec(Form("echo b=C0S00_gdpb0%d?etc/etof/ipbus_etof%d.xml > /home/cbm/starsoft/dpbcontrols/latency%d_best.gdpb ",iSec,iSec,iSec));
      for (Int_t iGbtx=0; iGbtx<NGbTx; iGbtx++) {
      	gSystem->Exec(Form("echo SetLatencyGbtx %d,0x%x%x%x%x%x%x%x%x,0x%x%x%x%x%x%x%x%x,0x%x%x%x%x%x%x%x%x,0x%x%x%x%x%x%x%x%x,0x%x%x%x%x%x%x%x%x >> /home/cbm/starsoft/dpbcontrols/latency%d_best.gdpb ",iGbtx,
			   h1Nok->GetBinContent(iGbtx*40+MGet4[39]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[39]+1) :( uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[39]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[38]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[38]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[38]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[37]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[37]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[37]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[36]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[36]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[36]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[35]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[35]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[35]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[34]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[34]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[34]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[33]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[33]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[33]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[32]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[32]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[32]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[31]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[31]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[31]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[30]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[30]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[30]+1),
   			   h1Nok->GetBinContent(iGbtx*40+MGet4[29]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[29]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[29]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[28]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[28]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[28]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[27]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[27]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[27]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[26]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[26]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[26]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[25]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[25]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[25]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[24]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[24]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[24]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[23]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[23]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[23]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[22]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[22]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[22]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[21]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[21]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[21]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[20]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[20]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[20]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[19]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[19]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[19]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[18]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[18]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[18]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[17]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[17]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[17]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[16]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[16]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[16]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[15]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[15]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[15]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[14]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[14]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[14]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[13]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[13]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[13]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[12]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[12]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[12]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[11]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[11]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[11]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[10]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[10]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[10]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[9]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[9]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[9]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[8]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[8]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[8]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[7]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[7]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[7]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[6]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[6]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[6]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[5]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[5]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[5]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[4]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[4]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[4]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[3]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[3]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[3]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[2]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[2]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[2]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[1]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[1]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[1]+1),
			   h1Nok->GetBinContent(iGbtx*40+MGet4[0]+1) > 0 ?	(uint)h1res->GetBinContent(iGbtx*40+MGet4[0]+1) : (uint)h1LatMin->GetBinContent(iGbtx*40+MGet4[0]+1),iSec));
      }
      gSystem->Exec(Form("echo quit >> /home/cbm/starsoft/dpbcontrols/latency%d_best.gdpb ",iSec));
      
      TDatime *pTime=new TDatime();
      cout << Form("%d,%d",pTime->GetDate(),pTime->GetTime()) << ": AFCK " << iGdpb << " has " << Nbad << " bad GET4s" << endl;
      NBad += Nbad;
    } // end of Gdpb loop
    cout << "eTOF has " << NBad << " bad GET4s" << endl;
    gSystem->Exec(Form("echo %d > /tmp/BadGet4 ",NBad));
    fResultFile->Write();
    return;
  }

}
