#include "TSteppingAction.hh"
#include "G4StepStatus.hh"

void TSteppingAction::UserSteppingAction(const G4Step* step)
{
  auto preStep = step -> GetPreStepPoint();
  auto pstStep = step -> GetPostStepPoint();

  if (pstStep -> GetStepStatus() == fWorldBoundary)
    return;

  auto preID = preStep -> GetPhysicalVolume() -> GetCopyNo();

  bool isPrimaryTrack = (step -> GetTrack() -> GetTrackID() == 1);
  auto time = preStep -> GetGlobalTime();
  auto edep = step -> GetTotalEnergyDeposit();

  if (fWallDetector -> IsVetoID(preID)) {
    fEventAction -> SetPointVeto(time, edep);
  }
  else if (fWallDetector -> IsWallID(preID)) {
    auto energy = preStep -> GetKineticEnergy();
    auto momentum = preStep -> GetMomentum();
    fEventAction -> SetPointWall(energy, momentum, time, edep, isPrimaryTrack);
  }
}
