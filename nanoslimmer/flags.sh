#! /bin/sh

# CMSSW headers are here

echo -n "-I${CMSSW_FULL_RELEASE_BASE}/src "

# CMSSW libraries are here

echo -n "-L${CMSSW_FULL_RELEASE_BASE}/lib/${SCRAM_ARCH} "
echo -n "-L${CMSSW_RELEASE_BASE}/external/lib/${SCRAM_ARCH} "

# Need this from CMSSW

echo -n "-lFWCoreMessageLogger -lCondFormatsJetMETObjects -lJetMETCorrectionsModules "

# Get other flags from scram

cd $CMSSW_BASE

# These flags are needed for boost

echo -n "-I$(scram tool tag boost INCLUDE) "
echo -n "-L$(scram tool tag boost LIBDIR) "
echo -n "-l$(scram tool tag boost LIB | sed 's/ / -l/g') -lboost_filesystem "
