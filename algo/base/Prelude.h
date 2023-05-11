/* Copyright (C) 2023 FIAS Frankfurt Institute for Advanced Studies, Frankfurt / Main
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Felix Weiglhofer [committer] */
#ifndef CBM_ALGO_BASE_PRELUDE_H
#define CBM_ALGO_BASE_PRELUDE_H

#include <cstdint>
#include <gsl/span>

namespace cbm::algo
{

  // typealias for Rust-like fixed size integer types
  using i8  = std::int8_t;
  using u8  = std::uint8_t;
  using i16 = std::int16_t;
  using u16 = std::uint16_t;
  using i32 = std::int32_t;
  using u32 = std::uint32_t;
  using i64 = std::int64_t;
  using u64 = std::uint64_t;
  using f32 = float;
  using f64 = double;

#ifdef CBM_ALGO_REAL64
  using real = f64;
#else
  using real = f32;
#endif

#if !XPU_IS_CUDA && !XPU_IS_HIP  // FIXME why doesn't this work with CUDA?
  template<typename T>
  using span = gsl::span<T>;

  template<typename T>
  using cspan = gsl::span<const T>;
#endif

}  // namespace cbm::algo

#endif  // CBM_ALGO_BASE_PRELUDE_H
