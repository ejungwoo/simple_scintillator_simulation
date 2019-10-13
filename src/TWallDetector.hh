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
    //double fBarWidth = 50.*CLHEP::mm;
    double fBarWidth = 47.*CLHEP::mm;
    int fNumLayers = 8;

    double fXLengthVetoBar = 320.*CLHEP::mm;
    double fYLengthVetoBar = 1900.*CLHEP::mm;
    double fZLengthVetoBar = 10.*CLHEP::mm;
    double fXOffsetVetoBar0 = -1099.75*CLHEP::mm;
    double fXOffsetVetoBarNext = 315.28*CLHEP::mm;
    double fZOffsetVetoBar[2] = {-363.58*CLHEP::mm, -340.58*CLHEP::mm};
    int fNumVetoBars = 8;

    int fWallIDBase = 2000;
    int fWallIDMax = fWallIDBase + 399;

    int fVetoIDBase = 3000;
    int fVetoIDMax = fVetoIDBase + 7;

  public:
    TWallDetector() {}
    ~TWallDetector() {}

    int GetVetoID(int i) const;
    int GetBarID(int layer, int row) const;
    int GetRow(int id) const;
    int GetLayer(int id) const;

    G4ThreeVector LocalToGlobalPos(G4ThreeVector localPos);
    G4ThreeVector GlobalToLocalPos(G4ThreeVector globalPos);

    TVector3 LocalToGlobalPos(TVector3 localPos);
    TVector3 GlobalToLocalPos(TVector3 globalPos);

    void SetDistanceFromTheTarget(double distance) { fDistTarget = distance; }
    void SetRotationYGlobal(double rotation) { fRotationY = rotation; }
    void SetBarWidth(double width) { fBarWidth = width; }

    double GetDistanceFromTheTarget() const { return fDistTarget; }
    double GetRotationYGlobal() const { return fRotationY; }
    double GetBarLength() const { return fBarLength; }
    double GetBarWidth() const { return fBarWidth; }
    double GetNumLayers() const { return fNumLayers; }
    double GetWallZThickness() const { return fNumLayers * fBarWidth; }

    TVector3 GetWallOffset() const;

    double GetXLengthVetoBar() const { return fXLengthVetoBar; }
    double GetYLengthVetoBar() const { return fYLengthVetoBar; }
    double GetZLengthVetoBar() const { return fZLengthVetoBar; }
    double GetXOffsetVetoBar0() const { return fXOffsetVetoBar0; }
    double GetXOffsetVetoBarNext() const { return fXOffsetVetoBarNext; }
    double GetZOffsetVetoBar(int i) const { return fZOffsetVetoBar[i]; }
    int GetNumVetoBars() const { return fNumVetoBars; }

    bool IsWallID(int id);
    bool IsVetoID(int id);
};

#endif
