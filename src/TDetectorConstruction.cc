#include "TDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4UserLimits.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "TMagneticField.hh"

#include <iostream>
#include <cmath>
using namespace std;

G4VPhysicalVolume* TDetectorConstruction::Construct()
{
  auto nist = G4NistManager::Instance();

  auto STPTemperature = 273.15;
  auto labTemperature = STPTemperature + 20.*kelvin;
  
  auto elH = new G4Element("elH", "H", 1.,  1.00794*g/mole);
  auto elC = new G4Element("elC", "C", 6., 12.011*g/mole);

  auto denArGas = 1.782e-3*g/cm3*STPTemperature/labTemperature;
  auto matArGas = new G4Material("ArgonGas", 18, 39.948*g/mole, denArGas, kStateGas, labTemperature);
 
  auto denMethane = 0.717e-3*g/cm3*STPTemperature/labTemperature;
  auto matMethaneGas = new G4Material("matMethaneGas ", denMethane, 2, kStateGas, labTemperature);
       matMethaneGas -> AddElement(elH, 4);
       matMethaneGas -> AddElement(elC, 1);

  auto denGas = .9*denArGas + .1*denMethane;
  auto matGas = new G4Material("matP10", denGas, 2, kStateGas, labTemperature);
       matGas -> AddMaterial(matArGas,      0.9*denArGas  /denGas);
       matGas -> AddMaterial(matMethaneGas, 0.1*denMethane/denGas);

  auto denBC408 = 1.023*g/cm3;
  auto matBC408 = new G4Material("matBC408", denBC408, 2, kStateSolid, labTemperature);
       matBC408 -> AddElement(elH, 0.524);
       matBC408 -> AddElement(elC, 0.476);

  auto matAir = nist -> FindOrBuildMaterial("G4_AIR");
  auto matVeto = matBC408;
  auto matBar = matBC408;
  auto matWallSpace = matAir;

  ////////////////////////////////////////////////////////////////////////////////////////////////////////

  auto idWorld = 0;
  auto idTpc = 1;
  auto idBar0 = fWallDetector -> GetBarID(0,0);
  auto idWallSpace = 3;

  ////////////////////////////////////////////////////////////////////////////////////////////////////////

  auto xLengthWorld = 2*10000.*mm;
  auto yLengthWorld = 4* 1000.*mm;
  auto zLengthWorld = 2*10000.*mm;

  auto xLengthTpc =  864.*mm;
  auto yLengthTpc =  530.*mm;
  auto zLengthTpc = 1344.*mm;
  auto yOffsetTpc = -yLengthTpc/2.;
  auto zOffsetTpc =  zLengthTpc/2.;

  auto widthBar = fWallDetector -> GetBarWidth();
  auto lengthBar = fWallDetector -> GetBarLength();
  auto offsetWall = fWallDetector -> GetWallOffset();
  auto xOffsetWall = offsetWall.x();
  auto yOffsetWall = offsetWall.y();
  auto zOffsetWall = offsetWall.z();
  auto phiWallGlobal = fWallDetector -> GetRotationYGlobal();
  auto zThicknessWall = fWallDetector -> GetWallZThickness();

  ////////////////////////////////////////////////////////////////////////////////////////////////////////

  auto solidWorld = new G4Box("World", xLengthWorld/2., yLengthWorld/2., zLengthWorld/2.);
  auto logicWorld = new G4LogicalVolume(solidWorld, matAir, "World");
  auto physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, idWorld, true);

  {
    auto att = new G4VisAttributes();
    att -> SetForceWireframe(true);
    logicWorld -> SetVisAttributes(att);
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (fSetBuildBar0)
  {
    PrintMessage("Set bar at target origin");

    auto solidBar = new G4Box("bar", .5*lengthBar, .5*widthBar, .5*widthBar);
    auto logicBar = new G4LogicalVolume(solidBar, matBar, "bar");
    if (fBarStepLimit>0) logicBar -> SetUserLimits(new G4UserLimits(fBarStepLimit*mm));
    auto rotation = new G4RotationMatrix();
    rotation -> rotateY(-phiWallGlobal);
    new G4PVPlacement(rotation, G4ThreeVector(0,yOffsetWall,.5*widthBar), logicBar, "bar", logicWorld, false, idBar0, true);

    auto att = new G4VisAttributes(G4Colour(G4Colour::Black()));
    att -> SetForceWireframe(true);
    logicBar -> SetVisAttributes(att);
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (fSetBuildTPC)
  {
    PrintMessage("Set TPC");

    auto solidTPC = new G4Box("TPC", .5*xLengthTpc, .5*yLengthTpc, .5*zLengthTpc);
    auto logicTPC = new G4LogicalVolume(solidTPC, matGas, "TPC");
    if (fTPCStepLimit>0) logicTPC -> SetUserLimits(new G4UserLimits(fTPCStepLimit*mm));
    new G4PVPlacement(0, G4ThreeVector(0,yOffsetTpc,zOffsetTpc), logicTPC, "TPC", logicWorld, false, idTpc, true);

    auto att = new G4VisAttributes(G4Colour(G4Colour::Blue()));
    att -> SetForceWireframe(true);
    logicTPC -> SetVisAttributes(att);
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (fSetBuildBar)
  {
    PrintMessage("Set bar");

    auto solidBar = new G4Box("bar", .5*lengthBar, .5*widthBar, .5*widthBar);
    auto logicBar = new G4LogicalVolume(solidBar, matBar, "bar");
    if (fBarStepLimit>0) logicBar -> SetUserLimits(new G4UserLimits(fBarStepLimit*mm));
    auto rotation = new G4RotationMatrix();
    rotation -> rotateY(-phiWallGlobal);
    new G4PVPlacement(rotation, G4ThreeVector(xOffsetWall,yOffsetWall,zOffsetWall), logicBar, "bar", logicWorld, false, idBar0, true);

    auto att = new G4VisAttributes(G4Colour(G4Colour::Black()));
    att -> SetForceWireframe(true);
    logicBar -> SetVisAttributes(att);
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////

  if (fSetBuildWall)
  {
    PrintMessage("Set wall");

    auto solidWallSpace = new G4Box("wallSpace", .8*lengthBar, .8*lengthBar, 2.*zThicknessWall);
    auto logicWallSpace = new G4LogicalVolume(solidWallSpace, matWallSpace, "wallSpace");
    auto rotation = new G4RotationMatrix();
    rotation -> rotateY(-phiWallGlobal);
    new G4PVPlacement(rotation, G4ThreeVector(xOffsetWall,yOffsetWall,zOffsetWall), logicWallSpace, "wallspace", logicWorld, false, idWallSpace, true);

    {
      auto att = new G4VisAttributes(G4Colour(G4Colour::Magenta()));
      att -> SetForceWireframe(true);
      logicWallSpace -> SetVisAttributes(att);
    }

    // event layers
    {
      auto solidBar = new G4Box("bar", .5*lengthBar, .5*widthBar, .5*widthBar);
      auto logicBar = new G4LogicalVolume(solidBar, matBar, "bar");
      if (fBarStepLimit>0) logicBar -> SetUserLimits(new G4UserLimits(fBarStepLimit*mm));
      for (auto layer : {0,2,4,6}) {
        auto zOffsetBarLocal = widthBar*layer;
        for (auto row=0; row<50; ++row) {
          auto yOffsetBarLocal = row*widthBar - .5*(widthBar*50) + .5*widthBar;
          auto idBar = fWallDetector -> GetBarID(layer, row);
          new G4PVPlacement(0, G4ThreeVector(0,yOffsetBarLocal,zOffsetBarLocal), logicBar, "bar", logicWallSpace, false, idBar, true);
        }
      }

      auto att = new G4VisAttributes(G4Colour(G4Colour::Black()));
      att -> SetForceWireframe(true);
      logicBar -> SetVisAttributes(att);
    }

    // odd layers
    {
      auto solidBar = new G4Box("bar", .5*widthBar, .5*lengthBar, .5*widthBar);
      auto logicBar = new G4LogicalVolume(solidBar, matBar, "bar");
      if (fBarStepLimit>0) logicBar -> SetUserLimits(new G4UserLimits(fBarStepLimit*mm));

      for (auto layer : {1,3,5,7}) {
        auto zOffsetBarLocal = widthBar*layer;
        for (auto row=0; row<50; ++row) {
          auto xOffsetBarLocal = row*widthBar - .5*(widthBar*50) + .5*widthBar;
          auto idBar = fWallDetector -> GetBarID(layer, row);
          new G4PVPlacement(0, G4ThreeVector(xOffsetBarLocal,0,zOffsetBarLocal), logicBar, "bar", logicWallSpace, false, idBar, true);
        }
      }

      auto att = new G4VisAttributes(G4Colour(G4Colour::Gray()));
      att -> SetForceWireframe(true);
      logicBar -> SetVisAttributes(att);
    }

    if (fSetBuildVeto)
    {
      double xLengthVetoBar     = fWallDetector -> GetXLengthVetoBar();
      double yLengthVetoBar     = fWallDetector -> GetYLengthVetoBar();
      double zLengthVetoBar     = fWallDetector -> GetZLengthVetoBar();
      double xOffsetVetoBar0    = fWallDetector -> GetXOffsetVetoBar0();
      double xOffsetVetoBarNext = fWallDetector -> GetXOffsetVetoBarNext();
      double zOffsetVetoBar0    = fWallDetector -> GetZOffsetVetoBar(0);
      double zOffsetVetoBar1    = fWallDetector -> GetZOffsetVetoBar(1);
         int numVetoBars        = fWallDetector -> GetNumVetoBars();

      auto solidBar = new G4Box("veto", .5*xLengthVetoBar, .5*yLengthVetoBar, .5*zLengthVetoBar);
      auto logicBar = new G4LogicalVolume(solidBar, matVeto, "veto");
      if (fBarStepLimit>0) logicBar -> SetUserLimits(new G4UserLimits(fBarStepLimit*mm));

      for (auto iBar=0; iBar<numVetoBars; ++iBar) {
        auto zOffsetBarLocal = iBar%2==0 ? zOffsetVetoBar0 : zOffsetVetoBar1;
        auto idVeto = fWallDetector -> GetVetoID(iBar);
        new G4PVPlacement(0, G4ThreeVector(xOffsetVetoBar0+xOffsetVetoBarNext*iBar,0,zOffsetBarLocal), logicBar, "veto", logicWallSpace, false, idVeto, true);
      }

      auto att = new G4VisAttributes(G4Colour(G4Colour::Yellow()));
      att -> SetForceWireframe(true);
      logicBar -> SetVisAttributes(att);
    }
  }

  if (fCreateField)
    ConstructField();

  return physiWorld;
}


void TDetectorConstruction::ConstructField()
{
  PrintMessage("Initializing field : ", fFieldMapFileName.data());

  TMagneticField *bfield = new TMagneticField(fFieldMapFileName.data());

  auto fieldManager = G4TransportationManager::GetTransportationManager() -> GetFieldManager();
  fieldManager -> SetDetectorField(bfield);
  fieldManager -> CreateChordFinder(bfield);
}


void TDetectorConstruction::PrintMessage(const char *message, const char *value)
{
  G4cout << "  [TDetectorConstruction] " << message << value << G4endl;
}

void TDetectorConstruction::SetFieldName(G4String name)
{
  if (!name.isNull())
    fCreateField = true;
  fFieldMapFileName = name;
}
