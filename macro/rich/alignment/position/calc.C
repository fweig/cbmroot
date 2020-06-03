void calc(Double_t x, Double_t y, Double_t z)
{
	Double_t rho=0., theta=0., phi=0.;

	rho = sqrt(x*x + y*y + z*z);
	cout << "rho = " << rho << endl;
	theta = TMath::ACos(z/rho)*TMath::RadToDeg();
	phi = TMath::ATan(y/x)*TMath::RadToDeg();
	cout << "theta = " << theta << ", phi = " << phi << endl;
}