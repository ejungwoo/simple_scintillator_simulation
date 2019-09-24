#include "TEventAction.hh"

TEventAction::TEventAction(TRunAction *runAction)
: G4UserEventAction(), fRunAction(runAction)
{
}

void TEventAction::BeginOfEventAction(const G4Event*)
{
  fEdepSum = 0.;

  fKinEnergy0 = -1.;
  fKinEnergy1 = -1.;
  fKinEnergy2 = -1.;

  fMomentum0 = G4ThreeVector(0,0,0);
  fMomentum1 = G4ThreeVector(0,0,0);
  fMomentum2 = G4ThreeVector(0,0,0);

  fTime1 = -1.;
  fTime2 = -1.;

  fIsSetPoint0 = false;
  fIsSetPoint1 = false;
  fIsSetPoint2 = false;
}

void TEventAction::EndOfEventAction(const G4Event*)
{
  fRunAction -> FillEvent(
      fEdepSum,
      fKinEnergy0,
      fKinEnergy1, 
      fKinEnergy2, 
      fMomentum0,
      fMomentum1,
      fMomentum2,
      fTime1,
      fTime2);
}

void TEventAction::AddEnergyDeposit(double edep)
{
  fEdepSum += edep;
}

void TEventAction::SetPoint0(double kine, G4ThreeVector momentum)
{
  if (!fIsSetPoint0) {
    fKinEnergy0 = kine;
    fMomentum0 = momentum;
    fIsSetPoint0 = true;
  }
}

void TEventAction::SetPoint1(double kine, G4ThreeVector momentum, double time)
{
  if (!fIsSetPoint1) {
    fKinEnergy1 = kine;
    fMomentum1 = momentum;
    fTime1 = time;
    fIsSetPoint1 = true;
  }
}

void TEventAction::SetPoint2(double kine, G4ThreeVector momentum, double time)
{
  if (!fIsSetPoint2) {
    fKinEnergy2 = kine;
    fMomentum2 = momentum;
    fTime2 = time;
    fIsSetPoint2 = true;
  }
}
