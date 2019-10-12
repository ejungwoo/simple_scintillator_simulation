#ifndef TTRACKINGACTION_HH
#define TTRACKINGACTION_HH

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"
#include "TEventAction.hh"

class TTrackingAction : public G4UserTrackingAction
{
  private:
    TEventAction *fEventAction;

  public:
    TTrackingAction(): G4UserTrackingAction() {}
    virtual ~TTrackingAction() {}

    virtual void PreUserTrackingAction(const G4Track *);

    void SetEventAction(TEventAction *ea) { fEventAction = ea; }
};

#endif
