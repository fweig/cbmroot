/* Copyright (C) 2022 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer]*/
#include "TimingsFormat.h"

#include <iomanip>
#include <sstream>

#include <fmt/format.h>
#include <xpu/host.h>

namespace cbm::algo
{

  class TimingsFormat {

  public:
    void Begin(size_t align)
    {
      fAlign = align;
      fSS    = {};
    }

    void Title(std::string_view title) { fSS << fmt::format("{:<{}}\n", title, fAlign); }

    std::string Finalize() { return fSS.str(); }

    void Fmt(const xpu::timings& t)
    {
      fIndent += 2;
      Measurement("Memcpy(h2d)", t.copy(xpu::h2d), t.throughput_copy(xpu::h2d));
      NewLine();
      Measurement("Memcpy(d2h)", t.copy(xpu::d2h), t.throughput_copy(xpu::d2h));
      NewLine();
      Measurement("Memset", t.memset(), t.throughput_memset());
      NewLine();

      for (xpu::timings& st : t.children()) {
        if (st.kernels().empty()) {
          Measurement(st.name(), st.wall(), st.throughput());
          NewLine();
        }
        else {
          Title(st.name());
          Fmt(st);
          NewLine();
        }
      }

      for (xpu::kernel_timings& kt : t.kernels()) {
        Measurement(KernelName(kt), kt.total(), kt.throughput());
        NewLine();
      }

      Measurement("Kernel time", t.kernel_time(), t.throughput_kernels());
      NewLine();
      Measurement("Wall time", t.wall(), t.throughput());
      fIndent -= 2;
    }

    void FmtSubtimers(const xpu::timings& t)
    {
      const auto subtimes = t.children();
      for (auto it = subtimes.begin(); it != subtimes.end(); ++it) {
        Title(it->name());
        Fmt(*it);
        if (std::next(it) != subtimes.end()) { NewLine(); }
      }
    }

    void FmtSummary(const xpu::timings& t)
    {
      fIndent += 2;
      Measurement("Memcpy(h2d)", t.copy(xpu::h2d), t.throughput_copy(xpu::h2d));
      NewLine();
      Measurement("Memcpy(d2h)", t.copy(xpu::d2h), t.throughput_copy(xpu::d2h));
      NewLine();
      Measurement("Memset", t.memset(), t.throughput_memset());
      NewLine();
      Measurement("Kernel time", t.kernel_time(), t.throughput_kernels());
      NewLine();
      Measurement("Wall time", t.wall(), t.throughput());
      fIndent -= 2;
    }

    void NewLine() { fSS << "\n"; }

  private:
    void Measurement(std::string_view name, f64 time, f64 throughput)
    {
      fSS << std::setw(fIndent) << std::setfill(' ') << std::right << "";
      fSS << std::setw(fAlign) << std::setfill(' ') << std::left << fmt::format("{}:", name);
      Real(time, 10, 3, "ms");
      if (std::isnormal(throughput)) {
        fSS << " (";
        Real(throughput, 7, 3, "GB/s");
        fSS << ")";
      }
    }

    void Real(double x, int width, int precision, std::string_view unit)
    {
      fSS << std::setw(width) << std::setfill(' ') << std::right << std::fixed << std::setprecision(precision) << x
          << " " << unit;
    }

    std::string_view KernelName(xpu::kernel_timings& kt)
    {
      std::string prefix = "cbm::algo::";
      if (kt.name().compare(0, prefix.size(), prefix) == 0) { return kt.name().substr(11); }
      else {
        return kt.name();
      }
    }

    size_t fAlign  = 0;
    size_t fIndent = 0;
    std::stringstream fSS;

  };  // class TimingsFormat

  std::string MakeReport(std::string_view title, const xpu::timings& t, size_t align)
  {
    TimingsFormat tf;
    tf.Begin(align);
    tf.Title(title);
    tf.Fmt(t);
    return tf.Finalize();
  }

  std::string MakeReportSubtimers(std::string_view title, const xpu::timings& t, size_t align)
  {
    TimingsFormat tf;
    tf.Begin(align);
    tf.Title(title);
    tf.FmtSubtimers(t);
    return tf.Finalize();
  }

  std::string MakeReportSummary(std::string_view title, const xpu::timings& t, size_t align)
  {
    TimingsFormat tf;
    tf.Begin(align);
    tf.Title(title);
    tf.FmtSummary(t);
    return tf.Finalize();
  }

}  // namespace cbm::algo
