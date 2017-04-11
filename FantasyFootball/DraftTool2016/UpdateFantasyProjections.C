TTree *MyTree;

TH1D *h_al;
TH1D *h_qb;
TH1D *h_rb;
TH1D *h_wr;
TH1D *h_te;
char saythis[100];
char TheCondition[12000];
char TheQBCondition[12000];
char TheRBCondition[12000];
char TheWRCondition[12000];
char TheTECondition[12000];
char TheConditionJJ[12000];
//char PreviousCondition[12000];
char NextPlayer[50];

TF1 *f_gaus_qb;
TF1 *f_gaus_rb;
TF1 *f_gaus_wr;
TF1 *f_gaus_te;

void FitWithGaus(TH1D *h1, TF1 *f1);
void TellMe_HighestPlayers(char *Position, char *ListOfPlayers, int nPlayersToList);

void UpdateFantasyProjections()
{
  gStyle->SetOptStat(0);
  TFile *finput = TFile::Open("FantasyTree.root");

  h_al = (TH1D*)finput->Get("h_al");
  h_qb = (TH1D*)finput->Get("h_qb");
  h_rb = (TH1D*)finput->Get("h_rb");
  h_wr = (TH1D*)finput->Get("h_wr");
  h_te = (TH1D*)finput->Get("h_te");

  MyTree = (TTree*)finput->Get("MyTree");

  f_gaus_qb = new TF1("f_gaus_qb","gaus(0)",0,25);
  f_gaus_rb = new TF1("f_gaus_rb","gaus(0)",0,25);
  f_gaus_wr = new TF1("f_gaus_wr","gaus(0)",0,25);
  f_gaus_te = new TF1("f_gaus_te","gaus(0)",0,25);

  ifstream fin;
  //fin.open("PlayersTaken.txt");
  fin.open("DraftedList.txt");
  if(fin){
    fin >> NextPlayer;
    //sprintf(TheCondition,"ProjPoints>1 && Position!=\"K\" && Position!=\"DST\" && ");
    //sprintf(TheCondition,"PlayerName!=\"%s\"",NextPlayer);
    sprintf(TheCondition,"ProjPoints>1 && Position!=\"K\" && Position!=\"DST\" && PlayerName!=\"%s\"",NextPlayer);
    while(!fin.eof()){
      fin >> NextPlayer;
      sprintf(TheCondition,"%s && PlayerName!=\"%s\"",TheCondition,NextPlayer);
    }
  }
  else{
    sprintf(TheCondition,"ProjPoints>1 && Position!=\"K\" && Position!=\"DST\"");
  }

  //sprintf(PreviousCondition,"PlayerName!=\"A.Rodgers\"");
  //sprintf(TheCondition,"%s && PlayerName!=\"%s\"",PreviousCondition,TheCondition);

  cout << endl << TheCondition << endl;

  TCanvas *c1 = new TCanvas("c1","c1");

  MyTree->Draw("ProjPoints/16.0>>h_al",TheCondition);
  sprintf(TheQBCondition,"%s && Position==\"QB\" && ProjPoints>1",TheCondition);
  MyTree->Draw("ProjPoints/16.0>>h_qb",TheQBCondition);
  sprintf(TheRBCondition,"%s && Position==\"RB\" && ProjPoints>1",TheCondition);
  MyTree->Draw("ProjPoints/16.0>>h_rb",TheRBCondition);
  sprintf(TheWRCondition,"%s && Position==\"WR\" && ProjPoints>1",TheCondition);
  MyTree->Draw("ProjPoints/16.0>>h_wr",TheWRCondition);
  sprintf(TheTECondition,"%s && Position==\"TE\" && ProjPoints>1",TheCondition);
  MyTree->Draw("ProjPoints/16.0>>h_te",TheTECondition);


  //########### Get My Team ############
  ifstream fin_myteam;
  fin_myteam.open("DraftedTeam_1.txt");
  TLine* lMyTeam[17];
  Char_t PlayerNameJJ[17][50];
  Char_t PositionJJ[5];
  Double_t ProjPointsJJ[17];
  int linecolor[17];
  int iSizeMyTeam = 0;
  if(fin_myteam){
    while(!fin_myteam.eof()){
      fin_myteam >> PlayerNameJJ[iSizeMyTeam];
      sprintf(TheConditionJJ,"PlayerName==\"%s\"",PlayerNameJJ[iSizeMyTeam]);

      sprintf(saythis,"ProjPoints/16.0>>htempPoints_%d",iSizeMyTeam);
      MyTree->Draw(saythis,TheConditionJJ);
      sprintf(saythis,"htempPoints_%d",iSizeMyTeam);
      TH1D *htemp1 = (TH1D*)gDirectory->Get(saythis);
      ProjPointsJJ[iSizeMyTeam] = htemp1->GetMean();

      sprintf(saythis,"Position>>htempPosition_%d",iSizeMyTeam);
      MyTree->Draw(saythis,TheConditionJJ);
      sprintf(saythis,"htempPosition_%d",iSizeMyTeam);
      TH1D *htemp2 = (TH1D*)gDirectory->Get(saythis);
      sprintf(PositionJJ,"%s",htemp2->GetXaxis()->GetBinLabel(1));

      if(strcmp(PositionJJ,"QB")==0)
        linecolor[iSizeMyTeam] = 2;
      if(strcmp(PositionJJ,"RB")==0)
        linecolor[iSizeMyTeam] = 4;
      if(strcmp(PositionJJ,"WR")==0)
        linecolor[iSizeMyTeam] = 6;
      if(strcmp(PositionJJ,"TE")==0)
        linecolor[iSizeMyTeam] = 8;

      lMyTeam[iSizeMyTeam] = new TLine(ProjPointsJJ[iSizeMyTeam],0,ProjPointsJJ[iSizeMyTeam],100);
      lMyTeam[iSizeMyTeam]->SetLineColor(linecolor[iSizeMyTeam]);
      lMyTeam[iSizeMyTeam]->SetLineStyle(2);

      iSizeMyTeam++;
    }
  }

  cout << endl << endl << "#################################" << endl;
  cout << "I have already drafted " << iSizeMyTeam-1 << " players: " << endl;
  for(int i=0; i<iSizeMyTeam-1; i++){
    cout << i+1 << "  " << printf("%2.1f",ProjPointsJJ[i]) << "  " << PlayerNameJJ[i] << endl;
  }
  cout << "#################################" << endl << endl;

  //########### Get My Team ############


  char ALtext[11]; sprintf(ALtext,"All Players");
  char qbtext[5];  sprintf(qbtext,"QB");
  char rbtext[5];  sprintf(rbtext,"RB");
  char wrtext[5];  sprintf(wrtext,"WR");
  char tetext[5];  sprintf(tetext,"TE");

  TellMe_HighestPlayers(ALtext,TheCondition, 10);
  TellMe_HighestPlayers(qbtext,TheQBCondition,5);
  TellMe_HighestPlayers(rbtext,TheRBCondition,5);
  TellMe_HighestPlayers(wrtext,TheWRCondition,5);
  TellMe_HighestPlayers(tetext,TheTECondition,5);

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

  FitWithGaus(h_qb,f_gaus_qb);
  FitWithGaus(h_rb,f_gaus_rb);
  FitWithGaus(h_wr,f_gaus_wr);
  FitWithGaus(h_te,f_gaus_te);

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

  //h_al->SetTitle("Jagoffs Fantasy Football Draft");
  h_al->SetTitle("Jason's Fantasy Football Draft");
  h_al->GetXaxis()->SetTitle("Projected Points per Game");
  h_al->GetYaxis()->SetTitle("Number of Players Available");
  h_al->Draw("");
  h_qb->Draw("same");
  h_rb->Draw("same");
  h_wr->Draw("same");
  h_te->Draw("same");


  for(int i=0; i<iSizeMyTeam-1; i++)
    lMyTeam[i]->Draw("same");


  TLegend *leg1 = new TLegend(0.5,0.6,0.89,0.89);
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

  c1->SaveAs("liveUpdatedCanvas.gif");
  c1->SaveAs("liveUpdatedCanvas.pdf");


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
  f1->FixParameter(1,MyMean);
  f1->FixParameter(2,MySigma);
  f1->SetLineColorAlpha(h1->GetLineColor(),0.4);
  f1->SetLineWidth(1);
  h1->Fit(f1,"QN","",MyMean-1.0*MySigma, MyMean+1.0*MySigma);
  h1->Fit(f1,"Q","",MyMean-3.0*MySigma, MyMean+3.0*MySigma);

  return;
}




void TellMe_HighestPlayers(char *Position, char *ListOfPlayers, int nPlayersToList)
{

  cout << "Top " << nPlayersToList << " " << Position << ": " << endl;

  TH1D *htemp = new TH1D("htemp","htemp",1000,0.5,1000.5);
  MyTree->Draw("PlayerRank>>htemp",ListOfPlayers);
  int counter = 0;
  for(int i=1; i<1000; i++){

    if(htemp->GetBinContent(i)>0){
      ifstream fin;
      fin.open("FantasyProjections2016.txt");
      Int_t PlayerRankJJ;
      Char_t PlayerNameJJ[50];
      Char_t PositionJJ[5];
      Double_t ProjPointsJJ;
      while(!fin.eof()){
        fin >> PlayerNameJJ >> PositionJJ >> PlayerRankJJ >> ProjPointsJJ;
        if(PlayerRankJJ==i)
          cout << i << "  " << printf("%2.1f",ProjPointsJJ/16.0) << "  " << PlayerNameJJ << endl;
      }

      counter++;
    }
    if(counter>=nPlayersToList)
      break;
  }
  htemp->Delete();

  cout << endl;

  return;

}

