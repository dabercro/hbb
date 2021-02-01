#! /usr/bin/env perl

use strict;
use warnings;

while (<>) {
    my $file = $_;

    $file =~ m=^/store/= || die 'Please begin file name with "/store/"';

    my @split = split('/', $file);

    @split >= 7 || die 'File name not long enough to generate a dataset name';

    print "/$split[4]/$split[3]-$split[6]/$split[5]\n";
}
