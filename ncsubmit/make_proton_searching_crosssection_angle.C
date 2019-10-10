void make_proton_searching_crosssection_angle()
{
  auto numEvents = 2000000;
  auto numProgress = numEvents / 50;

  TString runName = "ang"; 
  TString particleName = "proton";
  TString arxivName = TString("arxiv")+"_"+runName+"_"+particleName;

  bool setBar = true;
  bool setBar0 = false;
  bool setWall = false;

  TVector3 vpos(0,-205,-13.2);

  TString submitAllName = Form("all_%s_submits_%s.sh", runName.Data(), particleName.Data());
  ofstream allFile(submitAllName);
  cout << "all file: " << submitAllName << endl;

  ofstream arxivFile(arxivName+".sh");
  arxivFile << "mkdir -p " << arxivName << endl;
  arxivFile << "mv " << arxivName << ".sh " << arxivName << endl;
  arxivFile << "mv " << submitAllName << " " << arxivName << endl;

  for (auto split = 0; split < 200; split++)
  {
    TString submitName = Form("sub_s%d_%s.sh" ,split,particleName.Data());
    TString anaName = Form("%s_s%d_%s",runName.Data(),split,particleName.Data());
    TString macroName = anaName + ".mac";

    allFile << "csub " << submitName << endl;

    cout << "submit file: " << submitName << endl;
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
    macFile << "/gps/particle " << particleName << endl;
    macFile << "/gps/position " << vpos.x() << " " << vpos.y() << " " << vpos.z() << " mm" << endl;

    macFile << "#" << endl;
    macFile << "/gps/ang/type iso" << endl;
    macFile << "/gps/ang/mintheta 90 deg" << endl;
    macFile << "/gps/ang/maxtheta 160 deg" << endl;
    macFile << "/gps/ang/minphi 179.99 deg" << endl;
    macFile << "/gps/ang/maxphi 180.01 deg" << endl;

    macFile << "#" << endl;
    macFile << "/gps/ene/type Lin" << endl;
    macFile << "/gps/ene/min 100 MeV" << endl;
    macFile << "/gps/ene/max 3000 MeV" << endl;
    macFile << "/gps/ene/intercept 100" << endl;

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
