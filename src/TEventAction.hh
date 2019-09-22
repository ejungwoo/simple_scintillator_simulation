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
    double fKinEnergy1;
    double fKinEnergy2;
    G4ThreeVector fMomentum1;
    G4ThreeVector fMomentum2;

    bool fIsSetKinEnergy1 = false;
    bool fIsSetKinEnergy2 = false;
    bool fIsSetMomentum1  = false;
    bool fIsSetMomentum2  = false;

  public:
    TEventAction(TRunAction *runAction);
    virtual ~TEventAction() {}

    virtual void BeginOfEventAction(const G4Event *);
    virtual void EndOfEventAction(const G4Event *);

    void AddEnergyDeposit(double edep)   { fEdepSum += edep; }
    void SetKineticEnergy1(double kine)  { if (!fIsSetKinEnergy1) fKinEnergy1 = kine; fIsSetKinEnergy1 = true; }
    void SetKineticEnergy2(double kine)  { if (!fIsSetKinEnergy2) fKinEnergy2 = kine; fIsSetKinEnergy2 = true; }
    void SetMomentum1(G4ThreeVector mom) { if (!fIsSetMomentum1)  fMomentum1  = mom;  fIsSetMomentum1  = true; }
    void SetMomentum2(G4ThreeVector mom) { if (!fIsSetMomentum2)  fMomentum2  = mom;  fIsSetMomentum2  = true; }
};

#endif
