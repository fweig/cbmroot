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

Bool_t readFile(TString inFile, std::map<Int_t, TH1I*>&mVoltage, std::map<Int_t, TH1I*>&mCurrent, std::map<Int_t, TGraph*>&mTrendingI, std::map<Int_t, TGraph*>&mTrendingU, TH1I* hChID, TGraph* gCurrentSum1, TH1I* hCurrent, TH1I* hVoltage, Bool_t debug){

  Bool_t fileStat;
  ifstream in;
  in.open(inFile);
  Int_t year(-1), month(-1), day(-1), hour(-1), min(-1), sec(-1), msec(-1), chID(-1), deltaT(0), lineLength(0), fileTime(0);
  Int_t lastDay(-1), lastHour(-1), lastMin(-1), lastSec(-1), lastMsec(-1);
  Double_t voltage(0.0), current(0.0), currentSum1(0.0);
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
      if (nLines % 500000 == 0)
	cout << inFile << ": line " << nLines << endl;
      in.getline(cline,500);
      line = cline;
      lineLength = (Int_t)line.Length();
      line.ReplaceAll("= Opaque: Float: ",""); 

      if (line.BeginsWith("201")){
	//gCurrentSum1->SetPoint((gCurrentSum1->GetN()),fileTime,currentSum1); // write summed current to plot and
	//currentSum1 = 0.; // reset currentSum in a new time bin
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
	  //	  hDeltaT->Fill(deltaT);
	  //	  gTime->SetPoint(gTime->GetN(),fileTime,deltaT);
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
	  cout<< "A: ";
	lineLength = (Int_t)line.Length();
	sChID = line(0,3);
	chID = sChID.Atoi();
	// continue for lv channels
	if (chID ==312){
	  gCurrentSum1->SetPoint((gCurrentSum1->GetN()),fileTime,currentSum1); // write summed current to plot and
	  currentSum1 = 0.; // reset currentSum in a new time bin
	}
	// continue for lv channels
	if (chID < 200) {
	  nLines++;
	  continue;
	}
	// continue for empty hv channels
	if (chID <= 215 && !(chID < 201)) {
	  nLines++;
	  continue;
	}
	if (chID <= 315 && !(chID < 313)) {
	  nLines++;
	  continue;
	}
	if (chID <= 407 && !(chID < 401)) {
	  nLines++;
	  continue;
	}
	if (chID <= 515 && !(chID < 501)) {
	  nLines++;
	  continue;
	}
	hChID->Fill(chID);
	sCurrent = line(4,lineLength-4);
	current = sCurrent.Atof();
	// add to currentSum1 if from frankfurt chamber
	if(chID >=300 && chID < 313) currentSum1 += current;
	hCurrent->Fill(current);
	//	if(chID >=300 && chID < 313) hCurrentMap->Fill(chID,1,(1E6*current)); // IKF channels
	//	if(chID >=500 && chID < 501) hCurrentMap->Fill((chID-200),2,(1E6*current)); // IKP Ms channel

	lineStyle = (Int_t)(chID/100)+1;
	lineColor = (chID%100)+3;
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
	  mTrendingI[chID]->SetLineWidth(2.);
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
	// continue for lv channels
	if (chID < 200) {
	  nLines++;
	  continue;
	}
	// continue for empty hv channels
	if (chID <= 215 && !(chID < 201)) {
	  nLines++;
	  continue;
	}
	if (chID <= 315 && !(chID < 313)) {
	  nLines++;
	  continue;
	}
	if (chID <= 407 && !(chID < 401)) {
	  nLines++;
	  continue;
	}
	if (chID <= 515 && !(chID < 501)) {
	  nLines++;
	  continue;
	}
	//	hChID->Fill(chID);
	voltage = sVoltage.Atof();
	hVoltage->Fill(voltage);
	lineStyle = (Int_t)(chID/100)+1;
	lineColor = (chID%100)+1;
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
	  mTrendingU[chID]->SetLineWidth(2.);
	}
	mVoltage[chID]->Fill(voltage);
	mTrendingU[chID]->SetPoint(mTrendingU[chID]->GetN(),fileTime,voltage);
      } else if (line.BeginsWith("Loop")) {
	//	cout << endl << inFile << line << endl;
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


void monHVlong(TString listoffilenames="tempname.config", TString path="/mount/scr1/p_kaeh01/hv/", TString exportpath="/mount/scr1/p_kaeh01/hv/plot/")
{
  //  TH1I* hTime = new TH1I("hTime","hTime",60000,0,60000);
  TH1I* hChID = new TH1I("hChID","hChID",991,-0.5,990.5);
  hChID->SetXTitle("channel ID");
  //  TH1I* hDeltaT = new TH1I("hDeltaT","hDeltaT",1001,-0.5,1000.5);
  //  hDeltaT->SetXTitle("#Deltat (ms)");
  //  TGraph* gTime = new TGraph();
  //  gTime->SetTitle("ReadoutTimes");
  TH1I* hCurrent = new TH1I("hCurrent","hCurrent",100001,-1E-7,8E-6);
  hCurrent->SetXTitle("I (A)");
  // prepare x limits for channel numbers to be filled, z is the current in nanoampere
  //  TProfile2D* hCurrentMap = new TProfile2D("hCurrentMap","hCurrentMap",13,299.5,312.5,2,0.5,2.5,0.,4.);
  TH1I* hVoltage = new TH1I("hVoltage","hVoltage",200001,-1,2000);
  hVoltage->SetXTitle("U (V)");
  TGraph* gCurrentSum1 = new TGraph();
  std::map<Int_t, TH1I*> mVoltage;
  std::map<Int_t, TH1I*> mCurrent;
  std::map<Int_t, TGraph*> mTrendingI;
  std::map<Int_t, TGraph*> mTrendingU;

  TCanvas *c6 = new TCanvas("c6","CurrentTrend",2539,1610);
  TCanvas *c7 = new TCanvas("c7","VoltageTrend",2539,1610);

  TDatime da(2015,10,30,12,00,00);
  gStyle->SetTimeOffset(da.Convert());
  Bool_t debug(false), diffFile(false), nextFile(true);
  TString configline("");

  char cname[200];

  if (listoffilenames!="") //part to read all files given in a meta file
    {
    ifstream filestream(path+listoffilenames);
    string infile;
    string pathedinfile;

    while(getline(filestream, infile))
      {
      cout<<infile<<endl;
      pathedinfile = path + infile;
      readFile(pathedinfile, mVoltage, mCurrent, mTrendingI, mTrendingU, hChID, gCurrentSum1, hCurrent, hVoltage, debug);
      /*    }
    }
  else
    {
      cout << "Do some single file stuff" << endl;
    }
      */

      /*
      TCanvas *c1 = new TCanvas("c1","CurrentDist",800,600);
      c1->SetLogy(1);
      hCurrent->DrawCopy();
      for (std::map<Int_t, TH1I*>::iterator it=mCurrent.begin(); it!=mCurrent.end();it++){
	it->second->DrawCopy("same");
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
      */


      c6->cd();
      c6->SetLogy();
      c6->SetTickx();
      c6->SetTicky();
      c6->SetGridy();
      TMultiGraph *multiI = new TMultiGraph();
      gCurrentSum1->SetLineColor(kRed);
      multiI->Add(gCurrentSum1);
      for (std::map<Int_t, TGraph*>::iterator it=mTrendingI.begin(); it!=mTrendingI.end();it++){
    //    if (it->first == 304 || it->first == 405)
    //    switch (it->first)
    //      {
    //      case 304: case 404: case 405:
	//      cout << it->first << "blub" << endl;
    //    if(it->second > 100E-9)    {
	multiI->Add(it->second);
      //  }
      //      break;
      //    default:
      //    continue;
      //      }
  }
      multiI->Draw("AL");
      multiI->GetYaxis()->SetRangeUser(1E-10,2E-5);
      multiI->GetXaxis()->SetTitle("Time (ms)");
      multiI->GetYaxis()->SetTitle("I (A)");
      //  c6->Update(); // why doesnt this update the titles and range properly?
      //multiI->Draw("AL");
      //      c6->BuildLegend();

      c7->cd();
      //      c7->SetLogy();
      c7->SetTickx();
      c7->SetTicky();
      c7->SetGridy();
      TMultiGraph *multiU = new TMultiGraph();
      for (std::map<Int_t, TGraph*>::iterator it=mTrendingU.begin(); it!=mTrendingU.end();it++){
	multiU->Add(it->second);
      }
      multiU->Draw("AL");
      multiU->GetYaxis()->SetRangeUser(0,2000);
      multiU->GetXaxis()->SetTitle("Time (ms)");
      multiU->GetYaxis()->SetTitle("U (V)");
      c7->Update();
      //      c7->BuildLegend();

      /*
      TCanvas *c8 = new TCanvas("c8","CurrentMap",800,600);
      c8->SetLogz(1);
      hCurrentMap->DrawCopy("COLZ");
      hCurrentMap->GetXaxis()->SetTitle("chID");
      hCurrentMap->GetYaxis()->SetTitle("chamber");
      hCurrentMap->GetZaxis()->SetTitle("I (#muA)");
      //  c8->Update(); // why does update not update the axis titles? following new drawing neccessary!
      hCurrentMap->DrawCopy("COLZ");
      */

      TString outFile = exportpath + infile;
      outFile.ReplaceAll(".txt",".png");
      outFile.ReplaceAll(".log",".png");
      //  c1->SaveAs(outFile.ReplaceAll(".png","-CurrentDist.png"));
      //  c2->SaveAs(outFile.ReplaceAll("-CurrentDist.png","-VoltageDist.png"));
      //  c3->SaveAs(outFile.ReplaceAll("-VoltageDist.png","-DeltaTDist.png"));
      //  c4->SaveAs(outFile.ReplaceAll("-DeltaTDist.png","-DeltaTTrend.png"));
      //  c5->SaveAs(outFile.ReplaceAll("-DeltaTTrend.png","-chIDDist.png"));
      //c6->SaveAs(outFile.ReplaceAll("-chIDDist.png","-CurrentTrend.png"));
      c6->SaveAs(outFile.ReplaceAll(".png","-CurrentTrend.png"));
      c7->SaveAs(outFile.ReplaceAll("-CurrentTrend.png","-VoltageTrend.png"));
    }
    }
}
