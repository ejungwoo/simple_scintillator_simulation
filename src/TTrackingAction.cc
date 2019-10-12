#include "TTrackingAction.hh"
#include "globals.hh"

void TTrackingAction::PreUserTrackingAction(const G4Track *track)
{
  if (track -> GetTrackID() == 1)
    fEventAction -> SetPointPrimary(track -> GetKineticEnergy(), track -> GetMomentum());
}
