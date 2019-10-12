#ifndef TSTEPPINGACTION_HH
#define TSTEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "G4Step.hh"
#include "globals.hh"
#include "g4root.hh"

#include "TEventAction.hh"
#include "TWallDetector.hh"

class TSteppingAction : public G4UserSteppingAction
{
  private:
    TEventAction *fEventAction = nullptr;
    TWallDetector *fWallDetector = nullptr;

  public:
    TSteppingAction() : G4UserSteppingAction() {}
    virtual ~TSteppingAction() {}
    virtual void UserSteppingAction(const G4Step*);

    void SetEventAction(TEventAction *ea) { fEventAction = ea; }
    void SetWallDetector(TWallDetector *wd) { fWallDetector = wd; }
};

#endif
