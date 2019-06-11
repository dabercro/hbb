#include <cmath>
#include <iostream>

#include "TLorentzVector.h"

#include "nanoaod.h"
#include "nanotree.h"

#include "crombie/EtaPhiMap.h"

TLorentzVector make_vec (unsigned index, float* pt, float* eta, float* phi, float* mass) {
  TLorentzVector vec;
  vec.SetPtEtaPhiM(pt[index], eta[index], phi[index], mass[index]);
  return vec;
}

int main (int argc, char** argv) {

  if (argc != 3) {
    std::cout << argv[0] << " IN OUT" << std::endl;
    return 1;
  }

  TFile infile {argv[1]};
  auto* _intree = static_cast<TTree*>(infile.Get("Events"));
  if (not _intree) {
    std::cerr << "Tree not found" << std::endl;
    return 2;
  }
  nanoaod intree {_intree};
  nanotree output {argv[2]};

  EtaPhiMap<TLorentzVector> genjetmap {0.1, 2.5, 
      [] (auto* vec) {return vec->Eta();},
      [] (auto* vec) {return vec->Phi();}
  };

  auto nentry = intree.fChain->GetEntries();

  for (decltype(nentry) i_entry = 0; i_entry < nentry; ++i_entry) {

    if (not (i_entry % 20000))
      std::cout << i_entry << std::endl;

    intree.GetEntry(i_entry);
    output.reset();

    std::vector<TLorentzVector> genjets;
    auto ngen = intree.nGenJet;
    genjets.reserve(ngen);

    for (decltype(ngen) ijet = 0; ijet < ngen; ++ijet) {
      if (std::abs(intree.GenJet_partonFlavour[ijet]) == 5)
        genjets.push_back(make_vec(ijet, intree.GenJet_pt, intree.GenJet_eta, intree.GenJet_phi, intree.GenJet_mass));
    }

    genjetmap.AddParticles(genjets);

    auto njet = intree.nJet;
    float maxcmva = -1;
    float nextcmva = -1;
    TLorentzVector maxvec;
    TLorentzVector nextvec;

    TLorentzVector maxvecreg;
    TLorentzVector nextvecreg;

    for (decltype(njet) ijet = 0; ijet < njet; ++ijet) {
      if (std::abs(intree.Jet_eta[ijet]) > 2.5 or
          not genjetmap.GetParticles(intree.Jet_eta[ijet], intree.Jet_phi[ijet], 0.4).size())
        continue;

      auto tag = intree.Jet_btagCMVA[ijet];

      if (tag > nextcmva) {
        auto vec = make_vec(ijet, intree.Jet_pt, intree.Jet_eta, intree.Jet_phi, intree.Jet_mass);
        auto vecreg = make_vec(ijet, intree.Jet_bReg, intree.Jet_eta, intree.Jet_phi, intree.Jet_mass);

        if (tag > maxcmva) {
          nextcmva = maxcmva;
          nextvec = maxvec;
          nextvecreg = maxvecreg;
          maxcmva = tag;
          maxvec = vec;
          maxvecreg = vecreg;
        }
        else {
          nextcmva = tag;
          nextvec = vec;
          nextvecreg = vecreg;
        }
      }
    }

    if (nextcmva == -1)
      continue;

    output.set_dijet(nanotree::dijet::breg, maxvecreg + nextvecreg);
    output.set_dijet(nanotree::dijet::noreg, maxvec + nextvec);

    output.set_flav(0);
    output.set_flav(1);

    output.fill();

  }

  return 0;
}
