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

void TEventAction::AddEnergyDepositWall(double edep, bool isPrimaryTrack)
{
  if (isPrimaryTrack) fEdepSumPrimary += edep;
  else                fEdepSumSecondary += edep;
}

void TEventAction::AddEnergyDepositVeto(double edep)
{
  fEdepSumVeto += edep;
}

void TEventAction::SetPointPrimary(double kine, g4v3_t mom)
{
  if (!fIsSetPointPrim) {
    fKinEnergyPrim = kine;
    fMomentumPrim = mom;
    fIsSetPointPrim = true;
  }
}

void TEventAction::SetPointStartOfWall(double kine, g4v3_t mom, double time)
{
  if (!fIsSetPointWall) {
    fKinEnergyWall = kine;
    fMomentumWall = mom;
    fTimeWall = time;
    fIsSetPointWall = true;
  }
}

void TEventAction::SetPointStartOfVeto(double time)
{
  if (!fIsSetPointVeto) {
    fTimeVeto = time;
    fIsSetPointVeto = true;
  }
}
