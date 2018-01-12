LDFLAGS = `root-config --glibs`
CFLAGS = `root-config --cflags`
exename = makehbbtree
flatclass = hbbfile
testfile = F6F880D2-ACBE-E611-B59D-0CC47A706D40.root
corrections = data/corrections.d
breg = data/breg.d
effs = data/btag_effs.root
data = data/data.d

ifeq ($(origin CMSSW_BASE), undefined)
	pandainc = ../..
	pandalib = ../../PandaTree/lib
	pandalibs = -lpanda
	target = ${HOME}/bin/$(exename)

all: $(target) $(corrections) $(effs) $(breg)

else
	pandainc = ${CMSSW_BASE}/src
	pandalib = ${CMSSW_BASE}/lib/${SCRAM_ARCH}
	target =  ${CMSSW_BASE}/bin/${SCRAM_ARCH}/$(exename)
	pandalibs = -lPandaTreeFramework -lPandaTreeObjects -lPandaTreeUtils
	testfile = /mnt/hadoop/scratch/dabercro/panda/F6AA5BDD-AA0F-E711-847F-F832E4CC4E51.root

	checker = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/findtree.py
	thejq = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/jq
	tarball = ${CMSSW_BASE}/condor.tgz

all: $(tarball) $(corrections) $(effs) $(breg)

$(tarball): $(target) $(checker) $(thejq) $(data)
	tar -czf $(tarball) --directory ${CMSSW_BASE} bin lib external data

$(checker): ${CROMBIEPATH}/scripts/findtree.py
	cp ${CROMBIEPATH}/scripts/findtree.py $(checker)

$(thejq): /home/dabercro/bin/jq
	cp /home/dabercro/bin/jq $(thejq)

$(data): data/*.csv data/*.xml
	test -d ${CMSSW_BASE}/data && rm ${CMSSW_BASE}/data/* || mkdir ${CMSSW_BASE}/data
	cp data/*.csv data/*.xml ${CMSSW_BASE}/data
	touch $(data)

endif

$(target): $(flatclass).h slimmer.cpp feedpanda.h checkrun.h btagreaders.h \
	${CROMBIEPATH}/PlotTools/interface/KinematicFunctions.h ${CROMBIEPATH}/SkimmingTools/interface/StoreParticles.h ${CROMBIEPATH}/SkimmingTools/interface/CmsswParse.h \
	bcal/BTagCalibrationStandalone.h bcal/BTagCalibrationStandalone.cpp $(corrections)
	g++ -I$(pandainc) -I${CROMBIEPATH} -Ibcal $(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -lTMVA -o $(target) slimmer.cpp bcal/BTagCalibrationStandalone.cpp

checkrun.h: certs/* ${CROMBIEPATH}/scripts/goodruns.py
	crombie goodruns certs/* checkrun

feedpanda.h: $(flatclass).h slimmer.cpp ${CROMBIEPATH}/scripts/feedpanda.pl
	crombie feedpanda $(pandainc)/PandaTree/defs/panda.def $(flatclass).h slimmer.cpp feedpanda.h

$(flatclass).h: $(flatclass).txt ${CROMBIEPATH}/scripts/maketree.py
	crombie maketree $(flatclass).txt

$(effs): data/make_effs.py data/*eff.txt
	data/make_effs.py

JETS = $(shell perl -ne '/Bool_t (.*_jet\d);/ && print "$$1 "' $(flatclass).h)
$(breg): data/branches.txt $(flatclass).h
	$(foreach jet, $(JETS), sed 's/<>/$(jet)/g' data/branches.txt > data/$(jet)_branches.cfg;)
	touch $(breg)

$(corrections): data/downloads.url
	cd data; rm *.root; rm *.csv; rm *.xml; wget -i downloads.url
	touch $(corrections)

.PHONY: clean test

testdir = ${HOME}/public_html/plots/testhbb

test: $(tarball) $(target)
	test ! -f test.root || rm test.root
	DYLD_LIBRARY_PATH=$(pandalib) $(exename) $(testfile) test.root
	test -z ${CMSSW_BASE} || test ! -d $(testdir) || rm -r $(testdir)
	test -z ${CMSSW_BASE} || crombie plotdump test.root $(testdir)

clean:
	rm $(target) $(flatclass).h feedpanda.h checkrun.h $(effs) $(breg) $(data)
