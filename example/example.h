#ifndef CROMBIE_EXAMPLE_H
#define CROMBIE_EXAMPLE_H

#include <string>
#include <vector>
#include "TObject.h"
#include "TFile.h"
#include "TTree.h"

class example {

 public:
  example(const char* outfile_name, const char* name = "events");
  ~example() { write(t); f->Close(); }

  Float_t dimuon_m;
  Float_t dimuon_pt;
  Bool_t muon1;
  Float_t muon1_eta;
  Float_t muon1_m;
  Float_t muon1_phi;
  Float_t muon1_pt;
  Bool_t muon2;
  Float_t muon2_eta;
  Float_t muon2_m;
  Float_t muon2_phi;
  Float_t muon2_pt;
  Int_t n_lep;

  void reset();
  void fill();
  void write(TObject* obj) { f->WriteTObject(obj, obj->GetName()); }
  
  enum class muon {
    muon1,
    muon2
  };
  void set_muon(const muon base, const panda::Muon& mu);

 private:
  TFile* f;
  TTree* t;

};

example::example(const char* outfile_name, const char* name)
: f {new TFile(outfile_name, "CREATE", "", 1)},
  t {new TTree(name, name)}
{
  t->Branch("dimuon_m", &dimuon_m, "dimuon_m/F");
  t->Branch("dimuon_pt", &dimuon_pt, "dimuon_pt/F");
  t->Branch("muon1", &muon1, "muon1/O");
  t->Branch("muon1_eta", &muon1_eta, "muon1_eta/F");
  t->Branch("muon1_m", &muon1_m, "muon1_m/F");
  t->Branch("muon1_phi", &muon1_phi, "muon1_phi/F");
  t->Branch("muon1_pt", &muon1_pt, "muon1_pt/F");
  t->Branch("muon2", &muon2, "muon2/O");
  t->Branch("muon2_eta", &muon2_eta, "muon2_eta/F");
  t->Branch("muon2_m", &muon2_m, "muon2_m/F");
  t->Branch("muon2_phi", &muon2_phi, "muon2_phi/F");
  t->Branch("muon2_pt", &muon2_pt, "muon2_pt/F");
  t->Branch("n_lep", &n_lep, "n_lep/I");
}

void example::reset() {
  dimuon_m = -1;
  dimuon_pt = -1;
  muon1 = 0;
  muon1_eta = 0;
  muon1_m = 0;
  muon1_phi = 0;
  muon1_pt = 0;
  muon2 = 0;
  muon2_eta = 0;
  muon2_m = 0;
  muon2_phi = 0;
  muon2_pt = 0;
  n_lep = 0;
}

void example::fill() {
  if (!(muon2))
    return;
  t->Fill();
}

void example::set_muon(const muon base, const panda::Muon& mu) {
  switch(base) {
  case example::muon::muon1:
    muon1 = true;
    muon1_pt = mu.pt();
    muon1_eta = mu.eta();
    muon1_phi = mu.phi();
    muon1_m = mu.m();
    break;
  case example::muon::muon2:
    muon2 = true;
    muon2_pt = mu.pt();
    muon2_eta = mu.eta();
    muon2_phi = mu.phi();
    muon2_m = mu.m();
    break;
  default:
    throw;
  }
}

#endif
