LDFLAGS = `root-config --glibs`
CFLAGS = `root-config --cflags`

pandainc = ${CMSSW_BASE}/src
pandalib = ${CMSSW_BASE}/lib/${SCRAM_ARCH}
pandalibs = -lPandaTreeFramework -lPandaTreeObjects
smear = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/smeartree

smeartestfile = /data/t3home000/dabercro/scratch/nano/*.root

all: $(smear)

include/feedsmear.h: include/[!f]*.h src/smear.cpp include/smearfile.h ${CROMBIEPATH}/scripts/feedpanda.pl
	crombie feedpanda $(pandainc)/PandaTree/defs/nanoaod.def include src/smear.cpp include/feedsmear.h

include/smearfile.h: treedefs/smearfile.txt ${CROMBIEPATH}/scripts/maketree.py $(corrections)
	crombie maketree treedefs/smearfile.txt
	mv smearfile.h include/

include/checkrun.h: ../slimmer/certs/* ${CROMBIEPATH}/scripts/goodruns.py
	crombie goodruns ../slimmer/certs/* include/checkrun

$(smear): include/feedsmear.h include/smearfile.h include/checkrun.h src/smear.cpp include/*.h
	g++ -Iinclude -I$(pandainc) -I${CROMBIEPATH}/src/include \
	-I${CROMBIEPATH}/old -g \
	$(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -o $(smear) src/smear.cpp

.PHONY: smeartest

smeartestdir = ${HOME}/public_html/plots/testnanosmear

smeartest: $(smear)
	test ! -f smear.root || rm smear.root
	smeartree $(smeartestfile) smear.root
	test -z ${CMSSW_BASE} || crombie plotdump smear.root $(smeartestdir)
