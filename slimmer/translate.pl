#! /usr/bin/env perl

while (<>) {
    chomp;

    s/Jet/\[\]/;

    s/chEmEF/cef/;
    s/chHEF/chf/;
    s/neEmEF/nef/;
    s/neHEF/nhf/;
    s/mass/m/;
    s/nPVs/npv/;
    s/^is/\[\]_is/;

    print;
    eof || print ", ";
}
