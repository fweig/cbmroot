#include "Riostream.h"
#include "TString.h"
#include <map>
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TDatime.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TMultiGraph.h"

Int_t firstDay(0), firstHour(0), firstMin(0), firstSec(0), firstMsec(0);


Bool_t readFile(TString inFile, std::map<Int_t, TH1I*>&mVoltage, std::map<Int_t, TH1I*>&mCurrent, std::map<Int_t, TGraph*>&mTrendingI, std::map<Int_t, TGraph*>&mTrendingU, TGraph* gTrendingT, TH1I* hTime, TH1I* hChID, TH1I* hDeltaT, TH1I* hCurrent, TH1I* hVoltage, Bool_t debug){
  Bool_t fileStat;
  ifstream in;
  in.open(inFile);
  Int_t year(-1), month(-1), day(-1), hour(-1), min(-1), sec(-1), msec(-1), chID(-1), deltaTime_rel(0), lineLength(0), deltaTime_abs(0);
  Int_t lastDay(-1), lastHour(-1), lastMin(-1), lastSec(-1), lastMsec(-1);
  Float_t voltage(0.0), current(0.0);
  Int_t lineColor(1), lineStyle(1);
  TString line("");
  TString sTime(""), sChID(""), sVoltage(""), sCurrent(""), name("");
  char cline[500];
  if (!in){
    cout << inFile << " not found!" << endl;
    fileStat = false;
  } else {
    cout << inFile << " found!" << endl;
    Int_t nLines = 0;
    cout << "Reading file";
    while(in){
      //sTime = "";
      if (nLines % 1000 == 0)
	cout << " .";
      in.getline(cline,500);
      line = cline;
      lineLength = (Int_t)line.Length();
      line.ReplaceAll("= Opaque: Float: ",""); 


      if (line.BeginsWith("201")){
	sTime = line;
	year  = TString(line( 0,4)).Atoi(); 
	month = TString(line( 5,2)).Atoi();
	day   = TString(line( 8,2)).Atoi();
	hour  = TString(line(11,2)).Atoi();
	min   = TString(line(14,2)).Atoi();
	sec   = TString(line(17,2)).Atoi();
	msec  = TString(line(20,3)).Atoi();
	if (firstDay==0){
	  firstDay = day;
	  firstHour = hour;
	  firstMin = min;
	  firstSec = sec;
	  firstMsec = msec;
	}
	/*
	if (lastDay != -1)
	  if (lastDay <= day && lastHour <= hour && lastMin <= min && lastSec <= sec && lastMsec <= msec){
	  }else{
	    cout << "ERROR: wrong time order in file"<< endl;
	    return false;
	  }
	*/
	if (debug)
	  printf("T: %s\n   %i-%02i-%02i:%02i:%02i:%02i:%03i\n\n",sTime.Data(),year,month,day,hour,min,sec,msec);
	deltaTime_abs = (msec - firstMsec) + ((sec - firstSec) + ((min - firstMin) + ((hour - firstHour) + (day - firstDay)*24)*60)*60)*1000;
	if (lastDay > 0){
	  deltaTime_rel = (msec - lastMsec) + ((sec - lastSec) + ((min - lastMin) + ((hour - lastHour) + (day - lastDay)*24)*60)*60)*1000;
	  hDeltaT->Fill(deltaTime_rel);
	  if (debug)
	    cout << deltaTime_rel << endl;
	}
	gTrendingT->SetPoint(gTrendingT->GetN(),deltaTime_abs,deltaTime_rel);
	lastDay = day;
	lastHour = hour;
	lastMin = min;
	lastSec = sec;
	lastMsec = msec;
      } else if (line.EndsWith("A")){
	line.ReplaceAll("WIENER-CRATE-MIB::outputMeasurementCurrent.u","");
	line.ReplaceAll(" A","");
	if (debug)
	  cout << "A: ";
	lineLength = (Int_t)line.Length();
	sCurrent = line(4,lineLength-4);
	current = sCurrent.Atof();
	hCurrent->Fill(current);
	sChID = line(0,3);
	chID = sChID.Atoi();
	hChID->Fill(chID);
	lineStyle = (Int_t)(chID/100)+1;
	lineColor = chID - 100*(lineStyle-1);
	if (mCurrent.find(chID) == mCurrent.end()){
	  name.Form("hCurrent%03i",chID);
	  mCurrent[chID] = new TH1I(name,name,100001,-1,1);
	  mCurrent[chID]->SetXTitle("I (A)");
	  mCurrent[chID]->SetLineStyle(lineStyle);
	  mCurrent[chID]->SetLineColor(lineColor);
	  name.Form("hDeltaT_Current%03i",chID);
	  mTrendingI[chID] = new TGraph();
	  mTrendingI[chID]->SetTitle(name);
	  mTrendingI[chID]->GetXaxis()->SetTitle("#Deltat (ms)");
	  mTrendingI[chID]->GetYaxis()->SetTitle("I (A)");
	  mTrendingI[chID]->SetLineStyle(lineStyle);
	  mTrendingI[chID]->SetLineColor(lineColor);
	}
	mCurrent[chID]->Fill(current);
	mTrendingI[chID]->SetPoint(mTrendingI[chID]->GetN(),deltaTime_abs,current);
      } else if (line.EndsWith("V")){
	line.ReplaceAll("WIENER-CRATE-MIB::outputMeasurementTerminalVoltage.u","");
	line.ReplaceAll(" V","");
	if (debug)
	  cout << "V: ";
	lineLength = (Int_t)line.Length();
	sVoltage = line(4,lineLength-4);
	//sCurrent = "";
	voltage = sVoltage.Atof();
	hVoltage->Fill(voltage);
	sChID = line(0,3);
	chID = sChID.Atoi();  
	hChID->Fill(chID);
	lineStyle = (Int_t)(chID/100)+1;
	lineColor = chID - 100*(lineStyle-1);  
	if (mVoltage.find(chID) == mVoltage.end()){
	  name.Form("hVoltage%03i",chID);
	  mVoltage[chID] = new TH1I(name,name,200001,-1,2000);
	  mVoltage[chID]->SetXTitle("U (V)");
	  mVoltage[chID]->SetLineStyle(lineStyle);
	  mVoltage[chID]->SetLineColor(lineColor);
	  name.Form("hDeltaT_Voltage%03i",chID);
	  mTrendingU[chID] = new TGraph();
	  mTrendingU[chID]->SetTitle(name);
	  mTrendingU[chID]->GetXaxis()->SetTitle("#Deltat (ms)");
	  mTrendingU[chID]->GetYaxis()->SetTitle("U (V)");
	  mTrendingU[chID]->SetLineStyle(lineStyle);
	  mTrendingU[chID]->SetLineColor(lineColor);
	}
	mVoltage[chID]->Fill(voltage);
	mTrendingU[chID]->SetPoint(mTrendingU[chID]->GetN(),deltaTime_abs,voltage);
      } else if (line.BeginsWith("Loop")) {
	cout << endl << line << endl;
      } else {
	cout << endl << "/" << line << "/ : Unknown content! To be ignored" << endl;
      }
      if (debug)
	cout << lineLength << ":   " << line << endl;
  
      if (debug){
	cout << "T=" << sTime  << endl;
	cout << "C=" << chID << endl;
	cout << "V=" << voltage << endl;
	cout << "A=" << current << endl;
      }
      nLines++;
    }
    cout << endl << "Done" << endl << "Found " << nLines << " lines in file " << inFile << endl;

    fileStat = true;
  }
  in.close();
  return fileStat;
}

void readHV(TString inFile="hv.log"/*exampleHV.txt"*/)
{
  TDatime da(2015,10,30,12,00,00);
  gStyle->SetTimeOffset(da.Convert());
  Bool_t debug(false), nextFile(true);
  TString outFile = inFile;
  outFile.ReplaceAll(".txt",".root");
  outFile.ReplaceAll(".log",".root");
  TFile *out = new TFile(outFile,"RECREATE");

  TH1I* hTime = new TH1I("hTime","hTime",60000,0,60000);
  TH1I* hChID = new TH1I("hChID","hChID",991,-0.5,990.5);
  hChID->SetXTitle("channel ID");
  TH1I* hDeltaT = new TH1I("hDeltaT","hDeltaT",1001,-0.5,1000.5);
  hDeltaT->SetXTitle("#Deltat_{abs.} (ms)");
  TH1I* hCurrent = new TH1I("hCurrent","hCurrent",100001,-1,1);
  hCurrent->SetXTitle("I (A)");
  TH1I* hVoltage = new TH1I("hVoltage","hVoltage",200001,-1,2000);
  hVoltage->SetXTitle("U (V)");
  std::map<Int_t, TH1I*> mVoltage;
  std::map<Int_t, TH1I*> mCurrent;
  std::map<Int_t, TGraph*> mTrendingI;
  std::map<Int_t, TGraph*> mTrendingU;
  TGraph* gTrendingT = new TGraph();
  gTrendingT->SetTitle("gTrendingT");;
  while (nextFile) {
    readFile(inFile, mVoltage, mCurrent, mTrendingI, mTrendingU, gTrendingT, hTime, hChID, hDeltaT, hCurrent, hVoltage, debug);
   
    cout << "Read new file?: (1,0)   ";
    cin >> nextFile;
    if (nextFile){
      cout << "File name?:   ";
      cin >> inFile;
    }
  }
  TCanvas *c = new TCanvas("view","view",3*800,3*600);
  c->Divide(3,3);
  c->cd(1)->SetLogy(1);
  hCurrent->DrawCopy();
  hCurrent->Write("",TObject::kOverwrite);
  c->cd(2)->SetLogy(1);
  hVoltage->DrawCopy();
  hVoltage->Write("",TObject::kOverwrite);
  c->cd(3);
  hDeltaT->DrawCopy();
  hDeltaT->Write("",TObject::kOverwrite);
  c->cd(7);
  hChID->DrawCopy();
  hChID->Write("",TObject::kOverwrite);
  out->cd();
  c->cd(1);
  for (std::map<Int_t, TH1I*>::iterator it=mCurrent.begin(); it!=mCurrent.end();it++){
    it->second->DrawCopy("same");
    it->second->Write("",TObject::kOverwrite);
  }
  c->Update();
  c->cd(2);
  for (std::map<Int_t, TH1I*>::iterator it=mVoltage.begin(); it!=mVoltage.end();it++){
    it->second->DrawCopy("same");
    it->second->Write("",TObject::kOverwrite);
  }
  c->Update();
  c->cd(4)->SetLogy(0);
  TMultiGraph *multiI = new TMultiGraph();
  for (std::map<Int_t, TGraph*>::iterator it=mTrendingI.begin(); it!=mTrendingI.end();it++){
    multiI->Add(it->second);
    it->second->Write("",TObject::kOverwrite);
  }
  multiI->Draw("AL");
  multiI->GetXaxis()->SetTitle("#Deltat_{rel.} (ms)");
  multiI->GetYaxis()->SetTitle("I (A)");
  multiI->SetTitle("multiI");
  multiI->Write("multiI",TObject::kOverwrite);
  c->Update();
  c->cd(5)->SetLogy(0);
  TMultiGraph *multiU = new TMultiGraph();
  for (std::map<Int_t, TGraph*>::iterator it=mTrendingU.begin(); it!=mTrendingU.end();it++){
    multiU->Add(it->second);
    it->second->Write("",TObject::kOverwrite);
  }
  multiU->Draw("AL");
  multiU->GetXaxis()->SetTitle("#Deltat_{rel.} (ms)");
  multiU->GetYaxis()->SetTitle("U (V)");
  multiU->SetTitle("multiU");
  multiU->Write("multiU",TObject::kOverwrite);
  c->Update();
  c->cd(6)->SetLogy(0);
  gTrendingT->Draw("AL");
  gTrendingT->GetXaxis()->SetTitle("#Deltat_{rel.} (ms)");
  gTrendingT->GetYaxis()->SetTitle("#Deltat_{abs.} (ms)");
  gTrendingT->Write("",TObject::kOverwrite);
  c->Update();  
  c->Write("",TObject::kOverwrite);
  c->SaveAs(outFile.ReplaceAll(".root",".png"));
}
