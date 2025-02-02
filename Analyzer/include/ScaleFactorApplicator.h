#pragma once
#include "LEAF/Analyzer/include/AnalysisModule.h"
#include "LEAF/Analyzer/include/Config.h"
#include "LEAF/Analyzer/include/useful_functions.h"
#include "LEAF/Analyzer/include/constants.h"
#include <TH1D.h>
#include <TH2D.h>
#include <TGraphAsymmErrors.h>


class ScaleFactorApplicator: public AnalysisModule<RecoEvent> {
public:
  explicit ScaleFactorApplicator();
  explicit ScaleFactorApplicator(const Config& cfg, TString year, TString infilename, TString histname);
  virtual ~ScaleFactorApplicator() = default;

  // virtual bool process(RecoEvent & event){};
  virtual void load_histogram(TString year, TString infilename, TString histname);
  void load_histogram(TString infilename, TString histname);
  void reset();
  int find_bin(double val, TAxis* axis);
  void set_bin(double val);
  void set_bin(double valx, double valy);
  double get_scalefactor();
  double get_uncertainty();
  double get_uncertainty_up();
  double get_uncertainty_down();

protected:

  std::unique_ptr<TFile> m_infile;
  std::unique_ptr<TH1D> m_hist_1d;
  std::unique_ptr<TH2D> m_hist_2d;
  std::unique_ptr<TGraphAsymmErrors> m_graph_1d;

  bool m_is_2d, m_is_graph;
  double m_factor_uncertainty;
  int m_bin_x;
  int m_bin_y;
  int m_bin_graph;

};
