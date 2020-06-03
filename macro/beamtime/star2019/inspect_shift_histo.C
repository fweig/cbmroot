void inspect_shift_histo(TString hFile="data/HistosMonitorTofSync.root"){
  using namespace std; 

 TFile *fHistFile = TFile::Open(hFile,"READ");
 TString hname;
 TH1* h1;
 TH1* h11;
 TH1* h13;
 TH1* h14;
 TH2* h2;
 Int_t NGdpb=12;
 Int_t NGbTx=6;
 const Int_t NHIGH=3;
 Double_t dHighCts[NHIGH]={NHIGH*0.};
 Int_t iHighCts_Get4[NHIGH];
 
 // find Get4s with highest rates
 if (1) 
 for (Int_t iGdpb=0; iGdpb<NGdpb; iGdpb++) {
   hname=Form("hGdpbGet4MessType_%02d",iGdpb);
   h2=(TH2 *)fHistFile->FindObjectAny(hname);
   if(NULL == h2) {
     cout << " Histo " << hname << " not found " << endl;
     continue;
   }
   Int_t iSec=iGdpb+13;
   h11 = h2->ProjectionX("_px",1,1);  // get hit count
   for (Int_t iGbtx=0; iGbtx<NGbTx; iGbtx++) {
     Int_t iStart=iGbtx*40;
     for (Int_t iGet4=iStart; iGet4<iStart+24; iGet4++) {
       Double_t dCts=h11->GetBinContent(iGet4+1);
       for(Int_t i=0; i<NHIGH; i++) {
	 if ( dCts > dHighCts[i] ) {
	   if ( dHighCts[i] > 0. ) {
	     for (Int_t j=i; j<NHIGH-1; j++) {
	       dHighCts[j+1]=dHighCts[j];
	       iHighCts_Get4[j+1]=iHighCts_Get4[j];
	     }
	   }
	   iHighCts_Get4[i]=(iGdpb*NGbTx+iGbtx)*40+iGet4-iStart;
	   /*
	   cout << "Process Gbdpb " << iGdpb << ", iGbtx "<<iGbtx << ", Get4 " << iGet4-iStart
		<< ", ID " <<  iHighCts_Get4[i] << ", Cts " << dCts
		<< ", i= " << i << ", Cts " << dHighCts[i] << endl;
	   */
	   dHighCts[i]=dCts;
	   break;
	 }
       }
     }
   }
 }
 const Double_t CTSMAX=1.E6;
 cout << "High score list of Get4s" << endl;
 for (Int_t i=0; i<NHIGH; i++) {
   cout << " # " << i << ", Get4 " << iHighCts_Get4[i] << ", counts: " << dHighCts[i] << endl;
   if ( dHighCts[i] > CTSMAX ) {
     Int_t iGet4 = iHighCts_Get4[i] % 40;
     Int_t iGbtx = (( iHighCts_Get4[i] - iGet4 ) / 40 ) % 6;
     Int_t iGdpb = ( iHighCts_Get4[i] - iGet4 - iGbtx*40 ) / 240;
     TDatime *pTime=new TDatime();
     cout << Form("%d,%d",pTime->GetDate(),pTime->GetTime()) << Form(": Disable Get4 %d %d %d %e ",iGdpb,iGbtx,iGet4,dHighCts[i]) << endl;
     gSystem->Exec(Form("/home/cbm/starsoft/dpbcontrols/mask_get4.sh %d %d %d ",iGdpb,iGbtx,iGet4));
     // cout << Form("%d,%d",pTime->GetDate(),pTime->GetTime()) << Form(": Disable all Get4s in  %d %d %d %e ",iGdpb,iGbtx,iGet4,dHighCts[i]) << endl;
     // gSystem->Exec(Form("/home/cbm/starsoft/dpbcontrols/mask_gbtx.sh %d %d %d ",iGdpb,iGbtx,iGet4));
   }
 }
 
 // reinitialze all Get4s with no slow control message
 if (1) {
   Int_t iMissingGet4s=0;
   std::vector<std::array<Int_t,3>> vMisGet4;
   
   for (Int_t iGdpb=0; iGdpb<NGdpb; iGdpb++){
     hname=Form("hGdpbGet4MessType_%02d",iGdpb);
     h2=(TH2 *)fHistFile->FindObjectAny(hname);
     if(NULL == h2) {
       cout << " Histo " << hname << " not found " << endl;
       continue;
     }
     Int_t iSec=iGdpb+13;
     h13 = h2->ProjectionX("_px",3,3);  // get s.c.m line
     for (Int_t iGbtx=0; iGbtx<NGbTx; iGbtx++) {
       Int_t iStart=iGbtx*40;
       for (Int_t iGet4=iStart; iGet4<iStart+24; iGet4++) {
	 if(h13->GetBinContent(iGet4+1) == 0) {
	   iMissingGet4s++;
	   std::array<Int_t,3> aGet4;
	   aGet4[0]=iSec;
	   aGet4[1]=iGbtx;
	   aGet4[2]=iGet4-iStart;
	   vMisGet4.push_back(aGet4);
	   TDatime *pTime=new TDatime();
	   cout << Form("%d,%d",pTime->GetDate(),pTime->GetTime()) << Form(": Reconfig Get4 %d %d ",iSec,iGet4) << endl;
	   //gSystem->Exec(Form("/home/cbm/starsoft/dpbcontrols/etc/etof/scripts/reconfig_get4.sh %d %d > /home/cbm/starsoft/dpbcontrols/LOG/ReconfigGet4_%d_%d",iSec,iGet4,iSec,iGet4));	 
	 }
       }
     }
   }
   TString Fname="/home/cbm/starsoft/dpbcontrols/LOG/MissingGet4Sum";
   std::fstream inFile(Fname.Data());
   Int_t iMissedGet4=0;
   if(! (inFile >> iMissedGet4)) {
       std::cerr << "Reading previous number failed!\n";
       iMissedGet4=0;
   }
   if(iMissingGet4s != iMissedGet4) {// notify star log
     TDatime *pTime=new TDatime();
     cout << Form("%d,%d",pTime->GetDate(),pTime->GetTime())
	  << ": Number of missing Get4s changed from " << iMissedGet4 << " to " <<  iMissingGet4s << endl;
     TString Notify = Form("/home/tonko/rtsLog -d WARN -p 8008 -c etofEvb \"ETOF number of missing Get4s changed: %d -> %d \"",iMissedGet4,iMissingGet4s);
     cout << Notify.Data() << endl;
     gSystem->Exec(Notify.Data());
     for (Int_t i=0; i<iMissingGet4s; i++) {
       Notify = Form("/home/tonko/rtsLog -d WARN -p 8008 -c etofEvb \"ETOF Get4 inactive in sector %d, gbtx %d, get4# %d \"",vMisGet4[i][0],vMisGet4[i][1],vMisGet4[i][2]) ;
       cout << Notify.Data() << endl;
       gSystem->Exec(Notify.Data());
     }
   }
   gSystem->Exec(Form("echo %d > %s",iMissingGet4s,Fname.Data()));
   if ( iMissingGet4s > 7 ) { // most likely a full GBTX missing, try to revive
     gSystem->Exec("nohup /home/cbm/software2020/startup_scripts/request_tune.sh > /home/cbm/logs/2020/RequestTune.log &");
   }
 }
 
 // reinitialize Gbtx 
 if(1)
 for (Int_t iGdpb=0; iGdpb<NGdpb; iGdpb++){
   hname=Form("hGdpbGet4MessType_%02d",iGdpb);
   h2=(TH2 *)fHistFile->FindObjectAny(hname);
   if(NULL == h2) {
     cout << " Histo " << hname << " not found " << endl;  
     continue;
   }
   h1 = h2->ProjectionX("_px",3,3);  // get scm line
   for (Int_t iGbtx=0; iGbtx<NGbTx; iGbtx++) {
     Int_t iStart=iGbtx*40;
     Int_t NGet4=0;
     for (Int_t iGet4=iStart; iGet4<iStart+32; iGet4++) {
       if(h1->GetBinContent(iGet4) > 0) NGet4++;
     }
     if (NGet4 < 8) {
       cout << "AFCK " << iGdpb << ", Gbtx " << iGbtx << ", low active Get4 " << NGet4 << endl;
       // if(iGdpb == 9 && iGbtx == 2) continue;  // sector 22, module 2 
       //gSystem->Exec(Form("/home/cbm/starsoft/dpbcontrols/afck_reinit_gbtx.sh %d %d &",iGdpb,iGbtx));
       gSystem->Exec(Form("rm /tmp/GBTX-OK%d; rm /tmp/GBTX-OK",iGdpb+13));    
       break;  // continue with next gdpb
     } else {
       //if(NGet4 != 24)
       ;
     }
     /*
     TDatime *pTime=new TDatime();
     cout << Form("%d,%d",pTime->GetDate(),pTime->GetTime()) << ": AFCK " << iGdpb << ", Gbtx " << iGbtx << " has " << NGet4 <<" active GET4s" << endl; 
     */ 
   }
 }
 // detect too many mismatches
 if(1) {
   hname=Form("hPatternMissmatch");
   h2=(TH2 *)fHistFile->FindObjectAny(hname);
   if(NULL == h2) {
     cout << " Histo " << hname << " not found " << endl;  
     return;
   }
   for (Int_t iGdpb=0; iGdpb<NGdpb; iGdpb++){
     h1 = h2->ProjectionX("_px",iGdpb+1,iGdpb+1);  // get gdpb line
     Int_t iSec=iGdpb+13;
     for (Int_t iGbtx=0; iGbtx<NGbTx; iGbtx++) {
       Int_t iStart=iGbtx*40;
       Int_t NBad=0;
       for (Int_t iGet4=iStart; iGet4<iStart+32; iGet4++) {
	 if( h1->GetBinContent(iGet4+1) > 1.E4) NBad++;
       }
       if (NBad > 0)
	 cout << NBad << "Get4 with high rate mismatches in Gdpb " << iGdpb << ", GBTx " << iGbtx << endl;
       if (NBad > 8) {
	 gSystem->Exec(Form("rm /tmp/GBTX-OK; rm /tmp/GBTX-OK%d;",iSec));
       }
     }
   }
 }
 
 // detect corrupt firmware
 if(1) {
   hname=Form("hPatternEnable");
    h2=(TH2 *)fHistFile->FindObjectAny(hname);
    if(NULL == h2) {
      cout << " Histo " << hname << " not found " << endl;  
      return;
    }
    Int_t NBad=0;
    for (Int_t iGdpb=0; iGdpb<NGdpb; iGdpb++){ 
      h1 = h2->ProjectionX("_px",iGdpb+1,iGdpb+1);  // get gdpb line
      Int_t NbadGbtx=0;  // counter for bad GBTXs
      // fill histograms
      const Double_t DEADMAX=0.5;
      for (Int_t iGbtx=0; iGbtx<NGbTx; iGbtx++) {
	Int_t iStart=iGbtx*40;
	Double_t dEnable=0.;
	Double_t dNorm=0.;
	for (Int_t iGet4=iStart;    iGet4<iStart+32; iGet4++)  dEnable += h1->GetBinContent(iGet4+1);       
	for (Int_t iGet4=iStart+32; iGet4<iStart+40; iGet4++)  dNorm   += h1->GetBinContent(iGet4+1);
	Double_t dFracDead=dEnable/32./(dNorm/8.);
	//cout << "Enable "<<dEnable<<", Norm "<<dNorm<<" , frac "<<dFracDead<<endl;
	if( dFracDead > DEADMAX ) NbadGbtx++;
      }
      //cout << "AFCK " << iGdpb+13 << " has " << NbadGbtx << " bad GBTX " << endl;
      if(NbadGbtx>3) { // request firmware reload
	NBad++;
	gSystem->Exec(Form("touch /tmp/MissingSync%d; rm /tmp/GBTX-OK; rm /tmp/GBTX-OK%d;",iGdpb,iGdpb+13));
      }
    }
    if(NBad > 0) 
      gSystem->Exec("nohup /home/cbm/software2020/startup_scripts/request_tune.sh > /home/cbm/logs/2020/RequestTune.log &");
 }
}
