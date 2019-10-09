#ifndef STNEULAND_HH
#define STNEULAND_HH

#include <cmath>
#include "TMath.h"
#include "TVector3.h"
#include "G4ThreeVector.hh"
#include "G4SystemOfUnits.hh"

using std::sin;
using std::cos;

class TWallDetector
{
  private:
    double fZTarget = -13.24*CLHEP::mm;
    double fYTarget = -205.*CLHEP::mm;
    double fDistTarget = 9093.85*CLHEP::mm;
    double fRotationY = 29.579*CLHEP::deg;
    double fBarLength = 2500.*CLHEP::mm;
    double fBarWidth = 50.*CLHEP::mm;
    int fNumLayers = 8;

  public:
    TWallDetector() {}
    ~TWallDetector() {}

    G4ThreeVector LocalToGlobalPos(G4ThreeVector localPos);
    G4ThreeVector GlobalToLocalPos(G4ThreeVector globalPos);

    TVector3 LocalToGlobalPos(TVector3 localPos);
    TVector3 GlobalToLocalPos(TVector3 globalPos);

    void SetDistanceFromTheTarget(double distance) { fDistTarget = distance; }
    void SetRotationYGlobal(double rotation) { fRotationY = rotation; }

    double GetDistanceFromTheTarget() const { return fDistTarget; }
    double GetRotationYGlobal() const { return fRotationY; }
    double GetBarLength() const { return fBarLength; }
    double GetBarWidth() const { return fBarWidth; }
    double GetNumLayers() const { return fNumLayers; }
    double GetWallZThickness() const { return fNumLayers * fBarWidth; }

    TVector3 GetWallOffset() const;
};

#endif
