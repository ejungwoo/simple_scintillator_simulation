#ifndef TMESSENGER_HH
#define TMESSENGER_HH

#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "TDetectorConstruction.hh"

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

  private:
    TDetectorConstruction *fDetectorConstruction = nullptr;

    G4UIcmdWithAString *fCmdFieldFileName;
    G4UIcmdWithoutParameter *fCmdBuildTPC;
    G4UIcmdWithoutParameter *fCmdBuildBar;
    G4UIcmdWithoutParameter *fCmdBuildBar0;
    G4UIcmdWithoutParameter *fCmsBuildWall;
};

#endif
