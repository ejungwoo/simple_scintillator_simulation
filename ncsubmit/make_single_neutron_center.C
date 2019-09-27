void make_single_neutron_center()
{
  auto numEvents = 200000;
  auto numProgress = numEvents / 20;

  TString runName = "runc"; 
  TString particleName = "neutron";
  TString arxivName = TString("arxiv")+"_"+runName+"_"+particleName;

  bool setBar = true;
  bool setBar0 = true;
  bool setWall = true;

  Double_t phi = 29.6;
  //Double_t phi = 40;
  Double_t theta = 0.;

  auto px = 1*TMath::Sin(phi*TMath::DegToRad());
  auto pz = 1*TMath::Cos(phi*TMath::DegToRad())*TMath::Cos(theta*TMath::DegToRad());
  auto py = 1*TMath::Cos(phi*TMath::DegToRad())*TMath::Sin(theta*TMath::DegToRad());
  TVector3 pdir(px,py,pz);

  TVector3 vpos(0,-205,-13.2);

  TString submitAllName = Form("all_%s_submits_%s.sh", runName.Data(), particleName.Data());
  ofstream allFile(submitAllName);
  cout << "all file: " << submitAllName << endl;

  ofstream arxivFile(arxivName+".sh");
  arxivFile << "mkdir -p " << arxivName << endl;
  arxivFile << "mv " << arxivName << ".sh " << arxivName << endl;
  arxivFile << "mv " << submitAllName << " " << arxivName << endl;

  for (auto kineticEnergy = 100; kineticEnergy <= 3000; kineticEnergy += 20)
  {
    TString submitName = Form("sub_k%d_%s.sh" ,kineticEnergy,particleName.Data());
    TString anaName = Form("%s_k%d_%s",runName.Data(),kineticEnergy,particleName.Data());
    TString macroName = anaName + ".mac";

    allFile << "csub " << submitName << endl;

    cout << "submit file: " << submitName << endl;
    ofstream subfile(submitName);
    subfile << "# cpu 2" << endl;
    subfile << "# mem 300" << endl;
    subfile << "./execute.g4sim " << macroName << endl;
    arxivFile << "mv " << submitName << " " << arxivName << endl;
    arxivFile << "mv " << submitName << "*.err " << arxivName << " 2>/dev/null" << endl;
    arxivFile << "mv " << submitName << "*.out " << arxivName << " 2>/dev/null" << endl;
    arxivFile << "mv " << submitName << "*.log " << arxivName << " 2>/dev/null" << endl;

    //cout << "macro file: " << macroName << endl;
    ofstream macFile(macroName);

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
    macFile << "/analysis/verbose 1" << endl;
    macFile << "/analysis/setFileName data/" << anaName << endl;

    macFile << "#" << endl;
    macFile << "/gps/position " << vpos.x() << " " << vpos.y() << " " << vpos.z() << " mm" << endl;
    macFile << "/gps/particle " << particleName << endl;
    macFile << "/gps/energy " << kineticEnergy << " MeV" << endl;
    macFile << "/gps/direction " << pdir.x() << " " << pdir.y() << " " << pdir.z() << endl;

    macFile << "#" << endl;
    macFile << "/run/printProgress " << numProgress << endl;
    macFile << "/run/beamOn " << numEvents << endl;
    arxivFile << "mv " << macroName << " " << arxivName << endl;
  }

  gApplication -> Terminate();
}
