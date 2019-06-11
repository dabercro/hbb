LDFLAGS = `root-config --glibs`
CFLAGS = `root-config --cflags`

pandalibs=-lPandaTreeFramework -lPandaTreeObjects -lPandaTreeUtils

example: example.h slimmer.cpp
	g++ $(LDFLAGS) $(CFLAGS) -I${CMSSW_BASE}/src -L${CMSSW_BASE}/lib/${SCRAM_ARCH} $(pandalibs) -o example slimmer.cpp

example.h: example.txt
	./maketree.py example.txt

