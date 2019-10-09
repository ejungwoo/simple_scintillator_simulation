#ifndef TMESSENGER_HH
#define TMESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWith3VectorAndUnit.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "TDetectorConstruction.hh"
#include "TPrimaryGeneratorAction.hh"
#include "TWallDetector.hh"

/**
 * - /my/init/detector/tpc
 * - /my/init/detector/bar
 * - /my/init/detector/bar0
 * - /my/init/detector/wall
 * - /my/init/field/name [name]
 *
 */

class TMessenger : public G4UImessenger
{
  public:
    TMessenger();
    virtual ~TMessenger();

    void SetNewValue(G4UIcommand *command, G4String value);

    void SetDetectorConstruction(TDetectorConstruction *dc) { fDetectorConstruction = dc; }
    void SetPrimaryGeneratorAction(TPrimaryGeneratorAction *pga) { fPrimaryGeneratorAction = pga; }
    void SetWallDetector(TWallDetector *wd) { fWallDetector = wd; }

  private:
    TDetectorConstruction *fDetectorConstruction = nullptr;
    TPrimaryGeneratorAction *fPrimaryGeneratorAction = nullptr;
    TWallDetector *fWallDetector = nullptr;

    G4UIcmdWithAString *fCmdFieldFileName;
    G4UIcmdWithoutParameter *fCmdBuildTPC;
    G4UIcmdWithoutParameter *fCmdBuildBar;
    G4UIcmdWithoutParameter *fCmdBuildBar0;
    G4UIcmdWithoutParameter *fCmsBuildWall;

    G4UIcmdWithoutParameter *fCmsSetParticleGun;
    G4UIcmdWithAnInteger *fCmdParticlePDG;
    G4UIcmdWithAString *fCmdParticleName;
    G4UIcmdWith3VectorAndUnit *fCmdParticleVertex;
    G4UIcmdWithADoubleAndUnit *fCmdParticleEnergy1;
    G4UIcmdWithADoubleAndUnit *fCmdParticleEnergy2;
    G4UIcmdWithADoubleAndUnit *fCmdParticlePhi1;
    G4UIcmdWithADoubleAndUnit *fCmdParticlePhi2;
    G4UIcmdWithADoubleAndUnit *fCmdParticleTheta1;
    G4UIcmdWithADoubleAndUnit *fCmdParticleTheta2;

    G4UIcmdWithADoubleAndUnit *fCmdDistTarget;
    G4UIcmdWithADoubleAndUnit *fCmdRotationY;
};

#endif
