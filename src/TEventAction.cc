#include "TEventAction.hh"

void TEventAction::BeginOfEventAction(const G4Event*)
{
  fEdepSumPrimary = 0.;
  fEdepSumSecondary = 0.;
  fEdepSumVeto = 0.;

  fKinEnergyPrim = -1.;
  fKinEnergyWall = -1.;

  fMomentumPrim = g4v3_t(0,0,0);
  fMomentumWall = g4v3_t(0,0,0);

  fTimeWall = -1.;
  fTimeVeto = -1.;

  fIsSetPointPrim = false;
  fIsSetPointWall = false;
  fIsSetPointVeto = false;
}

void TEventAction::EndOfEventAction(const G4Event*)
{
  fRunAction -> FillEvent(
      fEdepSumPrimary,
      fEdepSumSecondary,
      fEdepSumVeto,
      fMomentumPrim,
      fKinEnergyPrim,
      fKinEnergyWall,
      fTimeWall,
      fTimeVeto);
}

void TEventAction::SetPointPrimary(double kine, g4v3_t mom)
{
  if (!fIsSetPointPrim) {
    fKinEnergyPrim = kine;
    fMomentumPrim = mom;
    fIsSetPointPrim = true;
  }
}

void TEventAction::SetPointVeto(double time, double edep)
{
  fEdepSumVeto += edep;

  if (!fIsSetPointVeto) {
    fTimeVeto = time;
    fIsSetPointVeto = true;
  }
}

void TEventAction::SetPointWall(double kine, g4v3_t mom, double time, double edep, bool isPrimTrk)
{
  if (isPrimTrk) fEdepSumPrimary += edep;
  else           fEdepSumSecondary += edep;

  if (!fIsSetPointWall) {
    fKinEnergyWall = kine;
    fMomentumWall = mom;
    fTimeWall = time;
    fIsSetPointWall = true;
  }
}
