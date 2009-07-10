#!/usr/bin/perl
# Copyright (c) 2009 Jack Christopher Kastorff
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions, and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * The name Chris Kastorff may not be used to endorse or promote
#       products derived from this software without specific prior written
#       permission.

use warnings;
use strict;

sub extract_docs {
    my ($fn) = @_;
    open my $lf, "<", $fn or die $!;

    while ( my $line = <$lf> ) {
        chomp $line;

        if ( $line =~ /\s*\/\*\s*!doc:(.+)$/ ) {
            my $chunk = $1."\n";

            while ( $line = <$lf> ) {
                chomp $line;

                last if $line =~ /^\s*\*\/\s*$/;

                $line =~ s/^\s*\*//;
                $chunk .= $line."\n";
            }

            $chunk =~ s/\s+$//;

            print "$chunk\n\n";
        }
    }

    close $lf;
}

my @files = @ARGV;

for my $f ( @files ) {
    extract_docs($f);
}

