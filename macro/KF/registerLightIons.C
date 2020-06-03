void registerLightIons()
{
  KFPartEfficiencies eff;
  for(int jParticle=eff.fFirstStableParticleIndex+10; jParticle<=eff.fLastStableParticleIndex; jParticle++)
  {
    TDatabasePDG* pdgDB = TDatabasePDG::Instance();

    std::cout << "Adding particle " << jParticle << ", name " << eff.partTitle[jParticle].data()
        << ", pdg " << eff.partPDG[jParticle] << std::endl;

    if(!pdgDB->GetParticle(eff.partPDG[jParticle])){
      std::cout << "Particle not found in TDatabase, adding" << std::endl;
        pdgDB->AddParticle(eff.partTitle[jParticle].data(),eff.partTitle[jParticle].data(), eff.partMass[jParticle], kTRUE,
                           0, eff.partCharge[jParticle]*3,"Ion",eff.partPDG[jParticle]);
    }
  }
}
