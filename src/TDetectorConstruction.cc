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

  auto matAir = nist -> FindOrBuildMaterial("G4_AIR");
  auto matBar = nist -> FindOrBuildMaterial("G4_XYLENE");
  auto matWallSpace = matAir;




  auto id_world = 0;
  auto id_tpc = 1;
  auto id_bar = 2;
  auto id_wallspace = id_world;



  auto lx_world = 2*10000.*mm;
  auto ly_world = 4* 1000.*mm;
  auto lz_world = 2*10000.*mm;

  auto solidWorld = new G4Box("World", lx_world/2., ly_world/2., lz_world/2.);
  auto logicWorld = new G4LogicalVolume(solidWorld, matAir, "World");
  auto physiWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, id_world, true);

  {
    auto att = new G4VisAttributes();
    att -> SetForceWireframe(true);
    logicWorld -> SetVisAttributes(att);
  }



  auto oz_target = -13.24*mm;
  auto oy_target = -205.*mm;

  auto lx_tpc =  864.*mm;
  auto ly_tpc =  530.*mm;
  auto lz_tpc = 1344.*mm;
  auto oy_tpc = -ly_tpc/2.;
  auto oz_tpc =  lz_tpc/2. - oz_target;

  auto lx_bar = 2500.*mm;
  auto ly_bar =   50.*mm;
  auto lz_bar =   50.*mm;
  auto lz_wall= 400.*mm;

  auto dist_bar = 9093.85*mm;
  auto phi_bar = 29.579*CLHEP::deg;

  auto ox_bar = dist_bar * sin( phi_bar );
  auto oy_bar = oy_target;
  auto oz_bar = dist_bar * cos( phi_bar ) + oz_target;



  if (fSetBuildBar0)
  {
    PrintMessage("Set bar at target origin");

    auto solidBar = new G4Box("bar", .5*lx_bar, .5*ly_bar, .5*lz_bar);
    auto logicBar = new G4LogicalVolume(solidBar, matBar, "bar");
    if (fBarStepLimit>0) logicBar -> SetUserLimits(new G4UserLimits(fBarStepLimit*mm));
    auto rotation = new G4RotationMatrix();
    rotation -> rotateY(-phi_bar);
    new G4PVPlacement(rotation, G4ThreeVector(0,oy_bar,.5*lz_bar), logicBar, "bar", logicWorld, false, id_bar, true);

    auto att = new G4VisAttributes(G4Colour(G4Colour::Black()));
    att -> SetForceWireframe(true);
    logicBar -> SetVisAttributes(att);
  }


  if (fSetBuildTPC)
  {
    PrintMessage("Set TPC");

    auto solidTPC = new G4Box("TPC", .5*lx_tpc, .5*ly_tpc, .5*lz_tpc);
    auto logicTPC = new G4LogicalVolume(solidTPC, matGas, "TPC");
    if (fTPCStepLimit>0) logicTPC -> SetUserLimits(new G4UserLimits(fTPCStepLimit*mm));
    new G4PVPlacement(0, G4ThreeVector(0,oy_tpc,oz_tpc), logicTPC, "TPC", logicWorld, false, id_tpc, true);

    auto att = new G4VisAttributes(G4Colour(G4Colour::Blue()));
    att -> SetForceWireframe(true);
    logicTPC -> SetVisAttributes(att);
  }


  if (fSetBuildBar)
  {
    PrintMessage("Set bar");

    auto solidBar = new G4Box("bar", .5*lx_bar, .5*ly_bar, .5*lz_bar);
    auto logicBar = new G4LogicalVolume(solidBar, matBar, "bar");
    if (fBarStepLimit>0) logicBar -> SetUserLimits(new G4UserLimits(fBarStepLimit*mm));
    auto rotation = new G4RotationMatrix();
    rotation -> rotateY(-phi_bar);
    new G4PVPlacement(rotation, G4ThreeVector(ox_bar,oy_bar,oz_bar), logicBar, "bar", logicWorld, false, id_bar, true);

    auto att = new G4VisAttributes(G4Colour(G4Colour::Black()));
    att -> SetForceWireframe(true);
    logicBar -> SetVisAttributes(att);
  }


  if (fSetBuildWall)
  {
    PrintMessage("Set wall");

    auto solidWallSpace = new G4Box("wallSpace", .6*lx_bar, .6*lx_bar, 2.*lz_wall);
    auto logicWallSpace = new G4LogicalVolume(solidWallSpace, matWallSpace, "wallSpace");
    auto rotation = new G4RotationMatrix();
    rotation -> rotateY(-phi_bar);
    new G4PVPlacement(rotation, G4ThreeVector(ox_bar,oy_bar,oz_bar), logicWallSpace, "bar", logicWorld, false, id_wallspace, true);

    {
      auto att = new G4VisAttributes(G4Colour(G4Colour::Magenta()));
      att -> SetForceWireframe(true);
      logicWallSpace -> SetVisAttributes(att);
    }

    {
      auto solidBar = new G4Box("bar", .5*lx_bar, .5*ly_bar, .5*lz_bar);
      auto logicBar = new G4LogicalVolume(solidBar, matBar, "bar");
      if (fBarStepLimit>0) logicBar -> SetUserLimits(new G4UserLimits(fBarStepLimit*mm));
      for (auto layer : {0,2,4,6})
        for (auto pm : {-1,1})
          for (auto ibar=0; ibar<25; ++ibar) {
            auto oy_bar2 = .5*ly_bar+ibar*ly_bar;
            auto oz_bar2 = lz_bar*layer;
            new G4PVPlacement(0, G4ThreeVector(0,pm*oy_bar2,oz_bar2), logicBar, "bar", logicWallSpace, false, id_bar, true);
          }

      auto att = new G4VisAttributes(G4Colour(G4Colour::Black()));
      att -> SetForceWireframe(true);
      logicBar -> SetVisAttributes(att);
    }

    {
      auto solidBar = new G4Box("bar", .5*ly_bar, .5*lx_bar, .5*lz_bar);
      auto logicBar = new G4LogicalVolume(solidBar, matBar, "bar");
      if (fBarStepLimit>0) logicBar -> SetUserLimits(new G4UserLimits(fBarStepLimit*mm));
      auto rotation = new G4RotationMatrix();
      rotation -> rotateY(-phi_bar);
      for (auto layer : {1,3,5,7})
        for (auto pm : {-1,1})
          for (auto ibar=0; ibar<25; ++ibar) {
            auto oy_bar2 = .5*ly_bar+ibar*ly_bar;
            auto oz_bar2 = lz_bar*layer;
            new G4PVPlacement(rotation, G4ThreeVector(pm*oy_bar2,0,oz_bar2), logicBar, "bar", logicWallSpace, false, id_bar, true);
          }

      auto att = new G4VisAttributes(G4Colour(G4Colour::Gray()));
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
