#include <TString.h>
#include <TFile.h>
#include <TChain.h>
#include <iostream>

#include "LEAF/Analyzer/include/BaseTool.h"
#include "LEAF/Analyzer/include/useful_functions.h"
#include <sys/stat.h>
#include "LEAF/Analyzer/include/Registry.h"
#include "LEAF/Analyzer/include/JetHists.h"
#include "LEAF/Analyzer/include/MuonHists.h"
#include "LEAF/Analyzer/include/ElectronHists.h"
#include "LEAF/Analyzer/include/TauHists.h"
#include "LEAF/Analyzer/include/JetIds.h"
#include "LEAF/Analyzer/include/MuonIds.h"
#include "LEAF/Analyzer/include/ElectronIds.h"
#include "LEAF/Analyzer/include/TauIds.h"

#include "LEAF/Analyzer/include/NJetSelection.h"


#include "LEAF/$MYANALYSISNAME/include/$MYEVENTCLASS.h"
#include "LEAF/$MYANALYSISNAME/include/$MYHISTNAME.h"

using namespace std;

class $MYTOOLNAME : public BaseTool {

public:
  // Constructors, destructor
  $MYTOOLNAME(const Config & cfg);
  ~$MYTOOLNAME() = default;
  void ProcessDataset(const Config & cfg) override {LoopEvents<$MYTOOLNAME, $MYEVENTCLASS>(cfg, event, *this);};
  virtual bool Process() override;
  void book_histograms(vector<TString>);
  void fill_histograms(TString);


private:
  $MYEVENTCLASS* event;

  // Modules used in the analysis
  unique_ptr<JetCleaner> cleaner_jet;

  // Selections used in the analysis
  unique_ptr<NJetSelection> selection_njets;
};



$MYTOOLNAME::$MYTOOLNAME(const Config & cfg) : BaseTool(cfg){

  event = new $MYEVENTCLASS();
  event->reset();

  MultiID<Jet> jet_id = {PtEtaId(20, 2.5), JetID(JetID::WP_TIGHT), JetPUID(JetPUID::WP_TIGHT)};
  cleaner_jet.reset(new JetCleaner(jet_id));

  selection_njets.reset(new NJetSelection(cfg, 4, -1));


  // histfolders
  vector<TString> histtags = {"input", "cleaner", "njets", "nominal"};
  book_histograms(histtags);
}




bool $MYTOOLNAME::Process(){
  // cout << "+++ NEW EVENT" << endl;

  // order all objecs in pT
  sort_by_pt<GenParticle>(*event->genparticles_visibletaus);
  sort_by_pt<GenParticle>(*event->genparticles_pruned);
  sort_by_pt<GenJet>(*event->genjets);
  sort_by_pt<Jet>(*event->jets_ak4chs);
  sort_by_pt<Muon>(*event->muons);
  sort_by_pt<Electron>(*event->electrons);
  sort_by_pt<Tau>(*event->taus);
  fill_histograms("input");

  // run example cleaner
  cleaner_jet->process(*event);
  fill_histograms("cleaner");

  // run example selection
  if(!selection_njets->passes(*event)) return false;
  fill_histograms("njets");

  // fill one set of histograms called "nominal", which is necessary for PostAnalyzer scripts
  fill_histograms("nominal");

  // store events passing the full selection for the next step
  return true;
}











void $MYTOOLNAME::book_histograms(vector<TString> tags){
  for(const TString & tag : tags){
    TString mytag = tag+"_General";
    book_HistFolder(mytag, new $MYHISTNAME(mytag));
    mytag = tag+"_Jets";
    book_HistFolder(mytag, new JetHists(mytag));
    mytag = tag+"_Muons";
    book_HistFolder(mytag, new MuonHists(mytag));
    mytag = tag+"_Electrons";
    book_HistFolder(mytag, new ElectronHists(mytag));
    mytag = tag+"_Taus";
    book_HistFolder(mytag, new TauHists(mytag));
  }
}

void $MYTOOLNAME::fill_histograms(TString tag){
  TString mytag = tag+"_General";
  HistFolder<$MYHISTNAME>(mytag)->fill(*event);
  mytag = tag+"_Jets";
  HistFolder<JetHists>(mytag)->fill(*event);
  mytag = tag+"_Muons";
  HistFolder<MuonHists>(mytag)->fill(*event);
  mytag = tag+"_Electrons";
  HistFolder<ElectronHists>(mytag)->fill(*event);
  mytag = tag+"_Taus";
  HistFolder<TauHists>(mytag)->fill(*event);
}




REGISTER_TOOL($MYTOOLNAME)
