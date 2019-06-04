#! /bin/sh

# CMSSW headers are here

echo -n "-I${CMSSW_FULL_RELEASE_BASE}/src "

# CMSSW libraries are here

echo -n "-L${CMSSW_FULL_RELEASE_BASE}/lib/${SCRAM_ARCH} "
echo -n "-L${CMSSW_RELEASE_BASE}/external/lib/${SCRAM_ARCH} "

# Need this from CMSSW

echo -n "-lFWCoreMessageLogger -lCondFormatsJetMETObjects -lCondFormatsBTauObjects -lCondToolsBTau "

# Get other flags from scram

cd $CMSSW_BASE

# These flags are needed for boost

echo -n "-I$(scram tool tag boost INCLUDE) "
echo -n "-L$(scram tool tag boost LIBDIR) "
echo -n "-l$(scram tool tag boost LIB | sed 's/ / -l/g') "

# These flags are needed for fastjet

echo -n "-I$(scram tool tag fastjet INCLUDE) "
echo -n "-L$(scram tool tag fastjet LIBDIR) "
echo -n "-l$(scram tool tag fastjet LIB | sed 's/ / -l/g') "

# These flags are needed for tensorflow

echo -n "-DEIGEN_DONT_PARALLELIZE "
echo -n "-I$(scram tool tag tensorflow INCLUDE) "
echo -n "-L$(scram tool tag tensorflow LIBDIR) "
echo -n "-l$(scram tool tag tensorflow-cc LIB | sed 's/ / -l/g') "
echo -n "-l$(scram tool tag tensorflow-framework LIB | sed 's/ / -l/g') "
echo -n "-I$(scram tool tag protobuf INCLUDE) "
echo -n "-L$(scram tool tag protobuf LIBDIR) "
echo -n "-l$(scram tool tag protobuf LIB | sed 's/ / -l/g') "
echo -n "-I$(scram tool tag eigen INCLUDE) "
