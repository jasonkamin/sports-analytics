// #############################################################################################
//
// Data taken from https://www.ticketcity.com/nba/nba-finals-tickets/nba-finals-champions.html
//
//
// This project explores whether the NBA Finals outcomes are consistent with two 
// equally matched teams.  The metric is the "number of games won by the loser". 
// 
// First, it calculates the "ideal" N_games_won_by_loser under the assumption that
// each team had a 50% chance to win each game.  Incidentally, this "ideal" distribution 
// can also be calculated analytically ! 
//
// Second, it runs toys to determine a compatibility distribution (in this case, chi2)
// when compared to the "ideal".  
//
// Third, it calculates the actaul chi2 of the data compared to the "ideal". 
//
// Finally, it calculates the p-value of getting the actual historical results. 
//
// We compare the chi2 distribution to an ideal chisq distribution with the appropriate
// degrees of freedom.  Moreover, the p-value is ~0.8, indicating that the NBA Finals outcomes 
// (since 1947) are consistent with the teams being equally matched (ie. each team always has 
// a 50% chance to win).  We should note, however, that 70 years worth of data is actually 
// pretty low statistics.  In our N_games_won_by_loser distribution, there are 4 bins [0,1,2,3] 
// and each bin has between 8 and 26 counts.  The uncertainties are therefore quite large. 
// 
//
// run macro with ROOT by: 
// $ root -l NBAFinalsOutcomes.C
//
// #############################################################################################



TH1D *h_winsloser;
TH1D *h_NULL;

TH1D *h_toys[10000];
TH2D *h2_toys;
TH1D *h_chisq;
TF1 *f_chisq;

double CalcChiSq(TH1D *h1, TH1D *h2, int firstBin, int lastBin);
int FindNGamesLoser(int nGamesToWin, double equality=0.5);

char saythis[500];

void NBAFinalsOutcomes()
{

  h_winsloser = new TH1D("h_winsloser","h_winsloser",6,-0.5,5.5);
  h_winsloser->SetTitle("metric");
  h_winsloser->GetXaxis()->SetTitle("N_{wins} by losing team");
  h_winsloser->GetYaxis()->SetTitle("N_{finals}");
  h2_toys     = new TH2D("h2_toys","h2_toys", 6,-0.5,5.5, 51,-0.5,50.5);
  h2_toys->SetTitle("toy distributions");
  h2_toys->GetXaxis()->SetTitle("N_{wins} by losing team");
  h2_toys->GetYaxis()->SetTitle("N_{finals}");
  h2_toys->GetZaxis()->SetTitle("N_{toys}");

  h_chisq = new TH1D("h_chisq","h_chisq",200,0,40);
  h_chisq->SetTitle("compatibility distribution");
  h_chisq->GetXaxis()->SetTitle("#chi^{2}");
  h_chisq->GetYaxis()->SetTitle("N_{toys}");
  h_chisq->SetMarkerStyle(20);
  h_chisq->SetMarkerSize (0.5);

  int year[70] = {
    2016,
    2015,
    2014,
    2013,
    2012,
    2011,
    2010,
    2009,
    2008,
    2007,
    2006,
    2005,
    2004,
    2003,
    2002,
    2001,
    2000,
    1999,
    1998,
    1997,
    1996,
    1995,
    1994,
    1993,
    1992,
    1991,
    1990,
    1989,
    1988,
    1987,
    1986,
    1985,
    1984,
    1983,
    1982,
    1981,
    1980,
    1979,
    1978,
    1977,
    1976,
    1975,
    1974,
    1973,
    1972,
    1971,
    1970,
    1969,
    1968,
    1967,
    1966,
    1965,
    1964,
    1963,
    1962,
    1961,
    1960,
    1959,
    1958,
    1957,
    1956,
    1955,
    1954,
    1953,
    1952,
    1951,
    1950,
    1949,
    1948,
    1947
  };

  int WinsLoser[70] = {
    3,
    2,
    1,
    3,
    1,
    2,
    3,
    1,
    2,
    0,
    2,
    3,
    1,
    2,
    0,
    1,
    2,
    1,
    2,
    2,
    2,
    0,
    3,
    2,
    2,
    1,
    1,
    0,
    3,
    2,
    2,
    2,
    3,
    0,
    2,
    2,
    2,
    1,
    3,
    2,
    2,
    0,
    3,
    1,
    1,
    0,
    3,
    3,
    2,
    2,
    3,
    1,
    1,
    2,
    3,
    1,
    3,
    0,
    2,
    3,
    1,
    3,
    3,
    1,
    3,
    3,
    2,
    2,
    2,
    1
  };

  for(int i=0; i<70; i++){
    h_winsloser->Fill(WinsLoser[i]);
  }

  h_NULL = (TH1D*)h_winsloser->Clone("h_NULL");
  h_NULL->Reset();
  //for(int i=1; i<5; i++){
  //  h_NULL->SetBinContent(i,70.0/4.0);
  //  h_NULL->SetBinError  (i,0);
  //}
  for(int i=0; i<10000; i++)
    h_NULL->Fill(FindNGamesLoser(4));
  h_NULL->Sumw2();
  h_NULL->Scale(70.0/h_NULL->GetEntries());
  h_NULL->SetMarkerStyle(24);
  h_NULL->SetMarkerColor(1);
  h_NULL->SetLineColor  (1);

  h_winsloser->SetMarkerStyle(20);
  h_winsloser->SetMarkerColor(2);
  h_winsloser->SetLineColor  (2);

  TCanvas *c1 = new TCanvas("c1","c1");
  h_winsloser->Draw("pe");
  h_NULL     ->Draw("pe,same");

  TLegend *leg1 = new TLegend(0.5,0.2,0.89,0.4);
  leg1->AddEntry(h_winsloser, "historical data","PL");
  leg1->AddEntry(h_NULL     , "ideal distribution (50% / 50%)","PL");
  leg1->Draw();


  double chisq_data = CalcChiSq(h_winsloser,h_NULL,1,4);

  cout << "chisq/ndf = " << chisq_data << "/3 = " << chisq_data/3.0 << endl;



  TCanvas *c2 = new TCanvas("c2","c2",1000,500);
  c2->Divide(2,1);
  if(1){//running toys...

    for(int i=0; i<10000; i++){
      sprintf(saythis,"h_toys_%d",i);
      h_toys[i] = (TH1D*)h_NULL->Clone(saythis);
      h_toys[i]->Reset();
      for(int j=0; j<70; j++){
        int jj = FindNGamesLoser(4);
        h_toys[i]->Fill(jj);
      }
      for(int j=1; j<h_toys[i]->GetNbinsX()+1; j++)
        h2_toys->Fill(h_toys[i]->GetBinCenter(j),h_toys[i]->GetBinContent(j));
      h_chisq->Fill( CalcChiSq(h_toys[i],h_NULL,1,4) );
      c2->cd(1);
      if(i==0) h_toys[i]->Draw();
      else     h_toys[i]->Draw("same");
    }
    c2->cd(2);
    h_chisq->Draw("pe");

  }

  c2->cd(1)->Clear();
  h2_toys->Draw("colz");



  c2->cd(2);
  f_chisq = new TF1("f_chisq","[1]*(pow(x,[0]/2-1)*TMath::Exp(-x/2))/(pow(2,[0]/2)*TMath::Gamma([0]/2))",0,50);
  f_chisq->SetParameter(1,1);
  f_chisq->SetParameter(0,3);
  f_chisq->SetParameter(1,h_chisq->Integral(-1,-1,"width")/f_chisq->Integral(0,50));
  f_chisq->SetLineColor(8);
  f_chisq->SetLineWidth(2);
  f_chisq->SetLineStyle(2);
  f_chisq->Draw("same");

  TLine *linedata = new TLine(chisq_data,0,chisq_data,h_chisq->GetMaximum());
  linedata->SetLineStyle(2);
  linedata->SetLineWidth(2);
  linedata->SetLineColor(2);
  linedata->Draw("same");

  TLegend *leg2 = new TLegend(0.5,0.5,0.89,0.7);
  leg2->AddEntry(h_chisq, "toy distribution","PL");
  leg2->AddEntry(f_chisq, "ideal #chi^{2}, ndf = 3","L");
  leg2->AddEntry(linedata,"#chi^{2} of data","L");
  leg2->Draw();

  cout << "p-value = " << h_chisq->Integral(h_chisq->GetXaxis()->FindBin(chisq_data)+1,-1)/h_chisq->Integral(-1,-1) << endl;



}



double CalcChiSq(TH1D *h1, TH1D *h2, int firstBin, int lastBin)
{

  double chisq = 0.0;
  double delta = 0.0;
  double errsq = 0.0;

  if(h1->GetNbinsX() != h2->GetNbinsX()){
    cout << "JJ::CalcChiSq() --> histograms don't have same number of bins !!" << endl;
    return 0;
  }

  if(firstBin<0)
    firstBin=1;
  if(lastBin<0)
    lastBin=h1->GetNbinsX();

  for(int i=firstBin; i<lastBin+1; i++){
    double h1cont = h1->GetBinContent(i);
    double h2cont = h2->GetBinContent(i);
    double h1err  = h1->GetBinError  (i);
    double h2err  = h2->GetBinError  (i);

    if(h1cont !=0 && h2cont !=0){
      delta = (h1cont-h2cont);
      errsq = h1err*h1err + h2err*h2err;
      chisq += delta*delta/errsq;
    }

  }

  return chisq;
}


int FindNGamesLoser(int nGamesToWin, double equality=0.5)
{
  int nGamesLoser = 0;
  int nGamesT1    = 0;
  int nGamesT2    = 0;

  TRandom randTemp;
  randTemp.SetSeed(0);

  while(nGamesT1<nGamesToWin && nGamesT2<nGamesToWin){

    if(randTemp.Rndm()>equality)
      nGamesT1++;
    else
      nGamesT2++;

  }

  if(nGamesT1==nGamesToWin)
    nGamesLoser = nGamesT2;
  else
    nGamesLoser = nGamesT1;

  return nGamesLoser;
}
