#include "TRunAction.hh"
#include <cmath>

TRunAction::TRunAction()
: G4UserRunAction()
{
  fAnalysisManager = G4RootAnalysisManager::Instance();
  fAnalysisManager -> CreateNtuple("event", "event");
  fAnalysisManager -> CreateNtupleDColumn("ep");
  fAnalysisManager -> CreateNtupleDColumn("es");
  fAnalysisManager -> CreateNtupleDColumn("ev");
  fAnalysisManager -> CreateNtupleDColumn("k0");
  fAnalysisManager -> CreateNtupleDColumn("kw");
  fAnalysisManager -> CreateNtupleDColumn("phi0");
  fAnalysisManager -> CreateNtupleDColumn("theta0");
  fAnalysisManager -> CreateNtupleDColumn("tw");
  fAnalysisManager -> CreateNtupleDColumn("tv");
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

void TRunAction::FillEvent(
    double ep,
    double es,
    double ev,
    G4ThreeVector p0,
    double k0,
    double kw,
    double tw,
    double tv)
{
  int countColumn = 0;
  auto theta = std::atan2(sqrt(p0.x()*p0.x()+p0.z()*p0.z()),p0.y());

  fAnalysisManager -> FillNtupleDColumn(countColumn++, ep);
  fAnalysisManager -> FillNtupleDColumn(countColumn++, es);
  fAnalysisManager -> FillNtupleDColumn(countColumn++, ev);
  fAnalysisManager -> FillNtupleDColumn(countColumn++, k0);
  fAnalysisManager -> FillNtupleDColumn(countColumn++, kw);
  fAnalysisManager -> FillNtupleDColumn(countColumn++, std::atan2(p0.x(),p0.z()));
  fAnalysisManager -> FillNtupleDColumn(countColumn++, theta);
  fAnalysisManager -> FillNtupleDColumn(countColumn++, tw);
  fAnalysisManager -> FillNtupleDColumn(countColumn++, tv);

  fAnalysisManager -> AddNtupleRow();
}
