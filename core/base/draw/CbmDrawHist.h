/**
 * \file CbmDrawHist.h
 * \brief Helper functions for drawing 1D and 2D histograms and graphs.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2008
 **/
#ifndef CBMDRAWHIST_H_
#define CBMDRAWHIST_H_

#include <Rtypes.h>      // for kBlue, kGreen, kOrange, kRed, kYellow, kBlack
#include <RtypesCore.h>  // for Int_t, Double_t, Bool_t
#include <TAttMarker.h>  // for kFullCircle, kFullSquare, kFullTriangleUp

#include <string>  // for string
#include <vector>  // for vector

class TGraph2D;
class TGraph;
class TH1;
class TH2;
class TH2D;
class TH3;

/**
 * \class CbmDrawingOptions
 * \brief Default options for drawing.
 * \author Andrey Lebedev <andrey.lebedev@gsi.de>
 * \date 2012
 */
class CbmDrawingOptions {
public:
  static Int_t Color(Int_t colorIndex) {
    static const Int_t nofColors   = 15;
    static Int_t colors[nofColors] = {kRed,
                                      kBlue + 1,
                                      kGreen + 3,
                                      kMagenta + 4,
                                      kYellow + 2,
                                      kViolet,
                                      kOrange - 5,
                                      kOrange + 3,
                                      kRed - 8,
                                      kPink + 9,
                                      kBlue - 6,
                                      kCyan + 1,
                                      kGreen - 6,
                                      kYellow - 1,
                                      kGray + 1};
    return (colorIndex < nofColors) ? colors[colorIndex] : colorIndex;
  }

  static Int_t LineWidth() { return 2; }

  static Int_t LineStyle(Int_t lineStyleIndex) { return lineStyleIndex + 1; }

  static Int_t MarkerSize() { return 1; }

  static Int_t MarkerStyle(Int_t markerIndex) {
    static const Int_t nofMarkers    = 8;
    static Int_t markers[nofMarkers] = {kOpenCircle,
                                        kOpenSquare,
                                        kOpenTriangleUp,
                                        kOpenDiamond,
                                        kFullCircle,
                                        kFullSquare,
                                        kFullTriangleUp};
    return (markerIndex < nofMarkers) ? markers[markerIndex] : markerIndex;
  }

  static Double_t TextSize() { return 0.06; }
};

/**
 * \enum HistScale
 * \brief Define linear or logarithmic scale for drawing.
 */
enum HistScale {
  kLog    = 0, /**> Linear scale. */
  kLinear = 1  /**> Logarithmic scale */
};


/**
 * \fn SetStyles
 * \brief Set default styles for histograms.
 */
void SetDefaultDrawStyle();

/**
 * \fn DrawH1
 * \brief Draw 1D histogram.
 * \param[in] hist Pointer to histogram.
 * \param[in] logx Specify logarithmic or linear scale for X axis.
 * \param[in] logy Specify logarithmic or linear scale for Y axis.
 * \param[in] drawOpt Other drawing options (see ROOT documentation for details).
 * \param[in] color Color.
 * \param[in] lineWidth Line width.
 * \param[in] lineStyle Line style (see ROOT documentation for details).
 * \param[in] markerSize Marker size.
 * \param[in] markerStyle Marker style (see ROOT documentation for details).
 */
void DrawH1(TH1* hist,
            HistScale logx             = kLinear,
            HistScale logy             = kLinear,
            const std::string& drawOpt = "",
            Int_t color                = CbmDrawingOptions::Color(0),
            Int_t lineWidth            = CbmDrawingOptions::LineWidth(),
            Int_t lineStyle            = CbmDrawingOptions::LineStyle(0),
            Int_t markerSize           = CbmDrawingOptions::MarkerSize(),
            Int_t markerStyle          = CbmDrawingOptions::MarkerStyle(0));


/**
 * \fn DrawH2
 * \brief Draw 2D histogram.
 * \param[in] logx Specify logarithmic or linear scale for X axis.
 * \param[in] logy Specify logarithmic or linear scale for Y axis.
 * \param[in] logz Specify logarithmic or linear scale for Z axis.
 * \param[in] drawOpt Other drawing options (see ROOT documentation for details).
 */
void DrawH2(TH2* hist,
            HistScale logx             = kLinear,
            HistScale logy             = kLinear,
            HistScale logz             = kLinear,
            const std::string& drawOpt = "COLZ");


/**
 * \fn DrawH1
 * \brief Draw several TH1 histograms.
 * \param[in] histos Array of histograms.
 * \param[in] histLabels Array of histogram labels.
 * \param[in] logx Specify logarithmic or linear scale for X axis.
 * \param[in] logy Specify logarithmic or linear scale for Y axis.
 * \param[in] drawLegend If true than legend will be drawn.
 * \param[in] x1 X coordinate of legend in current pad.
 * \param[in] y1 Y coordinate of legend in current pad.
 * \param[in] x2 X coordinate of legend in current pad.
 * \param[in] y2 Y coordinate of legend in current pad.
 * \param[in] drawOpt Draw options for TH1->Draw method.
 */
void DrawH1(const std::vector<TH1*>& histos,
            const std::vector<std::string>& histLabels,
            HistScale logx             = kLinear,
            HistScale logy             = kLinear,
            Bool_t drawLegend          = true,
            Double_t x1                = 0.25,
            Double_t y1                = 0.99,
            Double_t x2                = 0.55,
            Double_t y2                = 0.75,
            const std::string& drawOpt = "");

/**
 * \fn DrawGraph
 * \brief Draw TGraph.
 * \param[in] graph Pointer to TGraph.
 * \param[in] logx Specify logarithmic or linear scale for X axis.
 * \param[in] logy Specify logarithmic or linear scale for Y axis.
 * \param[in] drawOpt Other drawing options (see ROOT documentation for details).
 * \param[in] color Color.
 * \param[in] lineWidth Line width.
 * \param[in] lineStyle Line style (see ROOT documentation for details).
 * \param[in] markerSize Marker size.
 * \param[in] markerStyle Marker style (see ROOT documentation for details).
 */
void DrawGraph(TGraph* graph,
               HistScale logx             = kLinear,
               HistScale logy             = kLinear,
               const std::string& drawOpt = "AC",
               Int_t color                = CbmDrawingOptions::Color(0),
               Int_t lineWidth            = CbmDrawingOptions::LineWidth(),
               Int_t lineStyle            = CbmDrawingOptions::LineStyle(0),
               Int_t markerSize           = CbmDrawingOptions::MarkerSize(),
               Int_t markerStyle          = CbmDrawingOptions::MarkerStyle(0));

/**
 * \fn DrawGraph.
 * \brief Draw upto 3 TGraphs. If graph == nullptr than graph will not be drawn.
 * \param[in] graphs Vector of TGraphs.
 * \param[in] graphLabels Vector of graph labels.
 * \param[in] logx Specify logarithmic or linear scale for X axis.
 * \param[in] logy Specify logarithmic or linear scale for Y axis.
 * \param[in] drawLegend If true than legend will be drawn.
 * \param[in] x1 X coordinate of legend in current pad.
 * \param[in] y1 Y coordinate of legend in current pad.
 * \param[in] x2 X coordinate of legend in current pad.
 * \param[in] y2 Y coordinate of legend in current pad.
 */
void DrawGraph(const std::vector<TGraph*>& graphs,
               const std::vector<std::string>& graphLabels,
               HistScale logx    = kLinear,
               HistScale logy    = kLinear,
               Bool_t drawLegend = true,
               Double_t x1       = 0.50,
               Double_t y1       = 0.78,
               Double_t x2       = 0.99,
               Double_t y2       = 0.99);

/**
 * \fn DrawGraph2D
 * \brief Draw 2D graph.
 * \param[in] graph Pointer to TGraph2D.
 * \param[in] logx Specify logarithmic or linear scale for X axis.
 * \param[in] logy Specify logarithmic or linear scale for Y axis.
 * \param[in] logz Specify logarithmic or linear scale for Z axis.
 * \param[in] drawOpt Other drawing options (see ROOT documentation for details).
 */
void DrawGraph2D(TGraph2D* graph,
                 HistScale logx             = kLinear,
                 HistScale logy             = kLinear,
                 HistScale logz             = kLinear,
                 const std::string& drawOpt = "colz");

/**
 * \fn DrawTextOnPad
 * \brief Draw text on a current pad. Latex expression can be used.
 * \param[in] text Your text. Latex can be also used.
 * \param[in] x1 X1 coordinate of legend in current pad.
 * \param[in] y1 Y1 coordinate of legend in current pad.
 * \param[in] x2 X2 coordinate of legend in current pad.
 * \param[in] y2 Y2 coordinate of legend in current pad.
 */
void DrawTextOnPad(const std::string& text,
                   Double_t x1,
                   Double_t y1,
                   Double_t x2,
                   Double_t y2);


/**
 * \fn DrawH1andFitGauss
 * \brief Draw TH1 and fit with gauss. Draw fit results on top of the histogramm.
 * \param[in] hist Histogram to be drawn and fitted.
 * \param[in] drawResults If true - mean/sigma will be drawn.
 * \param[in] doScale If true - histogramm is scaled to Integral 1.
 * \param[in] userRangeMin SetRangeUser minimum.
 * \param[in] userRangeMax SetRangeUser maximum.
 */
void DrawH1andFitGauss(TH1* hist,
                       Bool_t drawResults    = true,
                       Bool_t doScale        = true,
                       Double_t userRangeMin = 0.,
                       Double_t userRangeMax = 0.);

/**
 * \fn DrawH2WithProfile
 * \brief Draw TH2 and on top of TH2 draw TProfile with mean and RMS (represented as errors).
 * \param[in] hist Histogram to be drawn.
 * \param[in] doGaussFit Set to true if you want to fit each slice with Gauss.
 * \param[in] drawOnlyMean Set to false if you want to draw RMS/sigma as error bars.
 * \param[in] drawOpt Other drawing options for TH2 histogram (see ROOT documentation for details).
 * \param[in] color Color of the H1 Profile histogram.
 * \param[in] lineWidth Line width of the H1 Profile histogram.
 */
void DrawH2WithProfile(TH2* hist,
                       Bool_t doGaussFit          = false,
                       Bool_t drawOnlyMean        = false,
                       const std::string& drawOpt = "COLZ",
                       Int_t profileColor         = kBlack,
                       Int_t profileLineWidth     = 4);

/**
 * \fn DrawH3Profile
 * \brief Draw XY profile of the TH3 histogram.
 * \param[in] h TH3 histogram.
 * \param[in] drawMean Set to true for Mean values, set to false for RMS/Sigma values.
 * \param[in] doGaussFit Set to true if you want to fit each slice with Gauss.
 * \param[in] zMin SetRangeUser minimum for Z axis.
 * \param[in] zMax SetRangeUser minimum for Z axis.
 */
TH2D* DrawH3Profile(TH3* h,
                    Bool_t drawMean        = true,
                    Bool_t doGaussFit      = false,
                    Double_t zUserRangeMin = 0.,
                    Double_t zUserRangeMax = 0.);

#endif
