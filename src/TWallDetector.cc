#include "TWallDetector.hh"

TVector3 TWallDetector::LocalToGlobalPos(TVector3 localPos)
{
  TVector3 globalPos = localPos;
  globalPos.RotateY( fRotationY );
  globalPos += GetWallOffset();
  return globalPos;
}

TVector3 TWallDetector::GlobalToLocalPos(TVector3 globalPos)
{
  TVector3 localPos = globalPos;
  localPos -= GetWallOffset();
  localPos.RotateY( -fRotationY );
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

TVector3 TWallDetector::GetWallOffset() const
{
  return TVector3(fDistTarget*sin(fRotationY), fYTarget, fDistTarget*cos(fRotationY)+fZTarget);
}

int TWallDetector::GetVetoID(int i) const
{
  return fVetoIDBase + i;
}

int TWallDetector::GetBarID(int layer, int row) const
{
  return fWallIDBase + layer * 100 + row;
}

int TWallDetector::GetRow(int id) const
{
  if (id < fWallIDBase)
    return -1;

  id = id - fWallIDBase;
  auto row = id - (id/100)*100;
  return row;
}

int TWallDetector::GetLayer(int id) const
{
  if (id < fWallIDBase)
    return -1;

  id = id - fWallIDBase;
  auto layer = id/100;
  return layer;
}

bool TWallDetector::IsWallID(int id)
{
  if (id < fWallIDBase || id > fWallIDMax)
    return false;
  return true;
}

bool TWallDetector::IsVetoID(int id)
{
  if (id < fVetoIDBase || id > fVetoIDMax)
    return false;
  return true;
}
