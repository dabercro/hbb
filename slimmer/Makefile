LDFLAGS = `root-config --glibs` `fastjet-config --libs`
CFLAGS = `root-config --cflags` `fastjet-config --cxxflags`
installname = mkhbb
exename = testhbbtree
flatclass = hbbfile
testfile = F6F880D2-ACBE-E611-B59D-0CC47A706D40.root
corrections = data/corrections.d
effs = data/btag_effs.root
data = data/data.d

# These flags are needed for tensorflow
# Need to change if not using CMSSW_9_4_6
tensorflow = -DEIGEN_DONT_PARALLELIZE -I/cvmfs/cms.cern.ch/slc6_amd64_gcc630/external/tensorflow-cc/1.3.0-mmelna/tensorflow_cc/include -I/cvmfs/cms.cern.ch/slc6_amd64_gcc630/external/protobuf/3.4.0-fmblme/include -I/cvmfs/cms.cern.ch/slc6_amd64_gcc630/external/protobuf/3.4.0-fmblme/include -I/cvmfs/cms.cern.ch/slc6_amd64_gcc630/external/eigen/c7dc0a897676/include/eigen3 -I/cvmfs/cms.cern.ch/slc6_amd64_gcc630/external/tensorflow-c/1.1.0-mmelna/include -Wl,--allow-multiple-definition -Wl,--whole-archive -L${CMSSW_RELEASE_BASE}/lib/slc6_amd64_gcc630 -L${CMSSW_RELEASE_BASE}/external/slc6_amd64_gcc630/lib -ltensorflow_cc -lprotobuf -lrt -ltensorflow

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

$(data): data/*.csv data/*.xml data/*.root $(corrections) data/rcdata.2016.v3
	test -d ${CMSSW_BASE}/data && rm ${CMSSW_BASE}/data/*.pb ${CMSSW_BASE}/data/*.??? || mkdir ${CMSSW_BASE}/data
	cp data/*.csv data/*.xml data/*.root data/*.pb ${CMSSW_BASE}/data
	test ! -d ${CMSSW_BASE}/data/rcdata.2016.v3 || rm -r ${CMSSW_BASE}/data/rcdata.2016.v3
	cp -rH data/rcdata.2016.v3 ${CMSSW_BASE}/data
	test ! -d ${CMSSW_BASE}/data/jec || rm -r ${CMSSW_BASE}/data/jec
	cp -rH data/jec ${CMSSW_BASE}/data
	touch $(data)

endif

$(target): slimmer.cpp include/*.h include/checkrun.h include/feedpanda.h include/$(flatclass).h \
	${CROMBIEPATH}/src/include/crombie/*.h \
	include/roccor/RoccoR.cc include/jecsys data/jec/jec.d
	g++ -Iinclude -I$(pandainc) -I${CROMBIEPATH}/src/include \
	-I${CROMBIEPATH} \
	$(tensorflow) \
	$(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -lCondFormatsJetMETObjects -lTMVA -o $(target) slimmer.cpp

include/jecsys:
	cd include; git clone https://github.com/miquork/jecsys.git; cd jecsys; git checkout Summer16_23Sep2016V4

include/roccor/RoccoR.cc:
	test -d include/roccor || mkdir include/roccor
	cd include/roccor; test ! -f roccor.2016.v3.tgz || rm roccor.2016.v3.tgz; wget http://t3serv001.mit.edu/~dabercro/slimmerfiles/180302/roccor.2016.v3.tgz; tar -xf roccor.2016.v3.tgz
	cd data; test -f rcdata.2016.v3 || ln -s ../include/roccor/rcdata.2016.v3 rcdata.2016.v3

include/checkrun.h: certs/* ${CROMBIEPATH}/scripts/goodruns.py
	crombie goodruns certs/* include/checkrun

data/jec/jec.d: data/jec/download.url
	cd data/jec; rm *.tar.gz; wget -i download.url; for f in *.tar.gz; do tar -xvf $$f '*_AK4PFchs.txt' '*_AK8PFPuppi.txt'; done
	touch data/jec/jec.d

include/feedpanda.h: include/[!f]*.h include/$(flatclass).h include/checkrun.h slimmer.cpp ${CROMBIEPATH}/scripts/feedpanda.pl
	crombie feedpanda $(pandainc)/PandaTree/defs/panda.def include/*.h slimmer.cpp include/feedpanda.h

include/$(flatclass).h: $(flatclass).txt ${CROMBIEPATH}/scripts/maketree.py $(corrections)
	crombie maketree $(flatclass).txt
	mv $(flatclass).h include/

$(effs): data/make_effs.py data/*eff.txt $(corrections)
	data/make_effs.py

$(corrections): data/downloads.url
	cd data; rm *.root; rm *.csv; rm *.xml; rm *.pb; wget -i downloads.url
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
	input_dir='/test/pandaf/009/this' $(exename) $(testfile) test.root || rm test.root
	# Kill if test file generation didn't work
	test -f test.root
	cp test.root new
	crombie diff -b old new
	# Move plots, only if last test was valid
	test -z ${CMSSW_BASE} || test ! -d $(testdir)_old || rm -r $(testdir)_old
	test -z ${CMSSW_BASE} || test ! -d $(testdir) || mv $(testdir) $(testdir)_old
	test -z ${CMSSW_BASE} || cut='|!hbb_gen|hbb_gen' crombie plotdump test.root $(testdir)

clean:
	rm $(target) include/$(flatclass).h include/feedpanda.h include/checkrun.h $(effs) $(data) $(corrections) include/roccor/RoccoR.cc data/jec/jec.d
