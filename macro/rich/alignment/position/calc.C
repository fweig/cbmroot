/* Copyright (C) 2019-2021 Justus-Liebig-Universitaet Giessen, Giessen
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Jordan Bendarouach [committer] */

void calc(Double_t x, Double_t y, Double_t z)
{
  Double_t rho = 0., theta = 0., phi = 0.;

  rho = sqrt(x * x + y * y + z * z);
  cout << "rho = " << rho << endl;
  theta = TMath::ACos(z / rho) * TMath::RadToDeg();
  phi   = TMath::ATan(y / x) * TMath::RadToDeg();
  cout << "theta = " << theta << ", phi = " << phi << endl;
}
