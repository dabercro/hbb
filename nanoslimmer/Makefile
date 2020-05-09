LDFLAGS = `root-config --glibs`
CFLAGS = `root-config --cflags`

pandainc = ${CMSSW_BASE}/src
pandalib = ${CMSSW_BASE}/lib/${SCRAM_ARCH}
pandalibs = -lPandaTreeFramework -lPandaTreeObjects
smear = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/smearnano
hbb = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/hbbnano
tag = ${CMSSW_BASE}/tag
tarball = ${CMSSW_BASE}/nano.tgz

#smeartestfile = /data/t3home000/dabercro/scratch/nano/v6/*.root
smeartestfile = /data/t3home000/dabercro/scratch/nano/EGamma/*.root

nano_version = v5

all: $(tarball)

$(tag): ../.git/refs/heads/master
	cp ../.git/refs/heads/master $(tag)

../.git/refs/heads/master: $(shell git ls-files)
	git commit -am "Auto commit for tarball"

include/feedsmear.h: $(shell crombie deps src/smear.cpp | grep -v 'feedsmear.h') $(pandainc)/PandaTree/defs/nanoaod_$(nano_version).def feednano.pl
	./feednano.pl $(pandainc)/PandaTree/defs/nanoaod_$(nano_version).def `crombie deps src/smear.cpp` include/feedsmear.h

include/feedhbb.h: $(shell crombie deps src/hbb.cpp | grep -v 'feedhbb.h') $(pandainc)/PandaTree/defs/nanoaod_$(nano_version).def feednano.pl
	./feednano.pl $(pandainc)/PandaTree/defs/nanoaod_$(nano_version).def `crombie deps src/hbb.cpp` include/feedhbb.h

include/smearfile.h: $(shell crombie deps treedefs/smearfile.txt) ${CROMBIEPATH}/scripts/maketree.py
	crombie maketree treedefs/smearfile.txt
	mv smearfile.h include/

include/checkrun.h: ../slimmer/certs/* ${CROMBIEPATH}/scripts/goodruns.py
	crombie goodruns ../slimmer/certs/* include/checkrun

include/hbbfile.h: $(shell crombie deps treedefs/hbbfile.txt) ${CROMBIEPATH}/scripts/maketree.py
	crombie maketree treedefs/hbbfile.txt
	mv hbbfile.h include/

# Force generation of head files, just in case
include/headers.d: include/feedhbb.h include/feedsmear.h include/smearfile.h include/checkrun.h include/hbbfile.h
	touch include/headers.d

$(smear): $(shell crombie deps src/smear.cpp)
	g++ -Iinclude -I$(pandainc) -I${CROMBIEPATH}/src/include \
	-I${CROMBIEPATH}/old -g `./flags.sh` \
	$(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -o $(smear) src/smear.cpp

$(hbb): $(shell crombie deps src/hbb.cpp)
	g++ -Iinclude -I$(pandainc) -I${CROMBIEPATH}/src/include \
	-I${CROMBIEPATH}/old -g \
	$(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -o $(hbb) src/hbb.cpp

$(tarball): include/headers.d $(smear) $(hbb) ${CMSSW_BASE}/jec/* $(tag) ${CMSSW_BASE}/data/*
	tar -czf $(tarball) --directory ${CMSSW_BASE} bin lib jec tag data

.PHONY: smeartest install test

smeartestdir = ${HOME}/public_html/plots/testnanosmear

smeartest: $(smear)
	test ! -f smear.root || rm smear.root
	$(smear) 2017 $(smeartestfile) smear.root
	test ! -d $(smeartestdir)_old || rm -r $(smeartestdir)_old
	test ! -d $(smeartestdir) || mv $(smeartestdir) $(smeartestdir)_old
	test -z ${CMSSW_BASE} || crombie plotdump smear.root $(smeartestdir)

testdir = ${HOME}/public_html/plots/testnanohbb

test: $(hbb)
	test ! -f test.root || rm test.root
	$(hbb) 2017 /data/t3home000/dabercro/scratch/nano/2017/MET/*.root test.root
	test ! -d $(testdir)_old || rm -r $(testdir)_old
	test ! -d $(testdir) || mv $(testdir) $(testdir)_old
	test -z ${CMSSW_BASE} || crombie plotdump test.root $(testdir)
	crombie eventdump test.root test.txt 'event == 859137737 || event == 859161659 || event == 859181124 || event == 859201843 || event == 859207901 || event == 859220544 || event == 859300371 || event == 859317776 || event == 859405963 || event == 859499655 || event == 859506426' event fatjet1_pt jet1_pt_reg jet2_pt_reg dijet_m_reg
	cut -d '*' -f 3- test.txt | grep '^            8' | sort > test_8.txt

install: $(tarball)
