#include <string>
using namespace std;

double radius = 200; //in pixels
double deltaR = 1.0; // in pixels
double thresholdCoeff = 1.1;

void drawH2(TH2* h);
void drawGraph(TGraph* graph);
int calculateIntensity(TH2* h, double xc, double yc, double r, int threshold);
void drawCircle(double xc, double yc, double r, int color);
void findXYCenter(TH2* h, double &maxX, double &maxY, double threshold);
void runD0ForFile(const string& fileName, bool doDraw);

void run_d0() {
	string file = "/Users/slebedev/Development/cbm/data/d0/test_03_09_15/mirror3/2mm.asc";
	double d0 = runD0ForFile(file, true);

	return;

	string dir = "/Users/slebedev/Development/cbm/data/d0/test_03_09_15/mirror2/";

	const int N = 50;
	double d0Array[N];
	double x[N];

	for (int i = 1; i <= N; i++){
		x[i-1] = i;
		stringstream ss;
		ss << dir << i << "mm.asc";
		cout << "File #" << i << " fileName:" <<  ss.str() << endl;
		double d0 = runD0ForFile(ss.str(), false);
		d0Array[i-1] = d0;
	}

	TCanvas* can = new TCanvas("rich_d0_final", "rich_d0_final", 700, 700);
	TGraph* graph = new TGraph(N, x, d0Array);
	graph->GetXaxis()->SetTitle("distance [mm]");
	graph->GetYaxis()->SetTitle("D0 [mm]");
	drawGraph(graph);

}


double runD0ForFile(const string& fileName, bool doDraw){
   int dimX = 1024;
   int dimY = 1024;

   ifstream fin(fileName.c_str());
   int k;
   int x = 0;
   int y = 0;

   TH2D* hCamera = new TH2D("hCamera", "hCamera;x [pixel];Y [pixel];", dimX, 0, dimX, dimY, 0, dimY);
   while (!fin.eof()){
      x++;
      fin >> k;
      hCamera->SetBinContent(x, y, k);
      if (x == dimX+1){
         x = 0;
         y++;
      }
    }

   int threshold = thresholdCoeff * hCamera->Integral() / (dimX * dimY);
   cout << "threshold:" << threshold << endl;

   double centerX, centerY;
   findXYCenter((TH2D*)hCamera->Clone(), centerX, centerY, threshold);



   if (doDraw) {
	   TCanvas* can1 = new TCanvas("rich_d0_camera", "rich_d0_camera", 700, 600);
	   drawH2((TH2D*)hCamera->Clone());

	   TCanvas* can2 = new TCanvas("rich_d0_camera_threshold", "rich_d0_camera_threshold", 700, 600);
	   hCamera->SetMinimum(threshold);
	   drawH2((TH2D*)hCamera->Clone());

	   TCanvas* can3 = new TCanvas("rich_d0_camera_zoom", "rich_d0_camera_zoom", 700, 600);
	   hCamera->GetXaxis()->SetRangeUser(centerX - 1.2*radius, centerX + 1.2*radius);
	   hCamera->GetYaxis()->SetRangeUser(centerY - 1.2*radius, centerY + 1.2*radius);
	   drawH2((TH2D*)hCamera->Clone());
	   drawCircle(centerX, centerY, radius, kBlack);
   }

   int intensityInit = calculateIntensity(hCamera, centerX, centerY, radius, threshold);
  // cout << "Initial intensity = " << intensityInit << endl;

   int intensity95 = 0.95 * intensityInit;
   double newR = radius;
   for (; ; ){
      int newInt = calculateIntensity(hCamera, centerX, centerY, newR, threshold);
     // cout << "Radius=" <<newR << ", Intensity=" << newInt << ", "
     //       << 100. * (double)newInt / (double)intensityInit <<"%"<< endl;
      if (newInt <= intensity95) break;
      newR -= deltaR;
   }
   drawCircle(centerX, centerY, newR, kBlue);

   newR = 13.3 * newR / dimX;

   if (hCamera != NULL) delete hCamera;

   cout << "D0 (95%) = " << 2*newR << " cm" << endl;
   return 2. * newR;
}

void findXYCenter(TH2* h, double &maxX, double &maxY, double threshold)
{
	int nBins = 1;
	h->RebinX(nBins);
	h->RebinY(nBins);

	int nx = h->GetNbinsX();
	int ny = h->GetNbinsY();
	int sumW = 0;
	double mX = 0;
	double mY = 0;

	for (int ix = 2; ix <= nx - 1; ix++){
		for (int iy = 2; iy <= ny - 1; iy++){
			double binInt = h->GetBinContent(ix, iy);
			if (binInt < threshold) continue;
			mX += binInt * ix;
			mY += binInt * iy;
			sumW += binInt;
		}
	}
	maxX = mX / sumW;
	maxY = mY / sumW;
	cout << "maxX:" << maxX << " maxY:" << maxY << endl;

}

int calculateIntensity(
      TH2* h,
      double xc,
      double yc,
      double r,
	  int threshold)
{
   int nx = h->GetNbinsX();
   int ny = h->GetNbinsY();
   int counter = 0;
   int minIndX = xc - 1.05*radius;
   int maxIndX = xc + 1.05*radius;
   if (minIndX <= 0) minIndX = 1;
   if (maxIndX > nx) maxIndX = nx;

   int minIndY = yc - 1.05*radius;
   int maxIndY = yc + 1.05*radius;
   if (minIndY <= 0) minIndY = 1;
   if (maxIndY > ny) maxIndY = ny;

   for (int ix = minIndX; ix <= maxIndX; ix++){
      for (int iy = minIndY; iy <= maxIndY; iy++){
         int binInt = h->GetBinContent(ix, iy);
         if (binInt < threshold) continue;
         double d = sqrt( (xc-ix)*(xc-ix) + (yc-iy)*(yc-iy) );
         if (d < r) {
            counter += binInt;
         }
      }
   }
   return counter;
}

void drawCircle(
      double xc,
      double yc,
      double r,
      int color)
{
   TEllipse* ellipse = new TEllipse(xc, yc, r);
   ellipse->SetFillStyle(0);
   ellipse->SetLineWidth(2);
   ellipse->SetLineColor(color);
   ellipse->DrawClone();

   TEllipse* ellipse = new TEllipse(xc, yc, 1);
   ellipse->SetFillColor(kBlack);
   ellipse->SetLineColor(kBlack);
   ellipse->DrawClone();
}

void drawH2(TH2* h)
{
   h->Draw("COLZ");
   h->SetStats(false);
   Double_t textSize = 0.06;
   h->GetXaxis()->SetLabelSize(textSize);
   h->GetXaxis()->SetNdivisions(505, kTRUE);
   h->GetYaxis()->SetLabelSize(textSize);
   h->GetYaxis()->SetNdivisions(505, kTRUE);
   h->GetZaxis()->SetLabelSize(textSize);
// hist->GetZaxis()->SetNdivisions(505, kTRUE);
   h->GetXaxis()->SetTitleSize(textSize);
   h->GetYaxis()->SetTitleSize(textSize);
   h->GetZaxis()->SetTitleSize(textSize);
   h->GetXaxis()->SetTitleOffset(1.0);
   h->GetYaxis()->SetTitleOffset(1.3);
   h->GetZaxis()->SetTitleOffset(1.5);
   gPad->SetLeftMargin(0.17);
   gPad->SetRightMargin(0.30);
   gPad->SetBottomMargin(0.15);
   gPad->SetTicks(1, 1);
   gPad->SetGrid(true, true);
}

void drawGraph(TGraph* graph) {
	string drawOpt = "AC";
	Double_t textSize = 0.06;
	graph->SetLineColor(kBlue);
	graph->SetLineWidth(3);
	//graph->SetLineStyle(lineStyle);
	graph->SetMarkerColor(kBlue);
	graph->SetMarkerSize(3);
	graph->SetMarkerStyle(kOpenCircle);
	if (drawOpt.find("A") != string::npos) {
	  graph->GetXaxis()->SetLabelSize(textSize);
	  graph->GetXaxis()->SetNdivisions(505, kTRUE);
	  graph->GetYaxis()->SetLabelSize(textSize);
	  graph->GetXaxis()->SetTitleSize(textSize);
	  graph->GetYaxis()->SetTitleSize(textSize);
	  graph->GetXaxis()->SetTitleOffset(1.0);
	  graph->GetYaxis()->SetTitleOffset(1.3);
	}
	gPad->SetLeftMargin(0.17);
	gPad->SetBottomMargin(0.15);
	graph->Draw(drawOpt.c_str());
	gPad->SetGrid(true, true);
}
