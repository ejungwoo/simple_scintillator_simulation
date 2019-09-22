#include "TPrimaryGeneratorAction.hh"

TPrimaryGeneratorAction::TPrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction()
{
  fGun = new G4GeneralParticleSource();
}

TPrimaryGeneratorAction::~TPrimaryGeneratorAction()
{
  delete fGun;
}

void TPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
  fGun -> GeneratePrimaryVertex(event);
}
