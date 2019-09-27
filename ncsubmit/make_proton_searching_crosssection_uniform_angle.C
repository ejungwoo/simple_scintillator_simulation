void make_proton_searching_crosssection_uniform_angle()
{
  auto numEvents = 1000000;
  auto numProgress = numEvents / 50;

  TString runName = "uang"; 
  TString particleName = "proton";
  TString arxivName = TString("arxiv")+"_"+runName+"_"+particleName;

  bool setBar = true;
  bool setBar0 = true;
  bool setWall = true;

  TVector3 vpos(0,-205,-13.2);

  auto file = new TFile("summary_data/proton_energy_at_phi.root");
  auto up = (TGraph *) file -> Get("up");
  auto lo = (TGraph *) file -> Get("lo");

  TString submitAllName = Form("all_%s_submits_%s.sh", runName.Data(), particleName.Data());
  ofstream allFile(submitAllName);
  cout << "all file: " << submitAllName << endl;

  ofstream arxivFile(arxivName+".sh");
  arxivFile << "mkdir -p " << arxivName << endl;
  arxivFile << "mv " << arxivName << ".sh " << arxivName << endl;
  arxivFile << "mv " << submitAllName << " " << arxivName << endl;

  auto dEnergy = 15.;
  for (auto energy = 100; energy <= 3000; energy+=15)
  {
    TString submitName = Form("sub_e%d_%s.sh" ,energy,particleName.Data());
    TString anaName = Form("%s_e%d_%s",runName.Data(),energy,particleName.Data());
    TString macroName = anaName + ".mac";

    auto phi1 = lo -> Eval(energy) - .02;
    auto phi2 = up -> Eval(energy) + .02;

    allFile << "csub " << submitName << endl;

    //cout << "submit file: " << submitName << endl;
    ofstream subfile(submitName.Data());
    subfile << "# cpu 2" << endl;
    subfile << "# mem 300" << endl;
    subfile << "./execute.g4sim " << macroName << endl;
    arxivFile << "mv " << submitName << " " << arxivName << endl;
    arxivFile << "mv " << submitName << "*.err " << arxivName << " 2>/dev/null" << endl;
    arxivFile << "mv " << submitName << "*.out " << arxivName << " 2>/dev/null" << endl;
    arxivFile << "mv " << submitName << "*.log " << arxivName << " 2>/dev/null" << endl;

    cout << "macro file: " << macroName << endl;
    ofstream macFile(macroName.Data());

    macFile << "#" << endl;
    macFile << "/my/init/detector/tpc" << endl;
         if (setBar)  macFile << "/my/init/detector/bar" << endl;
    else if (setBar0) macFile << "/my/init/detector/bar0" << endl;
    else if (setWall) macFile << "/my/init/detector/wall" << endl;

    macFile << "#" << endl;
    macFile << "/my/init/field/name data/field.root" << endl;

    macFile << "#" << endl;
    macFile << "/run/initialize" << endl;

    macFile << "#" << endl;
    macFile << "/my/pga/gun" << endl;
    macFile << "/my/pga/name    " << particleName << endl;
    macFile << "/my/pga/vertex  " << vpos.x() << " " << vpos.y() << " " << vpos.z() << " mm" << endl;
    macFile << "/my/pga/energy1 " << energy-.5*dEnergy << " MeV" << endl;
    macFile << "/my/pga/energy2 " << energy+.5*dEnergy << " MeV" << endl;
    macFile << "/my/pga/phi1    " << phi1 << " deg" << endl;
    macFile << "/my/pga/phi2    " << phi2 << " deg" << endl;
    macFile << "/my/pga/theta1  " << "89.999 deg" << endl;
    macFile << "/my/pga/theta2  " << "90.001 deg" << endl;

    macFile << "#" << endl;
    macFile << "/analysis/verbose 1" << endl;
    macFile << "/analysis/setFileName data/" << anaName << endl;

    macFile << "#" << endl;
    macFile << "/run/printProgress " << numProgress << endl;
    macFile << "/run/beamOn " << numEvents << endl;
    arxivFile << "mv " << macroName << " " << arxivName << endl;
  }

  gApplication -> Terminate();
}
