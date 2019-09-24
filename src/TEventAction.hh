#ifndef TEVENTACTION_HH
#define TEVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "globals.hh"
#include "TRunAction.hh"

class TEventAction : public G4UserEventAction
{
  private:
    TRunAction* fRunAction;

    double fEdepSum;
    double fKinEnergy0;
    double fKinEnergy1;
    double fKinEnergy2;
    G4ThreeVector fMomentum0;
    G4ThreeVector fMomentum1;
    G4ThreeVector fMomentum2;
    double fTime1;
    double fTime2;

    bool fIsSetPoint0 = false;
    bool fIsSetPoint1 = false;
    bool fIsSetPoint2 = false;

  public:
    TEventAction(TRunAction *runAction);
    virtual ~TEventAction() {}

    virtual void BeginOfEventAction(const G4Event *);
    virtual void EndOfEventAction(const G4Event *);

    void AddEnergyDeposit(double edep);
    void SetPoint0(double kine, G4ThreeVector momentum);
    void SetPoint1(double kine, G4ThreeVector momentum, double time);
    void SetPoint2(double kine, G4ThreeVector momentum, double time);
};

#endif
