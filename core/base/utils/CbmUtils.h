#ifndef CBMUTILS_H_
#define CBMUTILS_H_

#include <sstream>  // for string, stringstream
#include <vector>   // for vector

class TCanvas;
class TH1;
class TH1D;
class TH2;
class TH2D;

namespace Cbm {

  template<class T>
  std::string ToString(const T& value) {
    std::stringstream ss;
    ss << (T) value;
    return ss.str();
  }

  template<class T>
  std::string NumberToString(const T& value, int precision = 1) {
    // First determine number of digits in float
    std::string digis = ToString<int>(value);
    int ndigis        = digis.size();

    std::stringstream ss;
    ss.precision(ndigis + precision);
    ss << value;
    return ss.str();
  }

  /* Returns -1 if x<0, +1 if x>0, 0 if x==0 */
  template<class T>
  int Sign(const T& x) {
    static const T ZERO = 0;
    return (x > ZERO) ? 1 : ((x < ZERO) ? -1 : 0);
  }

  void SaveCanvasAsImage(TCanvas* c,
                         const std::string& dir,
                         const std::string& option = "eps;png;gif");

  std::string FindAndReplace(const std::string& name,
                             const std::string& oldSubstr,
                             const std::string& newSubstr);

  std::vector<std::string> Split(const std::string& name, char delimiter);

  /*
 * \brief Divide 1D histograms and return result histogram h = h1 / h2.
 * \param[in] h1 Pointer to the first histogram.
 * \param[in] h2 Pointer to the second histogram.
 * \param[in] histName Name of the result histogram. if histName = "" then histName = h1->GetName() + "_divide"
 * \param[in] scale Scale factor of result histogram.
 * \param[in] titleYaxis Y axis title of result histogram.
 */
  TH1D* DivideH1(TH1* h1,
                 TH1* h2,
                 const std::string& histName   = "",
                 double scale                  = 100.,
                 const std::string& titleYaxis = "Efficiency [%]");


  /*
 * \brief Divide 2D histograms and return result histogram h = h1 / h2.
 * \param[in] h1 Pointer to the first histogram.
 * \param[in] h2 Pointer to the second histogram.
 * \param[in] histName Name of the result histogram. if histName = "" then histName = h1->GetName() + "_divide"
 * \param[in] scale Scale factor of result histogram.
 * \param[in] titleZaxis Z axis title of result histogram.
 */
  TH2D* DivideH2(TH2* h1,
                 TH2* h2,
                 const std::string& histName   = "",
                 double scale                  = 100.,
                 const std::string& titleZaxis = "Efficiency [%]");

}  // namespace Cbm

#endif /* CBMUTILS_H_ */
