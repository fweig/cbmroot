/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   LxTBMatEffs.h
 * Author: Timur Ablyazimov
 *
 * Created on 15 Mar 2016
 */

#ifndef LXTBMATEFFS_H
#define LXTBMATEFFS_H

#include <cmath>

static scaltype gMuonMass = 0;
static scaltype gElectronMass = 0;

struct LxTbAbsorber
{
    scaltype zCoord;// Z-coordinate of the absorber beginning.
    scaltype width;
    scaltype radLength;// Radiation length
    scaltype rho;// Density
    scaltype Z;// Atomic number
    scaltype A;// Atomic weight
    
    //LxTbAbsorber(scaltype w, scaltype rl, scaltype d, scaltype z, scaltype a) : width(w), radLength(rl), rho(d), Z(z), A(a) {}
};

static inline scaltype CalcI(scaltype Z)
{
   // mean excitation energy in eV
   if (Z > 16.) { return 10 * Z; }
   else { return 16 * std::pow(Z, 0.9); }
}

static inline scaltype BetheBloch(scaltype E,  const LxTbAbsorber* mat)
{
   scaltype K = 0.000307075; // GeV * g^-1 * cm^2
   scaltype z = 1.;//(par->GetQp() > 0.) ? 1 : -1.;
   scaltype Z = mat->Z;
   scaltype A = mat->A;

   scaltype M = gMuonMass;
   //scaltype p = std::abs(1. / par->GetQp()); //GeV
   //scaltype E = std::sqrt(M * M + p * p);
   
   scaltype p = std::sqrt(E * E - M * M); //GeV
   scaltype beta = p / E;
   scaltype betaSq = beta * beta;
   scaltype gamma = E / M;
   scaltype gammaSq = gamma * gamma;

   scaltype I = CalcI(Z) * 1e-9; // GeV

   scaltype me = gElectronMass; // GeV
   scaltype ratio = me/M;
   scaltype Tmax = (2*me*betaSq*gammaSq) / (1+2*gamma*ratio+ratio*ratio);

   // density correction
   scaltype dc = 0.;
   if (p > 0.5) { // for particles above 1 Gev
      scaltype rho = mat->rho;
      scaltype hwp = 28.816 * std::sqrt(rho*Z/A) * 1e-9 ; // GeV
      dc = std::log(hwp/I) + std::log(beta*gamma) - 0.5;
   }

   return K*z*z*(Z/A)*(1./betaSq) * (0.5*std::log(2*me*betaSq*gammaSq*Tmax/(I*I))-betaSq - dc);
}

static inline scaltype EnergyLoss(scaltype E, scaltype L, const LxTbAbsorber* mat)
{
    return BetheBloch(E, mat) * mat->rho * L;
    
    /*scaltype result = 0;
    
    if (L < 10)
    {
        result = BetheBloch(E, mat) * mat->rho * L;
        
        if (result <= 0)
            return 0;
        
        if (result > E - gMuonMass)
            result = E - gMuonMass;
        
        return result;
    }
    
    scaltype delta = 10;
    
    for (scaltype l = 10; l <= L;)
    {
        scaltype r = BetheBloch(E, mat) * mat->rho * delta;
        
        if (r <= 0)
            return result;
        
        if (r > E - gMuonMass)
            r = E - gMuonMass;
        
        result += r;
        E -= r;
        delta = L - l;
        
        if (delta < 0.5)
            break;
        else if (delta < 10)
            l = L;
        else
            l += 10;
    }
    
    return result;*/
}

static inline scaltype CalcThetaPrj(scaltype E, scaltype x, const LxTbAbsorber* mat)
{
   //scaltype p = std::abs(1. / par->GetQp()); //GeV
   scaltype muMass = gMuonMass;
   //scaltype E = std::sqrt(muMass * muMass + p * p);
   //scaltype p = std::abs(1. / par->GetQp()); //GeV
   scaltype p = std::sqrt(E * E - muMass * muMass); //GeV
   scaltype beta = p / E;
   //scaltype x = mat->GetLength(); //cm
   scaltype X0 = mat->radLength; //cm
   scaltype bcp = beta * p;
   scaltype z = 1.;

   scaltype theta = 0.0136 * (1./bcp) * z * std::sqrt(x/X0) *
               (1. + 0.038 * std::log(x/X0));
   return theta / std::sqrt(2.0);
   //return theta * theta;
}

#endif /* LXTBMATEFFS_H */
