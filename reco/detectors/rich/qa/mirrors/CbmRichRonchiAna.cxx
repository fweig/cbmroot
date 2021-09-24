/* Copyright (C) 2018-2019 GSI/JINR-LIT, Darmstadt/Dubna
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Semen Lebedev [committer] */

#include "CbmRichRonchiAna.h"

#include <boost/gil/extension/io/tiff.hpp>
#include <boost/gil/gil_all.hpp>
//#include <boost/gil/extension/io/tiff_dynamic_io.hpp>


#include "CbmDrawHist.h"

#include "TCanvas.h"
#include "TEllipse.h"
#include "TGeoArb8.h"
#include "TGeoManager.h"
#include "TH2D.h"
#include "TH3D.h"
#include "TLine.h"
#include "TVector3.h"

#include <iostream>
#include <set>

#include <math.h>

using namespace boost::gil;
using namespace std;

// For Ubuntu one needs dev version of libtiff
// sudo apt-get install libtiff-dev

CbmRichRonchiAna::CbmRichRonchiAna()
  :  // constructor

  // constant values
  fPi(3.141592654)
  , fRadiusMirror(3000000)
  ,  // in microns   !!! MIGHT HAVE TO BE CHANGED TO DISTANCE_MIRROR-POINTSOURCE IF IT IS NOT IN THE CENTER OF CURVATURE
  fEdgeLengthCCD(13300)
  ,  // in microns
  fCcdPixelSize(13)
  ,  // in microns
  fPitchGrid(200)
  ,  // in microns
  fImageWidth(1024)
  ,  // in pixels

  // values to be measured first
  fDistRulingCCD(21200)
  ,  // in microns
  fDistMirrorCCD(3118500)
  ,  // in microns
  fDistMirrorRuling(fDistMirrorCCD - fDistRulingCCD)
  ,  // in microns
  fOffsetCCDOptAxisX(0)
  ,  //(7500),                           // in microns
  fOffsetCCDOptAxisY(-58000)
  ,  // in microns
  fOffsetLEDOpticalAxisX(0)
  ,  //(14500), //(13000),                      // in microns
  fOffsetLEDOpticalAxisY(57000)
  ,  // in microns
  fCenterCcdX(0)
  ,               // in pixels
  fCenterCcdY(0)  // in pixels
{
}

CbmRichRonchiAna::~CbmRichRonchiAna() {}

void CbmRichRonchiAna::Run()
{
  // Initialization
  vector<vector<double>> dataV;
  vector<vector<double>> dataH;

  if (fTiffFileNameV == "" || fTiffFileNameH == "") { Fatal("CbmRichRonchiAna::Run:", "No FileNameV or FileNameH!"); }
  else {
    cout << "FileNameV: " << fTiffFileNameV << endl << "FileNameH: " << fTiffFileNameH << endl;
    dataV = ReadTiffFile(fTiffFileNameV);
    dataH = ReadTiffFile(fTiffFileNameH);
  }

  int width  = dataV.size();
  int height = dataV[0].size();


  SetDefaultDrawStyle();
  // initialisierung der histogramme: name, groesse usw
  TH2D* hInitH =
    new TH2D("hInitH", "hInitH;X [pixel];Y [pixel];Intensity", width, -.5, width - 0.5, height, -0.5, height - 0.5);
  TH2D* hMeanIntensityH = new TH2D("hMeanIntensityH", "hMeanIntensityH;X [pixel];Y [pixel];Intensity", width, -.5,
                                   width - 0.5, height, -0.5, height - 0.5);
  TH2D* hPeakH =
    new TH2D("hPeakH", "hPeakH;X [pixel];Y [pixel];Intensity", width, -.5, width - 0.5, height, -0.5, height - 0.5);
  TH2D* hSmoothLinesH = new TH2D("hSmoothLinesH", "hSmoothLinesH;X [pixel];Y [pixel];Intensity", width, -.5,
                                 width - 0.5, height, -0.5, height - 0.5);
  TH2D* hLineSearchH  = new TH2D("hLineSearchH", "hLineSearchH;X [pixel];Y [pixel];Intensity", width, -.5, width - 0.5,
                                height, -0.5, height - 0.5);

  TH2D* hInitV =
    new TH2D("hInitV", "hInitV;X [pixel];Y [pixel];Intensity", width, -.5, width - 0.5, height, -0.5, height - 0.5);
  TH2D* hMeanIntensityV = new TH2D("hMeanIntensityV", "hMeanIntensityV;X [pixel];Y [pixel];Intensity", width, -.5,
                                   width - 0.5, height, -0.5, height - 0.5);
  TH2D* hPeakV =
    new TH2D("hPeakV", "hPeakV;X [pixel];Y [pixel];Intensity", width, -.5, width - 0.5, height, -0.5, height - 0.5);
  TH2D* hSmoothLinesV = new TH2D("hSmoothLinesV", "hSmoothLinesV;X [pixel];Y [pixel];Intensity", width, -.5,
                                 width - 0.5, height, -0.5, height - 0.5);
  TH2D* hLineSearchV  = new TH2D("hLineSearchV", "hLineSearchV;X [pixel];Y [pixel];Intensity", width, -.5, width - 0.5,
                                height, -0.5, height - 0.5);

  TH2D* hSuperpose = new TH2D("hSuperpose", "hSuperpose;X [pixel];Y [pixel];Intensity", width, -.5, width - 0.5, height,
                              -0.5, height - 0.5);

  // vertical image
  DoRotation(dataV);
  FillH2WithVector(hInitV, dataV);
  DoMeanIntensityY(dataV);
  FillH2WithVector(hMeanIntensityV, dataV);
  DoPeakFinderY(dataV);
  FillH2WithVector(hPeakV, dataV);
  DoSmoothLines(dataV);
  FillH2WithVector(hSmoothLinesV, dataV);
  DoLineSearch(dataV);
  FillH2WithVector(hLineSearchV, dataV);
  DoRotation(dataV);


  // horizontal image
  FillH2WithVector(hInitH, dataH);
  DoMeanIntensityY(dataH);
  FillH2WithVector(hMeanIntensityH, dataH);
  DoPeakFinderY(dataH);
  FillH2WithVector(hPeakH, dataH);
  DoSmoothLines(dataH);
  FillH2WithVector(hSmoothLinesH, dataH);
  DoLineSearch(dataH);
  FillH2WithVector(hLineSearchH, dataH);

  // finding intersections
  vector<CbmRichRonchiIntersectionData> intersections = DoIntersection(dataH, dataV);
  vector<vector<double>> dataSup                      = DoSuperpose(dataH, dataV);
  FillH2WithVector(hSuperpose, dataSup);

  DoOrderLines(intersections, "x");
  DoOrderLines(intersections, "y");

  DoLocalNormal(intersections);
  DoSphere(intersections);

  // DrawGeomanager();

  DoDeviation(intersections);

  {
    TCanvas* c = new TCanvas("ronchi_2d_horizontal", "ronchi_2d_horizontal", 1500, 1000);
    c->Divide(3, 2);
    c->cd(1);
    DrawH2(hInitH);
    c->cd(2);
    DrawH2(hMeanIntensityH);
    c->cd(3);
    DrawH2(hPeakH);
    c->cd(4);
    DrawH2(hSmoothLinesH);
    c->cd(5);
    DrawH2(hLineSearchH);
  }

  {
    TCanvas* c = new TCanvas("ronchi_2d_vertical", "ronchi_2d_vertical", 1500, 1000);
    c->Divide(3, 2);
    c->cd(1);
    DrawH2(hInitV);
    c->cd(2);
    DrawH2(hMeanIntensityV);
    c->cd(3);
    DrawH2(hPeakV);
    c->cd(4);
    DrawH2(hSmoothLinesV);
    c->cd(5);
    DrawH2(hLineSearchV);
  }

  {
    TCanvas* c2 = new TCanvas("ronchi_1d_slices_horizontal", "ronchi_1d_slices_horizontal", 1200, 600);
    c2->Divide(2, 1);

    TH1D* h1  = hInitH->ProjectionY("_py2", 250, 250);
    TH1D* h2  = hInitH->ProjectionY("_py3", 300, 300);
    TH1D* hM1 = hMeanIntensityH->ProjectionY("_pyM1", 250, 250);
    TH1D* hM2 = hMeanIntensityH->ProjectionY("_pyM2", 300, 300);
    TH1D* hP1 = hPeakH->ProjectionY("_pyP1", 250, 250);
    TH1D* hP2 = hPeakH->ProjectionY("_pyP2", 300, 300);
    c2->cd(1);
    DrawH1({h1, hM1, hP1}, {"Init", "Mean", "Peak"});
    c2->cd(2);
    DrawH1({h2, hM2, hP2}, {"Init", "Mean", "Peak"});
  }


  {
    TCanvas* c = new TCanvas("ronchi_2d_intersection", "ronchi_2d_intersection", 1000, 1000);
    DrawH2(hSuperpose);
    for (size_t i = 0; i < intersections.size(); i++) {
      TEllipse* center = new TEllipse(intersections[i].fPixelX, intersections[i].fPixelY, 5);
      center->Draw();
    }
  }

  vector<int> colors = {kBlack, kGreen, kBlue, kRed, kYellow, kOrange, kCyan, kGray, kMagenta, kYellow + 2, kRed + 3};
  {
    TCanvas* c = new TCanvas("ronchi_2d_intersection_x", "ronchi_2d_intersection_x", 1000, 1000);
    DrawH2(hSuperpose);
    for (size_t i = 0; i < intersections.size(); i++) {
      TEllipse* center = new TEllipse(intersections[i].fPixelX, intersections[i].fPixelY, 5);
      center->SetFillColor(colors[intersections[i].fOrderedLineX % colors.size()]);
      center->Draw();
    }
  }

  {
    TCanvas* c = new TCanvas("ronchi_2d_intersection_y", "ronchi_2d_intersection_y", 1000, 1000);
    DrawH2(hSuperpose);
    for (size_t i = 0; i < intersections.size(); i++) {
      TEllipse* center = new TEllipse(intersections[i].fPixelX, intersections[i].fPixelY, 5);
      center->SetFillColor(colors[intersections[i].fOrderedLineY % colors.size()]);
      center->Draw();
    }
  }
}

vector<vector<double>> CbmRichRonchiAna::ReadTiffFile(const string& fileName)
{
  cout << "ReadTiffFile:" << fileName << endl;
  vector<vector<double>> data;
  //rgba8_image_t img;
  rgb16_image_t img;
  read_and_convert_image(fileName, img, boost::gil::tiff_tag());
  // tiff_read_and_convert_image(fileName, img);
  int height = img.height();
  int width  = img.width();

  data.resize(width);

  auto view = const_view(img);
  for (int x = 0; x < width; ++x) {
    auto it = view.col_begin(x);
    data[x].resize(height);
    for (int y = 0; y < height; ++y) {
      int r = boost::gil::at_c<0>(it[y]);
      //int g = boost::gil::at_c<1>(it[y]);
      //int b = boost::gil::at_c<2>(it[y]);
      //int a = boost::gil::at_c<3>(it[y]);
      data[x][y] = r;
    }
  }
  return data;
}

// rotating the image (actually flipping diagonally arranged corners)
void CbmRichRonchiAna::DoRotation(vector<vector<double>>& data)
{
  int nX = data.size();
  int nY = data[0].size();
  for (int x = 0; x < nX; x++) {
    for (int y = x + 1; y < nY; y++) {
      swap(data[x][y], data[y][x]);
    }
  }
}

// drawing histogram
void CbmRichRonchiAna::FillH2WithVector(TH2* hist, const vector<vector<double>>& data)
{
  int nX = data.size();
  int nY = data[0].size();

  for (int x = 0; x < nX; x++) {
    for (int y = 0; y < nY; y++) {
      if (data[x][y] != 0) { hist->SetBinContent(x, y, data[x][y]); }
    }
  }
}

// averaging intensity with neighboured values
void CbmRichRonchiAna::DoMeanIntensityY(vector<vector<double>>& data)
{
  int nX = data.size();
  int nY = data[0].size();

  int halfAvWindow = 5;  // number of neighbour pixels to each side to average over
  double threshold =
    6500;  // threshold for average intensity to delete areas that are too dark (and so don't belong to mirror image)
  vector<double> weightsX = {1., 0.75, 0.4, 0.2, 0.08, 0.04};  // weightings in dependence of distance of current pixel
  vector<double> weightsY = {1., 0.75, 0.4, 0.2, 0.08, 0.04};

  vector<vector<double>> dataNew(nX, std::vector<double>(nY, 0.));

  for (int x = 0; x < nX; x++) {
    for (int y = 0; y < nY; y++) {
      double total     = 0.;
      double weightSum = 0.;
      for (int xW = -halfAvWindow; xW <= halfAvWindow;
           xW++) {  // scanning window around current pixel to calculate average intensity
        for (int yW = -halfAvWindow; yW <= halfAvWindow; yW++) {

          int xWAbs = std::abs(xW);
          int yWAbs = std::abs(yW);

          double weightX =
            (xWAbs < (int) weightsX.size())
              ? weightsX[xWAbs]
              : weightsX[weightsX.size()
                         - 1];  // if corresponding pixel is farther away than number of weights, take smallest ...
          double weightY =
            (yWAbs < (int) weightsY.size()) ? weightsY[yWAbs] : weightsY[weightsY.size() - 1];  // ... weight value

          double weight = weightX * weightY;
          weightSum += weight;
          int indX = x + xW;
          if (indX < 0) indX = 0;  // preventing counting not existing pixels (beyond image)
          if (indX >= nX) indX = nX - 1;
          int indY = y + yW;
          if (indY < 0) indY = 0;
          if (indY >= nY) indY = nY - 1;

          total += data[indX][indY] * weight;
        }
      }

      dataNew[x][y] = total / weightSum;  // averaged intensity
      if (dataNew[x][y] <= threshold)
        dataNew[x][y] = 0.;  // deleting pixels that are too dark; to yield only data for mirror and not background
    }
  }
  data = dataNew;
}

// getting a one dimensional line
void CbmRichRonchiAna::DoPeakFinderY(vector<vector<double>>& data)
{
  int nX                 = data.size();
  int nY                 = data[0].size();
  int halfWindow         = 5;
  int samePeakCounterCut = 5;

  vector<vector<double>> dataNew(nX, std::vector<double>(nY, 0.));

  for (int x = 0; x < nX; x++) {
    for (int y = halfWindow; y < nY - halfWindow; y++) {
      bool isPeak = (data[x][y] > 0. && data[x][y] >= data[x][y - 1])
                    && (data[x][y] >= data[x][y + 1]);  // comparing current pixel with direct neighbours in y direction
      if (!isPeak) continue;

      // check if it is a plateau
      int samePeakCounter = 0;
      for (int yS = y; yS < nY; yS++) {
        if (data[x][y] == data[x][yS]) { samePeakCounter++; }
        else {
          break;
        }
      }
      if (samePeakCounter >= samePeakCounterCut) {  // if plateau, then jump beyond it
        y = y + samePeakCounter + 1;
        continue;
      }

      bool isBiggest = true;
      for (int iW = -halfWindow; iW <= halfWindow; iW++) {  // comparing current pixel with 'halfWindow' next neighbours
        if (iW == 0) continue;
        if (data[x][y + iW] > data[x][y]) {
          isBiggest = false;
          break;
        }
      }
      bool hasNeighbourPeak =
        (dataNew[x][y - 2] > 0. || dataNew[x][y - 1] > 0. || dataNew[x][y + 1] > 0. || dataNew[x][y + 2] > 0.);

      if (isBiggest && isPeak && !hasNeighbourPeak)
        dataNew[x][y] =
          data[x][y];  // set value if is the biggest of 'halfWindow' neighbours and has no neighboured peak
    }
  }
  data = dataNew;
}

// smoothing lines by calculating mean position in y
void CbmRichRonchiAna::DoSmoothLines(vector<vector<double>>& data)
{
  int meanHalfLength = 8;
  int meanHalfHeight = 3;
  int nX             = data.size();
  int nY             = data[0].size();

  vector<vector<double>> dataNew(nX, std::vector<double>(nY, 0.));

  for (int x = meanHalfLength; x < nX - meanHalfLength; x++) {  // scanning whole image (except small border)
    for (int y = meanHalfHeight; y < nY - meanHalfHeight; y++) {
      if (data[x][y] == 0.) continue;
      double sumY = 0.;
      int counter = 0;
      for (
        int x2 = -meanHalfLength; x2 <= meanHalfLength;
        x2++) {  // scanning small window around current pixel and summing up positions in y of current and neighboured pixels
        for (int y2 = -meanHalfHeight; y2 <= meanHalfHeight; y2++) {
          if (data[x + x2][y + y2] > 0) {
            sumY += y + y2;
            counter++;
          }
        }
      }
      int newY         = (int) sumY / counter;  // average y-value of pixel
      dataNew[x][newY] = data[x][y];
    }
  }

  data = dataNew;
}

void CbmRichRonchiAna::DoLineSearch(vector<vector<double>>& data)
{
  int nX = data.size();
  int nY = data[0].size();

  int missingCounterCut      = 15;
  int missingCounterTotalCut = 25;
  int halfWindowY            = 2;

  vector<vector<double>> dataNew(nX, std::vector<double>(nY, 0.));
  double curIndex = 0.;  // why double?

  for (int x = 0; x < nX; x++) {  // scanning whole image
    for (int y = 0; y < nY; y++) {
      if (data[x][y] > 0. && dataNew[x][y] == 0.) {  // if this line pixel is not filled yet in 'dataNew'
        curIndex++;                                  // line index
        dataNew[x][y]           = curIndex;          // found line is indexed
        int missingCounterTotal = 0.;
        int missingCounter      = 0.;
        int curY                = y;
        for (int x1 = x + 1; x1 < nX; x1++) {  // drawing line in x; values are not intensity but line index
          bool isFound = false;
          for (int y1 = -halfWindowY; y1 <= halfWindowY; y1++) {
            if (data[x1][curY + y1] > 0.
                && dataNew[x1][curY + y1] == 0.) {  // if pixel of line in next column is found ...
              dataNew[x1][curY + y1] = curIndex;    // ... fill current pixel with line index
              curY += y1;
              isFound = true;
              break;
            }
          }
          if (
            !isFound) {  // to prevent indexing lines with wrong index (e.g. if gap is too big and next found line would not be continuation of current line)
            missingCounterTotal++;
            missingCounter++;
          }
          else {
            missingCounter = 0;
          }
          if (missingCounterTotal >= missingCounterTotalCut || missingCounter >= missingCounterCut) break;
        }
      }
    }
  }
  cout << "curIndex:" << curIndex << endl;
  data = dataNew;
}


// finding intersections
vector<CbmRichRonchiIntersectionData> CbmRichRonchiAna::DoIntersection(vector<vector<double>>& dataH,
                                                                       const vector<vector<double>>& dataV)
{
  int nX = dataV.size();
  int nY = dataV[0].size();

  vector<CbmRichRonchiIntersectionData> intersections;

  for (int x = 0; x < nX; x++) {
    for (int y = 0; y < nY; y++) {
      if (dataH[x][y] > 0.
          && dataV[x][y]
               > 0.) {  // filling vector with data if both vertical and horizontal image have data greater ZERO here
        CbmRichRonchiIntersectionData data;
        data.fPixelX = x;
        data.fPixelY = y;
        data.fLineY =
          dataH[x][y];  // 'data[x][y]' contains the line index now, not intensity (see prev. function 'DoLineSearch')
        data.fLineX = dataV[x][y];
        intersections.push_back(data);
      }
    }
  }
  cout << "intersections.size(): " << intersections.size() << endl;

  // remove close intersections
  set<int> removeSet;
  for (size_t i1 = 0; i1 < intersections.size(); i1++) {
    for (size_t i2 = i1 + 1; i2 < intersections.size(); i2++) {
      double dX = intersections[i1].fPixelX
                  - intersections[i2].fPixelX;  // distances in x and y of current observed intersections
      double dY   = intersections[i1].fPixelY - intersections[i2].fPixelY;
      double dist = std::sqrt(dX * dX + dY * dY);
      if (dist <= 10.) {  // if their distance is below a certain threshold, insert content to 'removeSet'
        //                cout << i1 << " " << i2 << " " << intersections[i1].fPixelX <<  " " << intersections[i1].fPixelY << " "
        //                        << intersections[i2].fPixelX <<  " " << intersections[i2].fPixelY << " " << dist << endl;
        removeSet.insert(i2);  // set of values from one of both close intersections is stored in 'removeSet'
      }
    }
  }

  set<int>::iterator it;
  for (it = removeSet.begin(); it != removeSet.end(); ++it) {
    swap(intersections[*it],
         intersections
           [intersections.size()
            - 1]);  // move one of both double counted intersections to the end of vector and remove these afterwards
    intersections.resize(intersections.size() - 1);
  }
  cout << "removeSet.size():" << removeSet.size() << " intersections.size(): " << intersections.size() << endl;

  return intersections;
}

// superposing horizontal and vertical image
vector<vector<double>> CbmRichRonchiAna::DoSuperpose(const vector<vector<double>>& dataH,
                                                     const vector<vector<double>>& dataV)
{
  int nX = dataV.size();
  int nY = dataV[0].size();

  vector<vector<double>> dataSup(nX, std::vector<double>(nY, 0.));
  for (int x = 0; x < nX; x++) {
    for (int y = 0; y < nY; y++) {
      dataSup[x][y] = dataH[x][y] + dataV[x][y];  // data of horizontal and vertical image are being summed
    }
  }
  return dataSup;
}

void CbmRichRonchiAna::DoOrderLines(vector<CbmRichRonchiIntersectionData>& intersections, const string& option)
{
  map<int, CbmRichRonchiLineData> linesMap;  // lineIndex to lineData
  vector<CbmRichRonchiLineData> lines;

  // indexing intersection points with either 'x' or 'y' and assign values from class 'Cbm...LineData' and storing in map
  for (auto const& curIntr : intersections) {
    int lineInd =
      (option == "x") ? curIntr.fLineX : curIntr.fLineY;  // fLineX/Y is the line index, set in 'DoLineSearch'
    linesMap[lineInd].fMeanPrimary +=
      (option == "x") ? curIntr.fPixelX : curIntr.fPixelY;  // fPixelX/Y are the x/y-values, set in 'DoIntersection')
    linesMap[lineInd].fMeanSecondary += (option == "x") ? curIntr.fPixelY : curIntr.fPixelX;
    linesMap[lineInd].fNofPoints++;  //fNofPoints is never set back?
    linesMap[lineInd].fLineInd = lineInd;
  }

  for (auto& kv : linesMap) {
    kv.second.fMeanPrimary = (double) kv.second.fMeanPrimary
                             / kv.second.fNofPoints;  // calculating mean values of x and y positions to enable sorting
    kv.second.fMeanSecondary = (double) kv.second.fMeanSecondary / kv.second.fNofPoints;
    lines.push_back(kv.second);
    //cout << kv.first << " mean:" << kv.second.fMean << " nPoints:" << kv.second.fNofPoints << " lineInd:" << kv.second.fLineInd<< endl;
  }

  sort(lines.begin(), lines.end(), [](const CbmRichRonchiLineData& lhs, const CbmRichRonchiLineData& rhs) {
    return lhs.fMeanPrimary < rhs.fMeanPrimary;
  });

  // find first line with many points
  int lineIndMany = 0;
  for (size_t i = 0; i < lines.size() - 1; i++) {
    if (lines[i].fNofPoints >= 35) {
      lineIndMany = i;  //dataSup
      break;
    }
  }

  // for the left edges we need to go other direction
  for (int i = lineIndMany - 1; i >= 1; i--) {
    if (AreTwoSegmentsSameLine(&lines[i], &lines[i - 1])) {
      UpdateIntersectionLineInd(intersections, &lines[i], &lines[i - 1], option);
      i--;  // skip next line
    }
  }

  for (size_t i = lineIndMany; i < lines.size() - 1; i++) {
    if (AreTwoSegmentsSameLine(&lines[i], &lines[i + 1])) {
      UpdateIntersectionLineInd(intersections, &lines[i], &lines[i + 1], option);
      i++;  // skip next line
    }
  }

  // now we need to assign numbers in correct order
  int newLineIndex = 1;
  set<int> duplicateLines;  // duplicates can occure for line segments
  for (auto& curLine : lines) {
    if (duplicateLines.find(curLine.fLineInd) != duplicateLines.end()) continue;
    duplicateLines.insert(curLine.fLineInd);
    for (auto& curIntr : intersections) {
      if (option == "x" && curIntr.fLineX == curLine.fLineInd) curIntr.fOrderedLineX = newLineIndex;
      if (option == "y" && curIntr.fLineY == curLine.fLineInd) curIntr.fOrderedLineY = newLineIndex;
    }
    newLineIndex++;
  }
  cout << "newLineIndex:" << newLineIndex << endl;
}

bool CbmRichRonchiAna::AreTwoSegmentsSameLine(const CbmRichRonchiLineData* line1, const CbmRichRonchiLineData* line2)
{
  int nofPointsMergeCut    = 25;
  double pixelDistMergeCut = 200.;
  double pixelDiffMergeCut = 15.;

  return (line1->fNofPoints < nofPointsMergeCut && line2->fNofPoints < nofPointsMergeCut
          && abs(line1->fMeanSecondary - line2->fMeanSecondary) > pixelDistMergeCut
          && abs(line1->fMeanPrimary - line2->fMeanPrimary) <= pixelDiffMergeCut);
}

void CbmRichRonchiAna::UpdateIntersectionLineInd(vector<CbmRichRonchiIntersectionData>& intersections,
                                                 CbmRichRonchiLineData* line1, CbmRichRonchiLineData* line2,
                                                 const string& option)
{
  for (auto& curIntr : intersections) {
    if (option == "x" && curIntr.fLineX == line2->fLineInd) curIntr.fLineX = line1->fLineInd;
    if (option == "y" && curIntr.fLineY == line2->fLineInd) curIntr.fLineY = line1->fLineInd;
  }
  line2->fLineInd = line1->fLineInd;
}

void CbmRichRonchiAna::DoLocalNormal(vector<CbmRichRonchiIntersectionData>& data)
{
  int xMin = 1000;
  int xMax = -1000;
  int yMin = 1000;
  int yMax = -1000;

  // extracting minimal and maximum x and y values
  for (size_t i = 0; i < data.size(); i++) {
    if (data[i].fOrderedLineX < xMin) xMin = data[i].fOrderedLineX;
    if (data[i].fOrderedLineX > xMax) xMax = data[i].fOrderedLineX;
    if (data[i].fOrderedLineY < yMin) yMin = data[i].fOrderedLineY;
    if (data[i].fOrderedLineY > yMax) yMax = data[i].fOrderedLineY;
  }

  int centerLineX = (xMax - xMin) / 2;  // horizontal and vertical
  int centerLineY = (yMax - yMin) / 2;

  fCenterCcdX = -1;
  fCenterCcdY = -1;
  // defining reference point to calculate slopes
  for (size_t i = 0; i < data.size(); i++) {
    if (fCenterCcdX == -1 && data[i].fOrderedLineX == centerLineX) {
      fCenterCcdX = data[i].fPixelX + 20;  // correction parameter in X
    }
    if (fCenterCcdY == -1 && data[i].fOrderedLineY == centerLineY) {
      fCenterCcdY = data[i].fPixelY + 20;  // correction parameter in Y
    }
  }

  cout << "xMin:" << xMin << " xMax:" << xMax << " yMin:" << yMin << " yMax:" << yMax << endl;
  cout << "centerLineX:" << centerLineX << " centerLineY:" << centerLineY << endl;
  cout << "fCenterCcdX:" << fCenterCcdX << " fCenterCcdY:" << fCenterCcdY << endl;

  for (size_t i = 0; i < data.size(); i++) {
    // X and Y positions on CCD in microns
    int ccdX = (data[i].fPixelX - fCenterCcdX) * fCcdPixelSize;
    int ccdY = (data[i].fPixelY - fCenterCcdY) * fCcdPixelSize;

    data[i].fCcdV.SetXYZ(ccdX, ccdY, 0.);

    // XYZ positions on ronchi ruling in microns
    data[i].fRulingV.SetXYZ((data[i].fOrderedLineX - centerLineX) * fPitchGrid,
                            (data[i].fOrderedLineY - centerLineY) * fPitchGrid, fDistRulingCCD);

    // slopes in X and Y direction of reflected beam
    double sRefX =
      -(data[i].fRulingV.X() - data[i].fCcdV.X()) / fDistRulingCCD;  // 'minus' because Ruling is behind CoC
    double sRefY = -(data[i].fRulingV.Y() - data[i].fCcdV.Y()) / fDistRulingCCD;

    // extrapolating X and Y positions on mirror in microns
    data[i].fMirrorV.SetXYZ(ccdX + (sRefX * fDistMirrorRuling), ccdY + (sRefY * fDistMirrorRuling), fDistMirrorCCD);

    //cout << "fMirrorV_xyz = [" << data[i].fMirrorV.X() << ", " << data[i].fMirrorV.Y() << ", " << data[i].fMirrorV.Z() << "]" << endl;

    // calculating angles between optical axis of mirror and incident resp. reflected beam
    double mirrorCenterDist = sqrt(pow(data[i].fMirrorV.X() + fOffsetCCDOptAxisX, 2)
                                   + pow(data[i].fMirrorV.Y() + fOffsetCCDOptAxisY,
                                         2));  // distance from mirrors center in microns
    double sagitta =
      fRadiusMirror - sqrt(pow(fRadiusMirror, 2) - pow(mirrorCenterDist, 2));  // to calculate cathetus length
    double cathetus = fRadiusMirror - sagitta;  // if point source is in the center of curvature !!

    double angleIncX =
      atan(data[i].fMirrorV.X()
           / cathetus);  // if point source is in the center of curvature (CoC), otherwise add offset in Z direction !!
    double angleIncY    = atan((data[i].fMirrorV.Y() - sqrt(fOffsetLEDOpticalAxisY * fOffsetLEDOpticalAxisY)
                             - sqrt(fOffsetCCDOptAxisY * fOffsetCCDOptAxisY))
                            / cathetus);
    double angleRefX    = atan(sRefX);
    double angleRefY    = atan(sRefY);
    data[i].fNormalRadX = ((angleIncX + angleRefX) / 2.0);
    data[i].fNormalRadY = ((angleIncY + angleRefY) / 2.0);

    // data[i].fNormalX = tan(angleNormalX);
    // data[i].fNormalY = tan(angleNormalY);

    double segmentSize = 3500;  // half segment size; for the moment just assume that all segments have the same size
    data[i].fTL.SetXYZ(data[i].fMirrorV.X() - segmentSize, data[i].fMirrorV.Y() + segmentSize, data[i].fMirrorV.Z());
    RotatePointImpl(&data[i].fTL, &data[i].fTLRot, data[i].fNormalRadX, data[i].fNormalRadY, &data[i].fMirrorV);

    data[i].fTR.SetXYZ(data[i].fMirrorV.X() + segmentSize, data[i].fMirrorV.Y() + segmentSize, data[i].fMirrorV.Z());
    RotatePointImpl(&data[i].fTR, &data[i].fTRRot, data[i].fNormalRadX, data[i].fNormalRadY, &data[i].fMirrorV);

    data[i].fBL.SetXYZ(data[i].fMirrorV.X() - segmentSize, data[i].fMirrorV.Y() - segmentSize, data[i].fMirrorV.Z());
    RotatePointImpl(&data[i].fBL, &data[i].fBLRot, data[i].fNormalRadX, data[i].fNormalRadY, &data[i].fMirrorV);

    data[i].fBR.SetXYZ(data[i].fMirrorV.X() + segmentSize, data[i].fMirrorV.Y() - segmentSize, data[i].fMirrorV.Z());
    RotatePointImpl(&data[i].fBR, &data[i].fBRRot, data[i].fNormalRadX, data[i].fNormalRadY, &data[i].fMirrorV);
  }

  /*// correction value to ZERO reference point at center of mirror
    for (int i = 0; i < data.size(); i++) {
        if (data[i].fOrderedLineX == centerLineX && data[i].fOrderedLineY == centerLineY) {
            double radius = sqrt(pow(data[i].fTL.X(),2) + pow(data[i].fTL.Y(),2));
            double sagitta2 = fRadiusMirror - sqrt(pow(fRadiusMirror,2) - pow(radius,2));
            fCorrection = (double)(data[i].fTL.Z() - sagitta2 - fRadiusMirror);
            cout << "fTL.Z_Sagitta = " << fRadiusMirror - data[i].fTL.Z() << ", sagitta2 = " << sagitta2 << endl;
            cout << "CORRECTION = " << (double)fCorrection << endl << endl;
        }
    }*/

  {
    DrawXYMum(data);
  }

  {
    TCanvas* c = new TCanvas("ronchi_xz_mum_lineY20", "ronchi_xz_mum_lineY20", 1800, 900);
    c->Divide(2, 1);
    c->cd(1);
    DrawXZProjection(data, 20, 1.);
    c->cd(2);
    DrawXZProjection(data, 20, 0.025);
  }

  {
    DrawMirrorSegments(data, 20, 20);
  }
}

// constructing spherical surface
void CbmRichRonchiAna::DoSphere(vector<CbmRichRonchiIntersectionData>& data)
{
  int xMin = 1000;
  int xMax = -1000;
  int yMin = 1000;
  int yMax = -1000;

  // extracting minimal and maximum x and y values
  for (size_t i = 0; i < data.size(); i++) {
    if (data[i].fOrderedLineX < xMin) xMin = data[i].fOrderedLineX;
    if (data[i].fOrderedLineX > xMax) xMax = data[i].fOrderedLineX;
    if (data[i].fOrderedLineY < yMin) yMin = data[i].fOrderedLineY;
    if (data[i].fOrderedLineY > yMax) yMax = data[i].fOrderedLineY;
  }

  // assigning segments to their position on mirror plane (according to their values after rotation, before sphere construction)
  // in X direction
  for (int iX = xMin; iX <= xMax; iX++) {
    int iPX  = GetMinIndexForLineX(iX,
                                  data);  // 'GetMinIndex..' to get index of 'data' for current intersection
    int iPX0 = GetMinIndexForLineX(iX - 1, data);
    if (iPX < 0 || iPX0 < 0) continue;
    double shiftX = data[iPX0].fTRRot.X() - data[iPX].fTLRot.X();  // horizontal distance between neighboured corners
    data[iPX].fTLRot.SetXYZ(data[iPX].fTLRot.X() + shiftX, data[iPX].fTLRot.Y(), data[iPX].fTLRot.Z());
    data[iPX].fTRRot.SetXYZ(data[iPX].fTRRot.X() + shiftX, data[iPX].fTRRot.Y(), data[iPX].fTRRot.Z());
    data[iPX].fBLRot.SetXYZ(data[iPX].fBLRot.X() + shiftX, data[iPX].fBLRot.Y(), data[iPX].fBLRot.Z());
    data[iPX].fBRRot.SetXYZ(data[iPX].fBRRot.X() + shiftX, data[iPX].fBRRot.Y(), data[iPX].fBRRot.Z());
  }

  // in Y direction
  for (int iY = yMin; iY <= yMax; iY++) {
    int iPY  = GetMinIndexForLineY(iY, data);
    int iPY0 = GetMinIndexForLineY(iY - 1, data);
    if (iPY < 0 || iPY0 < 0) continue;
    double shiftY = data[iPY0].fTRRot.Y() - data[iPY].fBRRot.Y();
    data[iPY].fTLRot.SetXYZ(data[iPY].fTLRot.X(), data[iPY].fTLRot.Y() + shiftY, data[iPY].fTLRot.Z());
    data[iPY].fTRRot.SetXYZ(data[iPY].fTRRot.X(), data[iPY].fTRRot.Y() + shiftY, data[iPY].fTRRot.Z());
    data[iPY].fBLRot.SetXYZ(data[iPY].fBLRot.X(), data[iPY].fBLRot.Y() + shiftY, data[iPY].fBLRot.Z());
    data[iPY].fBRRot.SetXYZ(data[iPY].fBRRot.X(), data[iPY].fBRRot.Y() + shiftY, data[iPY].fBRRot.Z());
  }

  DrawMirrorSegments(data, 32, 32);

  // inserting X and Y shifts in f..Sph vectors
  for (int iX = xMin; iX <= xMax; iX++) {
    int iPX = GetMinIndexForLineX(iX, data);
    if (iPX < 0) continue;
    for (int iY = yMin; iY <= yMax; iY++) {
      int iC = GetIndexForLineXLineY(iX, iY, data);
      if (iC < 0) continue;
      int iPY = GetMinIndexForLineY(iY, data);
      if (iPY < 0) continue;

      double shiftX = data[iPX].fTLRot.X() - data[iC].fTLRot.X();
      double shiftY = data[iPY].fTLRot.Y() - data[iC].fTLRot.Y();

      data[iC].fTLSph.SetXYZ(data[iC].fTLRot.X() + shiftX, data[iC].fTLRot.Y() + shiftY, data[iC].fTLRot.Z());
      data[iC].fTRSph.SetXYZ(data[iC].fTRRot.X() + shiftX, data[iC].fTRRot.Y() + shiftY, data[iC].fTRRot.Z());
      data[iC].fBLSph.SetXYZ(data[iC].fBLRot.X() + shiftX, data[iC].fBLRot.Y() + shiftY, data[iC].fBLRot.Z());
      data[iC].fBRSph.SetXYZ(data[iC].fBRRot.X() + shiftX, data[iC].fBRRot.Y() + shiftY, data[iC].fBRRot.Z());
    }
  }

  // align Z positions in Y
  for (int iX = xMin; iX <= xMax; iX++) {
    for (int iY = yMin; iY <= yMax; iY++) {
      int iC = GetIndexForLineXLineY(iX, iY, data);
      //int iPX = GetIndexForLineXLineY(iX - 1, iY, data);
      int iPY = GetIndexForLineXLineY(iX, iY - 1, data);
      if (iPY < 0) iPY = GetIndexForLineXLineY(iX, iY - 2, data);  // one missing measurement is allowed
      if (iC < 0) continue;

      if (iPY >= 0) {
        //double shiftZAdd = (iPX > 0)?data[iPX].fTRSph.Z() - data[iC].fTLSph.Z():0;
        double shiftZ = data[iPY].fTLSph.Z() - data[iC].fBLSph.Z();
        data[iC].fTLSph.SetZ(data[iC].fTLSph.Z() + shiftZ);
        data[iC].fTRSph.SetZ(data[iC].fTRSph.Z() + shiftZ);
        data[iC].fBLSph.SetZ(data[iC].fBLSph.Z() + shiftZ);
        data[iC].fBRSph.SetZ(data[iC].fBRSph.Z() + shiftZ);
      }
    }
  }

  // align Z positions in X
  for (int iX = xMin; iX <= xMax; iX++) {
    int iPX  = GetIndexForLineXLineY(iX, 30, data);
    int iPX0 = GetIndexForLineXLineY(iX - 1, 30, data);
    if (iPX0 < 0) iPX0 = GetIndexForLineXLineY(iX - 2, 30, data);  // one missing measurement is allowed
    if (iPX < 0 || iPX0 < 0) continue;
    double shiftZ = data[iPX0].fTRSph.Z() - data[iPX].fTLSph.Z();
    for (int iY = yMin; iY <= yMax; iY++) {
      int iC = GetIndexForLineXLineY(iX, iY, data);
      data[iC].fTLSph.SetZ(data[iC].fTLSph.Z() + shiftZ);
      data[iC].fTRSph.SetZ(data[iC].fTRSph.Z() + shiftZ);
      data[iC].fBLSph.SetZ(data[iC].fBLSph.Z() + shiftZ);
      data[iC].fBRSph.SetZ(data[iC].fBRSph.Z() + shiftZ);
    }
  }

  {
    DrawMirrorSegmentsSphereAll(data);
    DrawMirrorSegmentsSphere(data, 21, 21);
    DrawMirrorSegmentsSphere(data, 30, 30);
    DrawMirrorSegmentsSphere(data, 40, 40);
  }
}

int CbmRichRonchiAna::GetMinIndexForLineX(int lineX, vector<CbmRichRonchiIntersectionData>& data)
{
  int ind = -1;
  for (int iY = 0; iY < 2000; iY++) {
    ind = GetIndexForLineXLineY(lineX, iY, data);
    if (ind != -1) return ind;
  }
  return ind;
}


int CbmRichRonchiAna::GetMinIndexForLineY(int lineY, vector<CbmRichRonchiIntersectionData>& data)
{
  int ind = -1;
  for (int iX = 0; iX < 2000; iX++) {
    ind = GetIndexForLineXLineY(iX, lineY, data);
    if (ind != -1) return ind;
  }
  return ind;
}


int CbmRichRonchiAna::GetIndexForLineXLineY(int lineX, int lineY, vector<CbmRichRonchiIntersectionData>& data)
{
  for (size_t i = 0; i < data.size(); i++) {
    if (data[i].fOrderedLineX == lineX && data[i].fOrderedLineY == lineY)
      return i;  // returns 'data' index of currently investigated intersection
  }
  return -1;
}

void CbmRichRonchiAna::DoDeviation(vector<CbmRichRonchiIntersectionData>& data)
{
  double mirX = 0.;
  double mirY = 0.;
  double mirZ = 0.;

  int correctionValue = 11800;
  int threshold       = 0;  //-50000;

  for (size_t i = 0; i < data.size(); i++) {
    double meanX = 0.25 * (data[i].fTLSph.X() + data[i].fTRSph.X() + data[i].fBLSph.X() + data[i].fBRSph.X());
    double meanY = 0.25 * (data[i].fTLSph.Y() + data[i].fTRSph.Y() + data[i].fBLSph.Y() + data[i].fBRSph.Y());
    double meanZ = 0.25 * (data[i].fTLSph.Z() + data[i].fTRSph.Z() + data[i].fBLSph.Z() + data[i].fBRSph.Z());
    double dX    = mirX - meanX;
    double dY    = mirY - meanY;
    double dZ    = mirZ - meanZ;
    double d     = sqrt(dZ * dZ);  //sqrt(dX*dX + dY*dY + dZ*dZ);

    // cout << "dX/dY/dZ = " << dX << "/" << dY << "/" << dZ << endl;
    // cout << "d = " << d << endl;

    //double mirrorCenterDist = sqrt(pow(data[i].fMirrorV.X(), 2) + pow(data[i].fMirrorV.Y(), 2));   // distance from mirrors center in microns
    double mirrorCenterDist = sqrt(pow(meanX, 2) + pow(meanY, 2));  // distance from mirrors center in microns
    double sagitta          = fRadiusMirror - sqrt(pow(fRadiusMirror, 2) - pow(mirrorCenterDist, 2));
    //data[i].fDeviation = d  - fRadiusMirror + sagitta;

    data[i].fDeviation = d - (fDistMirrorCCD - sagitta);  // (Ist-Soll) !! change to fDistMIrrorCCD
  }
  TH2D* hMirrorHeight =
    new TH2D("hMirrorDeviation", "hMirrorDeviation;index X;index Y;Deviation [mum]", 60, -0.5, 59.5, 60, -0.5, 59.5);
  TCanvas* c = new TCanvas("mirror_deviation", "mirror_deviation", 1000, 1000);
  for (size_t i = 0; i < data.size(); i++) {
    if (data[i].fDeviation > threshold) {
      hMirrorHeight->SetBinContent(data[i].fOrderedLineX, data[i].fOrderedLineY, data[i].fDeviation - correctionValue);
    }
  }
  DrawH2(hMirrorHeight);
}

void CbmRichRonchiAna::DrawXYMum(vector<CbmRichRonchiIntersectionData>& data)
{
  vector<int> colors = {kBlack, kGreen, kBlue, kRed, kYellow, kOrange, kCyan, kGray, kMagenta, kYellow + 2, kRed + 3};
  TH2D* hCcdXY       = new TH2D("hCcdXY", "hCcdXY;CCD_X [mum];CCD_Y [mum];", 1, -7000, 7000, 1, -7000, 7000);
  TH2D* hRulingXY = new TH2D("hRulingXY", "hRulingXY;Ruling_X [mum];Ruling_Y [mum];", 1, -7000, 7000, 1, -7000, 7000);
  TH2D* hMirrorXY =
    new TH2D("hMirrorXY", "hMirrorXY;Mirror_X [mum];Mirror_Y [mum];", 1, -250000, 250000, 1, -250000, 250000);
  TCanvas* c = new TCanvas("ronchi_xy_mum", "ronchi_xy_mum", 1800, 600);
  c->Divide(3, 1);
  c->cd(1);
  DrawH2(hCcdXY);
  gPad->SetRightMargin(0.10);
  for (size_t i = 0; i < data.size(); i++) {
    TEllipse* center = new TEllipse(data[i].fCcdV.X(), data[i].fCcdV.Y(), 50);
    center->SetFillColor(colors[data[i].fOrderedLineX % colors.size()]);
    center->Draw();
  }

  c->cd(2);
  DrawH2(hRulingXY);
  gPad->SetRightMargin(0.10);
  for (size_t i = 0; i < data.size(); i++) {
    TEllipse* center = new TEllipse(data[i].fRulingV.X(), data[i].fRulingV.Y(), 50);
    center->SetFillColor(colors[data[i].fOrderedLineX % colors.size()]);
    center->Draw();
  }

  c->cd(3);
  DrawH2(hMirrorXY);
  gPad->SetRightMargin(0.10);
  for (size_t i = 0; i < data.size(); i++) {
    TEllipse* center = new TEllipse(data[i].fMirrorV.X(), data[i].fMirrorV.Y(), 2500);
    center->SetFillColor(colors[data[i].fOrderedLineX % colors.size()]);
    center->Draw();
  }
}

void CbmRichRonchiAna::DrawXZProjection(vector<CbmRichRonchiIntersectionData>& data, int orderedLineY, double scale)
{
  string histName = "hZX_lineY" + to_string(orderedLineY) + "_scale" + to_string(scale);

  TH2D* hZX =
    new TH2D(histName.c_str(), (histName + ";Z [mum];X [mum];").c_str(), 100, -0.02 * scale * fDistMirrorRuling,
             1.05 * scale * fDistMirrorRuling, 100, scale * -250000, scale * 250000);
  DrawH2(hZX);
  gPad->SetRightMargin(0.10);
  for (size_t i = 0; i < data.size(); i++) {
    if (data[i].fOrderedLineY != orderedLineY) continue;

    TEllipse* ccdEllipse = new TEllipse(data[i].fCcdV.Z(), data[i].fCcdV.X(), scale * 20000, scale * 2000);
    ccdEllipse->SetFillColor(kRed);
    ccdEllipse->Draw();

    TEllipse* rulingEllipse = new TEllipse(data[i].fRulingV.Z(), data[i].fRulingV.X(), scale * 20000, scale * 2000);
    rulingEllipse->SetFillColor(kBlue);
    rulingEllipse->Draw();

    TEllipse* mirrorEllipse = new TEllipse(data[i].fMirrorV.Z(), data[i].fMirrorV.X(), scale * 20000, scale * 2000);
    mirrorEllipse->SetFillColor(kGreen);
    mirrorEllipse->Draw();

    TLine* rulingLine = new TLine(data[i].fCcdV.Z(), data[i].fCcdV.X(), data[i].fRulingV.Z(), data[i].fRulingV.X());
    rulingLine->Draw();

    TLine* mirrorLine =
      new TLine(data[i].fRulingV.Z(), data[i].fRulingV.X(), data[i].fMirrorV.Z(), data[i].fMirrorV.X());
    mirrorLine->Draw();

    double xNorm          = data[i].fMirrorV.X() - fDistMirrorRuling * tan(data[i].fNormalRadX);
    TLine* mirrorLineNorm = new TLine(fDistRulingCCD, xNorm, data[i].fMirrorV.Z(), data[i].fMirrorV.X());
    mirrorLineNorm->SetLineColor(kBlue);
    mirrorLineNorm->Draw();
  }
}


void CbmRichRonchiAna::DrawMirrorSegments(vector<CbmRichRonchiIntersectionData>& data, int orderedLineX,
                                          int orderedLineY)
{
  string cName = "ronchi_mirror_segments_rot_lineX" + to_string(orderedLineX) + "_lineY" + to_string(orderedLineY);
  TCanvas* c   = new TCanvas(cName.c_str(), cName.c_str(), 1800, 900);
  c->Divide(2, 1);

  string histNameXY = "hXY_mirror_segments_rot_lineX" + to_string(orderedLineX) + "_lineY" + to_string(orderedLineY);
  TH2D* hXY =
    new TH2D(histNameXY.c_str(), (histNameXY + "hXY;X [mum];Y [mum];").c_str(), 1, -250000, 250000, 1, -250000, 250000);
  vector<int> colors = {kBlack, kGreen, kBlue, kRed, kYellow, kOrange, kCyan, kGray, kMagenta, kYellow + 2, kRed + 3};
  c->cd(1);
  DrawH2(hXY);  // boxes
  gPad->SetRightMargin(0.10);
  for (size_t i = 0; i < data.size(); i++) {
    int color = colors[data[i].fOrderedLineY % colors.size()];
    DrawOneMirrorSegment(data[i].fTLRot, data[i].fTRRot, data[i].fBLRot, data[i].fBRRot, color);
  }

  double hSize      = 250000;
  string histNameZX = "hZX_mirror_segments_lrot_ineX" + to_string(orderedLineX) + "_lineY" + to_string(orderedLineY);
  TH2D* hZX = new TH2D(histNameZX.c_str(), (histNameZX + ";Z [mum];X [mum];").c_str(), 100, fDistMirrorRuling - hSize,
                       fDistMirrorRuling + hSize, 100, -1. * hSize, hSize);
  c->cd(2);
  DrawH2(hZX);
  gPad->SetRightMargin(0.10);
  for (size_t i = 0; i < data.size(); i++) {
    if (data[i].fOrderedLineY != orderedLineY) continue;

    TEllipse* mirrorEllipse = new TEllipse(data[i].fMirrorV.Z(), data[i].fMirrorV.X(), 2000, 2000);
    mirrorEllipse->SetFillColor(kGreen);
    mirrorEllipse->Draw();

    TLine* rulingLine = new TLine(data[i].fCcdV.Z(), data[i].fCcdV.X(), data[i].fRulingV.Z(), data[i].fRulingV.X());
    rulingLine->Draw();

    TLine* mirrorLine =
      new TLine(data[i].fRulingV.Z(), data[i].fRulingV.X(), data[i].fMirrorV.Z(), data[i].fMirrorV.X());
    mirrorLine->Draw();

    double xNorm          = data[i].fMirrorV.X() - fDistMirrorRuling * tan(data[i].fNormalRadX);
    TLine* mirrorLineNorm = new TLine(fDistRulingCCD, xNorm, data[i].fMirrorV.Z(), data[i].fMirrorV.X());
    mirrorLineNorm->SetLineColor(kBlue);
    mirrorLineNorm->Draw();

    TLine* mirrorLineSeg = new TLine(data[i].fTRRot.Z(), data[i].fTRRot.X(), data[i].fTLRot.Z(), data[i].fTLRot.X());
    mirrorLineSeg->SetLineColor(kRed);
    mirrorLineSeg->Draw();
  }
}

void CbmRichRonchiAna::DrawMirrorSegmentsSphereAll(vector<CbmRichRonchiIntersectionData>& data)
{
  string cName = "ronchi_mirror_segments_sphere_all";
  TCanvas* c   = new TCanvas(cName.c_str(), cName.c_str(), 900, 900);

  string histNameXY = "hXY_mirror_segments_sphere_all";
  TH2D* hXY =
    new TH2D(histNameXY.c_str(), (histNameXY + "hXY;X [mum];Y [mum];").c_str(), 1, -250000, 250000, 1, -250000, 250000);
  vector<int> colors = {kBlack, kGreen, kBlue, kRed, kYellow, kOrange, kCyan, kGray, kMagenta, kYellow + 2, kRed + 3};
  DrawH2(hXY);  // boxes
  gPad->SetRightMargin(0.10);
  for (size_t i = 0; i < data.size(); i++) {
    int color = colors[data[i].fOrderedLineY % colors.size()];
    DrawOneMirrorSegment(data[i].fTLSph, data[i].fTRSph, data[i].fBLSph, data[i].fBRSph, color);
  }
}

void CbmRichRonchiAna::DrawMirrorSegmentsSphere(vector<CbmRichRonchiIntersectionData>& data, int orderedLineX,
                                                int orderedLineY)
{
  string cName = "ronchi_mirror_segments_sphere_lineX" + to_string(orderedLineX) + "_lineY" + to_string(orderedLineY);
  TCanvas* c   = new TCanvas(cName.c_str(), cName.c_str(), 1800, 900);
  c->Divide(2, 1);

  string histNameXY = "hXY_mirror_segments_sphere_lineX" + to_string(orderedLineX) + "_lineY" + to_string(orderedLineY);
  TH2D* hXY =
    new TH2D(histNameXY.c_str(), (histNameXY + "hXY;X [mum];Y [mum];").c_str(), 1, -250000, 250000, 1, -250000, 250000);
  vector<int> colors = {kBlack, kGreen, kBlue, kRed, kYellow, kOrange, kCyan, kGray, kMagenta, kYellow + 2, kRed + 3};
  c->cd(1);
  DrawH2(hXY);  // boxes
  gPad->SetRightMargin(0.10);
  for (size_t i = 0; i < data.size(); i++) {
    if (data[i].fOrderedLineX == orderedLineX)
      DrawOneMirrorSegment(data[i].fTLSph, data[i].fTRSph, data[i].fBLSph, data[i].fBRSph, kBlue);
    if (data[i].fOrderedLineY == orderedLineY)
      DrawOneMirrorSegment(data[i].fTLSph, data[i].fTRSph, data[i].fBLSph, data[i].fBRSph, kRed);
  }

  double hSize      = 20000;
  string histNameZX = "hZX_mirror_segments_sphere_lineX" + to_string(orderedLineX) + "_lineY" + to_string(orderedLineY);
  TH2D* hZX         = new TH2D(histNameZX.c_str(), (histNameZX + ";Z [mum];X(Y) [mum];").c_str(), 100,
                       data[0].fBRSph.Z() - 0.25 * hSize, data[0].fBRSph.Z() + hSize, 100, -1. * 250000, 250000);
  c->cd(2);
  DrawH2(hZX);
  gPad->SetRightMargin(0.10);
  for (size_t i = 0; i < data.size(); i++) {
    if (data[i].fOrderedLineX == orderedLineX) {
      TLine* mirrorLineSeg = new TLine(data[i].fBRSph.Z(), data[i].fBRSph.Y(), data[i].fTRSph.Z(), data[i].fTRSph.Y());
      mirrorLineSeg->SetLineColor(kBlue);
      mirrorLineSeg->Draw();
    }

    if (data[i].fOrderedLineY == orderedLineY) {
      TLine* mirrorLineSeg = new TLine(data[i].fTRSph.Z(), data[i].fTRSph.X(), data[i].fTLSph.Z(), data[i].fTLSph.X());
      mirrorLineSeg->SetLineColor(kRed);
      mirrorLineSeg->Draw();
    }
  }
}

// void CbmRichRonchiAna::DrawMirrorSegmentsSphere(vector<CbmRichRonchiIntersectionData> &data, int orderedLineX, int orderedLineY)
// {
//     string cName = "ronchi_mirror_segments_sphere_lineX" + to_string(orderedLineX) + "_lineY" + to_string(orderedLineY);
//     TCanvas *c = new TCanvas(cName.c_str(), cName.c_str(), 1800, 900);
//     c->Divide(2, 1);

//     string histNameXY = "hXY_mirror_segments_sphere_lineX" + to_string(orderedLineX) + "_lineY" + to_string(orderedLineY);
//     TH2D *hXY = new TH2D(histNameXY.c_str(), (histNameXY + "hXY;X [mum];Y [mum];").c_str(), 1, -250000, 250000, 1, -250000, 250000);
//     vector<int> colors = {kBlack, kGreen, kBlue, kRed, kYellow, kOrange, kCyan, kGray, kMagenta, kYellow + 2, kRed + 3};
//     c->cd(1);
//     DrawH2(hXY); // boxes
//     gPad->SetRightMargin(0.10);
//     for (size_t i = 0; i < data.size(); i++){
//         if (data[i].fOrderedLineX == orderedLineX) DrawOneMirrorSegment(data[i].fTLSph, data[i].fTRSph, data[i].fBLSph, data[i].fBRSph, kBlue);
//         if (data[i].fOrderedLineY == orderedLineY) DrawOneMirrorSegment(data[i].fTLSph, data[i].fTRSph, data[i].fBLSph, data[i].fBRSph, kRed);
//     }

//     double hSize = 10000;
//     int corr = 9400;

//     string histNameZX = "hZX_mirror_segments_sphere_lineX" + to_string(orderedLineX) + "_lineY" + to_string(orderedLineY);
//     TH2D *hZX = new TH2D(histNameZX.c_str(), (histNameZX + ";Z [mum];X(Y) [mum];").c_str(), 100, data[0].fBRSph.Z()-(fDistMirrorCCD) - 1.0*hSize, data[0].fBRSph.Z()-(fDistMirrorCCD) + 1.0*hSize, 100, -1. * 250000, 250000);
//     c->cd(2);
//     DrawH2(hZX);
//     gPad->SetRightMargin(0.10);
//     for (size_t i = 0; i < data.size(); i++) {

//         double meanX = 0.25 * ( data[i].fTLSph.X() + data[i].fTRSph.X() + data[i].fBLSph.X() + data[i].fBRSph.X() );
//         double meanY = 0.25 * ( data[i].fTLSph.Y() + data[i].fTRSph.Y() + data[i].fBLSph.Y() + data[i].fBRSph.Y() );
//         double meanZ = 0.25 * ( data[i].fTLSph.Z() + data[i].fTRSph.Z() + data[i].fBLSph.Z() + data[i].fBRSph.Z() );
//         double mirrorCenterDist = sqrt(pow(meanX, 2) + pow(meanY-fOffsetCCDOptAxisY, 2));   // distance from mirrors center in microns
//         double sagitta          = fRadiusMirror - sqrt(pow(fRadiusMirror, 2) - pow(mirrorCenterDist, 2));
//         double zIdeal           = (fDistMirrorCCD) - sagitta;

//         if (data[i].fOrderedLineX == orderedLineX) {
//             //TLine *mirrorLineSeg = new TLine(data[i].fBRSph.Z(), data[i].fBRSph.Y(), data[i].fTRSph.Z(), data[i].fTRSph.Y());
//             TLine *mirrorLineSeg = new TLine(data[i].fBRSph.Z()-zIdeal-corr, data[i].fBRSph.Y(), data[i].fTRSph.Z()-zIdeal-corr, data[i].fTRSph.Y());
//             mirrorLineSeg->SetLineColor(kBlue);
//             mirrorLineSeg->Draw();
//         }

//         if (data[i].fOrderedLineY == orderedLineY) {
//             TLine *mirrorLineSeg = new TLine(data[i].fTRSph.Z()-zIdeal, data[i].fTRSph.X(), data[i].fTLSph.Z()-zIdeal, data[i].fTLSph.X());
//             mirrorLineSeg->SetLineColor(kRed);
//             mirrorLineSeg->Draw();
//         }
//     }
// }

void CbmRichRonchiAna::DrawOneMirrorSegment(const TVector3& tl, const TVector3& tr, const TVector3& bl,
                                            const TVector3& br, int color)
{
  TLine* line1 = new TLine(tl.X(), tl.Y(), tr.X(), tr.Y());
  line1->SetLineColor(color);
  line1->Draw();

  TLine* line2 = new TLine(tr.X(), tr.Y(), br.X(), br.Y());
  line2->SetLineColor(color);
  line2->Draw();

  TLine* line3 = new TLine(br.X(), br.Y(), bl.X(), bl.Y());
  line3->SetLineColor(color);
  line3->Draw();

  TLine* line4 = new TLine(bl.X(), bl.Y(), tl.X(), tl.Y());
  line4->SetLineColor(color);
  line4->Draw();
}


void CbmRichRonchiAna::RotatePointImpl(TVector3* inPos, TVector3* outPos, Double_t rotX, Double_t rotY, TVector3* cV)
{
  double x = inPos->X() - cV->X();
  double y = inPos->Y() - cV->Y();
  double z = inPos->Z() - cV->Z();

  double sinY = sin(rotY);
  double cosY = cos(rotY);
  double sinX = sin(rotX);
  double cosX = cos(rotX);

  double xNew = x * cosX - y * sinX * sinY + z * cosY * sinX + cV->X();
  double yNew = y * cosY + z * sinY + cV->Y();
  double zNew = -x * sinX - y * sinY * cosX + z * cosY * cosX + cV->Z();

  outPos->SetXYZ(xNew, yNew, zNew);
}

ClassImp(CbmRichRonchiAna)
