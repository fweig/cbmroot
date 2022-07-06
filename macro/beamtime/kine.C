/* Copyright (C) 2022 PI-UHd, GSI
   SPDX-License-Identifier: GPL-3.0-only
   Authors: Norbert Herrmann [committer] */

void kine(Double_t plab = 10)
{
  Double_t m   = 0.931;
  Double_t E1  = TMath::Sqrt(plab * plab + m * m);
  Double_t E2  = m;
  Double_t T1  = E1 - m;
  Double_t Sqs = TMath::Sqrt(2 * m * m + 2 * E1 * m);
  cout << "p: " << plab << ", T: " << T1 << ", sqrt(s) " << Sqs << endl;
}
