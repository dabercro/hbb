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
smear = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/smeartree
install = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/$(installname)
pandalibs = -lPandaTreeFramework -lPandaTreeObjects -lPandaTreeUtils
#testfile = /mnt/hadoop/scratch/dabercro/panda/$(vers)/*.root
testfile = /data/t3home000/dabercro/scratch/$(vers)/*.root

thejq = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/jq
tarball = ${CMSSW_BASE}/condor.tgz

all: $(target) $(effs)

$(install): $(target) $(effs)
	cp $(target) $(install)

$(tarball): $(install) $(thejq) $(data) $(regression) $(effs) $(smear)
	test -d ${CMSSW_BASE}/data || mkdir ${CMSSW_BASE}/data
	tar -czf $(tarball) --directory ${CMSSW_BASE} bin lib external data

$(thejq): /home/dabercro/bin/jq
	cp /home/dabercro/bin/jq $(thejq)

$(data): data/*.csv data/*.txt data/*.xml data/*.root $(corrections) data/roccor data/jec/*.txt
	test -d ${CMSSW_BASE}/data && rm ${CMSSW_BASE}/data/*.pb ${CMSSW_BASE}/data/*.??? || mkdir ${CMSSW_BASE}/data
	cp data/*.csv data/*.txt data/*.xml data/*.root data/*.pb ${CMSSW_BASE}/data
	test ! -d ${CMSSW_BASE}/data/roccor || rm -r ${CMSSW_BASE}/data/roccor
	cp -rH data/roccor ${CMSSW_BASE}/data
	test ! -d ${CMSSW_BASE}/data/jec || rm -r ${CMSSW_BASE}/data/jec
	mkdir ${CMSSW_BASE}/data/jec
	cp data/jec/*.txt ${CMSSW_BASE}/data/jec
	touch $(data)

$(target): src/slimmer.cpp include/[!f]*.h include/checkrun.h include/feedpanda.h include/$(flatclass).h \
	${CROMBIEPATH}/src/include/crombie/*.h ${CROMBIEPATH}/old/SkimmingTools/interface/*.h \
	include/roccor/RoccoR.cc include/jecsys \
	${CMSSW_BASE}/lib/${SCRAM_ARCH}/*.so \
	${PWD}/flags.sh
	g++ -Iinclude -I$(pandainc) -I${CROMBIEPATH}/src/include \
	-I${CROMBIEPATH}/old \
	$(external) -lrt -g \
	$(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -lTMVA -o $(target) src/slimmer.cpp

$(regression): src/regression.cpp include/[!f]*.h include/feedregression.h include/$(regclass).h \
	${CROMBIEPATH}/src/include/crombie/*.h ${CROMBIEPATH}/old/SkimmingTools/interface/*.h \
	include/roccor/RoccoR.cc include/jecsys \
	${CMSSW_BASE}/lib/${SCRAM_ARCH}/*.so \
	${PWD}/flags.sh
	g++ -Iinclude -I$(pandainc) -I${CROMBIEPATH}/src/include \
	$(external) -lboost_filesystem -lrt -g \
	$(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -lTMVA -o $(regression) src/regression.cpp

$(smear): src/smear.cpp include/[!f]*.h include/feedsmear.h include/smearfile.h include/checkrun.h \
	${CROMBIEPATH}/src/include/crombie/*.h ${CROMBIEPATH}/old/SkimmingTools/interface/*.h \
	include/roccor/RoccoR.cc include/jecsys \
	${CMSSW_BASE}/lib/${SCRAM_ARCH}/*.so \
	${PWD}/flags.sh
	g++ -Iinclude -I$(pandainc) -I${CROMBIEPATH}/src/include \
	-I${CROMBIEPATH}/old \
	$(external) -lboost_filesystem -lrt -g \
	$(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -lTMVA -o $(smear) src/smear.cpp


include/jecsys:
	cd include; git clone https://github.com/miquork/jecsys.git; cd jecsys; git checkout Summer16_23Sep2016V4

roccortar=roccor.Run2.v3

include/roccor/RoccoR.cc:
	rm -rf include/roccor
	mkdir include/roccor
	cd include/roccor; test ! -f $(roccortar).tar || rm $(roccortar).tar; wget http://t3serv001.mit.edu/~dabercro/slimmerfiles/190909/$(roccortar).tar; tar -xf $(roccortar).tar; mv roccor.Run2.v3/* .
	cd data; test -f roccor || ln -s ../include/roccor/ roccor

include/checkrun.h: certs/* ${CROMBIEPATH}/scripts/goodruns.py
	crombie goodruns certs/* include/checkrun

data/jec/jec.d: data/jec/download.url
	cp /data/t3home000/dabercro/tars/hbb/190819_puppijets_mc2018_013/data/jec/Winter19*.txt data/jec
	cd data/jec; rm *.tgz; wget -i download.url; cat download.url | xargs -n1 basename | xargs -n1 tar -xf
	touch data/jec/jec.d

include/feedregression.h: include/[!f]*.h src/regression.cpp include/$(regclass).h ${CROMBIEPATH}/scripts/feedpanda.pl
	crombie feedpanda $(pandainc)/PandaTree/defs/panda.def include src/regression.cpp include/feedregression.h

include/feedsmear.h: include/[!f]*.h src/smear.cpp include/smearfile.h ${CROMBIEPATH}/scripts/feedpanda.pl
	crombie feedpanda $(pandainc)/PandaTree/defs/panda.def include src/smear.cpp include/feedsmear.h

include/feedpanda.h: include/[!f]*.h include/$(flatclass).h include/checkrun.h src/slimmer.cpp ${CROMBIEPATH}/scripts/feedpanda.pl
	crombie feedpanda $(pandainc)/PandaTree/defs/panda.def include src/slimmer.cpp include/feedpanda.h

include/$(regclass).h: treedefs/$(regclass).txt ${CROMBIEPATH}/scripts/maketree.py $(corrections) treedefs/regression.txt treedefs/regeval.txt
	crombie maketree treedefs/$(regclass).txt
	mv $(regclass).h include/

include/$(flatclass).h: treedefs/$(flatclass).txt ${CROMBIEPATH}/scripts/maketree.py $(corrections) translate.pl \
	treedefs/regression.txt treedefs/regeval.txt treedefs/classeval.txt
	crombie maketree treedefs/$(flatclass).txt
	mv $(flatclass).h include/

include/smearfile.h: treedefs/smearfile.txt ${CROMBIEPATH}/scripts/maketree.py $(corrections) translate.pl treedefs/regression.txt treedefs/regeval.txt
	crombie maketree treedefs/smearfile.txt
	mv smearfile.h include/


$(smear): 

$(effs): data/make_effs.py data/*eff.txt $(corrections)
	data/make_effs.py

$(corrections): data/downloads.url
	cd data; rm *.root; rm *.csv; rm *.xml; rm *.pb; wget -i downloads.url
	touch $(corrections)

.PHONY: clean test install reg regtest smear

reg: $(regression)

regtestdir = ${HOME}/public_html/plots/testreg_$(vers)

regtest: $(regression) $(effs)
	test ! -f reg.root || rm reg.root
	input_dir='/test/pandaf/013/this' regresstree /data/t3home000/dabercro/scratch/TTToHadronic/661A1D00-21C9-634B-829E-48F95273CAC7.root reg.root
	test -z ${CMSSW_BASE} || test ! -d $(regtestdir)_old || rm -r $(regtestdir)_old
	test -z ${CMSSW_BASE} || test ! -d $(regtestdir) || mv $(regtestdir) $(regtestdir)_old
	test -z ${CMSSW_BASE} || crombie plotdump reg.root $(regtestdir)

smear: $(smear)

smeartestdir = ${HOME}/public_html/plots/testsmear_$(vers)

smeartest: $(smear) $(effs)
	test ! -f smear.root || rm smear.root
	input_dir='/test/pandaf/013/this' smeartree /data/t3home000/dabercro/scratch/DYJetsToLL_M-50/*.root smear.root
	test -z ${CMSSW_BASE} || test ! -d $(smeartestdir)_old || rm -r $(smeartestdir)_old
	test -z ${CMSSW_BASE} || test ! -d $(smeartestdir) || mv $(smeartestdir) $(smeartestdir)_old
	test -z ${CMSSW_BASE} || crombie plotdump smear.root $(smeartestdir)

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
	rm $(target) include/$(flatclass).h include/feedpanda.h include/smearfile.h include/checkrun.h $(effs) $(data) $(corrections) include/roccor/RoccoR.cc data/jec/jec.d $(regression) $(install) $(smear)
