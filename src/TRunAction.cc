#include "TRunAction.hh"
#include <cmath>

TRunAction::TRunAction()
: G4UserRunAction()
{
  fAnalysisManager = G4RootAnalysisManager::Instance();

  fAnalysisManager -> CreateNtuple("event", "event");

  fAnalysisManager -> CreateNtupleDColumn("ebar");
  fAnalysisManager -> CreateNtupleDColumn("kine0");
  fAnalysisManager -> CreateNtupleDColumn("phi0");
  fAnalysisManager -> CreateNtupleDColumn("kine1");
  fAnalysisManager -> CreateNtupleDColumn("kine2");
  fAnalysisManager -> CreateNtupleDColumn("tof1");
  fAnalysisManager -> FinishNtuple();
}

TRunAction::~TRunAction()
{
  delete fAnalysisManager;
}

void TRunAction::BeginOfRunAction(const G4Run*)
{
  fAnalysisManager -> OpenFile();
}

void TRunAction::EndOfRunAction(const G4Run*)
{
  fAnalysisManager -> Write();
  fAnalysisManager -> CloseFile();
}

void TRunAction::FillEvent(double e,
                           double k0,
                           double k1,
                           double k2,
                           G4ThreeVector p0,
                           G4ThreeVector p1,
                           G4ThreeVector p2,
                           double t1,
                           double t2)
{
  fAnalysisManager -> FillNtupleDColumn(0, e);
  fAnalysisManager -> FillNtupleDColumn(1, k0);
  fAnalysisManager -> FillNtupleDColumn(2, std::atan2(p0.z(),p0.x()));
  fAnalysisManager -> FillNtupleDColumn(3, k1);
  fAnalysisManager -> FillNtupleDColumn(4, k2);
  fAnalysisManager -> FillNtupleDColumn(5, t1);

  fAnalysisManager -> AddNtupleRow();
}
