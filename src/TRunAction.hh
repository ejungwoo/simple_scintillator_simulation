#ifndef TRUNACTION_HH
#define TRUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Run.hh"
#include "G4RootAnalysisManager.hh"

#include "TString.h"

class TRunAction : public G4UserRunAction
{
  public:
    TRunAction();
    virtual ~TRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void FillEvent(double e, double k1, double k2, double p1x, double p1z, double p2x, double p2z);

  private:
    G4RootAnalysisManager* fAnalysisManager = nullptr;
};

#endif
