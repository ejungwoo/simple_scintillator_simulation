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
  auto postStep = step -> GetPostStepPoint();

  auto preID = preStep -> GetPhysicalVolume() -> GetCopyNo();
  if (preID == 2)
    fEventAction -> AddEnergyDeposit(step -> GetTotalEnergyDeposit());

  if (step -> GetTrack() -> GetTrackID() != 1)
    return;

  if (step -> GetPostStepPoint() -> GetStepStatus() == fWorldBoundary)
    return;

  auto postID = postStep -> GetPhysicalVolume() -> GetCopyNo();

  if (preID == 1 && postID == 2)
  {
    fEventAction -> SetPoint1(
        postStep -> GetKineticEnergy(),
        postStep -> GetMomentum(),
        postStep -> GetGlobalTime()
        );
  }

  if (preID == 2 && postID == 1)
  {
    fEventAction -> SetPoint2(
        postStep -> GetKineticEnergy(),
        postStep -> GetMomentum(),
        postStep -> GetGlobalTime()
        );
  }
}
