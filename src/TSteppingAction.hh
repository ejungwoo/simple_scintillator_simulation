#ifndef TSTEPPINGACTION_HH
#define TSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "globals.hh"
#include "g4root.hh"

#include "TEventAction.hh"

class TSteppingAction : public G4UserSteppingAction
{
  private:
    TEventAction *fEventAction;

  public:
    TSteppingAction(TEventAction *eventAction);
    virtual ~TSteppingAction() {}
    virtual void UserSteppingAction(const G4Step*);
};

#endif
