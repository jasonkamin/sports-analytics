#include <TCanvas.h>
#include <TStyle.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <iostream>
#include <TLine.h>
#include <TMath.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TCut.h>
#include <TClonesArray.h>
#include <TLegend.h>
#include <TRandom3.h>

#include <TStyle.h>
#include <TLatex.h>
#include <TDirectory.h>
#include <TCollection.h>
#include <TPostScript.h>


using namespace std;

TTree *MyTree;
TH1D* h_starters[12];
TH1D* h_backupps[12];
TH1D *h_points;

char saythis[500];
char NextPlayer[20];
char TheCondition[1000];

//int COLORS[12] = {2,4,6,8,9,12,28,30,38,46,3,kOrange+8};
int COLORS[12] = {2,4,6,kGreen+3,kViolet-5,12,28,kYellow-3,38,46,kAzure+7,kOrange+8};

void PlotProjectedStarts()
{

  char TeamNames[12][50] = {
    "Vegas Hardrockers       ",
    "Colorado Megawatts      ",
    "Revolting Hedgehogs     ",
    "East End Rockhounds     ",
    "Burgh's Old Hippies     ",
    "Rowdy Turtle Neck Ninjas",
    "Team Donnie Iris        ",
    "Stony Brook Domination  ",
    "Ice n'at                ",
    "Black Street Beast      ",
    "Burbank Maulers         ",
    "Lebo Converts           ",
  };

  h_points = new TH1D("h_points","h_points",100,0,50);

  TCanvas *c1 = new TCanvas("c1","c1",900,530);

  gStyle->SetOptStat(0);
  TFile *finput = TFile::Open("FantasyTree.root");

  MyTree = (TTree*)finput->Get("MyTree");

  ifstream fin;
  fin.open("ImportLineups.txt");
  //char myfilename[20];
  //sprintf(myfilename,"ImportLineups.txt");
  //fin.open(myfilename);

  for(int i=0; i<12; i++){

    Double_t NextPoints = 0.0;
    Double_t TeamPoints = 0.0;

    sprintf(saythis,"h_starters_%d",i);
    h_starters[i] = new TH1D(saythis,TeamNames[i],28,0,28);
    sprintf(saythis,"h_backupps_%d",i);
    h_backupps[i] = new TH1D(saythis,TeamNames[i],28,0.5,28.5);

    h_starters[i]->SetLineColorAlpha(COLORS[i],1.0);
    h_backupps[i]->SetLineColorAlpha(COLORS[i],1.0);
    h_starters[i]->SetFillColorAlpha(COLORS[i],0.80);
    h_backupps[i]->SetFillColorAlpha(COLORS[i],1.0);
    h_starters[i]->SetMarkerColor(COLORS[i]);
    h_backupps[i]->SetMarkerColor(COLORS[i]);
    h_starters[i]->SetFillStyle(1001);
    h_backupps[i]->SetFillStyle(3002);
    h_starters[i]->SetMarkerSize(2.0);
    h_backupps[i]->SetMarkerSize(2.0);

    //fin >> NextPlayer;
    //sprintf(TheCondition,"PlayerName == \"%s\"",NextPlayer);
    for(int j=0; j<7; j++){
      fin >> NextPlayer;
      sprintf(TheCondition,"PlayerName == \"%s\"",NextPlayer);
      //cout << TheCondition << endl;

      MyTree->Draw("ProjPoints/16.0>>h_p(1000,0,50)",TheCondition);
      TH1F *h_points = (TH1F*)gDirectory->Get("h_p");
      NextPoints = h_points->GetMean();
      TeamPoints += NextPoints;
    }
    h_starters[i]->SetBinContent(i*2+3,TeamPoints);
    cout << TeamNames[i] << " starters: " << TeamPoints << endl;

    NextPoints = 0.0;
    TeamPoints = 0.0;
    for(int j=0; j<7; j++){
      fin >> NextPlayer;
      sprintf(TheCondition,"PlayerName == \"%s\"",NextPlayer);
      //cout << TheCondition << endl;

      MyTree->Draw("ProjPoints/16.0>>h_p(1000,0,50)",TheCondition);
      TH1F *h_points = (TH1F*)gDirectory->Get("h_p");
      NextPoints = h_points->GetMean();
      TeamPoints += NextPoints;
    }
    h_backupps[i]->SetBinContent(i*2+3,TeamPoints);
    cout << TeamNames[i] << " backups: " << TeamPoints << endl;

  }




  TH1D *h_frame = new TH1D("h_frame","Jagoffs Fantasy Projections",28,0,28);
  h_frame->GetYaxis()->SetTitle("Projected Points/Game");

  c1->cd();
  gStyle->SetPaintTextFormat("2.1f");
  c1->SetBottomMargin(0.15);
  h_frame->GetYaxis()->SetRangeUser(0,110);
  h_frame->GetXaxis()->SetLabelOffset(0.8e-2);
  h_frame->Draw();
  for(int i=0; i<12; i++){
    h_frame->GetXaxis()->SetBinLabel(i*2+3,TeamNames[i]);
    h_starters[i]->Draw("B,same,TEXT45");
    h_backupps[i]->Draw("B,same");
  }

  //TLegend *leg1 = new TLegend(0.20,0.83,0.78,0.89);
  TLegend *leg1 = new TLegend(0.32,0.83,0.66,0.89);
  leg1->SetNColumns(2);
  leg1->SetFillColor(0);
  leg1->AddEntry(h_starters[5],"starters","F");
  leg1->AddEntry(h_backupps[5],"backups ","F");
  leg1->Draw();




}
