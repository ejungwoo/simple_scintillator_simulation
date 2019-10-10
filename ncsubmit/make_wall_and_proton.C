void make_wall_and_proton()
{
  bool isVisMac = false;

  auto numEvents = 20000;
  if (isVisMac)
    numEvents = 100;

  auto minEnergy = 5;
  auto maxEnergy = 400;
  auto dEnergy = 10;

  auto numProgress = numEvents / 50;
  //auto dist = 11341.18;
  auto dist = 8817.41;

  TString runName = "wallpos"; 
  if (isVisMac)
    runName = TString("vis_") + runName;
  TString particleName = "proton";
  TString arxivName = TString("arxiv")+"_"+runName+"_"+particleName;

  bool setBar = false;
  bool setBar0 = false;
  bool setWall = true;

  TVector3 vpos(0,-205,-13.2);

  ifstream filephi("summary_data/proton_phi.dat");
  auto lo = new TGraph();
  Double_t energy_proint, phi1_point, phi2_point;
  auto up = new TGraph();
  while (filephi >> energy_proint >> phi1_point >> phi2_point) {
    lo -> SetPoint(lo -> GetN(), energy_proint, phi1_point);
    up -> SetPoint(up -> GetN(), energy_proint, phi2_point);
  }

  TString submitAllName = Form("all_%s_submits_%s.sh", runName.Data(), particleName.Data());
  ofstream allFile;
  cout << "all file: " << submitAllName << endl;
  if (!isVisMac) {
    allFile.open(submitAllName);
    cout << "all file: " << submitAllName << endl;
  }

  ofstream arxivFile(arxivName+".sh");
  arxivFile << "mkdir -p " << arxivName << endl;
  arxivFile << "mv " << arxivName << ".sh " << arxivName << endl;
  arxivFile << "mv " << submitAllName << " " << arxivName << endl;
  if (!isVisMac)
    arxivFile << "mv " << submitAllName << " " << arxivName << endl;

  for (auto phi0=25; phi0<=35; ++phi0)
  {
    auto dphi = phi0 - 30;
    for (auto energy = minEnergy; energy <= maxEnergy; energy+=dEnergy)
    {
      TString submitName = Form("S%s_a%d_e%d_%s.sh",runName.Data(),phi0,energy,particleName.Data());
      TString anaName = Form("%s_a%d_e%d_%s",runName.Data(),phi0,energy,particleName.Data());
      TString macroName = anaName + ".mac";

      auto phi1 = lo -> Eval(energy);
      auto phi2 = up -> Eval(energy);
      phi1 = phi1 - .05 + dphi*TMath::DegToRad();
      phi2 = phi2 + .05 + dphi*TMath::DegToRad();

      ofstream subfile;
      if (!isVisMac) {
        allFile << "csub " << submitName << endl;

        subfile.open(submitName.Data());
        subfile << "# cpu 2" << endl;
        subfile << "# mem 300" << endl;
        subfile << "./execute.g4sim " << macroName << endl;
        arxivFile << "mv " << submitName << " " << arxivName << endl;
        arxivFile << "mv " << submitName << "*.err " << arxivName << " 2>/dev/null" << endl;
        arxivFile << "mv " << submitName << "*.out " << arxivName << " 2>/dev/null" << endl;
        arxivFile << "mv " << submitName << "*.log " << arxivName << " 2>/dev/null" << endl;
      }

      cout << "macro file: " << macroName << endl;
      ofstream macFile(macroName.Data());

      macFile << "#" << endl;
      macFile << "/my/init/dist " << dist << " mm" << endl;
      macFile << "/my/init/roty " << phi0 << " deg" << endl;
      macFile << "#" << endl;
      macFile << "/my/init/detector/tpc" << endl;
      if (setBar)  macFile << "/my/init/detector/bar" << endl;
      else if (setBar0) macFile << "/my/init/detector/bar0" << endl;
      else if (setWall) macFile << "/my/init/detector/wall" << endl;

      macFile << "#" << endl;
      macFile << "/my/init/field/name data/field.root" << endl;

      macFile << "#" << endl;
      macFile << "/run/initialize" << endl;

      if (isVisMac) {
        macFile << "#" << endl;
        macFile << "/vis/open OGL 600x600-0+0" << endl;
        macFile << "/vis/drawVolume" << endl;
        macFile << "/vis/verbose warnings " << endl;
        macFile << "/vis/viewer/set/style surface" << endl;
        macFile << "/vis/viewer/set/viewpointVector 0.5 1 0.5" << endl;
        macFile << "/vis/viewer/set/projection perspective 10 deg" << endl;
        macFile << "/vis/scene/add/axes 0 0 0 400 mm" << endl;
        macFile << "/vis/viewer/set/background white" << endl;
        macFile << "/vis/scene/add/trajectories smooth" << endl;
        macFile << "/vis/modeling/trajectories/create/drawByCharge" << endl;
        macFile << "/vis/modeling/trajectories/drawByCharge-0/default/setDrawStepPts true" << endl;
        macFile << "/vis/modeling/trajectories/drawByCharge-0/default/setStepPtsSize 1" << endl;
        macFile << "/vis/scene/endOfEventAction accumulate" << endl;
      }

      macFile << "#" << endl;
      macFile << "/my/pga/gun" << endl;
      macFile << "/my/pga/name    " << particleName << endl;
      macFile << "/my/pga/vertex  " << vpos.x()<<" "<<vpos.y()<<" "<<vpos.z() << " mm" << endl;
      macFile << "/my/pga/energy1 " << energy-.5*dEnergy << " MeV" << endl;
      macFile << "/my/pga/energy2 " << energy+.5*dEnergy << " MeV" << endl;
      macFile << "/my/pga/phi2    " << phi1 << " rad" << endl;
      macFile << "/my/pga/phi1    " << phi2 << " rad" << endl;
      macFile << "/my/pga/theta1  " << "81 deg" << endl;
      macFile << "/my/pga/theta2  " << "99 deg" << endl;

      macFile << "#" << endl;
      macFile << "/analysis/verbose 1" << endl;
      macFile << "/analysis/setFileName data/" << anaName << endl;

      macFile << "#" << endl;
      macFile << "/run/printProgress " << numProgress << endl;
      macFile << "/run/beamOn " << numEvents << endl;
      arxivFile << "mv " << macroName << " " << arxivName << endl;
    }
  }

  gApplication -> Terminate();
}
