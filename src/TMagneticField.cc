#include "TMagneticField.hh"

#include "TFile.h"

TMagneticField::TMagneticField(const char* fileName)
: G4MagneticField()
{
  auto file = new TFile(fileName,"read");
  fHistField = (TH3D *) file -> Get("field");
}

void TMagneticField::GetFieldValue(const G4double positionTime[4], G4double *bField) const
{
  // field center offset
  Double_t x = positionTime[0] + 1.794;
  Double_t y = positionTime[1] + 20.5502;
  Double_t z = positionTime[2] - 580.526;

  bField[0] = 0;
  bField[1] = 0;
  bField[2] = 0;

  if (x < 0) x = -x;
  if (z < 0) z = -z;

  if (x > 3000. || y > 400. || y < -400 || z > 3000.)
    return;

  // const method of TH3::Interpolate

  auto xAxis = fHistField -> GetXaxis();
  auto yAxis = fHistField -> GetYaxis();
  auto zAxis = fHistField -> GetZaxis();

  // input from geant4 (x, y, z)is mm
  // histogram length unit is cm

  Int_t binx1 = xAxis -> FindBin(x);
  if ( x < xAxis->GetBinCenter(binx1) ) binx1 -= 1;
  Int_t binx2 = binx1 + 1;

  Int_t biny1 = yAxis -> FindBin(y);
  if ( y < yAxis->GetBinCenter(biny1) ) biny1 -= 1;
  Int_t biny2 = biny1 + 1;

  Int_t binz1 = zAxis -> FindBin(z);
  if ( z < zAxis->GetBinCenter(binz1) ) binz1 -= 1;
  Int_t binz2 = binz1 + 1;

  const int binw = 10;

  Double_t xDisplacement = (x - xAxis -> GetBinCenter(binx1)) / binw;
  Double_t yDisplacement = (y - yAxis -> GetBinCenter(biny1)) / binw;
  Double_t zDisplacement = (z - zAxis -> GetBinCenter(binz1)) / binw;

  Double_t v[] = { fHistField->GetBinContent( binx1, biny1, binz1 ), fHistField->GetBinContent( binx1, biny1, binz2 ),
                   fHistField->GetBinContent( binx1, biny2, binz1 ), fHistField->GetBinContent( binx1, biny2, binz2 ),
                   fHistField->GetBinContent( binx2, biny1, binz1 ), fHistField->GetBinContent( binx2, biny1, binz2 ),
                   fHistField->GetBinContent( binx2, biny2, binz1 ), fHistField->GetBinContent( binx2, biny2, binz2 ) };

  Double_t i1 = v[0] * (1 - zDisplacement) + v[1] * zDisplacement;
  Double_t i2 = v[2] * (1 - zDisplacement) + v[3] * zDisplacement;
  Double_t j1 = v[4] * (1 - zDisplacement) + v[5] * zDisplacement;
  Double_t j2 = v[6] * (1 - zDisplacement) + v[7] * zDisplacement;

  Double_t w1 = i1 * (1 - yDisplacement) + i2 * yDisplacement;
  Double_t w2 = j1 * (1 - yDisplacement) + j2 * yDisplacement;

  Double_t result = w1 * (1 - xDisplacement) + w2 * xDisplacement;

  bField[1] = result;
}
