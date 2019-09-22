#include "TMessenger.hh"
#include "G4ApplicationState.hh"
#include <iostream>

TMessenger::TMessenger()
{
  fCmdFieldFileName = new G4UIcmdWithAString("/my/init/field/name", this);
  fCmdBuildTPC = new G4UIcmdWithoutParameter("/my/init/detector/tpc", this);
  fCmdBuildBar = new G4UIcmdWithoutParameter("/my/init/detector/bar", this);
  fCmdBuildBar0 = new G4UIcmdWithoutParameter("/my/init/detector/bar0", this);
  fCmsBuildWall = new G4UIcmdWithoutParameter("/my/init/detector/wall", this);
}

TMessenger::~TMessenger()
{
  delete fCmdFieldFileName;
  delete fCmdBuildTPC;
  delete fCmdBuildBar;
  delete fCmdBuildBar0;
  delete fCmsBuildWall;
}

void TMessenger::SetNewValue(G4UIcommand *command, G4String value)
{
  if (command == fCmdFieldFileName) fDetectorConstruction -> SetFieldName(value);
  else if (command == fCmdBuildTPC) fDetectorConstruction -> SetBuildTPC();
  else if (command == fCmdBuildBar) fDetectorConstruction -> SetBuildBar();
  else if (command == fCmdBuildBar0) fDetectorConstruction -> SetBuildBar0();
  else if (command == fCmsBuildWall) fDetectorConstruction -> SetBuildWall();
}
