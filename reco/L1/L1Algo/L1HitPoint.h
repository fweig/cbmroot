/* Copyright (C) 2010-2020 Frankfurt Institute for Advanced Studies, Goethe-Universität Frankfurt, Frankfurt
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Valentina Akishina, Igor Kulakov [committer], Maksym Zyzak */

#ifndef _L1HitPoint_h_
#define _L1HitPoint_h_

/// contain strips positions and coordinates of the hit

struct L1HitPoint {

  L1HitPoint() = default;

  L1HitPoint(fscal z_, fscal v_, fscal u_, fscal t_, fscal du2_, fscal dv2_, fscal dt2_)
    : z(z_)
    , u(u_)
    , v(v_)
    , t(t_)
    , du2(du2_)
    , dv2(dv2_)
    , dt2(dt2_) {};

  fscal Z() const { return z; }
  fscal U() const { return u; }
  fscal V() const { return v; }
  fscal T() const { return t; }

  fscal dU2() const { return du2; }
  fscal dV2() const { return dv2; }
  fscal dT2() const { return dt2; }

  bool IsSuppressed() const { return fIsSuppressed; }

  void SetZ(fscal z_) { z = z_; }
  void SetU(fscal u_) { u = u_; }
  void SetV(fscal v_) { v = v_; }
  void SetT(fscal t_) { t = t_; }

  void Set(const fscal& z_, const fscal& u_, const fscal& v_, const fscal& t_, const fscal& du2_, const fscal& dv2_,
           const fscal& dt2_)
  {
    z   = z_;
    u   = u_;
    v   = v_;
    t   = t_;
    du2 = du2_;
    dv2 = dv2_;
    dt2 = dt2_;

    fIsSuppressed = 0;
  }

  void SetIsSuppresed(bool val) { fIsSuppressed = val; }

private:
  fscal z {0.}, u {0.}, v {0.}, t {0.}, du2 {0.}, dv2 {0.}, dt2 {0.};

  // fIsSuppressed flag is used to suppress duplicated hits at the module overlaps
  // TODO: collect those hits on the track instead of suppressing them
  bool fIsSuppressed {0};  // is the hit suppressed by another hit
};


#endif
