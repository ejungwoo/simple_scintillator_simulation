#include "ejungwoo.h"
using namespace ejungwoo;

void fit_phi()
{
  gversion("proton_cs2");
  gfig("figures_angle");
  gzcolor(10);

  bool drawEdgeFit = 0;
  gsave(0);
  auto breakCount = 0;

  auto graph_edge1 = new TGraph();
  auto graph_edge2 = new TGraph();

  for (auto energy = 105.; energy < 3000; energy+=30)
  {
    TString name = Form("energy_%d",int(energy));
    auto file = new TFile(TString("summary_data/")+name+"."+version()+".root");
    auto hist = (TH1D *) file -> Get(name);
    auto nbins = hist -> GetXaxis() -> GetNbins();

    auto max = hist -> GetBinContent(hist -> GetMaximumBin());
    auto cutratio = .2;
    auto rmax = max*cutratio;

    for (auto bin=1; bin<=nbins; ++bin) {
      auto content = hist -> GetBinContent(bin);
      if (content < rmax)
        hist -> SetBinContent(bin, 0);
    }

    Double_t cvalue1 = 0.;
    Double_t xvalue1 = 0.;
    auto graph1 = new TGraph();
    for (auto bin=1; bin<=nbins; ++bin) {
      auto value = hist -> GetBinContent(bin);
      if (value > cvalue1) {
        cvalue1 = value;
        if (graph1 -> GetN()==0) {
          graph1 -> SetPoint(graph1->GetN(),hist -> GetBinCenter(bin-1),0);
          xvalue1 = hist->GetBinCenter(bin-1);
        }
        graph1 -> SetPoint(graph1->GetN(),hist -> GetBinCenter(bin),value);
      }
      else if (value < cvalue1)
        break;

      if (graph1 -> GetN()==3)
        break;
    }

    Double_t cvalue2 = 0.;
    Double_t xvalue2 = 0.;
    auto graph2 = new TGraph();
    for (auto bin=nbins; bin>=1; --bin) {
      auto value = hist -> GetBinContent(bin);
      if (value > cvalue2) {
        cvalue2 = value;
        if (graph2 -> GetN()==0) {
          graph2 -> SetPoint(graph2->GetN(),hist -> GetBinCenter(bin+1),0);
          xvalue2 = hist->GetBinCenter(bin+1);
        }
        graph2 -> SetPoint(graph2->GetN(),hist -> GetBinCenter(bin),value);
      }
      else if (value < cvalue2)
        break;

      if (graph2 -> GetN()==3)
        break;
    }

    TCanvas *cvs = nullptr;
    if (drawEdgeFit) {
      cvs = cv(name);
      hist -> Draw();
    }

    graph1 -> SetMarkerStyle(20); graph1 -> SetMarkerColor(kBlue);    if (drawEdgeFit) graph1 -> Draw("samep");
    graph2 -> SetMarkerStyle(20); graph2 -> SetMarkerColor(kGreen+1); if (drawEdgeFit) graph2 -> Draw("samep");

    //auto f1 = new TF1("f1","[1]*(x-[0])",0,3); f1 -> SetParameters(xvalue1, 300000); graph1 -> Fit(f1,"Q0"); f1 -> SetNpx(1000); if (drawEdgeFit) f1 -> Draw("samel");
    //auto f2 = new TF1("f2","[1]*(x-[0])",0,3); f2 -> SetParameters(xvalue2,-300000); graph2 -> Fit(f2,"Q0"); f2 -> SetNpx(1000); if (drawEdgeFit) f2 -> Draw("samel");

    //auto x1 = f1 -> GetX(0);
    //auto x2 = f2 -> GetX(0);

    auto x1 = 0.;
    auto x2 = 0.;
    {
      Double_t x, y;
      auto n1 = graph1 -> GetN();
      auto ww = 0.;
      for (auto i=0; i<n1; ++i) {
        graph1 -> GetPoint(i,x,y);
        auto w = (max - y)*(max - y);
        x1 += w*x;
        ww += w;
      }
      x1 = x1/ww;
    }
    {
      Double_t x, y;
      auto n2 = graph2 -> GetN();
      auto ww = 0.;
      for (auto i=0; i<n2; ++i) {
        graph2 -> GetPoint(i,x,y);
        auto w = (max - y)*(max - y);
        x2 += w*x;
        ww += w;
      }
      x2 = x2/ww;
    }

    auto line1 = new TLine(x1,0,x1,max); line1 -> SetLineStyle(9);  line1 -> SetLineColor(kGray+1); if (drawEdgeFit) line1 -> Draw("samel");
    auto line2 = new TLine(x2,0,x2,max); line2 -> SetLineStyle(9);  line2 -> SetLineColor(kGray+1); if (drawEdgeFit) line2 -> Draw("samel");

    graph_edge1 -> SetPoint(graph_edge1->GetN(), energy, x1);
    graph_edge2 -> SetPoint(graph_edge2->GetN(), energy, x2);

    if (drawEdgeFit) {
      hist -> GetXaxis() -> SetRangeUser(x1-0.1,x2+0.1);
      save(cvs);
    }

    //if (breakCount++>0) break;
  }

  auto f2 = new TFile("summary_data/hist_kphi.proton_cs2.root");
  auto h2 = (TH2D *) f2 -> Get("hist_kphi");
  auto c2 = cc("total");
  h2 -> Draw("colz");

  graph_edge1 -> SetLineColor(kRed);
  graph_edge2 -> SetLineColor(kRed);

  graph_edge1 -> SetLineWidth(3);
  graph_edge2 -> SetLineWidth(3);

  graph_edge1 -> Draw("samel");
  graph_edge2 -> Draw("samel");

  save(c2);

  {
    auto file = new TFile("summary_data/proton_energy_at_phi.root","recreate");
    file -> cd();
    h2 -> Write();
    graph_edge1 -> SetName("lo");
    graph_edge2 -> SetName("up");
    graph_edge1 -> Write();
    graph_edge2 -> Write();
  }
}
