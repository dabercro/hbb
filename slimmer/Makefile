LDFLAGS = `root-config --glibs`
CFLAGS = `root-config --cflags`
exename = makehbbtree
flatclass = hbbfile
testfile = F6F880D2-ACBE-E611-B59D-0CC47A706D40.root
cmssw = ${CMSSW_BASE}

ifeq ($(cmssw), '')
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
endif

$(target): $(flatclass).h slimmer.cpp
	g++ -I$(pandainc) -I${CROMBIEPATH} $(LDFLAGS) $(CFLAGS) -L$(pandalib) $(pandalibs) -o $(target) slimmer.cpp

$(flatclass).h: hbbfile.txt ${CROMBIEPATH}/scripts/maketree.py
	crombie maketree $(flatclass).txt

.PHONY: clean test

test: $(target)
	test ! -f test.root || rm test.root
	DYLD_LIBRARY_PATH=$(pandalib) PATH=.:${PATH} $(target) $(testfile) test.root

clean:
	rm $(target) $(flatclass).h
