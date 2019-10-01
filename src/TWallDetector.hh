#ifndef STNEULAND_HH
#define STNEULAND_HH

#include "TMath.h"
#include "TVector3.h"
#include "G4ThreeVector.hh"

class TWallDetector
{
  public:
    TWallDetector() {}
    ~TWallDetector() {}

    G4ThreeVector LocalToGlobalPos(G4ThreeVector localPos);
    G4ThreeVector GlobalToLocalPos(G4ThreeVector globalPos);

    TVector3 LocalToGlobalPos(TVector3 localPos);
    TVector3 GlobalToLocalPos(TVector3 globalPos);

    Double_t fZTarget = -13.24; // mm
    Double_t fDistWall = 9093.85;
    Double_t fRotYWall_deg = 29.579;
    Double_t fRotYWall_rad = fRotYWall_deg*TMath::DegToRad();
    Double_t fOffxWall = fDistWall * TMath::Sin( fRotYWall_rad ); 
    Double_t fOffyWall = 0.;   
    Double_t fOffzWall = fDistWall * TMath::Cos( fRotYWall_rad ) + fZTarget;
};

#endif
