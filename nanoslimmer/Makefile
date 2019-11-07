LDFLAGS = `root-config --glibs`
CFLAGS = `root-config --cflags`

pandainc = ${CMSSW_BASE}/src
pandalib = ${CMSSW_BASE}/lib/${SCRAM_ARCH}
pandalibs = -lPandaTreeFramework -lPandaTreeObjects
smear = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/smearnano
tarball = ${CMSSW_BASE}/nano.tgz

smeartestfile = /data/t3home000/dabercro/scratch/nano/*.root

all: $(smear)

include/feednano.h: include/[!f]*.h src/smear.cpp include/smearfile.h feednano.pl
	./feednano.pl $(pandainc)/PandaTree/defs/nanoaod.def include/*.h src/smear.cpp include/feednano.h

include/smearfile.h: treedefs/smearfile.txt ${CROMBIEPATH}/scripts/maketree.py $(corrections)
	crombie maketree treedefs/smearfile.txt
	mv smearfile.h include/

include/checkrun.h: ../slimmer/certs/* ${CROMBIEPATH}/scripts/goodruns.py
	crombie goodruns ../slimmer/certs/* include/checkrun

$(smear): include/feednano.h include/smearfile.h include/checkrun.h src/smear.cpp include/*.h
	g++ -Iinclude -I$(pandainc) -I${CROMBIEPATH}/src/include \
	-I${CROMBIEPATH}/old -g \
	$(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -o $(smear) src/smear.cpp

$(tarball): $(smear)
	tar -czf $(tarball) --directory ${CMSSW_BASE} bin lib

.PHONY: smeartest install

smeartestdir = ${HOME}/public_html/plots/testnanosmear

smeartest: $(smear)
	test ! -f smear.root || rm smear.root
	$(smear) $(smeartestfile) smear.root
	test -z ${CMSSW_BASE} || crombie plotdump smear.root $(smeartestdir)

install: $(tarball)