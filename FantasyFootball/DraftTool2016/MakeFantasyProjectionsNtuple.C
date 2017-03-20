TTree *MyTree;

TH1D *h_qb;
TH1D *h_rb;
TH1D *h_wr;
TH1D *h_te;
char saythis[100];

void MakeFantasyProjectionsNtuple()
{
  gStyle->SetOptStat(0);
  char junk[10];

  Int_t PlayerRank;
  Char_t PlayerName[50];
  Char_t Position[5];
  Double_t ProjPoints;

  h_al = new TH1D("h_al","h_al",50,-0.5, 49.5);
  h_qb = new TH1D("h_qb","h_qb",50, 0.0, 50.0);
  h_rb = new TH1D("h_rb","h_rb",50, 0.5, 50.5);
  h_wr = new TH1D("h_wr","h_wr",50, 0.25,50.25);
  h_te = new TH1D("h_te","h_te",50,-0.25, 49.75);

  MyTree = new TTree("MyTree","Fantasy Football Tree");

  MyTree->Branch("PlayerRank",&PlayerRank,"PlayerRank/I");
  MyTree->Branch("PlayerName",&PlayerName,"PlayerName/C");
  MyTree->Branch("Position",  &Position,  "Position/C");
  MyTree->Branch("ProjPoints",&ProjPoints,"ProjPoints/D");

  ifstream fin;
  fin.open("FantasyProjections2016.txt");

  while(!fin.eof()){

    fin >> PlayerName >> Position >> PlayerRank >> ProjPoints;
    MyTree->Fill();

    //cout << PlayerName << "\t\t" << Position << " \t" << PlayerRank << " \t" << ProjPoints << endl;
    cout << PlayerName << endl;
  }

  TCanvas *c1 = new TCanvas("c1","c1");

  MyTree->Draw("ProjPoints/16.0>>h_al","Position == \"QB\" || Position == \"RB\" || Position == \"WR\" || Position == \"TE\"");
  MyTree->Draw("ProjPoints/16.0>>h_qb","Position == \"QB\"");
  MyTree->Draw("ProjPoints/16.0>>h_rb","Position == \"RB\"");
  MyTree->Draw("ProjPoints/16.0>>h_wr","Position == \"WR\"");
  MyTree->Draw("ProjPoints/16.0>>h_te","Position == \"TE\"");

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
  rb_max   = h_rb->GetXaxis()->GetBinCenter(qb_maxb);
  wr_max   = h_wr->GetXaxis()->GetBinCenter(qb_maxb);
  te_max   = h_te->GetXaxis()->GetBinCenter(qb_maxb);
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

  h_al->Draw("");
  h_qb->Draw("same");
  h_rb->Draw("same");
  h_wr->Draw("same");
  h_te->Draw("same");

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


  TFile *fout = new TFile("FantasyTree.root","NEW");
  MyTree->Write();
  h_al->Write();
  h_qb->Write();
  h_rb->Write();
  h_wr->Write();
  h_te->Write();
  fout->Close();


}
