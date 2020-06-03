#include "Riostream.h"
#include "TString.h"
#include <map>
#include "TH1.h"
#include "TH2.h"
#include "TProfile2D.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TDatime.h"
#include "TStyle.h"
#include "TGraph.h"
#include "TMultiGraph.h"

Int_t firstDay(0), firstHour(0), firstMin(0), firstSec(0), firstMsec(0);

//martin: if set true no questions form user required
Bool_t kohnmode=kFALSE;

Bool_t readFile(TString inFile, std::map<Int_t, TH1I*>&mVoltage, std::map<Int_t, TH1I*>&mCurrent, std::map<Int_t, TGraph*>&mTrendingI, std::map<Int_t, TGraph*>&mTrendingU, TH1I* hTime, TH1I* hChID, TH1I* hDeltaT, TGraph* gTime, TH1I* hCurrentDrift, TH1I* hCurrentAnode, TProfile2D* hCurrentMap, TH1I* hVoltage, TH2I* hVoltageCurrent, Bool_t debug){
  Bool_t fileStat;
  ifstream in;
  in.open(inFile);
  Int_t year(-1), month(-1), day(-1), hour(-1), min(-1), sec(-1), msec(-1), chID(-1), deltaT(0), lineLength(0), fileTime(0);
  Int_t lastDay(-1), lastHour(-1), lastMin(-1), lastSec(-1), lastMsec(-1);
  Double_t voltage(0.0), current(0.0);
  Int_t lineColor(1), lineStyle(1), lineWidth (1);
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
	fileTime = (msec - firstMsec) + ((sec - firstSec) + ((min - firstMin) + ((hour - firstHour) + (day - firstDay)*24)*60)*60)*1000;
	if (lastDay > 0){
	  deltaT = (msec - lastMsec) + ((sec - lastSec) + ((min - lastMin) + ((hour - lastHour) + (day - lastDay)*24)*60)*60)*1000;
	  hDeltaT->Fill(deltaT);
	  gTime->SetPoint(gTime->GetN(),fileTime,deltaT);
	  if (debug)
	    cout << deltaT << endl;
	}
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
	sChID = line(0,3);
	chID = sChID.Atoi();
	// continue for lv channels:
	if (chID < 400) continue;
	// continue for empty hv channels on the CBM TRD Iseg modules:
	if (chID <= 499 && chID > 406 || chID == 400 || chID == 402 || chID == 405 ) continue;
	if (chID <= 599 && chID > 503) continue;
	// continue for unused Iseg modules:
	if (chID >= 600) continue;
	hChID->Fill(chID);
	sCurrent = line(4,lineLength-4);
	current = sCurrent.Atof();
	if(chID >= 401 && chID <= 406 && chID != 402 && chID != 405) hCurrentDrift->Fill(current); //drift channels
	if(chID >= 500 && chID <= 503) hCurrentAnode->Fill(current); //anode channels
	if(chID >=401 && chID <= 406 && chID != 402 && chID != 405) hCurrentMap->Fill((chID-400+1),1,(1E6*current)); // drift channels
	if(chID >=500 && chID <= 503) hCurrentMap->Fill((chID-500+1),2,(1E6*current)); // anode channel
	
	if(chID >=500 && chID <= 503) hVoltageCurrent->Fill(voltage,current);
	
	lineStyle = 1;
	lineColor = (chID%100)+2;
	lineWidth = 5;
	if(lineColor >= 5) lineColor++;
	if (mCurrent.find(chID) == mCurrent.end()){
	  name.Form("hCurrent%03i",chID);
	  mCurrent[chID] = new TH1I(name,name,100001,-1,1);
	  mCurrent[chID]->SetXTitle("I (A)");
	  //	  mCurrent[chID]->SetLineStyle(lineStyle);
	  //	  mCurrent[chID]->SetLineColor(lineColor);
	  name.Form("hDeltaT_Current%03i",chID);
	  mTrendingI[chID] = new TGraph();
	  mTrendingI[chID]->SetTitle(name);
	  mTrendingI[chID]->GetYaxis()->SetTitle("I (A)");
	  mTrendingI[chID]->SetLineStyle(lineStyle);
	  mTrendingI[chID]->SetLineColor(lineColor);
	  mTrendingI[chID]->SetLineWidth(lineWidth);
	}
	mCurrent[chID]->Fill(current);
	mTrendingI[chID]->SetPoint(mTrendingI[chID]->GetN(),fileTime,current);
      } else if (line.EndsWith("V")){
	line.ReplaceAll("WIENER-CRATE-MIB::outputMeasurementTerminalVoltage.u","");
	line.ReplaceAll(" V","");
	if (debug)
	  cout << "V: ";
	lineLength = (Int_t)line.Length();
	sVoltage = line(4,lineLength-4);
	//sCurrent = "";
	sChID = line(0,3);
	chID = sChID.Atoi();  
	// continue for lv channels:
	if (chID < 400) continue;
	// continue for empty hv channels on the CBM TRD Iseg modules:
	if (chID <= 499 && chID > 406 || chID == 400 || chID == 402 || chID == 405 ) continue;
	if (chID <= 599 && chID > 503) continue;
	// continue for unused Iseg modules:
	if (chID >= 600) continue;
	//	hChID->Fill(chID);
	voltage = sVoltage.Atof();
	hVoltage->Fill(voltage);
	
	lineStyle = 1;
	lineColor = (chID%100)+1;
	lineWidth = 2;
	if(lineColor >= 5) lineColor++;
	if (mVoltage.find(chID) == mVoltage.end()){
	  name.Form("hVoltage%03i",chID);
	  mVoltage[chID] = new TH1I(name,name,200001,-1,2000);
	  mVoltage[chID]->SetXTitle("U (V)");
	  //	  mVoltage[chID]->SetLineStyle(lineStyle);
	  //	  mVoltage[chID]->SetLineColor(lineColor);
	  name.Form("hDeltaT_Voltage%03i",chID);
	  mTrendingU[chID] = new TGraph();
	  mTrendingU[chID]->SetTitle(name);
	  mTrendingU[chID]->GetYaxis()->SetTitle("U (V)");
	  mTrendingU[chID]->SetLineStyle(lineStyle);
	  mTrendingU[chID]->SetLineColor(lineColor);
	  mTrendingU[chID]->SetLineWidth(lineWidth);
	}
	mVoltage[chID]->Fill(voltage);
	mTrendingU[chID]->SetPoint(mTrendingU[chID]->GetN(),fileTime,voltage);
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


void monHV(TString configFile="/data2/cern_nov2016/hv/filename.config")
{
  TDatime da(2015,10,30,12,00,00);
  gStyle->SetTimeOffset(da.Convert());
  Bool_t debug(false), diffFile(false), nextFile(true);
  TString inFile("/data/cern2016/hv/2015-11-28_13-43_hv0.log");
  TString configline("");

  char cname[200];
  ifstream config;
  config.open(configFile);

  if (!config){
    cout << configFile << " not found, assuming " << inFile << endl;
  } else {
    cout << configFile << " found." << endl;
    config.getline(cname,200);
    configline = cname;
    inFile = configline;
  }
  if (!kohnmode) {
  cout << "File " << inFile << " will be read. Deviating wishes? (1 for yes, 0 for no): ";
  cin >> diffFile;
   if (diffFile){
    cout << "File name?:   ";
    cin >> inFile;
   }
  }

  // invent variables for nbins and limits here!

  TH1I* hTime = new TH1I("hTime","hTime",60000,0,60000);
  TH1I* hChID = new TH1I("hChID","hChID",991,-0.5,990.5);
  hChID->SetXTitle("channel ID");
  TH1I* hDeltaT = new TH1I("hDeltaT","hDeltaT",1001,-0.5,1000.5);
  hDeltaT->SetXTitle("#Deltat (ms)");
  TGraph* gTime = new TGraph();
  gTime->SetTitle("ReadoutTimes");
  TH1I* hCurrentDrift = new TH1I("hCurrentDrift","hCurrentDrift",100001,-1E-7,8E-6);
  TH1I* hCurrentAnode = new TH1I("hCurrentAnode","hCurrentAnode",100001,-1E-7,8E-6);
  hCurrentDrift->SetXTitle("I (A)");
  hCurrentAnode->SetXTitle("I (A)");
  // prepare x limits for channel numbers to be filled, z is the current in nanoampere
  TProfile2D* hCurrentMap = new TProfile2D("hCurrentMap","hCurrentMap",7,0.5,7.5,2,0.5,2.5);
  TH1I* hVoltage = new TH1I("hVoltage","hVoltage",200001,-1,2000);
  hVoltage->SetXTitle("U (V)");
  TH2I* hVoltageCurrent = new TH2I("hVoltageCurrent","hVoltageCurrent Anodes",2000,1849,1851,1000,-1E-7,6E-6);
  std::map<Int_t, TH1I*> mVoltage;
  std::map<Int_t, TH1I*> mCurrent;
  std::map<Int_t, TGraph*> mTrendingI;
  std::map<Int_t, TGraph*> mTrendingU;

  if(kohnmode){
    readFile(inFile, mVoltage, mCurrent, mTrendingI, mTrendingU, hTime, hChID, hDeltaT, gTime, hCurrentDrift, hCurrentAnode, hCurrentMap, hVoltage, hVoltageCurrent, debug);
  }
    else{
    while (nextFile) {
      readFile(inFile, mVoltage, mCurrent, mTrendingI, mTrendingU, hTime, hChID, hDeltaT, gTime, hCurrentDrift, hCurrentAnode, hCurrentMap, hVoltage, hVoltageCurrent, debug);
   
      cout << "Read further file? (1 for yes, 0 for no): ";
      cin >> nextFile;
    if (nextFile){
      cout << "File name?:   ";
      cin >> inFile;
    }
    }}
    //}
  TCanvas *c0 = new TCanvas("c0","CurrentDriftDist",800,600);
  c0->SetLogy(1);
  hCurrentDrift->DrawCopy();
  for (std::map<Int_t, TH1I*>::iterator it=mCurrent.begin(); it!=mCurrent.end();it++){
    //  it->second-DrawCopy("same");
  }
  c0->Update();
  TCanvas *c1 = new TCanvas("c1","CurrentAnodeDist",800,600);
  c1->SetLogy(1);
  hCurrentAnode->DrawCopy();
  for (std::map<Int_t, TH1I*>::iterator it=mCurrent.begin(); it!=mCurrent.end();it++){
    //  it->second->DrawCopy("same");
  }
  c1->Update();
  TCanvas *c2 = new TCanvas("c2","VoltageDist",800,600);
  c2->SetLogy(1);
  hVoltage->DrawCopy();
  for (std::map<Int_t, TH1I*>::iterator it=mVoltage.begin(); it!=mVoltage.end();it++){
    it->second->DrawCopy("same");
  }
  c2->Update();
  TCanvas *c3 = new TCanvas("c3","DeltaTDist",800,600);
  hDeltaT->DrawCopy();
  TCanvas *c4 = new TCanvas("c4","DeltaTTrend",800,600);
  gTime->GetXaxis()->SetTitle("Time (ms)");
  gTime->GetYaxis()->SetTitle("#Deltat (ms)");
  gTime->Draw();
  c4->Update();
  TCanvas *c5 = new TCanvas("c5","chIDDist",800,600);
  hChID->DrawCopy();
  TCanvas *c6d = new TCanvas("c6d","CurrentDriftTrend",800,600);
  c6d->SetLogy(0);
  TMultiGraph *multiIDrift = new TMultiGraph();
  for (std::map<Int_t, TGraph*>::iterator it=mTrendingI.begin(); it!=mTrendingI.end();it++){
    if (it->first >= 400 && it->first <= 406) {
      multiIDrift->Add(it->second); // select drift channels by chID
    }
  }
  multiIDrift->Draw("AL");
  multiIDrift->GetYaxis()->SetRangeUser(0.,5E-7);
  multiIDrift->GetXaxis()->SetTitle("Time (ms)");
  multiIDrift->GetYaxis()->SetTitle("I (A)");
  //  c6d->Update(); // why doesnt this update the titles and range properly?
  //multiIDrift->Draw("AL");
  c6d->BuildLegend(0.5,0.72,0.85,0.88);
  TCanvas *c6a = new TCanvas("c6a","CurrentAnodeTrend",800,600);
  c6a->SetLogy(0);
  TMultiGraph *multiIAnode = new TMultiGraph();
  for (std::map<Int_t, TGraph*>::iterator it=mTrendingI.begin(); it!=mTrendingI.end();it++){
    if (it->first >= 500 && it->first <= 504) {
      multiIAnode->Add(it->second); // select anodes by chID
    }
  }
  multiIAnode->Draw("AL");
  multiIAnode->GetYaxis()->SetRangeUser(0.,5E-7);
  multiIAnode->GetXaxis()->SetTitle("Time (ms)");
  multiIAnode->GetYaxis()->SetTitle("I (A)");
  //  c6a->Update(); // why doesnt this update the titles and range properly?
  //multiIAnode->Draw("AL");
  c6a->BuildLegend(0.5,0.72,0.85,0.88);
  TCanvas *c7 = new TCanvas("c7","VoltageTrend",800,600);
  c7->SetLogy(0);
  TMultiGraph *multiU = new TMultiGraph();
  for (std::map<Int_t, TGraph*>::iterator it=mTrendingU.begin(); it!=mTrendingU.end();it++){
    multiU->Add(it->second);
  }
  multiU->Draw("AL");
  multiU->GetXaxis()->SetTitle("Time (ms)");
  multiU->GetYaxis()->SetTitle("U (V)");
  c7->Update();
  c7->BuildLegend();
  TCanvas *c8 = new TCanvas("c8","CurrentMap",800,600);
  c8->SetLogz(1);
  hCurrentMap->DrawCopy("COLZ");
  hCurrentMap->GetXaxis()->SetTitle("chID");
  hCurrentMap->GetYaxis()->SetTitle("1=drift / 2=anode");
  hCurrentMap->GetZaxis()->SetTitle("I (#muA)");
  //  c8->Update(); // why does update not update the axis titles? following new drawing neccessary!
  hCurrentMap->DrawCopy("COLZ");

  TCanvas *c9 = new TCanvas("c9","CurrentVoltageMap",800,600);
  c9->SetLogz(1);
  hVoltageCurrent->DrawCopy("COLZ");
  hVoltageCurrent->GetXaxis()->SetTitle("U (V)");
  hVoltageCurrent->GetYaxis()->SetTitle("I (A)");
  //  c9->Update(); // why does update not update the axis titles? following new drawing neccessary!
  hVoltageCurrent->DrawCopy("COLZ");
  
  TString outFile = inFile;
  outFile.ReplaceAll(".txt",".png");
  outFile.ReplaceAll(".log",".png");
  //  c1->SaveAs(outFile.ReplaceAll(".png","-CurrentDist.png"));
  //  c2->SaveAs(outFile.ReplaceAll("-CurrentDist.png","-VoltageDist.png"));
  //  c3->SaveAs(outFile.ReplaceAll("-VoltageDist.png","-DeltaTDist.png"));
  //  c4->SaveAs(outFile.ReplaceAll("-DeltaTDist.png","-DeltaTTrend.png"));
  //  c5->SaveAs(outFile.ReplaceAll("-DeltaTTrend.png","-chIDDist.png"));
  //  c6->SaveAs(outFile.ReplaceAll("-chIDDist.png","-CurrentTrend.png"));
  //  c7->SaveAs(outFile.ReplaceAll("-CurrentTrend.png","-VoltageTrend.png"));
  }
