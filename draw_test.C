void draw_test()
{
  for (TString particle : {"proton","neutron"})
  {
    auto file = new TFile(TString("data/run_test_")+particle+".root");
    auto tree = (TTree *) file -> Get("event");
    tree -> Print();

    TString name;
    TString vname;
    SCanvas *cvs;
    //auto cvs = new SCanvas(name+">>"+name+"(100,)");

    vname = "ebar";
    name = particle+"_"+vname;
    cvs = new SCanvas(name);
    tree -> Draw(vname+">>"+name+"(100,0,40)","ebar>0");
    cvs -> Draw();

    vname = "kine0";
    name = particle+"_"+vname;
    cvs = new SCanvas(name);
    tree -> Draw(vname+">>"+name+"(101,0,3030)");
    cvs -> Draw();

    vname = "phi0";
    name = particle+"_"+vname;
    cvs = new SCanvas(name);
    tree -> Draw(vname+">>"+name+"(100,0,3.2)");
    tree -> Draw(vname+">>"+name+"_in(100,0,3.2)","ebar>0","same");
    cvs -> Draw();
  }
}
