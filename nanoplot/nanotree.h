#ifndef CROMBIE_NANOTREE_H
#define CROMBIE_NANOTREE_H

#include "TTree.h"
#include "TObject.h"
#include <string>
#include <vector>
#include "TFile.h"

class nanotree {

 public:
  nanotree(const char* outfile_name, const char* name = "events");
  ~nanotree() { write(t); f->Close(); }

  Float_t breg_mass;
  std::vector<Float_t> hbbjtidx;
  Float_t hbbm;
  std::vector<Float_t> jotFlav;
  Float_t noreg_mass;

  void reset();
  void fill();
  void write(TObject* obj) { f->WriteTObject(obj, obj->GetName()); }
  void set_flav(const int i);
  
  enum class dijet {
    breg,
    noreg
  };
  template <typename T> void set_dijet(const dijet base, const T& vec);

 private:
  TFile* f;
  TTree* t;

};

nanotree::nanotree(const char* outfile_name, const char* name)
: f {new TFile(outfile_name, "CREATE", "", 1)},
  t {new TTree(name, name)}
{
  t->Branch("breg_mass", &breg_mass, "breg_mass/F");
  t->Branch("hbbjtidx", &hbbjtidx);
  t->Branch("hbbm", &hbbm, "hbbm/F");
  t->Branch("jotFlav", &jotFlav);
  t->Branch("noreg_mass", &noreg_mass, "noreg_mass/F");
}

void nanotree::reset() {
  breg_mass = 0;
  hbbjtidx.clear();
  hbbm = 5;
  jotFlav.clear();
  noreg_mass = 0;
}

void nanotree::fill() {
  t->Fill();
}

void nanotree::set_flav(const int i) {
  jotFlav.push_back(5);
  hbbjtidx.push_back(i);
}

template <typename T> void nanotree::set_dijet(const dijet base, const T& vec) {
  switch(base) {
  case nanotree::dijet::breg:
    breg_mass = vec.M();;
    break;
  case nanotree::dijet::noreg:
    noreg_mass = vec.M();;
    break;
  default:
    throw;
  }
}

#endif
