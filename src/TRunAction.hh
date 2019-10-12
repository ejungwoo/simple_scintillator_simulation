#ifndef TRUNACTION_HH
#define TRUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4RootAnalysisManager.hh"
#include "G4ThreeVector.hh"

#include "TString.h"

class TRunAction : public G4UserRunAction
{
  private:
    G4RootAnalysisManager* fAnalysisManager = nullptr;

  public:
    TRunAction();
    virtual ~TRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void FillEvent(
        double ep,
        double es,
        double ev,
        G4ThreeVector p0,
        double k0,
        double k1,
        double tw,
        double tv);
};

#endif
