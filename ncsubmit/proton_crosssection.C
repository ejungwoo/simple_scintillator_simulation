#include "ejungwoo.h"
void proton_crosssection()
{
  auto file = new TFile("summary_data/proton_energy_at_phi.root");

  ejungwoo::cc();
  file -> Get("hist_kphi") -> Draw("colz");
  auto up = (TGraph *) file -> Get("up");
  auto lo = (TGraph *) file -> Get("lo");
  up -> Draw("samelp");
  lo -> Draw("samelp");

  auto graph1 = new TGraph();
  auto graph2 = new TGraph();

  for (auto energy = 0.; energy < 3000; energy += 10.)
  {
    graph1 -> SetPoint(graph1->GetN(),energy,up->Eval(energy)+.02);
    graph2 -> SetPoint(graph2->GetN(),energy,lo->Eval(energy)-.02);
  }
  graph1 -> Draw("samel");
  graph2 -> Draw("samel");
}
