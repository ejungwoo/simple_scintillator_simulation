#include "TPrimaryGeneratorAction.hh"
#include "Randomize.hh"
#include <cmath>

TPrimaryGeneratorAction::TPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction()
{
  fGPS = new G4GeneralParticleSource();
  fGun = new G4ParticleGun();
  fTable = G4ParticleTable::GetParticleTable();
}

TPrimaryGeneratorAction::~TPrimaryGeneratorAction()
{
  delete fGPS;
}

void TPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  if (fSetParticleGun) {
    auto energy = UniformRandomInRange(fEnergy1, fEnergy2);
    auto phi = UniformRandomInRange(fPhi1, fPhi2);
    auto theta = UniformRandomInRange(fTheta1, fTheta2);
    auto pdir = DirectionByAngle(phi, theta);
    fGun -> SetParticleEnergy(energy);
    fGun -> SetParticleMomentumDirection(pdir);
    fGun -> GeneratePrimaryVertex(event);
  }
  else
    fGPS -> GeneratePrimaryVertex(event);
}

double TPrimaryGeneratorAction::UniformRandomInRange(double x1, double x2)
{
  if (x2 < x1) {
    auto x3 = x1;
    x1 = x2;
    x2 = x3;
  }

  auto value = G4UniformRand() * (x2-x1) + x1;

  return value;
}

G4ThreeVector TPrimaryGeneratorAction::DirectionByAngle(double phi, double theta)
{
  auto px = std::sin(theta)*std::sin(phi);
  auto pz = std::sin(theta)*std::cos(phi);
  auto py = std::cos(theta);

  return G4ThreeVector(px,py,pz);
}
