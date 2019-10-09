#include "TRunManager.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4strstreambuf.hh"
#include "globals.hh"

#include "QGSP_INCLXX_HP.hh"
#include "TDetectorConstruction.hh"
#include "TPrimaryGeneratorAction.hh"
#include "TRunAction.hh"
#include "TEventAction.hh"
#include "TTrackingAction.hh"
#include "TSteppingAction.hh"

TRunManager::TRunManager()
{
}

TRunManager::~TRunManager()
{
  delete fMessenger;
}

void TRunManager::Run(int argc, char **argv, const G4String &type)
{
  G4String macroName = "vis_test.mac";
  if (argc != 1) {
    macroName = argv[1];
  }

  bool isVisMacro = false;
  if (macroName.index("vis")==0)
    isVisMacro = true;

  BuildInitsAndActions();

  G4UImanager* uiManager = G4UImanager::GetUIpointer();
  G4String command = "/control/execute ";

  if (isVisMacro) {
    G4VisManager* visManager = new G4VisExecutive;
    visManager -> Initialize();

    G4UIExecutive* uiExecutive = new G4UIExecutive(argc,argv,type);
    uiManager -> ApplyCommand(command+macroName);
    uiExecutive -> SessionStart();

    delete uiExecutive;
    delete visManager;
  }
  else {
    uiManager -> ApplyCommand(command+macroName);
  }
}

void TRunManager::BuildInitsAndActions()
{
  if (fSuppressSSBInitPhysics) {
    G4strstreambuf* suppressSSB = dynamic_cast<G4strstreambuf*>(G4cout.rdbuf(0));
    // Suppress print outs in between here ------------->
    auto myPhysicsList = new QGSP_INCLXX_HP;
    SetUserInitialization(myPhysicsList);
    // <------------- to here
    G4cout.rdbuf(suppressSSB);
  }
  else {
    auto myPhysicsList = new QGSP_INCLXX_HP;
    SetUserInitialization(myPhysicsList);
  }

  auto wallDetector = new TWallDetector();

  auto detectorConstruction = new TDetectorConstruction();
  detectorConstruction -> SetWallDetector(wallDetector);
  SetUserInitialization(detectorConstruction);

  auto primaryGenerator = new TPrimaryGeneratorAction();
  SetUserAction(primaryGenerator);

  auto runAction = new TRunAction();
  SetUserAction(runAction);

  auto eventAction = new TEventAction(runAction);
  eventAction -> SetDetectorConstruction(detectorConstruction);
  eventAction -> SetWallDetector(wallDetector);
  SetUserAction(eventAction);

  auto trackingAction = new TTrackingAction(eventAction);
  SetUserAction(trackingAction);

  auto steppingAction = new TSteppingAction(eventAction);
  SetUserAction(steppingAction);

  fMessenger = new TMessenger;
  fMessenger -> SetDetectorConstruction(detectorConstruction);
  fMessenger -> SetPrimaryGeneratorAction(primaryGenerator);
  fMessenger -> SetWallDetector(wallDetector);
}

void TRunManager::InitializeGeometry()
{
  if (fSuppressSSBInitGeometry) {
    G4strstreambuf* suppressSSB = dynamic_cast<G4strstreambuf*>(G4cout.rdbuf(0));
    // Suppress print outs in between here ------------->
    G4RunManager::InitializeGeometry();
    // <------------- to here
    G4cout.rdbuf(suppressSSB);
  }
  else
    G4RunManager::InitializeGeometry();
}

void TRunManager::InitializePhysics()
{
  if (fSuppressSSBInitPhysics) {
    G4strstreambuf* suppressSSB = dynamic_cast<G4strstreambuf*>(G4cout.rdbuf(0));
    // Suppress print outs in between here ------------->
    G4RunManager::InitializePhysics();
    // <------------- to here
    G4cout.rdbuf(suppressSSB);
  }
  else
    G4RunManager::InitializePhysics();
}

void TRunManager::BeamOn(G4int numEvents, const char *macroFile, G4int numSelect)
{
  //G4RunManager::BeamOn(numEvents, macroFile, numSelect);
  {
    if(numEvents<=0) { fakeRun = true; }
    else { fakeRun = false; }
    G4bool cond = ConfirmBeamOnCondition();
    if(cond)
    {
      numberOfEventToBeProcessed = numEvents;
      numberOfEventProcessed = 0;
      ConstructScoringWorlds();

      if (fSuppressSSBBeamOn) {
        G4strstreambuf* suppressSSB = dynamic_cast<G4strstreambuf*>(G4cout.rdbuf(0));
        // Suppress print outs in between here ------------->
        RunInitialization();
        // <------------- to here
        G4cout.rdbuf(suppressSSB);
      }
      else
        RunInitialization();

      DoEventLoop(numEvents,macroFile,numSelect);
      RunTermination();
    }
    fakeRun = false;
  }
}
