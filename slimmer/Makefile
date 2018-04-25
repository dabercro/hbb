LDFLAGS = `root-config --glibs` `fastjet-config --libs`
CFLAGS = `root-config --cflags` `fastjet-config --cxxflags`
installname = makehbbtree
exename = testhbbtree
flatclass = hbbfile
testfile = F6F880D2-ACBE-E611-B59D-0CC47A706D40.root
corrections = data/corrections.d
effs = data/btag_effs.root
data = data/data.d
rootdata = data/puppiCorr.root data/btag_effs.root data/puid.root 

ifeq ($(origin CMSSW_BASE), undefined)
	pandainc = ../..
	pandalib = ../../PandaTree/lib
	pandalibs = -lpanda
	target = ${HOME}/bin/$(exename)
	install = ${HOME}/bin/$(installname)

all: $(target) $(effs)

$(install): $(target)
	cp $(target) $(install)

else
	pandainc = ${CMSSW_BASE}/src
	pandalib = ${CMSSW_BASE}/lib/${SCRAM_ARCH}
	target = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/$(exename)
	install = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/$(installname)
	pandalibs = -lPandaTreeFramework -lPandaTreeObjects -lPandaTreeUtils
	testfile = /mnt/hadoop/scratch/dabercro/panda/009/*.root

	checker = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/findtree.py
	thejq = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/jq
	tarball = ${CMSSW_BASE}/condor.tgz

all: $(target) $(effs)

$(install): $(target)
	cp $(target) $(install)

$(tarball): $(install) $(checker) $(thejq) $(data)
	test -d ${CMSSW_BASE}/data || mkdir ${CMSSW_BASE}/data
	tar -czf $(tarball) --directory ${CMSSW_BASE} bin lib external data

$(checker): ${CROMBIEPATH}/scripts/findtree.py
	cp ${CROMBIEPATH}/scripts/findtree.py $(checker)

$(thejq): /home/dabercro/bin/jq
	cp /home/dabercro/bin/jq $(thejq)

$(data): data/*.csv data/*.xml $(rootdata) $(corrections) data/rcdata.2016.v3
	test -d ${CMSSW_BASE}/data && rm ${CMSSW_BASE}/data/*.??? || mkdir ${CMSSW_BASE}/data
	cp data/*.csv data/*.xml $(rootdata) ${CMSSW_BASE}/data
	test ! -d ${CMSSW_BASE}/data/rcdata.2016.v3 || rm -r ${CMSSW_BASE}/data/rcdata.2016.v3
	cp -rH data/rcdata.2016.v3 ${CMSSW_BASE}/data
	touch $(data)

endif

$(target): slimmer.cpp include/*.h include/checkrun.h include/feedpanda.h include/$(flatclass).h \
	${CROMBIEPATH}/PlotTools/interface/KinematicFunctions.h ${CROMBIEPATH}/SkimmingTools/interface/ObjectStore.h ${CROMBIEPATH}/SkimmingTools/interface/CmsswParse.h \
	roccor/RoccoR.cc
	g++ -I$(pandainc) -I${CROMBIEPATH} -Iinclude -Ibcal -Iroccor $(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -lTMVA -o $(target) slimmer.cpp

roccor/RoccoR.cc:
	test -d roccor || mkdir roccor
	cd roccor; test ! -f roccor.2016.v3.tgz || rm roccor.2016.v3.tgz; wget http://t3serv001.mit.edu/~dabercro/slimmerfiles/180302/roccor.2016.v3.tgz; tar -xf roccor.2016.v3.tgz
	cd data; test -f rcdata.2016.v3 || ln -s ../roccor/rcdata.2016.v3 rcdata.2016.v3

include/checkrun.h: certs/* ${CROMBIEPATH}/scripts/goodruns.py
	crombie goodruns certs/* include/checkrun

include/feedpanda.h: include/[!f]*.h include/$(flatclass).h include/checkrun.h slimmer.cpp ${CROMBIEPATH}/scripts/feedpanda.pl
	crombie feedpanda $(pandainc)/PandaTree/defs/panda.def include/*.h slimmer.cpp include/feedpanda.h

include/$(flatclass).h: $(flatclass).txt ${CROMBIEPATH}/scripts/maketree.py $(corrections)
	crombie maketree $(flatclass).txt
	mv $(flatclass).h include/

$(effs): data/make_effs.py data/*eff.txt $(corrections)
	data/make_effs.py

$(corrections): data/downloads.url
	cd data; rm *.root; rm *.csv; rm *.xml; wget -i downloads.url
	touch $(corrections)

.PHONY: clean test install

install: $(tarball) $(install) $(effs)

testdir = ${HOME}/public_html/plots/testhbb

test: $(target) $(effs)
	test -d old || mkdir old
	test -d new || mkdir new
	# Move old test.root to a place for comparison
	test ! -f test.root || mv test.root old
	# Don't keep around new test file that's garbage
	DYLD_LIBRARY_PATH=$(pandalib) $(exename) $(testfile) test.root || rm test.root
	# Kill if test file generation didn't work
	test -f test.root
	cp test.root new
	crombie diff -b old new
	# Move plots, only if last test was valid
	test -z ${CMSSW_BASE} || test ! -d $(testdir)_old || rm -r $(testdir)_old
	test -z ${CMSSW_BASE} || test ! -d $(testdir) || mv $(testdir) $(testdir)_old
	test -z ${CMSSW_BASE} || cut='|!cmva_hbb_gen|cmva_hbb_gen' crombie plotdump test.root $(testdir)

clean:
	rm $(target) include/$(flatclass).h include/feedpanda.h include/checkrun.h $(effs) $(data) $(corrections) roccor/RoccoR.cc
