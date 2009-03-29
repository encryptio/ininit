#!/usr/bin/perl
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

