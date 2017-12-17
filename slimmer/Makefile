LDFLAGS = `root-config --glibs`
CFLAGS = `root-config --cflags`
exename = makehbbtree
flatclass = hbbfile
testfile = F6F880D2-ACBE-E611-B59D-0CC47A706D40.root

ifeq ($(origin CMSSW_BASE), undefined)
	pandainc = ../..
	pandalib = ../../PandaTree/lib
	pandalibs = -lpanda
	target = $(exename)
else
	pandainc = ${CMSSW_BASE}/src
	pandalib = ${CMSSW_BASE}/lib/${SCRAM_ARCH}
	target =  ${CMSSW_BASE}/bin/${SCRAM_ARCH}/$(exename)
	pandalibs = -lPandaTreeFramework -lPandaTreeObjects -lPandaTreeUtils
	testfile = /mnt/hadoop/scratch/jenkins/panda/dabercro/PandaProd/jenkins-clean/F6F880D2-ACBE-E611-B59D-0CC47A706D40.root

	checker = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/findtree.py
	thejq = ${CMSSW_BASE}/bin/${SCRAM_ARCH}/jq
	tarball = ${CMSSW_BASE}/condor.tgz

$(tarball): $(target) $(checker) $(thejq)
	tar -czf $(tarball) --directory ${CMSSW_BASE} bin lib external

$(checker): ${CROMBIEPATH}/scripts/findtree.py
	cp ${CROMBIEPATH}/scripts/findtree.py $(checker)

$(thejq): /home/dabercro/bin/jq
	cp /home/dabercro/bin/jq $(thejq)

endif


$(target): $(flatclass).h slimmer.cpp ${CROMBIEPATH}/SkimmingTools/interface/StoreParticles.h feedpanda.h
	g++ -I$(pandainc) -I${CROMBIEPATH} $(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -o $(target) slimmer.cpp

feedpanda.h: $(flatclass).h slimmer.cpp
	crombie feedpanda $(pandainc)/PandaTree/defs/panda.def $(flatclass).h slimmer.cpp feedpanda.h

$(flatclass).h: hbbfile.txt ${CROMBIEPATH}/scripts/maketree.py
	crombie maketree $(flatclass).txt

.PHONY: clean test

test: $(target)
	test ! -f test.root || rm test.root
	DYLD_LIBRARY_PATH=$(pandalib) PATH=.:${PATH} $(target) $(testfile) test.root

clean:
	rm $(target) $(flatclass).h feedpanda.h
