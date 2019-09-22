#include "TEventAction.hh"

TEventAction::TEventAction(TRunAction *runAction)
: G4UserEventAction(), fRunAction(runAction)
{
}

void TEventAction::BeginOfEventAction(const G4Event*)
{
  fEdepSum = 0.;

  fKinEnergy1 = -1.;
  fKinEnergy2 = -1.;

  fMomentum1 = G4ThreeVector(0,0,0);
  fMomentum2 = G4ThreeVector(0,0,0);

  fIsSetKinEnergy1 = false;
  fIsSetKinEnergy2 = false;
  fIsSetMomentum1  = false;
  fIsSetMomentum2  = false;
}

void TEventAction::EndOfEventAction(const G4Event*)
{
  fRunAction -> FillEvent(
      fEdepSum,
      fKinEnergy1, 
      fKinEnergy2, 
      fMomentum1.x(),
      fMomentum1.z(), 
      fMomentum2.x(),
      fMomentum2.z());
}
