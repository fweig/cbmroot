/**
 *    file CbmKresTrainAnn.cxx
 *
 *    author Ievgenii Kres
 *    date 29.05.2017
 *    modified 30.01.2020
 *
 *    Class for training of ANN for pi^0 reconstruction.
 *    One provides sample of e+e- pairs coming from converted photon what come from pi^0 -> gamma + gamma decay. 
 *    One provides sample of e+e- pairs coming some other sources as a background.
 *    Using these two sample one trains ANN to reconstruct signal photons depending from values of InvariantMass, OpeningAngle, PlaneAngle_last, ZPos, Momentum1, Momentum2
 *    At the end of traaining the weight parameters are stored to .txt file and can be used for analysis.
 *    To verify the training one used two other independent samples of signal and background events. The verification is done using the weight parameters from .txt file
 *    Using final histograms one can define ANN cuts for futher analysis.
 *
 **/

#include "CbmKresTrainAnn.h"

#include <boost/assign/list_of.hpp>

#include <iostream>
#include <vector>
#include <cmath>
#include <string>

#include "TTree.h"
#include "TMath.h"
#include "TMultiLayerPerceptron.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "CbmDrawHist.h"


using boost::assign::list_of;
using namespace std;

CbmKresTrainAnn::CbmKresTrainAnn()
  : fMaxNofTrainSamples(1500),
	fAnnCut(0.6),
	fNofWrongLikeCorrect(0),
	fNofCorrectLikeWrong(0),
	IM_correct(),
	OA_correct(),
	Angle_correct(),
	Z_correct(),
	Mom1_correct(),
	Mom2_correct(),
	IM_wrong(),
	OA_wrong(),
	Angle_wrong(),
	Z_wrong(),
	Mom1_wrong(),
	Mom2_wrong(),
	fHists(),
	fhAnnOutput_correct(nullptr),
	fhAnnOutput_wrong(nullptr),
	fhCumProb_correct(nullptr),
	fhCumProb_wrong(nullptr)
{

}

CbmKresTrainAnn::~CbmKresTrainAnn()
{

}

void CbmKresTrainAnn::Init()
{
	InitHistograms();
}

void CbmKresTrainAnn::Exec(int event, int IdForANN, double InvariantMass, double OpeningAngle, double PlaneAngle_last, double ZPos, TVector3 Momentum1, TVector3 Momentum2)
{
	double p1 = TMath::Sqrt(Momentum1.X()*Momentum1.X() + Momentum1.Y()*Momentum1.Y() + Momentum1.Z()*Momentum1.Z());
	double p2 = TMath::Sqrt(Momentum2.X()*Momentum2.X() + Momentum2.Y()*Momentum2.Y() + Momentum2.Z()*Momentum2.Z());
	if(IdForANN == 1){
		//if (IM_correct.size() < fMaxNofTrainSamples){
			IM_correct.push_back(InvariantMass);
			OA_correct.push_back(OpeningAngle);
			Angle_correct.push_back(PlaneAngle_last);
			Z_correct.push_back(ZPos);
			Mom1_correct.push_back(p1);
			Mom2_correct.push_back(p2);
		//}
	} else {
		//if (IM_wrong.size() < fMaxNofTrainSamples){
			IM_wrong.push_back(InvariantMass);
			OA_wrong.push_back(OpeningAngle);
			Angle_wrong.push_back(PlaneAngle_last);
			Z_wrong.push_back(ZPos);
			Mom1_wrong.push_back(p1);
			Mom2_wrong.push_back(p2);
		//}
	}

	if(IM_correct.size()%100 == 0 && IdForANN == 1) cout << "correct = " << IM_correct.size() << ";  wrong = " << IM_wrong.size() << endl;

	//if (IM_correct.size() >= fMaxNofTrainSamples && IM_wrong.size() >= fMaxNofTrainSamples) {
	if (event == 2000 && IM_correct.size() >= fMaxNofTrainSamples) {
		TrainAndTestAnn();
		Draw();

		IM_correct.clear();
		OA_correct.clear();
		Angle_correct.clear();
		Z_correct.clear();
		Mom1_correct.clear();
		Mom2_correct.clear();
		IM_wrong.clear();
		OA_wrong.clear();
		Angle_wrong.clear();
		Z_wrong.clear();
		Mom1_wrong.clear();
		Mom2_wrong.clear();
	}



}

void CbmKresTrainAnn::TrainAndTestAnn()
{
	//cout << "Do TrainAndTestAnn" << endl;
	TTree *simu = new TTree ("MonteCarlo","MontecarloData");
	Double_t x[6];
	Double_t xOut;
	
	simu->Branch("x0", &x[0],"x0/D");
	simu->Branch("x1", &x[1],"x1/D");
	simu->Branch("x2", &x[2],"x2/D");
	simu->Branch("x3", &x[3],"x3/D");
	simu->Branch("x4", &x[4],"x4/D");
	simu->Branch("x5", &x[5],"x5/D");
	simu->Branch("xOut", &xOut,"xOut/D");

	for (size_t i = 0; i < IM_correct.size(); i++){
		x[0] = IM_correct[i] / 0.1;
		x[1] = OA_correct[i] / 30;
		x[2] = Angle_correct[i] / 30;
		x[3] = Z_correct[i] / 100;
		x[4] = Mom1_correct[i] / 5;
		x[5] = Mom2_correct[i] /5 ;

		if (x[0] > 1.0) x[0] = 1.0;
		if (x[1] > 1.0) x[1] = 1.0;
		if (x[2] > 1.0) x[2] = 1.0;
		if (x[3] > 1.0) x[3] = 1.0;
		if (x[4] > 1.0) x[4] = 1.0;
		if (x[5] > 1.0) x[5] = 1.0;

		xOut = 1.;
		simu->Fill();
		if (i >= fMaxNofTrainSamples) break;
	}
	for (size_t i = 0; i < IM_wrong.size(); i++){
		x[0] = IM_wrong[i] / 0.1;
		x[1] = OA_wrong[i] / 30;
		x[2] = Angle_wrong[i] / 30;
		x[3] = Z_wrong[i] / 100;
		x[4] = Mom1_wrong[i] / 5;
		x[5] = Mom2_wrong[i] / 5;

		if (x[0] > 1.0) x[0] = 1.0;
		if (x[1] > 1.0) x[1] = 1.0;
		if (x[2] > 1.0) x[2] = 1.0;
		if (x[3] > 1.0) x[3] = 1.0;
		if (x[4] > 1.0) x[4] = 1.0;
		if (x[5] > 1.0) x[5] = 1.0;

		xOut = -1.;
		simu->Fill();
		if (i >= fMaxNofTrainSamples) break;
	}
	
	TMultiLayerPerceptron network("x0,x1,x2,x3,x4,x5:12:xOut",simu,"Entry$+1");
	network.Train(300,"text,update=10");
	network.DumpWeights("../../../analysis/conversion2/KresAnalysis_ann_weights.txt");

	
	Double_t params[6];
	fNofWrongLikeCorrect = 0;
	fNofCorrectLikeWrong = 0;

	for (size_t i = 0; i < IM_correct.size(); i++){
		params[0] = IM_correct[i] / 0.1;
		params[1] = OA_correct[i] / 30;
		params[2] = Angle_correct[i] / 30;
		params[3] = Z_correct[i] / 100;
		params[4] = Mom1_correct[i] / 5;
		params[5] = Mom2_correct[i] / 5;

		if (params[0] > 1.0) params[0] = 1.0;
		if (params[1] > 1.0) params[1] = 1.0;
		if (params[2] > 1.0) params[2] = 1.0;
		if (params[3] > 1.0) params[3] = 1.0;
		if (params[4] > 1.0) params[4] = 1.0;
		if (params[5] > 1.0) params[5] = 1.0;
		 
		Double_t netEval = network.Evaluate(0,params);
		fhAnnOutput_correct->Fill(netEval);
		if (netEval < fAnnCut) fNofCorrectLikeWrong++;
	}
	for (size_t i = 0; i < IM_wrong.size(); i++){
		params[0] = IM_wrong[i] / 0.1;
		params[1] = OA_wrong[i] / 30;
		params[2] = Angle_wrong[i] / 30;
		params[3] = Z_wrong[i] / 100;
		params[4] = Mom1_wrong[i] / 5;
		params[5] = Mom2_wrong[i] / 5;
 
		if (params[0] > 1.0) params[0] = 1.0;
		if (params[1] > 1.0) params[1] = 1.0;
		if (params[2] > 1.0) params[2] = 1.0;
		if (params[3] > 1.0) params[3] = 1.0;
		if (params[4] > 1.0) params[4] = 1.0;
		if (params[5] > 1.0) params[5] = 1.0;
		  
		Double_t netEval = network.Evaluate(0,params);
		fhAnnOutput_wrong->Fill(netEval);
		if (netEval >= fAnnCut) fNofWrongLikeCorrect++;
	}

}


void CbmKresTrainAnn::Draw()
{
	cout <<"nof correct pairs = " << IM_correct.size() << endl;
	cout <<"nof wrong pairs = " << IM_wrong.size() << endl;
	cout <<"wrong like correct = " << fNofWrongLikeCorrect << ", wrong supp = " << (Double_t) IM_wrong.size() / fNofWrongLikeCorrect << endl;
	cout <<"Correct like wrong = " << fNofCorrectLikeWrong << ", correct lost eff = " << 100.* (Double_t)fNofCorrectLikeWrong / IM_correct.size() << endl;

	Double_t cumProbFake = 0.;
	Double_t cumProbTrue = 0.;
	Int_t nofTrue = (Int_t)fhAnnOutput_correct->GetEntries();
	Int_t nofFake = (Int_t)fhAnnOutput_wrong->GetEntries();

	for (Int_t i = 1; i <= fhAnnOutput_wrong->GetNbinsX(); i++){
		cumProbTrue += fhAnnOutput_correct->GetBinContent(i);
		fhCumProb_correct->SetBinContent(i, 1. - (Double_t)cumProbTrue / nofTrue);

		cumProbFake += fhAnnOutput_wrong->GetBinContent(i);
		fhCumProb_wrong->SetBinContent(i, (Double_t)cumProbFake/nofFake);
	}


	TCanvas * c1 = new TCanvas("ann_correct_ann_output", "ann_correct_ann_output", 400, 400);
        c1->SetTitle("ann_correct_ann_output");
	fhAnnOutput_correct->Draw();

	TCanvas * c2 = new TCanvas("ann_wrong_ann_output", "ann_wrong_ann_output", 400, 400);
        c2->SetTitle("ann_wrong_ann_output");     
	fhAnnOutput_wrong->Draw();

	TCanvas * c3 = new TCanvas("ann_correct_cum_prob", "ann_correct_cum_prob", 400, 400);
        c3->SetTitle("ann_correct_cum_prob");
	fhCumProb_correct->Draw();

	TCanvas * c4 = new TCanvas("ann_wrong_cum_prob", "ann_wrong_cum_prob", 400, 400);
        c4->SetTitle("ann_wrong_cum_prob");
	fhCumProb_wrong->Draw();
}


void CbmKresTrainAnn::InitHistograms()
{

	fhAnnOutput_correct = new TH1D("fhAnnOutput_correct","ANN output;ANN output;Counter",100, -1.2, 1.2);
	fHists.push_back(fhAnnOutput_correct);
	fhAnnOutput_wrong = new TH1D("fhAnnOutput_wrong","ANN output;ANN output;Counter",100, -1.2, 1.2);
	fHists.push_back(fhAnnOutput_wrong);

	fhCumProb_correct = new TH1D("fhCumProb_correct","ANN output;ANN output;Cumulative probability",100, -1.2, 1.2);
	fHists.push_back(fhCumProb_correct);
	fhCumProb_wrong = new TH1D("fhCumProb_wrong","ANN output;ANN output;Cumulative probability",100, -1.2, 1.2);
	fHists.push_back(fhCumProb_wrong);
}
