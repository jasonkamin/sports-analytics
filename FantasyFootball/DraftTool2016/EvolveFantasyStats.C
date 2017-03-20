#include <vector>
#include "TROOT.h"
#include "TSystem.h"
#include "TStyle.h"
#include "TLorentzVector.h"
#include "TMath.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TH1F.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TH3F.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include <cstring>
#include <iomanip>
using namespace std;

#include <TCanvas.h>
#include <TStyle.h>
#include <TF1.h>
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

TH1D *h_al;
TH1D *h_qb;
TH1D *h_rb;
TH1D *h_wr;
TH1D *h_te;
char saythis[100];
char TheCondition[6000];
char TheQBCondition[6000];
char TheRBCondition[6000];
char TheWRCondition[6000];
char TheTECondition[6000];
//char PreviousCondition[6000];
char NextPlayer[50];

TF1 *f_gaus_al;
TF1 *f_gaus_qb;
TF1 *f_gaus_rb;
TF1 *f_gaus_wr;
TF1 *f_gaus_te;
int ROUND = 0;

void FitWithGaus(TH1D *h1, TF1 *f1);

void EvolveFantasyStats()
{
  gStyle->SetOptStat(0);
  TFile *finput = TFile::Open("FantasyTree.root");

  h_al = (TH1D*)finput->Get("h_al");
  h_qb = (TH1D*)finput->Get("h_qb");
  h_rb = (TH1D*)finput->Get("h_rb");
  h_wr = (TH1D*)finput->Get("h_wr");
  h_te = (TH1D*)finput->Get("h_te");

  MyTree = (TTree*)finput->Get("MyTree");

  f_gaus_al = new TF1("f_gaus_al","gaus(0)+gaus(3)+gaus(6)+gaus(9)",0,25);
  f_gaus_al->SetLineColorAlpha(h_al->GetLineColor(),0.4);
  f_gaus_al->SetLineWidth(2);
  h_al->Rebin(2)->Scale(0.5);
  f_gaus_qb = new TF1("f_gaus_qb","gaus(0)",0,25);
  f_gaus_rb = new TF1("f_gaus_rb","gaus(0)",0,25);
  f_gaus_wr = new TF1("f_gaus_wr","gaus(0)",0,25);
  f_gaus_te = new TF1("f_gaus_te","gaus(0)",0,25);

  char FirstPlayerOfRound[16][50] = {
    "A.Peterson",
    "J.Nelson",
    "E.Sanders",
    "B.Cooks",
    "R.Jennings",
    "C.Ivory",
    "B.Marshall",
    "D.McFadden",
    "C.Palmer",
    "K.White",
    "T.West",
    "D.Williams",
    "A.Foster",
    "F.Jackson",
    "J.McKinnon",
    "C.Kaepernick",
  };

  TCanvas *c1 = new TCanvas("c1","c1");

  ifstream fin;
  fin.open("PlayersTaken.txt");
  fin >> NextPlayer;
  sprintf(TheCondition,"PlayerName!=\"%s\"",NextPlayer);
  for(int i=1; i<158; i++){
  //for(int i=1; i<30; i++){

    fin >> NextPlayer;
    sprintf(TheCondition,"%s && PlayerName!=\"%s\"",TheCondition,NextPlayer);
    if(strcmp(NextPlayer,FirstPlayerOfRound[ROUND])==0)
      ROUND++;
    //sprintf(PreviousCondition,"PlayerName!=\"A.Rodgers\"");
    //sprintf(TheCondition,"%s && PlayerName!=\"%s\"",PreviousCondition,TheCondition);


    if(i<12){
      MyTree->Draw("ProjPoints/16.0>>h_al");
      sprintf(TheQBCondition,"Position==\"QB\" && ProjPoints/16.0>12.0");
      MyTree->Draw("ProjPoints/16.0>>h_qb",TheQBCondition);
      sprintf(TheRBCondition,"Position==\"RB\" && ProjPoints/16.0>1.0");
      MyTree->Draw("ProjPoints/16.0>>h_rb",TheRBCondition);
      sprintf(TheWRCondition,"Position==\"WR\" && ProjPoints/16.0>1.0");
      MyTree->Draw("ProjPoints/16.0>>h_wr",TheWRCondition);
      sprintf(TheTECondition,"Position==\"TE\" && ProjPoints/16.0>1.0");
      MyTree->Draw("ProjPoints/16.0>>h_te",TheTECondition);
    }
    else{
      MyTree->Draw("ProjPoints/16.0>>h_al",TheCondition);
      sprintf(TheQBCondition,"%s && Position==\"QB\" && ProjPoints/16.0>12.0",TheCondition);
      MyTree->Draw("ProjPoints/16.0>>h_qb",TheQBCondition);
      sprintf(TheRBCondition,"%s && Position==\"RB\" && ProjPoints/16.0>1.0",TheCondition);
      MyTree->Draw("ProjPoints/16.0>>h_rb",TheRBCondition);
      sprintf(TheWRCondition,"%s && Position==\"WR\" && ProjPoints/16.0>1.0",TheCondition);
      MyTree->Draw("ProjPoints/16.0>>h_wr",TheWRCondition);
      sprintf(TheTECondition,"%s && Position==\"TE\" && ProjPoints/16.0>1.0",TheCondition);
      MyTree->Draw("ProjPoints/16.0>>h_te",TheTECondition);
      cout << TheCondition << endl;
    }

    double qb_max  = 0.0;
    double rb_max  = 0.0;
    double wr_max  = 0.0;
    double te_max  = 0.0;
    int    qb_maxb = 0;
    int    rb_maxb = 0;
    int    wr_maxb = 0;
    int    te_maxb = 0;
    double qb_mean = 0.0;
    double rb_mean = 0.0;
    double wr_mean = 0.0;
    double te_mean = 0.0;
    double qb_sig  = 0.0;
    double rb_sig  = 0.0;
    double wr_sig  = 0.0;
    double te_sig  = 0.0;

    for(int i=0; i<51; i++){
      if(h_qb->GetBinContent(i)>0)
        qb_maxb  = i;
      if(h_rb->GetBinContent(i)>0)
        rb_maxb  = i;
      if(h_wr->GetBinContent(i)>0)
        wr_maxb  = i;
      if(h_te->GetBinContent(i)>0)
        te_maxb  = i;
    }
    qb_max   = h_qb->GetXaxis()->GetBinCenter(qb_maxb);
    rb_max   = h_rb->GetXaxis()->GetBinCenter(rb_maxb);
    wr_max   = h_wr->GetXaxis()->GetBinCenter(wr_maxb);
    te_max   = h_te->GetXaxis()->GetBinCenter(te_maxb);
    qb_mean  = h_qb->GetMean();
    rb_mean  = h_rb->GetMean();
    wr_mean  = h_wr->GetMean();
    te_mean  = h_te->GetMean();
    qb_sig   = h_qb->GetRMS();
    rb_sig   = h_rb->GetRMS();
    wr_sig   = h_wr->GetRMS();
    te_sig   = h_te->GetRMS();

    h_al->Scale(0.5);
    for(int i=0; i<h_al->GetNbinsX()+1; i++)
      h_al->SetBinContent(i,h_al->GetBinContent(i)+0.5);
    h_al->SetMinimum(0.10);
    h_al->GetXaxis()->SetRangeUser(0,25);

    h_al->SetLineWidth(4);
    h_qb->SetLineWidth(3);
    h_rb->SetLineWidth(3);
    h_wr->SetLineWidth(3);
    h_te->SetLineWidth(3);

    h_al->SetLineColor(14);
    h_qb->SetLineColor(2);
    h_rb->SetLineColor(4);
    h_wr->SetLineColor(6);
    h_te->SetLineColor(8);

    h_qb->GetXaxis()->SetRangeUser(11,25);
    h_te->GetXaxis()->SetRangeUser(3,15);
    FitWithGaus(h_qb,f_gaus_qb);
    FitWithGaus(h_rb,f_gaus_rb);
    FitWithGaus(h_wr,f_gaus_wr);
    FitWithGaus(h_te,f_gaus_te);
    for(int j=0; j<3; j++){
      f_gaus_al->SetParameter(j+0,f_gaus_qb->GetParameter(j));
      f_gaus_al->SetParameter(j+3,f_gaus_rb->GetParameter(j));
      f_gaus_al->SetParameter(j+6,f_gaus_wr->GetParameter(j));
      f_gaus_al->SetParameter(j+9,f_gaus_te->GetParameter(j));
    }

    h_qb->SetFillColor(kRed-7);
    h_rb->SetFillColor(kBlue-7);
    h_wr->SetFillColor(kMagenta-10);
    h_te->SetFillColor(kGreen-7);

    h_qb->SetFillStyle(3004);
    h_rb->SetFillStyle(3005);
    h_wr->SetFillStyle(3006);
    h_te->SetFillStyle(3007);

    h_qb->GetXaxis()->SetRange(0, qb_maxb);
    h_rb->GetXaxis()->SetRange(0, rb_maxb);
    h_wr->GetXaxis()->SetRange(0, wr_maxb);
    h_te->GetXaxis()->SetRange(0, te_maxb);

    h_al->SetTitle("Jagoffs Fantasy Football Draft");
    h_al->GetXaxis()->SetTitle("Projected Points per Game");
    h_al->GetYaxis()->SetTitle("Number of Players Available");
    h_al->GetYaxis()->SetRangeUser(0.1,30);
    h_al->Draw("C");
    h_qb->Draw("same");
    h_rb->Draw("same");
    h_wr->Draw("same");
    h_te->Draw("same");
    f_gaus_al->Draw("same");

    //TLegend *leg1 = new TLegend(0.5,0.6,0.89,0.89);
    TLegend *leg1 = new TLegend(0.43,0.46,0.95,0.92);
    leg1->SetFillColor(0);
    sprintf(saythis,"QB (max %2.2f, #mu %2.1f, #sigma %2.1f)", qb_max, qb_mean, qb_sig);
    leg1->AddEntry(h_qb,saythis,"L");
    sprintf(saythis,"RB (max %2.2f, #mu %2.1f, #sigma %2.1f)", rb_max, rb_mean, rb_sig);
    leg1->AddEntry(h_rb,saythis,"L");
    sprintf(saythis,"WR (max %2.2f, #mu %2.1f, #sigma %2.1f)", wr_max, wr_mean, wr_sig);
    leg1->AddEntry(h_wr,saythis,"L");
    sprintf(saythis,"TE (max %2.2f, #mu %2.1f, #sigma %2.1f)", te_max, te_mean, te_sig);
    leg1->AddEntry(h_te,saythis,"L");
    leg1->Draw();

    c1->Modified();
    c1->Update();
    //sprintf(saythis,"FantasyUpdates/FantasyPlot_%d.gif",i);
    //c1->SaveAs(saythis);
    if(i<12){
      TLegend *leg2 = new TLegend(0.60,0.35,0.85,0.45);
      leg2->SetFillColor(0);
      leg2->SetBorderSize(0);
      leg2->AddEntry(h_al,"All Players    ","");
      leg2->Draw();
      c1->Print("FantasyUpdates/gifs/FantasyAni.gif+20");
    }
    else if(i<13){
      TLegend *leg2 = new TLegend(0.60,0.35,0.85,0.45);
      leg2->SetFillColor(0);
      leg2->SetBorderSize(0);
      leg2->AddEntry(h_al,"Keepers Removed","");
      leg2->Draw();
      c1->Print("FantasyUpdates/gifs/FantasyAni.gif+200");
    }
    else if(i<157){
      TLegend *leg2 = new TLegend(0.60,0.35,0.85,0.45);
      leg2->SetFillColor(0);
      leg2->SetBorderSize(0);
      sprintf(saythis,"Round %d       ",ROUND);
      leg2->AddEntry(h_al,saythis,"");
      leg2->Draw();
      c1->Print("FantasyUpdates/gifs/FantasyAni.gif+20");
    }
    else{
      TLegend *leg2 = new TLegend(0.60,0.35,0.85,0.45);
      leg2->SetFillColor(0);
      leg2->SetBorderSize(0);
      leg2->AddEntry(h_al,"Leftovers...   ","");
      leg2->Draw();
      c1->Print("FantasyUpdates/gifs/FantasyAni.gif++200++");
    }

    c1->Clear();

  }


}



void FitWithGaus(TH1D *h1, TF1 *f1)
{

  //f_gaus_qb->SetLineColorAlpha(2,0.4);
  //f_gaus_rb->SetLineColorAlpha(4,0.4);
  //f_gaus_wr->SetLineColorAlpha(6,0.4);
  //f_gaus_te->SetLineColorAlpha(8,0.4);
  //f_gaus_qb->SetLineSize(1);
  //f_gaus_rb->SetLineSize(1);
  //f_gaus_wr->SetLineSize(1);
  //f_gaus_te->SetLineSize(1);

  double MyMean  = h1->GetMean();
  double MySigma = h1->GetRMS();

  f1->SetParameters(1,MyMean,MySigma);
  //f1->FixParameter(1,MyMean);
  //f1->FixParameter(2,MySigma);
  f1->SetParLimits(1,0.90*MyMean, 1.10*MyMean);
  f1->SetParLimits(2,0.90*MySigma,1.10*MySigma);
  f1->SetLineColorAlpha(h1->GetLineColor(),0.4);
  f1->SetLineWidth(1);
  h1->Fit(f1,"QN","",MyMean-1.0*MySigma, MyMean+1.0*MySigma);
  h1->Fit(f1,"","",MyMean-3.0*MySigma, MyMean+3.0*MySigma);

  return;
}




