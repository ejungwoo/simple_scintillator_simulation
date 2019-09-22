#ifndef TMAGNETICFIELD_HH
#define TMAGNETICFIELD_HH 1

#include "G4MagneticField.hh"

#include "TH3D.h"

class TMagneticField : public G4MagneticField
{
  public:
    TMagneticField(const char* fileName);
    virtual ~TMagneticField() {}

    virtual void GetFieldValue(const G4double positionTime[4], G4double *bField) const;

  private:
    TH3D *fHistField = nullptr;
};

#endif

