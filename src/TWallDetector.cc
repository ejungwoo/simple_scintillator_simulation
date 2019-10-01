#include "TWallDetector.hh"

TVector3 TWallDetector::LocalToGlobalPos(TVector3 localPos)
{
  TVector3 globalPos = localPos;
  globalPos.RotateY( fRotYWall_deg * TMath::DegToRad() );
  globalPos += TVector3(fOffxWall, fOffyWall, fOffzWall);
  return globalPos;
}

TVector3 TWallDetector::GlobalToLocalPos(TVector3 globalPos)
{
  TVector3 localPos = globalPos;
  localPos -= TVector3(fOffxWall, fOffyWall, fOffzWall);
  localPos.RotateY( -fRotYWall_deg * TMath::DegToRad() );
  return localPos;
}

G4ThreeVector TWallDetector::LocalToGlobalPos(G4ThreeVector localPos)
{
  auto pos = LocalToGlobalPos(TVector3(localPos.x(),localPos.y(),localPos.z()));
  return G4ThreeVector(pos.x(),pos.y(),pos.z());
}

G4ThreeVector TWallDetector::GlobalToLocalPos(G4ThreeVector globalPos)
{
  auto pos = GlobalToLocalPos(TVector3(globalPos.x(),globalPos.y(),globalPos.z()));
  return G4ThreeVector(pos.x(),pos.y(),pos.z());
}