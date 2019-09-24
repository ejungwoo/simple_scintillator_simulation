#include "TTrackingAction.hh"
#include "globals.hh"

TTrackingAction::TTrackingAction(TEventAction *runAction)
: G4UserTrackingAction()
{
  fEventAction = runAction;
}

void TTrackingAction::PreUserTrackingAction(const G4Track *track)
{
  if (track -> GetTrackID() == 1)
    fEventAction -> SetPoint0(track -> GetKineticEnergy(),track -> GetMomentum());
}
