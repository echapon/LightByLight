#include "TFile.h"
#include "TTree.h"

// WEIGHTS
// (from https://twiki.cern.ch/twiki/bin/view/CMS/MCDileptonTreesAndSkims2015Data)
// pt 0-3 => 0.2469/150659
// pt 3-6 => 0.1721/191736 (extended 0.1721/3842575)
// pt 6-9 => 0.0204/227986 (extended 0.0204/2268977)
// pt 9-12 => 0.00403/168628
// pt 12-15 => 0.001211/155793 
// pt 15-30 => 0.0007182/104729 
// pt 30-inf => 0.0000328/47059 

void setTreeWeight(const char* filename, double weight) {
   TFile *f1 = TFile::Open(filename,"UPDATE");
   TTree* tree = (TTree*)f1->Get("tpTree/fitter_tree");
   tree->SetWeight(weight);
   tree->AutoSave();
   TTree* treeTrk = (TTree*)f1->Get("tpTreeTrk/fitter_tree");
   treeTrk->SetWeight(weight);
   treeTrk->AutoSave();
   TTree* treeSta = (TTree*)f1->Get("tpTreeSta/fitter_tree");
   treeSta->SetWeight(weight);
   treeSta->AutoSave();
   f1->Close();
}
