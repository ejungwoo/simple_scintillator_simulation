#ifndef TPRIMARYGENERATORACTION_HH
#define TPRIMARYGENERATORACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"
#include "globals.hh"

class TPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    TPrimaryGeneratorAction();    
    virtual ~TPrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event*);         
  
  private:
    G4GeneralParticleSource* fGun;
};

#endif
