#ifndef TEVENTACTION_HH
#define TEVENTACTION_HH

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "globals.hh"

#include "TRunAction.hh"
#include "TWallDetector.hh"

typedef G4ThreeVector g4v3_t;

class TEventAction : public G4UserEventAction
{
  private:
    TRunAction* fRunAction = nullptr;
    TWallDetector *fWallDetector = nullptr;

    double fEdepSumPrimary = 0.;
    double fEdepSumSecondary = 0.;
    double fEdepSumVeto = 0.;

    double fKinEnergyPrim;
    double fKinEnergyWall;

    double fTimeWall;
    double fTimeVeto;

    g4v3_t fMomentumPrim;
    g4v3_t fMomentumWall;

    bool fIsSetPointPrim = false;
    bool fIsSetPointWall = false;
    bool fIsSetPointVeto = false;

  public:
    TEventAction() : G4UserEventAction() {}
    virtual ~TEventAction() {}

    void SetRunAction(TRunAction *wd) { fRunAction = wd; }
    void SetWallDetector(TWallDetector *wd) { fWallDetector = wd; }

    virtual void BeginOfEventAction(const G4Event *);
    virtual void EndOfEventAction(const G4Event *);

    void SetPointPrimary(double kine, g4v3_t mom);
    void SetPointVeto(double time, double edep);
    void SetPointWall(double kine, g4v3_t mom, double time, double edep, bool isPrimTrk);
};

#endif
