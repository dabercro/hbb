LDFLAGS = `root-config --glibs`
CFLAGS = `root-config --cflags`
exename = makehbbtree
flatclass = hbbfile
testfile = F6F880D2-ACBE-E611-B59D-0CC47A706D40.root
corrections = data/corrections.d
bcalheader = bcal/BCalStrings.h

ifeq ($(origin CMSSW_BASE), undefined)
	pandainc = ../..
	pandalib = ../../PandaTree/lib
	pandalibs = -lpanda
	target = ${HOME}/bin/$(exename)

all: $(target) $(corrections)

else
	pandainc = ${CMSSW_BASE}/src
	pandalib = ${CMSSW_BASE}/lib/${SCRAM_ARCH}
	target =  ${CMSSW_BASE}/bin/${SCRAM_ARCH}/$(exename)
	pandalibs = -lPandaTreeFramework -lPandaTreeObjects -lPandaTreeUtils
	testfile = /mnt/hadoop/scratch/dabercro/panda/F6AA5BDD-AA0F-E711-847F-F832E4CC4E51.root

	checker = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/findtree.py
	thejq = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/jq
	tarball = ${CMSSW_BASE}/condor.tgz

all: $(tarball) $(corrections)

$(tarball): $(target) $(checker) $(thejq)
	tar -czf $(tarball) --directory ${CMSSW_BASE} bin lib external

$(checker): ${CROMBIEPATH}/scripts/findtree.py
	cp ${CROMBIEPATH}/scripts/findtree.py $(checker)

$(thejq): /home/dabercro/bin/jq
	cp /home/dabercro/bin/jq $(thejq)

endif

$(target): $(flatclass).h slimmer.cpp feedpanda.h checkrun.h $(bcalheader) btagreaders.h \
	${CROMBIEPATH}/PlotTools/interface/KinematicFunctions.h ${CROMBIEPATH}/SkimmingTools/interface/StoreParticles.h ${CROMBIEPATH}/SkimmingTools/interface/CmsswParse.h \
	bcal/BTagCalibrationStandalone.h bcal/BTagCalibrationStandalone.cpp
	g++ -I$(pandainc) -I${CROMBIEPATH} -Ibcal $(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -o $(target) slimmer.cpp bcal/BTagCalibrationStandalone.cpp

checkrun.h: certs/* ${CROMBIEPATH}/scripts/goodruns.py
	crombie goodruns certs/* checkrun

feedpanda.h: $(flatclass).h slimmer.cpp ${CROMBIEPATH}/scripts/feedpanda.pl
	crombie feedpanda $(pandainc)/PandaTree/defs/panda.def $(flatclass).h slimmer.cpp feedpanda.h

$(bcalheader): data/*.csv ${CROMBIEPATH}/scripts/bcalstrings.pl
	crombie bcalstrings csv=data/CSVv2_Moriond17_B_H.csv cmva=data/cMVAv2_Moriond17_B_H.csv deepcsv=data/DeepCSV_Moriond17_B_H.csv $(bcalheader)

$(flatclass).h: $(flatclass).txt ${CROMBIEPATH}/scripts/maketree.py
	crombie maketree $(flatclass).txt

$(corrections): data/downloads.url
	cd data; rm *.root; rm *.csv; wget -i downloads.url
	touch $(corrections)

.PHONY: clean test

testdir = ${HOME}/public_html/plots/testhbb

test: $(tarball) $(target)
	test ! -f test.root || rm test.root
	DYLD_LIBRARY_PATH=$(pandalib) $(exename) $(testfile) test.root
	test -z ${CMSSW_BASE} || test ! -d $(testdir) || rm -r $(testdir)
	test -z ${CMSSW_BASE} || crombie plotdump test.root $(testdir)

clean:
	rm $(target) $(flatclass).h feedpanda.h checkrun.h $(bcalheader)
