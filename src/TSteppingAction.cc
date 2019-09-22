#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"

#include "TSteppingAction.hh"

TSteppingAction::TSteppingAction(TEventAction *eventAction)
: G4UserSteppingAction()
{
  fEventAction = eventAction;
}

void TSteppingAction::UserSteppingAction(const G4Step* step)
{
  auto pre = step -> GetPreStepPoint();

  G4int preid = pre -> GetPhysicalVolume() -> GetCopyNo();
  if (preid == 2)
  {
    fEventAction -> AddEnergyDeposit(step -> GetTotalEnergyDeposit());

    if (step -> GetTrack() -> GetTrackID() == 1)
    {
      auto post = step -> GetPostStepPoint();
      fEventAction -> SetKineticEnergy1(pre -> GetKineticEnergy());
      fEventAction -> SetMomentum2(post -> GetMomentum());

      G4int postid = post -> GetPhysicalVolume() -> GetCopyNo();
      if (postid != 2) {
        fEventAction -> SetKineticEnergy2(post -> GetKineticEnergy());
        fEventAction -> SetMomentum2(post -> GetMomentum());
      }
    }
  }
}
