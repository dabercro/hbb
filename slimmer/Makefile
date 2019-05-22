LDFLAGS = `root-config --glibs`
CFLAGS = `root-config --cflags`
installname = mkhbb
flatclass = hbbfile
regclass = regfile
corrections = data/corrections.d
effs = data/btag_effs.root
data = data/data.d

# These flags are needed for fastjet
external = `${PWD}/flags.sh`

vers = 013

pandainc = ${CMSSW_BASE}/src
pandalib = ${CMSSW_BASE}/lib/${SCRAM_ARCH}
target = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/testhbbtree
regression = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/regresstree
install = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/$(installname)
pandalibs = -lPandaTreeFramework -lPandaTreeObjects -lPandaTreeUtils
#testfile = /mnt/hadoop/scratch/dabercro/panda/$(vers)/*.root
testfile = /data/t3home000/dabercro/scratch/$(vers)/*.root

checker = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/findtree.py
thejq = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/jq
tarball = ${CMSSW_BASE}/condor.tgz

all: $(target) $(effs)

$(install): $(target)
	cp $(target) $(install)

$(tarball): $(install) $(checker) $(thejq) $(data) $(regression) $(effs)
	test -d ${CMSSW_BASE}/data || mkdir ${CMSSW_BASE}/data
	tar -czf $(tarball) --directory ${CMSSW_BASE} bin lib external data

$(checker): ${CROMBIEPATH}/scripts/findtree.py
	cp ${CROMBIEPATH}/scripts/findtree.py $(checker)

$(thejq): /home/dabercro/bin/jq
	cp /home/dabercro/bin/jq $(thejq)

$(data): data/*.csv data/*.txt data/*.xml data/*.root $(corrections) data/rcdata.2016.v3 data/jec/jec.d
	test -d ${CMSSW_BASE}/data && rm ${CMSSW_BASE}/data/*.pb ${CMSSW_BASE}/data/*.??? || mkdir ${CMSSW_BASE}/data
	cp data/*.csv data/*.txt data/*.xml data/*.root data/*.pb ${CMSSW_BASE}/data
	test ! -d ${CMSSW_BASE}/data/rcdata.2016.v3 || rm -r ${CMSSW_BASE}/data/rcdata.2016.v3
	cp -rH data/rcdata.2016.v3 ${CMSSW_BASE}/data
	test ! -d ${CMSSW_BASE}/data/jec || rm -r ${CMSSW_BASE}/data/jec
	mkdir ${CMSSW_BASE}/data/jec
	cp data/jec/*.txt ${CMSSW_BASE}/data/jec
	touch $(data)

$(target): slimmer.cpp include/*.h include/checkrun.h include/feedpanda.h include/$(flatclass).h \
	${CROMBIEPATH}/src/include/crombie/*.h \
	include/roccor/RoccoR.cc include/jecsys \
	${CMSSW_BASE}/lib/${SCRAM_ARCH}/*.so \
	${PWD}/flags.sh
	g++ -Iinclude -I$(pandainc) -I${CROMBIEPATH}/src/include \
	-I${CROMBIEPATH}/old \
	$(external) -lrt -g \
	$(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -lTMVA -o $(target) slimmer.cpp

$(regression): regression.cpp include/*.h include/feedregression.h include/$(regclass).h \
	${CROMBIEPATH}/src/include/crombie/*.h \
	include/roccor/RoccoR.cc include/jecsys \
	${CMSSW_BASE}/lib/${SCRAM_ARCH}/*.so \
	${PWD}/flags.sh
	g++ -Iinclude -I$(pandainc) -I${CROMBIEPATH}/src/include \
	$(external) -lrt -g \
	$(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -o $(regression) regression.cpp


include/jecsys:
	cd include; git clone https://github.com/miquork/jecsys.git; cd jecsys; git checkout Summer16_23Sep2016V4

include/roccor/RoccoR.cc:
	test -d include/roccor || mkdir include/roccor
	cd include/roccor; test ! -f roccor.2016.v3.tgz || rm roccor.2016.v3.tgz; wget http://t3serv001.mit.edu/~dabercro/slimmerfiles/180302/roccor.2016.v3.tgz; tar -xf roccor.2016.v3.tgz
	cd data; test -f rcdata.2016.v3 || ln -s ../include/roccor/rcdata.2016.v3 rcdata.2016.v3

include/checkrun.h: certs/* ${CROMBIEPATH}/scripts/goodruns.py
	crombie goodruns certs/* include/checkrun

data/jec/jec.d: data/jec/download.url
	cd data/jec; rm *.tar.gz; rm *.txt; wget -i download.url; for f in *.tar.gz; do tar -xvf $$f '*_AK4PFchs.txt' '*_AK8PFPuppi.txt'; done
	touch data/jec/jec.d

include/feedregression.h: include/[!f]*.h regression.cpp include/$(regclass).h ${CROMBIEPATH}/scripts/feedpanda.pl
	crombie feedpanda $(pandainc)/PandaTree/defs/panda.def \
	include/genjet.h include/regression.h include/regression.h include/$(regclass).h regression.cpp include/feedregression.h

include/feedpanda.h: include/[!f]*.h include/$(flatclass).h include/checkrun.h slimmer.cpp ${CROMBIEPATH}/scripts/feedpanda.pl
	crombie feedpanda $(pandainc)/PandaTree/defs/panda.def include/*.h slimmer.cpp include/feedpanda.h

include/$(regclass).h: $(regclass).txt ${CROMBIEPATH}/scripts/maketree.py $(corrections)
	crombie maketree $(regclass).txt
	mv $(regclass).h include/

include/$(flatclass).h: $(flatclass).txt ${CROMBIEPATH}/scripts/maketree.py $(corrections)
	crombie maketree $(flatclass).txt
	mv $(flatclass).h include/

$(effs): data/make_effs.py data/*eff.txt $(corrections)
	data/make_effs.py

$(corrections): data/downloads.url
	cd data; rm *.root; rm *.csv; rm *.xml; rm *.pb; wget -i downloads.url
	touch $(corrections)

.PHONY: clean test install reg regtest

reg: $(regression)

regtest: $(regression)
	test ! -f reg.root || rm reg.root
	input_dir='/test/pandaf/013/this' regresstree /data/t3home000/dabercro/scratch/TTToHadronic/661A1D00-21C9-634B-829E-48F95273CAC7.root reg.root

install: $(tarball)

testdir = ${HOME}/public_html/plots/testhbb_$(vers)

test: $(target) $(effs) data/jec/jec.d
	test -d old || mkdir old
	test -d new || mkdir new
	# Move old test.root to a place for comparison
	test ! -f test.root || mv test.root old
	# Don't keep around new test file that's garbage
	input_dir='/test/pandaf/$(vers)/this' testhbbtree $(testfile) test.root || rm test.root
	# Kill if test file generation didn't work
	test -f test.root
	cp test.root new
	crombie diff -b old new
	# Move plots, only if last test was valid
	test -z ${CMSSW_BASE} || test ! -d $(testdir)_old || rm -r $(testdir)_old
	test -z ${CMSSW_BASE} || test ! -d $(testdir) || mv $(testdir) $(testdir)_old
	test -z ${CMSSW_BASE} || cut='|!hbb_gen|hbb_gen' crombie plotdump test.root $(testdir)

clean:
	rm $(target) include/$(flatclass).h include/feedpanda.h include/checkrun.h $(effs) $(data) $(corrections) include/roccor/RoccoR.cc data/jec/jec.d $(regression) $(install)
