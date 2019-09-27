#include "TMessenger.hh"
#include "G4ApplicationState.hh"
#include <iostream>

TMessenger::TMessenger()
{
  fCmdFieldFileName  = new G4UIcmdWithAString("/my/init/field/name"    ,this);
  fCmdBuildTPC  = new G4UIcmdWithoutParameter("/my/init/detector/tpc"  ,this);
  fCmdBuildBar  = new G4UIcmdWithoutParameter("/my/init/detector/bar"  ,this);
  fCmdBuildBar0 = new G4UIcmdWithoutParameter("/my/init/detector/bar0" ,this);
  fCmsBuildWall = new G4UIcmdWithoutParameter("/my/init/detector/wall" ,this);

  fCmsSetParticleGun  = new G4UIcmdWithoutParameter("/my/pga/gun" ,this);
  fCmdParticlePDG     = new G4UIcmdWithAnInteger("/my/pga/pdg"    ,this);
  fCmdParticleName    = new G4UIcmdWithAString("/my/pga/name"     ,this);
  fCmdParticleVertex  = new G4UIcmdWith3VectorAndUnit("/my/pga/vertex"   ,this);
  fCmdParticleEnergy1 = new G4UIcmdWithADoubleAndUnit("/my/pga/energy1"  ,this);
  fCmdParticleEnergy2 = new G4UIcmdWithADoubleAndUnit("/my/pga/energy2"  ,this);
  fCmdParticlePhi1    = new G4UIcmdWithADoubleAndUnit("/my/pga/phi1"     ,this);
  fCmdParticlePhi2    = new G4UIcmdWithADoubleAndUnit("/my/pga/phi2"     ,this);
  fCmdParticleTheta1  = new G4UIcmdWithADoubleAndUnit("/my/pga/theta1"   ,this);
  fCmdParticleTheta2  = new G4UIcmdWithADoubleAndUnit("/my/pga/theta2"   ,this);
}

TMessenger::~TMessenger()
{
  delete fCmdFieldFileName;
  delete fCmdBuildTPC;
  delete fCmdBuildBar;
  delete fCmdBuildBar0;
  delete fCmsBuildWall;

  delete fCmsSetParticleGun;
  delete fCmdParticlePDG;
  delete fCmdParticleName;
  delete fCmdParticleVertex;
  delete fCmdParticleEnergy1;
  delete fCmdParticleEnergy2;
  delete fCmdParticlePhi1;
  delete fCmdParticlePhi2;
  delete fCmdParticleTheta1;
  delete fCmdParticleTheta2;
}

void TMessenger::SetNewValue(G4UIcommand *command, G4String value)
{
  if (command == fCmdFieldFileName)  fDetectorConstruction -> SetFieldName(value);
  else if (command == fCmdBuildTPC)  fDetectorConstruction -> SetBuildTPC();
  else if (command == fCmdBuildBar)  fDetectorConstruction -> SetBuildBar();
  else if (command == fCmdBuildBar0) fDetectorConstruction -> SetBuildBar0();
  else if (command == fCmsBuildWall) fDetectorConstruction -> SetBuildWall();

  else if (command == fCmsSetParticleGun)  fPrimaryGeneratorAction -> SetParticleGun();
  else if (command == fCmdParticlePDG)     fPrimaryGeneratorAction -> SetParticle( fCmdParticlePDG -> GetNewIntValue(value) );
  else if (command == fCmdParticleName)    fPrimaryGeneratorAction -> SetParticle( value );
  else if (command == fCmdParticleVertex)  fPrimaryGeneratorAction -> SetParticleVertex( fCmdParticleVertex -> GetNew3VectorValue(value) );
  else if (command == fCmdParticleEnergy1) fPrimaryGeneratorAction -> SetParticleEnergy1( fCmdParticleEnergy1 -> GetNewDoubleValue(value) );
  else if (command == fCmdParticleEnergy2) fPrimaryGeneratorAction -> SetParticleEnergy2( fCmdParticleEnergy2 -> GetNewDoubleValue(value) );
  else if (command == fCmdParticlePhi1)    fPrimaryGeneratorAction -> SetParticlePhi1( fCmdParticlePhi1 -> GetNewDoubleValue(value) );
  else if (command == fCmdParticlePhi1)    fPrimaryGeneratorAction -> SetParticlePhi2( fCmdParticlePhi1 -> GetNewDoubleValue(value) );
  else if (command == fCmdParticleTheta1)  fPrimaryGeneratorAction -> SetParticleTheta1( fCmdParticleTheta1 -> GetNewDoubleValue(value) );
  else if (command == fCmdParticleTheta1)  fPrimaryGeneratorAction -> SetParticleTheta2( fCmdParticleTheta1 -> GetNewDoubleValue(value) );
}
