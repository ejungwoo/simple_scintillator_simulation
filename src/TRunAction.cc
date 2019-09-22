#include "TRunAction.hh"

TRunAction::TRunAction()
: G4UserRunAction()
{
  fAnalysisManager = G4RootAnalysisManager::Instance();

  fAnalysisManager -> CreateNtuple("event", "event");
  fAnalysisManager -> CreateNtupleDColumn("ebar");
  fAnalysisManager -> CreateNtupleDColumn("kine1");
  fAnalysisManager -> CreateNtupleDColumn("kine2");
  fAnalysisManager -> CreateNtupleDColumn("px1");
  fAnalysisManager -> CreateNtupleDColumn("pz1");
  fAnalysisManager -> CreateNtupleDColumn("px2");
  fAnalysisManager -> CreateNtupleDColumn("pz2");
  fAnalysisManager -> FinishNtuple();
}

TRunAction::~TRunAction()
{
  delete fAnalysisManager;
}

void TRunAction::BeginOfRunAction(const G4Run* run)
{
  fAnalysisManager -> OpenFile();
}

void TRunAction::EndOfRunAction(const G4Run*)
{
  fAnalysisManager -> Write();
  fAnalysisManager -> CloseFile();
}

void TRunAction::FillEvent(double e, double k1, double k2, double p1x, double p1z, double p2x, double p2z)
{
  fAnalysisManager -> FillNtupleDColumn(0, e);
  fAnalysisManager -> FillNtupleDColumn(1, k1);
  fAnalysisManager -> FillNtupleDColumn(2, k2);
  fAnalysisManager -> FillNtupleDColumn(3, p1x);
  fAnalysisManager -> FillNtupleDColumn(4, p1z);
  fAnalysisManager -> FillNtupleDColumn(5, p2x);
  fAnalysisManager -> FillNtupleDColumn(6, p2z);
  fAnalysisManager -> AddNtupleRow();
}
