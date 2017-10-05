#!/bin/perl
# This script generate the intermediete files for making the binary module of pseaac-builder
# Usage: gpalist.pl UNIPROT_DOWNLOADED_TABLE col_for_pr_id col_for_go_id
#	The UNIPROT_DOWNLOADED_TABLE should be a table format text (TSV) file downloaded from Uniprot query system
#	col_for_pr_id is the column number containing protein entry name or accession number
#	col_for_go_id is the column number containing protein go id list. The GO ids in this column should be seperated by ';'
#	
use strict;
my $srcFile = $ARGV[0];
my $seqidcol = $ARGV[1];# 1
my $goidcol = $ARGV[2];	# 4
my %golist = ();	# Hash for ordering Gene Ontology IDs
my %prlist = ();	# Hash for ordering Protein IDs
my %assocList = ();	# Hash for recording the Protein->GO relations
my $cntPr = 0;	# Number of readed proteins
my $cntGo = 0;	# Number of parsed GO IDs
# First round scanning...
open (SRC_FILE, "<".$srcFile) or die "GPALIST ABORT: Error opening file $srcFile\n";
print STDERR "GPALIST INFO: Reading from $srcFile...\n";
my $tmpLine = <SRC_FILE>;
while (my $tmpLine = <SRC_FILE>)
{
	chomp($tmpLine);
	my @tmpWords = split(/\t/,$tmpLine);
        my $curPrID = $tmpWords[$seqidcol];
        $curPrID =~ s/^\s+|\s+$//g;	# Trimming the spaces on parsed fields of protein IDs
	# Check whether the protein id is unique, if not, give a warning message
        if (!(exists $prlist{$curPrID}))
        {
		$prlist{$curPrID}=$cntPr;
		$cntPr ++;
		$assocList{$curPrID}=$tmpWords[$goidcol];
        }
        else
        {
		print STDERR "GPALIST WARNING: Duplicate protein names discovered for $curPrID on line $cntPr. \n\t-It also appeared on line $prlist{$curPrID}\n";
		# Joining the dulicate record of gene ontology
		$assocList{$curPrID}=$assocList{$curPrID}.";".$tmpWords[$goidcol];
        }
	# Split the GO IDs for generating GO lists
	my @goids = split(/;/,$tmpWords[$goidcol]);
	my $cntcurgoids = $#goids + 1;
	for (my $i = 0 ; $i < $cntcurgoids ; $i++)
	{
		$goids[$i] =~ s/^\s+|\s+$//g;	#Trimming the GO IDs
		if ($goids[$i] eq "")
		{
			next;
		}
                if(!exists $golist{$goids[$i]})
                {
			$golist{$goids[$i]} = $cntGo;
			$cntGo++;
                }
	}
}
close (SRC_FILE);	# Data Loading finished
# Compiling the table to intermediate format file
# Intermediate files are three files containing the list for GO, PR and ASSOC seperately
my $goFN = $srcFile."-gol";	# GO List
my $prFN = $srcFile."-prl";	# PR List
my $maFN = $srcFile."-mas";	# Assiciation Matrix
print STDERR "GPALIST INFO: First stage compiling...\n";
# Writing the GO List
# The format is CSV, first field = Total number of GO
open(GO_LIST, ">".$goFN);
my %inversGOList = reverse %golist;
print GO_LIST $cntGo;
for (my $i = 0 ; $i < $cntGo ; $i ++)
{
	print GO_LIST ",".$inversGOList{$i};
}
print GO_LIST "\n";
close(GO_LIST);
#Writing the PR List and ASSOC List
open(PR_LIST, ">".$prFN);
open(MA_LIST, ">".$maFN);
my %inversePrList = reverse %prlist;	# Reverse indices for coverting protein to orders
my $cumCoord = 0;	# Cummulative coordinate of the corresponding protein in the ASSOC List
my $totalNumbers = 0;	# Counting the total number of fields in the ASSOC List
print PR_LIST $cntPr;	# The first field of PR List = The number of all PR IDs
print MA_LIST "-1";	# The guardian field of ASSOC List
# Write the data for every protein
for (my $i = 0; $i < $cntPr ; $i ++)
{
	my $curPrName = $inversePrList{$i};	# Retrieving the PR ID according to the order
	my $curGOS = $assocList{$curPrName};	# Get the GO ID List for this protein, the GO ID List was seperated by ';'
	my @goWords = split(/;/,$curGOS);	# Split the GO ID List for this protein
	my $cntCurGOS = $#goWords + 1;
	# Writing the ASSOC List
	my $tmpCntEff = 0;
	for (my $j = 0 ; $j < $cntCurGOS ; $j ++)
	{
		$goWords[$j] =~ s/^\s+|\s+$//g;	# Trimming the GO ID
		if ($goWords[$j] eq "")
		{
			next;
		}
		print MA_LIST ",".$golist{$goWords[$j]};	# Convert the GO ID to its order in GO List and write into ASSOC
		$totalNumbers ++;	# Compute the total number of writtern ASSOC
		$tmpCntEff ++;
	}
	print PR_LIST ",".$curPrName.",".$cumCoord.",".$tmpCntEff;	# Write the coordinate of this protein in ASSOC List
	$cumCoord += $tmpCntEff;	# Adjust for the next protein


}
print MA_LIST ",".$totalNumbers."\n";	# Last field of ASSOC List
print PR_LIST "\n";	# Last fo PR List
close (MA_LIST);	# Finishing the ASSOC List
close (PR_LIST);	# Finishing the PR List
print STDERR "GPALIST INFO: First stage compile finished.\n"
