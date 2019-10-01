#include "TEventAction.hh"

TEventAction::TEventAction(TRunAction *runAction)
: G4UserEventAction(), fRunAction(runAction)
{
  fWallDetector = new TWallDetector();
}

void TEventAction::BeginOfEventAction(const G4Event*)
{
  fEdepSum = 0.;

  fKinEnergy0 = -1.;
  fKinEnergy1 = -1.;
  fKinEnergy2 = -1.;

  fMomentum0 = G4ThreeVector(0,0,0);
  fMomentum1 = G4ThreeVector(0,0,0);
  fMomentum2 = G4ThreeVector(0,0,0);

  fTime1 = -1.;
  fTime2 = -1.;

  fPosition1 = G4ThreeVector(-99999,-99999,-99999);
  fPosition2 = G4ThreeVector(-99999,-99999,-99999);

  fIsSetPoint0 = false;
  fIsSetPoint1 = false;
  fIsSetPoint2 = false;

  fCountActiveLayer = 0;
  for (auto layer=0; layer<8; ++layer)
    fIsActiveLayer[layer] = 0;

  fCountActiveBar = 0;
  for (auto layer=0; layer<8; ++layer)
    for (auto row=0; row<50; ++row)
      fIsActiveBar[layer][row] = 0;
}

void TEventAction::EndOfEventAction(const G4Event*)
{
  auto local1 = fWallDetector -> GlobalToLocalPos(fPosition1);
  auto local2 = fWallDetector -> GlobalToLocalPos(fPosition2);

  fRunAction -> FillEvent( fEdepSum,
      fKinEnergy0, fKinEnergy1, fKinEnergy2,
      fMomentum0, fMomentum1, fMomentum2,
      fTime1, fTime2,
      local1, local2,
      fCountActiveLayer, fCountActiveBar
      );
}

void TEventAction::AddEnergyDeposit(double edep, int copyNo)
{
  fEdepSum += edep;

  auto layer = fDetectorConstruction -> GetLayer(copyNo);
  auto row = fDetectorConstruction -> GetRow(copyNo);

  if (!fIsActiveLayer[layer]) {
    fIsActiveLayer[layer] = 1;
    fCountActiveLayer++;
  }

  if (!fIsActiveBar[layer][row]) {
    fIsActiveBar[layer][row] = 1;
    fCountActiveBar++;
  }
}

void TEventAction::SetPoint0(double kine, G4ThreeVector momentum)
{
  if (!fIsSetPoint0) {
    fKinEnergy0 = kine;
    fMomentum0 = momentum;
    fIsSetPoint0 = true;
  }
}

void TEventAction::SetPoint1(double kine, G4ThreeVector momentum, double time, G4ThreeVector pos)
{
  if (!fIsSetPoint1) {
    fKinEnergy1 = kine;
    fMomentum1 = momentum;
    fTime1 = time;
    fPosition1 = pos;
    fIsSetPoint1 = true;
  }
}

void TEventAction::SetPoint2(double kine, G4ThreeVector momentum, double time, G4ThreeVector pos)
{
  if (!fIsSetPoint2) {
    fKinEnergy2 = kine;
    fMomentum2 = momentum;
    fTime2 = time;
    fPosition2 = pos;
    fIsSetPoint2 = true;
  }
}

void TEventAction::SetDetectorConstruction(TDetectorConstruction *detectorConstruction)
{
  fDetectorConstruction = detectorConstruction;
}
