void make_wall()
{
  bool isVisMac = false;

  TString runName = "dist_found"; 
  TString particleName = "neutron";
  TString phiFileName = "data_summary/proton_phi.dat";

  auto numEvents = 100000;
  if (particleName == "gamma") numEvents = 10000; 
  else if (particleName == "neutron") numEvents = 1000000; 

  auto minEnergy = 5;
  auto maxEnergy = 1200;
  auto dEnergy = 10;
  bool setBar = false;
  bool setBar0 = false;
  bool setWall = true;
  TVector3 vpos(0,-205,-13.2);

  if (isVisMac) numEvents = 100;
  auto numProgress = numEvents / 50;

  auto lo = new TGraph();
  auto up = new TGraph();
  ifstream filephi(phiFileName);
  Double_t energy_proint, phi1_point, phi2_point;
  while (filephi >> energy_proint >> phi1_point >> phi2_point) {
    lo -> SetPoint(lo -> GetN(), energy_proint, phi1_point);
    up -> SetPoint(up -> GetN(), energy_proint, phi2_point);
  }

  if (isVisMac) runName = TString("vis_") + runName;
  TString arxivName = TString("arxiv")+"_"+runName+"_"+particleName;
  TString submitAllName = Form("all_%s_submits_%s.sh", runName.Data(), particleName.Data());

  ofstream allFile;
  if (!isVisMac) {
    allFile.open(submitAllName);
    cout << "all file: " << submitAllName << endl;
  }

  ofstream arxivFile(arxivName+".sh");
  arxivFile << "mkdir -p " << arxivName << endl;
  arxivFile << "mv " << arxivName << ".sh " << arxivName << endl;
  if (!isVisMac)
    arxivFile << "mv " << submitAllName << " " << arxivName << endl;

  TString commentout = "#";
  if (isVisMac)
    commentout = "";

  for (auto dist : {8981.44})
  for (auto barw : {50})
  for (auto phi0 : {30})
  {
    auto dphi = phi0 - 30.;

    TString groupName = Form("%s_%s_d%d_w%d_i%d",runName.Data(),particleName.Data(),int(dist),barw,phi0);
    TString doneName = Form("done_%s.dat", groupName.Data());
    arxivFile << "mv " << doneName << " " << arxivName << " 2>/dev/null" << endl;

    for (auto energy = minEnergy; energy <= maxEnergy; energy+=dEnergy)
    {
      TString submitName = Form("S%s_e%d.sh",groupName.Data(),energy);
      TString analysName = Form( "%s_e%d.root",groupName.Data(),energy);
      TString gmacroName = Form( "%s_e%d.mac",groupName.Data(),energy);

      auto phi1 = lo -> Eval(energy);
      auto phi2 = up -> Eval(energy);
      phi1 = phi1 - .2 + dphi*TMath::DegToRad();
      phi2 = phi2 + .2 + dphi*TMath::DegToRad();
      if (particleName == "neutron" || particleName == "gamma") {
        phi1 = (phi0-10.)*TMath::DegToRad();
        phi2 = (phi0+10.)*TMath::DegToRad();
      }

      ofstream subfile;
      if (!isVisMac) {
        allFile << "csub " << submitName << endl;

        subfile.open(submitName.Data());
        subfile << "# cpu 2" << endl;
        subfile << "# mem 300" << endl;
        subfile << "./execute.g4sim " << gmacroName << endl;
        subfile << "echo " << analysName << " >> " << doneName << endl;
        arxivFile << "mv " << submitName << " " << arxivName << endl;
        arxivFile << "mv " << submitName << "*.err " << arxivName << " 2>/dev/null" << endl;
        arxivFile << "mv " << submitName << "*.out " << arxivName << " 2>/dev/null" << endl;
        arxivFile << "mv " << submitName << "*.log " << arxivName << " 2>/dev/null" << endl;
      }

      ofstream macFile(gmacroName.Data());
      macFile << "#" << endl;
      macFile << "/my/init/barw " << barw << " mm" << endl;
      macFile << "/my/init/dist " << dist << " mm" << endl;
      macFile << "/my/init/roty " << phi0 << " deg" << endl;
      macFile << "/my/init/detector/tpc" << endl;
      if (setBar)  macFile << "/my/init/detector/bar" << endl;
      else if (setBar0) macFile << "/my/init/detector/bar0" << endl;
      else if (setWall) {
        macFile << "/my/init/detector/wall" << endl;
        macFile << "/my/init/detector/veto" << endl;
      }
      macFile << "/my/init/field/name data/field.root" << endl;
      macFile << "/run/initialize" << endl;
      macFile << "#" << endl;
      macFile << commentout << "/vis/open OGL 600x600-0+0" << endl;
      macFile << commentout << "/vis/drawVolume" << endl;
      macFile << commentout << "/vis/verbose warnings " << endl;
      macFile << commentout << "/vis/viewer/set/style surface" << endl;
      macFile << commentout << "/vis/viewer/set/viewpointVector 0.5 1 0.5" << endl;
      macFile << commentout << "/vis/viewer/set/projection perspective 10 deg" << endl;
      macFile << commentout << "/vis/scene/add/axes 0 0 0 400 mm" << endl;
      macFile << commentout << "/vis/viewer/set/background white" << endl;
      macFile << commentout << "/vis/scene/add/trajectories smooth" << endl;
      macFile << commentout << "/vis/modeling/trajectories/create/drawByCharge" << endl;
      macFile << commentout << "/vis/modeling/trajectories/drawByCharge-0/default/setDrawStepPts true" << endl;
      macFile << commentout << "/vis/modeling/trajectories/drawByCharge-0/default/setStepPtsSize 1" << endl;
      macFile << commentout << "/vis/scene/endOfEventAction accumulate" << endl;
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
      macFile << "/analysis/setFileName data/" << analysName << endl;
      macFile << "/run/printProgress " << numProgress << endl;
      macFile << "/run/beamOn " << numEvents << endl;

      arxivFile << "mv " << gmacroName << " " << arxivName << endl;
    }
  }

  gApplication -> Terminate();
}
