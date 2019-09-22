using namespace ejungwoo;

void draw_summary(TString filename = "data/run_proton_notpc.root")
{
  gstat(0);
  gfstat(1);

  auto graphk = new TGraph();
  graphk -> SetName("graphk");
  auto graphp = new TGraph();
  graphp -> SetName("graphp");

  auto mass_proton = 938.27200;

  for (auto kine = 100; kine <= 2000; kine += 50)
  {
    auto file = new TFile(Form("data/bar0_run_proton_k%d.root",kine));
    auto tree = (TTree *) file -> Get("event");
    gheader(Form("kine%d",kine));
    auto hist = tp("ebar",tree,"ebar","ebar>0","",200,0,40);
    auto f1 = fitg(hist,1.5);

    if (0) {
      auto cvs = new SCanvas(Form("ebar_kine%d",kine));
      hist -> Draw();
      f1 -> Draw("samel");
      cvs -> Draw();
    }

    auto mean = f1 -> GetParameter(1);
    graphk -> SetPoint(graphk->GetN(),kine,mean);

    auto momentum = sqrt((kine + mass_proton) * (kine + mass_proton) - mass_proton * mass_proton);
    graphp -> SetPoint(graphp->GetN(),momentum,mean);
  }

  auto cvsk = new SCanvas("cvsk");
  cvsk -> SetVersion("proton eloss to neuland bar");
  auto histk = new TH1D("histk",";Kinetic Energy (MeV);MPED (MeV);",100,0,2100);
  histk -> SetMaximum(45);
  histk -> Draw();
  graphk -> Draw("samepl");
  cvsk -> Draw();

  auto cvsp = new SCanvas("cvsp");
  cvsp -> SetVersion("proton eloss to neuland bar");
  auto histp = new TH1D("histp",";Momentum (MeV/c);MPED (MeV);",100,0,3000);
  histp -> SetMaximum(45);
  histp -> Draw();
  graphp -> Draw("samepl");
  cvsp -> Draw();

  auto file_proton = new TFile("data/summary_proton_energy_deposit.root","recreate");
  graphk -> Write();
  graphp -> Write();

  return;

  auto file = new TFile(filename);
  auto tree = (TTree *) file -> Get("event");

  addto("ebar",tp("ebar",tree,"ebar","ebar>0",";energy deposit",200,0,20));
  addto("kine",tp("kine1",tree,"kine1","kine1>0","entering ;kinetic energy",200,1980,2000));
  addto("kine",tp("kine2",tree,"kine2","kine2>0","exiting ;kinetic energy",200,1980,2000));
  addto("kinee",tp("kinee",tree,"kine2:kine1","kine2>0&&kine1>0",";e2;e1",200,1980,2000,200,1980,2000),"colz");

  drawall();
}
