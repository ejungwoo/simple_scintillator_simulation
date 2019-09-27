#include "SCanvas.hh"
#include "SCanvas.cc"
#include "ejungwoo.h"

void draw_proton_searching_crosssection_angle()
{
  ejungwoo::gversion("proton_cs2");
  ejungwoo::gdata("summary_data");

  auto tree = new TChain("event");
  for (auto split = 0; split < 200; split++)
    tree -> AddFile(Form("data/ang_s%d_proton.root",split));

  if (1) {
    //auto cvs = new SCanvas("kphi","",100,100,500,500);
    auto hist = new TH2D("hist_kphi",";energy;phi",100,0,3000,300,0,2);
    tree -> Draw("phi0:kine0>>hist_kphi","ebar>0","colz");
    //cvs -> Draw();
    ejungwoo::write(hist);
  }

  if (0) {
    //auto cvs = new SCanvas("cvs_et","",100,100,500,500);
    auto hist = new TH2D("hist_et",";1/TOF;energy",300,0.005,0.035,300,0,3000);
    tree -> Draw("kine0:1/tof1>>hist_et","ebar>0&&tof1>0","colz");
    //cvs -> Draw();
    ejungwoo::write(hist);
  }
}
