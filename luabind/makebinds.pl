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

sub DEBUG { 0 }

my $registerdefs = '';

sub make_bindings {
    my ($fn) = @_;
    print STDERR "make_bindings: $fn\n" if DEBUG;
    open my $lf, "<", $fn or die $!;

    my $thisfile = 0;

    while ( my $line = <$lf> ) {
        chomp $line;
        if ( $line =~ /!lua:(\S+)\s+->\s*(.+)$/ ) {
            my ($luafnname, $cdef) = ($1, $2);

            print STDERR "found $luafnname -> $cdef\n" if DEBUG;

            my ($cname, @args) = parse_cdef($cdef);

            if ( $thisfile++ == 0 ) {
                print "/*** Automatically generated from makebinds.pl for $fn ***/\n";
                print "#include \"$fn\"\n\n";
            }

            print "static int bind_$luafnname(lua_State *lst) {\n";

            for my $ar ( @args ) {
                if ( $ar->[0] eq 'numvar' ) {
                    print "    float $ar->[1];\n";
                } elsif ( $ar->[0] eq 'signal' ) {
                    print "    float *$ar->[1];\n";
                } elsif ( $ar->[0] eq 'string' ) {
                    print "    char *$ar->[1];\n";
                } elsif ( $ar->[0] eq 'bool' ) {
                    print "    int $ar->[1];\n";
                } elsif ( $ar->[0] eq 'int' ) {
                    print "    int $ar->[1];\n";
                } else { die }
            }

            print "    void *ret;\n\n";

            for my $i ( 0 .. $#args ) {
                my $argno = $i+1;
                my $ar = $args[$i];
                if ( $ar->[0] eq 'numvar' ) {
                    print "    $ar->[1] = (float) luaL_checknumber(lst, $argno);\n\n";
                } elsif ( $ar->[0] eq 'signal' ) {
                    print "    if ( ($ar->[1] = (float *) lua_touserdata(lst, $argno)) == NULL ) {\n";
                    print "        if ( ($ar->[1] = malloc(sizeof(float))) == NULL )\n";
                    print "            die(\"bind_$luafnname: couldn't malloc $ar->[1]\");\n";
                    print "\n";
                    print "        *$ar->[1] = (float) luaL_checknumber(lst, $argno);\n";
                    print "    }\n\n";
                } elsif ( $ar->[0] eq 'string' ) {
                    print "    $ar->[1] = (char *) luaL_checkstring(lst, $argno);\n\n";
                } elsif ( $ar->[0] eq 'bool' ) {
                    print "    $ar->[1] = lua_toboolean(lst, $argno) ? 1 : 0;\n\n";
                } elsif ( $ar->[0] eq 'int' ) {
                    print "    $ar->[1] = (int) lua_tointeger(lst, $argno);\n\n";
                } else { die }
            }

            print "    ret = (void *) $cname(".join(", ", map $_->[1], @args).");\n";
            print "    lua_pushlightuserdata(lst, ret);\n";
            print "    return 1;\n";
            print "}\n\n";

            $registerdefs .= "lua_register(lst, \"$luafnname\", bind_$luafnname);\n";
        }
    }

    close $lf;
}

sub parse_cdef {
    my ($cdef) = @_;

    if ( $cdef =~ /^\s*([a-z_]+)\(\s*\)\s*$/i ) {
        return $1; # no arguments
    }

    $cdef =~ /^\s*([a-z_]+)\s*\((.+)\)\s*$/i
        or die "Couldn't parse cdef '$cdef'";
    my ($fname, $args) = ($1, $2);

    my @argdefs = ();
    for my $ar ( split /,/, $args ) {
        $ar =~ s/^\s+//;
        $ar =~ s/\s+$//;

        if ( $ar =~ /^float\s*([a-z_]+)$/ ) {
            push @argdefs, ['numvar', $1];
        } elsif ( $ar =~ /^float\s*\*\s*([a-z_]+)$/ ) {
            push @argdefs, ['signal', $1];
        } elsif ( $ar =~ /^char\s*\*\s*([a-z_]+)$/ ) {
            push @argdefs, ['string', $1];
        } elsif ( $ar =~ /^bool\s+([a-z_]+)$/ ) {
            push @argdefs, ['bool', $1];
        } elsif ( $ar =~ /^int\s+([a-z_])+$/ ) {
            push @argdefs, ['int', $1];
        } else {
            die "Don't know how to deal with cdef argument '$ar'";
        }
    }

    return ($fname, @argdefs);
}

sub read_builtins {
    my ($fn) = @_;
    print STDERR "read_builtins: $fn\n" if DEBUG;
    open my $lf, "<", $fn or die $!;

    while ( my $line = <$lf> ) {
        chomp $line;
        if ( $line =~ /!lua:(\S+)\s+->\s*(\S+)\s*$/ ) {
            my ($luafnname, $cname) = ($1, $2);

            print STDERR "found builtin binding $luafnname -> $cname\n" if DEBUG;

            $registerdefs .= "lua_register(lst, \"$luafnname\", $cname);\n";
        }

        print "$line\n";
    }

    close $lf;
}

###

my $type;
while ( @ARGV ) {
    my $f = shift @ARGV;
    if ( $f eq '-header' ) {
        $type = 'header';
    } elsif ( $f eq '-builtins' ) {
        $type = 'builtins';
    } else {
        die if not defined $type;
        if ( $type eq 'header' ) {
            make_bindings($f);
        } elsif ( $type eq 'builtins' ) {
            read_builtins($f);
        } else { die }
    }
}

print "void bind_ininit_lua_fns(lua_State *lst) {\n";
print $registerdefs;
print "}\n\n";

