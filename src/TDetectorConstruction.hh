#ifndef TDETECTORCONSTRUCTION_HH
#define TDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "TWallDetector.hh"

class G4VPhysicalVolume;

class TDetectorConstruction : public G4VUserDetectorConstruction
{
  private:
    bool fSetBuildTPC  = false;
    bool fSetBuildBar  = false;
    bool fSetBuildBar0 = true;
    bool fSetBuildWall = false;

    double fTPCStepLimit = -1; ///< mm
    double fBarStepLimit = -1; ///< mm

    bool fCreateField = false;
    G4String fFieldMapFileName = "data/field.root";

    TWallDetector *fWallDetector = nullptr;

  private:
    void PrintMessage(const char *message, const char *value="");

  public:
    TDetectorConstruction(): G4VUserDetectorConstruction() {}
    virtual ~TDetectorConstruction() {}
    virtual G4VPhysicalVolume* Construct();
    void ConstructField();

    void Clear() {
      fSetBuildBar  = false;
      fSetBuildBar0 = false;
      fSetBuildWall = false;
    }

    void SetBuildTPC()  {          fSetBuildTPC  = true; }
    void SetBuildBar()  { Clear(); fSetBuildBar  = true; }
    void SetBuildBar0() { Clear(); fSetBuildBar0 = true; }
    void SetBuildWall() { Clear(); fSetBuildWall = true; }

    void SetTPCStepLimits(G4double limit_mm) { fTPCStepLimit = limit_mm; }
    void SetBarStepLimits(G4double limit_mm) { fBarStepLimit = limit_mm; }

    void SetNoField() { fCreateField = false; }
    void SetFieldName(G4String name);

    int GetBarID(int layer, int row);
    int GetRow(int id);
    int GetLayer(int id);

    void SetWallDetector(TWallDetector *wd) { fWallDetector = wd; }
};

#endif
