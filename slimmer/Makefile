LDFLAGS = `root-config --glibs` `fastjet-config --libs`
CFLAGS = `root-config --cflags` `fastjet-config --cxxflags`
exename = makehbbtree
flatclass = hbbfile
testfile = F6F880D2-ACBE-E611-B59D-0CC47A706D40.root
corrections = data/corrections.d
effs = data/btag_effs.root
data = data/data.d

ifeq ($(origin CMSSW_BASE), undefined)
	pandainc = ../..
	pandalib = ../../PandaTree/lib
	pandalibs = -lpanda
	target = ${HOME}/bin/$(exename)

all: $(target) $(effs)

else
	pandainc = ${CMSSW_BASE}/src
	pandalib = ${CMSSW_BASE}/lib/${SCRAM_ARCH}
	target =  ${CMSSW_BASE}/bin/${SCRAM_ARCH}/$(exename)
	pandalibs = -lPandaTreeFramework -lPandaTreeObjects -lPandaTreeUtils
	testfile = /mnt/hadoop/scratch/dabercro/panda/*.root

	checker = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/findtree.py
	thejq = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/jq
	tarball = ${CMSSW_BASE}/condor.tgz

all: $(tarball) $(effs)

$(tarball): $(target) $(checker) $(thejq) $(data)
	test -d ${CMSSW_BASE}/data || mkdir ${CMSSW_BASE}/data
	tar -czf $(tarball) --directory ${CMSSW_BASE} bin lib external data

$(checker): ${CROMBIEPATH}/scripts/findtree.py
	cp ${CROMBIEPATH}/scripts/findtree.py $(checker)

$(thejq): /home/dabercro/bin/jq
	cp /home/dabercro/bin/jq $(thejq)

$(data): data/*.csv data/*.xml $(corrections)
	test -d ${CMSSW_BASE}/data && rm ${CMSSW_BASE}/data/* || mkdir ${CMSSW_BASE}/data
	cp data/*.csv data/*.xml ${CMSSW_BASE}/data
	touch $(data)

endif

$(target): $(flatclass).h slimmer.cpp feedpanda.h checkrun.h btagreaders.h misc.h \
	${CROMBIEPATH}/PlotTools/interface/KinematicFunctions.h ${CROMBIEPATH}/SkimmingTools/interface/ObjectStore.h ${CROMBIEPATH}/SkimmingTools/interface/CmsswParse.h \
	bcal/BTagCalibrationStandalone.h bcal/BTagCalibrationStandalone.cpp
	g++ -I$(pandainc) -I${CROMBIEPATH} -Ibcal $(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -lTMVA -o $(target) slimmer.cpp bcal/BTagCalibrationStandalone.cpp

checkrun.h: certs/* ${CROMBIEPATH}/scripts/goodruns.py
	crombie goodruns certs/* checkrun

feedpanda.h: $(flatclass).h slimmer.cpp ${CROMBIEPATH}/scripts/feedpanda.pl
	crombie feedpanda $(pandainc)/PandaTree/defs/panda.def $(flatclass).h slimmer.cpp feedpanda.h

$(flatclass).h: $(flatclass).txt ${CROMBIEPATH}/scripts/maketree.py $(corrections)
	crombie maketree $(flatclass).txt

$(effs): data/make_effs.py data/*eff.txt $(corrections)
	data/make_effs.py

$(corrections): data/downloads.url
	cd data; rm *.root; rm *.csv; rm *.xml; wget -i downloads.url
	touch $(corrections)

.PHONY: clean test

testdir = ${HOME}/public_html/plots/testhbb

test: $(tarball) $(target)
	test ! -f test.root || rm test.root
	DYLD_LIBRARY_PATH=$(pandalib) $(exename) $(testfile) test.root
	test -z ${CMSSW_BASE} || test ! -d $(testdir) || rm -r $(testdir)
	test -z ${CMSSW_BASE} || cut='|!cmva_hbb_gen|cmva_hbb_gen' crombie plotdump test.root $(testdir) \
	'cmva_hbb_m_reg|cmva_hbb_m; cmva_hbb && cmva_hbb_m_reg < 200' \
	'cmva_gen_withnu_dijet_m|cmva_hbb_m_reg|cmva_hbb_m; cmva_hbb && cmva_gen_withnu_dijet_m < 250' \
	'1 - cmva_hbb_m/cmva_gen_dijet_m|1 - cmva_hbb_m_reg/cmva_gen_withnu_dijet_m; cmva_hbb && cmva_jet1_gen && cmva_jet2_gen'

clean:
	rm $(target) $(flatclass).h feedpanda.h checkrun.h $(effs) $(data) $(corrections)
