#include "SCanvas.hh"
#include "SCanvas.cc"
#include "ejungwoo.h"

void get_proton_cross_section()
{
  ejungwoo::gversion("proton_cs2");
  ejungwoo::gdata("summary_data");

  if (0)
  {
    auto file = new TFile("data/hist_et.proton_cs2.root");
    auto hist = (TH2D *) file -> Get("hist_et");
    auto cvs = new SCanvas("et","",100,100,500,500);
    hist -> Draw("colz");
    cvs -> Draw();
  }

  if (1) {
    auto file = new TFile("summary_data/hist_kphi.proton_cs2.root");
    auto hist = (TH2D *) file -> Get("hist_kphi");
    auto cvs = new SCanvas("kphi","",100,100,500,500);
    hist -> Draw("colz");
    cvs -> Draw();

    int count = 1;
    for (auto bin=100; bin>=1; --bin)
    //for (auto bin=100; bin>=95; --bin)
    {
      auto energy = hist -> GetXaxis() -> GetBinCenter(bin);
      auto energy1 = hist -> GetXaxis() -> GetBinLowEdge(bin);
      auto energy2 = hist -> GetXaxis() -> GetBinUpEdge(bin);
      TString name = Form("energy_%d",int(energy));
      cout << name << endl;
      auto h1 = hist -> ProjectionY(name,bin,1+bin);
      hist -> SetNameTitle(name,Form("energy = %d ~ %d MeV; #phi_{0} (Rad.)", int(energy1), int(energy2)));
      ejungwoo::write(h1);
    }
  }
}
