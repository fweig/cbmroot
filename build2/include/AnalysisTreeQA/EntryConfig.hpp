#ifndef ANALYSISTREE_QA_ENTRYCONFIG_H
#define ANALYSISTREE_QA_ENTRYCONFIG_H

#include <string>
#include <vector>

#include <TAxis.h>

#include "AnalysisTree/Cuts.hpp"
#include "AnalysisTree/Utils.hpp"

class TH1;
class TH2;
class TProfile;

namespace AnalysisTree {
namespace QA {

class Axis : public Variable, public TAxis {
 public:
  Axis() = default;
  Axis(const std::string& title, const Variable& var, const TAxis& a) : Variable(var), TAxis(a) {
    this->SetTitle(title.c_str());
    if(this->GetFields().size() == 1 && this->GetFields().at(0).GetName() == "ones"){
//      fields_[0] = Field(fields_[0].GetBranchName(), "ones");
//      fields_.clear();
      this->lambda_ = [](const std::vector<double>& ){ return 1; };
      this->name_ = "Ones";
    }
  }
  const char* GetName() const override { return Variable::GetName().c_str(); }

 protected:
  ClassDefOverride(Axis, 1);
};

class EntryConfig {

  using PlotPointer = ANALYSISTREE_UTILS_VARIANT<TH1*, TH2*, TProfile*>;

 public:
  enum class PlotType : short {
    kHisto1D,
    kHisto2D,
    kProfile,
    kIntegral1D,
    kIntegral2D
  };

  EntryConfig() = default;
  explicit EntryConfig(const Axis& axis, Cuts* cuts = nullptr, bool is_integral = false);
  EntryConfig(const Axis& x, const Axis& y, Cuts* cuts = nullptr, bool is_profile = false);
  EntryConfig(const Axis& x, Cuts* cuts_x, const Axis& y, Cuts* cuts_y);

  EntryConfig(const EntryConfig&) = default;
  EntryConfig(EntryConfig&&) = default;
  EntryConfig& operator=(EntryConfig&&) = default;
  EntryConfig& operator=(const EntryConfig&) = default;
  virtual ~EntryConfig() = default;

  void Fill(double value);
  void Fill(double value1, double value2);
  void Write() const;

  ANALYSISTREE_ATTR_NODISCARD const std::vector<Axis>& GetAxes() const { return axes_; }
  std::vector<Axis>& Axes() { return axes_; }

  ANALYSISTREE_ATTR_NODISCARD unsigned int GetNdimentions() const { return axes_.size(); }
  ANALYSISTREE_ATTR_NODISCARD Cuts* GetEntryCuts() const { return entry_cuts_; }
  ANALYSISTREE_ATTR_NODISCARD PlotType GetType() const { return type_; }

  ANALYSISTREE_ATTR_NODISCARD std::vector<std::pair<int, int>> GetVariablesId() const { return vars_id_; }
  void SetVariablesId(const std::vector<std::pair<int, int>>& var_id) { vars_id_ = var_id; }

  ANALYSISTREE_ATTR_NODISCARD std::vector<Variable> GetVariables() const {
    std::vector<Variable> vars{};
    for (const auto& axis : axes_) {
      vars.emplace_back(axis);
    }
    return vars;
  }

  ANALYSISTREE_ATTR_NODISCARD std::string GetDirectoryName() const;

  void SetOutDir(TDirectory* out_dir) { out_dir_ = out_dir; }

  PlotPointer GetPlot() { return plot_; }

 protected:
  void InitPlot();
  ANALYSISTREE_ATTR_NODISCARD TH1* CreateHisto1D() const;
  ANALYSISTREE_ATTR_NODISCARD TH2* CreateHisto2D() const;
  ANALYSISTREE_ATTR_NODISCARD TProfile* CreateProfile() const;
  void Set2DName();

  PlotPointer plot_;
  std::string name_;///< Name of the plot
  std::string title_;
  PlotType type_{PlotType(-1)};

  std::vector<Axis> axes_{};
  Cuts* entry_cuts_{nullptr};
  std::vector<std::pair<int, int>> vars_id_{};

  TDirectory* out_dir_{nullptr};
  ClassDef(EntryConfig, 1);
};

}// namespace QA
}// namespace AnalysisTree
#endif//ANALYSISTREE_QA_ENTRYCONFIG_H
