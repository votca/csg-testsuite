#!/usr/bin/perl -W
# use strict;

# loading reference data & assigning variables
my @ref = read_ref();

# parsing and loading kmc_test output
system("tail test_kmc.dat -n 49 > test.dat");
my @tst = read_tst();

# checking that files have identical size

if (@ref != @tst){
  die "Reference and test file do not have the same size! MaxTimeKMC must be 1E-11.";
}

# assigning values from files to arrays

$i = 0;
foreach $line_ref (@ref){
  chomp($line_ref); # removes the \n characters
  ($t_ref[$i],$I_ref[$i])=split(' ',$line_ref);
  # print "$t_ref[$i] \n";
  $i++;
}
$j = 0;
foreach $line_tst (@tst){
  chomp($line_tst); # removes the \n characters
  ($t_tst[$j],$I_tst[$j])=split(' ',$line_tst);
  $j++;
}

# Calculating mean square deviation of currents

$msd = 0;
for ($count = 0; $count < $i; $count++){
  if($t_ref[$count] != $t_tst[$count]){
    die "Time values for test and reference are not equal! MaxTimeKMC must be 1E-11.";
  }
  $msd += ($I_ref[$count]-$I_tst[$count])*($I_ref[$count]-$I_tst[$count]);
  #print "$I_ref[$count] $I_tst[$count] $msd \n";
}

# checking if things went okay or not

if($msd < 1E-3){
  exit 0; # returns 0, if mean square deviation is small
}
else{
  exit 1; # returns 1, if mean square deviation is large
}

# routine to open reference file
sub read_ref {
        my( $ref_file ) = "reference.dat";
        # print "Opening file $ref_file for reading.\n";
        open(IN, "$ref_file") || die "cannot open $ref_file for reading: $!  ";
        @ref = <IN>;
        my $nlines = @ref;
        close(IN);
        #print "Number of lines in file 'reference.dat': $nlines \n";
        return @ref;
}

# routine to open kmc_test file
sub read_tst {
        my( $tst_file ) = "test.dat";
        # print "Opening file $tst_file for reading.\n";
        open(IN, "$tst_file") || die "cannot open $tst_file for reading: $!  ";
        @tst = <IN>;
        my $nlines = @tst;
        close(IN);
        #print "Number of lines in file 'test.dat': $nlines \n";
        return @tst;
}
