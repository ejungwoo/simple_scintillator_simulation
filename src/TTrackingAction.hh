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
    TTrackingAction(TEventAction *runAction);
    virtual ~TTrackingAction() {}

    virtual void PreUserTrackingAction(const G4Track *);
};

#endif
