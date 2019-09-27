#ifndef TPRIMARYGENERATORACTION_HH
#define TPRIMARYGENERATORACTION_HH

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"
#include "globals.hh"

class TPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  public:
    TPrimaryGeneratorAction();
    virtual ~TPrimaryGeneratorAction();
    virtual void GeneratePrimaries(G4Event*);

    void SetParticleGun() { fSetParticleGun = true; }
    void SetGeneralParticleSource() { fSetParticleGun = false; }

    void SetParticle(G4String name) { fGun -> SetParticleDefinition(fTable -> FindParticle(name)); }
    void SetParticle(int pdg)       { fGun -> SetParticleDefinition(fTable -> FindParticle(pdg));  }
    void SetParticleVertex(G4ThreeVector vertex) { fGun -> SetParticlePosition(vertex); }
    void SetParticleEnergy1(double val)  { fEnergy1 = val; }
    void SetParticleEnergy2(double val)  { fEnergy1 = val; }
    void SetParticlePhi1(double val)     { fPhi1 = val; }
    void SetParticlePhi2(double val)     { fPhi1 = val; }
    void SetParticleTheta1(double val)   { fTheta1 = val; }
    void SetParticleTheta2(double val)   { fTheta1 = val; }
  
  private:
    G4ThreeVector DirectionByAngle(double phi, double theta);
    double UniformRandomInRange(double x1, double x2);

    G4GeneralParticleSource* fGPS;
    G4ParticleGun *fGun;
    G4ParticleTable *fTable;

    bool fSetParticleGun = false;
    double fEnergy1;
    double fEnergy2;
    double fPhi1;
    double fPhi2;
    double fTheta1;
    double fTheta2;
};

#endif
