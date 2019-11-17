LDFLAGS = `root-config --glibs`
CFLAGS = `root-config --cflags`

pandainc = ${CMSSW_BASE}/src
pandalib = ${CMSSW_BASE}/lib/${SCRAM_ARCH}
pandalibs = -lPandaTreeFramework -lPandaTreeObjects
smear = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/smearnano
hbb = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/hbbnano
tarball = ${CMSSW_BASE}/nano.tgz

smeartestfile = /data/t3home000/dabercro/scratch/nano/*.root

nano_version = v5

all: $(tarball)

include/feednano.h: include/[!f]*.h src/*.cpp include/smearfile.h $(pandainc)/PandaTree/defs/nanoaod_$(nano_version).def feednano.pl
	./feednano.pl $(pandainc)/PandaTree/defs/nanoaod_$(nano_version).def include/*.h src/*.cpp include/feednano.h

include/smearfile.h: treedefs/smearfile.txt treedefs/eventinfo.txt ${CROMBIEPATH}/scripts/maketree.py
	crombie maketree treedefs/smearfile.txt
	mv smearfile.h include/

include/checkrun.h: ../slimmer/certs/* ${CROMBIEPATH}/scripts/goodruns.py
	crombie goodruns ../slimmer/certs/* include/checkrun

include/hbbfile.h: treedefs/hbbfile.txt treedefs/eventinfo.txt ${CROMBIEPATH}/scripts/maketree.py
	crombie maketree treedefs/hbbfile.txt
	mv hbbfile.h include/

$(smear): include/feednano.h include/smearfile.h include/checkrun.h src/smear.cpp include/*.h
	g++ -Iinclude -I$(pandainc) -I${CROMBIEPATH}/src/include \
	-I${CROMBIEPATH}/old -g \
	$(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -o $(smear) src/smear.cpp

$(hbb): include/feednano.h include/hbbfile.h include/checkrun.h src/hbb.cpp include/*.h
	g++ -Iinclude -I$(pandainc) -I${CROMBIEPATH}/src/include \
	-I${CROMBIEPATH}/old -g \
	$(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -o $(hbb) src/hbb.cpp

$(tarball): $(smear) $(hbb)
	tar -czf $(tarball) --directory ${CMSSW_BASE} bin lib

.PHONY: smeartest install test

smeartestdir = ${HOME}/public_html/plots/testnanosmear

smeartest: $(smear)
	test ! -f smear.root || rm smear.root
	$(smear) $(smeartestfile) smear.root
	test -z ${CMSSW_BASE} || crombie plotdump smear.root $(smeartestdir)

testdir = ${HOME}/public_html/plots/testnanohbb

test: $(hbb)
	test ! -f test.root || rm test.root
	$(hbb) $(smeartestfile) test.root
	test ! -d $(testdir)_old || rm -r $(testdir)_old
	test ! -d $(testdir) || mv $(testdir) $(testdir)_old
	test -z ${CMSSW_BASE} || crombie plotdump test.root $(testdir)

install: $(tarball)
