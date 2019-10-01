#include "TSteppingAction.hh"
#include "G4StepStatus.hh"

TSteppingAction::TSteppingAction(TEventAction *eventAction)
: G4UserSteppingAction()
{
  fEventAction = eventAction;
}

void TSteppingAction::UserSteppingAction(const G4Step* step)
{
  auto preStep = step -> GetPreStepPoint();
  auto pstStep = step -> GetPostStepPoint();

  auto preCopyNo = preStep -> GetPhysicalVolume() -> GetCopyNo();
  if (preCopyNo >= 2000)
    fEventAction -> AddEnergyDeposit(step -> GetTotalEnergyDeposit(), preCopyNo);

  if (pstStep -> GetStepStatus() == fWorldBoundary)
    return;

  auto pstCopyNo = pstStep -> GetPhysicalVolume() -> GetCopyNo();

  if (step -> GetTrack() -> GetTrackID() == 1)
  {
    if (preCopyNo == 0 && pstCopyNo >= 2000)
    {
      fEventAction -> SetPoint1(
          pstStep -> GetKineticEnergy(),
          pstStep -> GetMomentum(),
          pstStep -> GetGlobalTime(),
          pstStep -> GetPosition()
          );
    }

    if (preCopyNo >= 2000 && pstCopyNo == 0)
    {
      fEventAction -> SetPoint2(
          pstStep -> GetKineticEnergy(),
          pstStep -> GetMomentum(),
          pstStep -> GetGlobalTime(),
          pstStep -> GetPosition()
          );
    }
  }
}
