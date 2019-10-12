#include "TSteppingAction.hh"
#include "G4StepStatus.hh"

void TSteppingAction::UserSteppingAction(const G4Step* step)
{
  auto preStep = step -> GetPreStepPoint();
  auto pstStep = step -> GetPostStepPoint();

  if (pstStep -> GetStepStatus() == fWorldBoundary)
    return;

  auto preCopyNo = preStep -> GetPhysicalVolume() -> GetCopyNo();
  auto pstCopyNo = pstStep -> GetPhysicalVolume() -> GetCopyNo();

  bool isPrimaryTrack = (step -> GetTrack() -> GetTrackID() == 1);

  if (fWallDetector -> IsVetoID(preCopyNo)) {
    auto edep = step -> GetTotalEnergyDeposit();
    fEventAction -> AddEnergyDepositVeto(edep);
  }
  else if (fWallDetector -> IsWallID(preCopyNo)) {
    auto edep = step -> GetTotalEnergyDeposit();
    fEventAction -> AddEnergyDepositWall(edep, isPrimaryTrack);
  }

  if (isPrimaryTrack)
  {
    if (preCopyNo == 3)
    {
      if (fWallDetector -> IsVetoID(pstCopyNo)) {
        fEventAction -> SetPointStartOfVeto(pstStep -> GetGlobalTime());
      }
      else if (fWallDetector -> IsWallID(pstCopyNo)) {
        fEventAction -> SetPointStartOfWall(
            pstStep -> GetKineticEnergy(),
            pstStep -> GetMomentum(),
            pstStep -> GetGlobalTime()
            );
      }
    }
  }
}
