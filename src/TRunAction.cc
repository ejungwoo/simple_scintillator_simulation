#include "TRunAction.hh"
#include <cmath>

TRunAction::TRunAction()
: G4UserRunAction()
{
  fAnalysisManager = G4RootAnalysisManager::Instance();

  fAnalysisManager -> CreateNtuple("event", "event");

  fAnalysisManager -> CreateNtupleDColumn("edep");
  fAnalysisManager -> CreateNtupleDColumn("nlayers");
  fAnalysisManager -> CreateNtupleDColumn("nbars");

  fAnalysisManager -> CreateNtupleDColumn("kine0");
  fAnalysisManager -> CreateNtupleDColumn("phi0");
  fAnalysisManager -> CreateNtupleDColumn("theta0");

  fAnalysisManager -> CreateNtupleDColumn("kine1");
  fAnalysisManager -> CreateNtupleDColumn("tof1");
  fAnalysisManager -> CreateNtupleDColumn("x1");
  fAnalysisManager -> CreateNtupleDColumn("y1");

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
                           double t2,
                           G4ThreeVector pos1,
                           G4ThreeVector pos2,
                           double nlayers,
                           double nbars)
{
  int countColumn = 0;
  fAnalysisManager -> FillNtupleDColumn(countColumn++, e);
  fAnalysisManager -> FillNtupleDColumn(countColumn++, nlayers);
  fAnalysisManager -> FillNtupleDColumn(countColumn++, nbars);
  fAnalysisManager -> FillNtupleDColumn(countColumn++, k0);
  fAnalysisManager -> FillNtupleDColumn(countColumn++, std::atan2(p0.z(),p0.x()));
  fAnalysisManager -> FillNtupleDColumn(countColumn++, std::atan2(sqrt(p0.x()*p0.x()+p0.z()*p0.z()),p0.y()));
  fAnalysisManager -> FillNtupleDColumn(countColumn++, k1);
  fAnalysisManager -> FillNtupleDColumn(countColumn++, t1);
  fAnalysisManager -> FillNtupleDColumn(countColumn++, pos1.x());
  fAnalysisManager -> FillNtupleDColumn(countColumn++, pos1.y());

  fAnalysisManager -> AddNtupleRow();
}
