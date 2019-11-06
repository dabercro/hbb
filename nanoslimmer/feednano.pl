#! /usr/bin/env perl

use strict;
use warnings;

use v5.010;
use Data::Dumper;

# Location of Panda definitions
my $def_file = shift @ARGV;
my @source;
my @branches;

my %type_branches;
my $type;

my %event_types;

open(my $handle, '<', $def_file);

for (<$handle>) {
    if (/[\<\[\{](\w+)(\>(\w+))?(\<(\w+))?/) {
        $type = $1;
        if ($3) {
            push @{$type_branches{$type}}, @{$type_branches{$3}};
        }
        if ($5) {
            push @{$type_branches{$type}}, @{$type_branches{$5}};
        }
    }

    # Add branches for each type
    if (/^(\w+)\/\w$/) {
        push @{$type_branches{$type}}, $1
    }

    elsif (/^(\w+)\/(\w+)/) {
        $event_types{$1} = $2;
    }

}

close $handle;

my @primary;    # Primary branches
my @secondary;  # Secondary branches
my @possible;   # Possible secondary branches

my @full;

for (@ARGV) {
    open(my $handle, '<', $_);

    for (<$handle>) {
        if (/\be(vent)?(\.|->)(\w+)(?!\w*\()/) {
            push @primary, $3;
        }
    }

    close $handle;
}

sub uniq_sort {
    my %seen;
    return sort(grep {! $seen{$_}++ } @_);
}

for (uniq_sort @primary) {
    my $check_type = $event_types{$_};
    if ($check_type) {
        if ($check_type =~ /(\w+)Collection/) {
            $check_type = $1;
            $event_types{$_} = $1;
        }
        push @possible, @{$type_branches{$check_type}};
    }

}

my %poss_hash = map { $_ => 1 } @possible;

for (@ARGV) {
    open(my $handle, '<', $_);

    for (<$handle>) {
        if (/\b\w+\.(\w+)(?!\w*\()/) {
            if ($poss_hash{$1}) {
                push @secondary, $1;
            }
        }
    }

    close $handle;
}


foreach my $obj (@primary) {

    if ($event_types{$obj}) {

        my %look = map { $_ => 1 } @{$type_branches{$event_types{$obj}}};

        for (@secondary) {
            if ($look{$_}) {
                push @full, "${obj}_$_";
            }
        }
    }
    else {
        push @full, $obj;
    }
}


print Dumper uniq_sort @full;

