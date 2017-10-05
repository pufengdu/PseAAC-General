#!/bin/sh
echo -n "Checking perl... "
if which perl > /dev/null ; then
	echo "OK"
else
	echo "STOP" ;
	exit
fi
echo -n "Checking bmc... "
if [ -e "../bmc/bmc" ] ; then
	echo "OK"
else
	echo "STOP" ;
	exit
fi
echo -n "Checking gpalist.pl... "
if [ -e "./gpalist.pl" ] ; then
	echo "OK"
else
	echo "STOP" ;
	exit
fi
echo "Preparing for online update of BEMs..."
datestr=`date +%m%d%Y`
base_url="http://www.uniprot.org/uniprot/?query="
# base_url="http://www.expasy.uniprot.org/uniprot/?query="
# default_query="reviewed:yes+AND+organism:4932&format=tab&columns=id,entry%20name,protein%20names,go-id,interpro&compress=yes"
default_query="reviewed:yes&format=tab&columns=id,entry%20name,protein%20names,go-id,interpro&compress=yes"
uniprot_url=$base_url$default_query
option_col=0
bemfilename="uniprot-"
if [ $1 = "GO" -o $1 = "go" ] ; then
	option_col=3;
	bemfilename="uniprot-go-"$datestr
elif [ $1 = "ipr" -o $1 = "IPR" ] ; then
	option_col=4;
	bemfilename="uniprot-ipr-"$datestr
fi
if [ $option_col -eq 0 ] ; then
	echo "Error in parameter "$1 ;
	exit
fi
echo "Downloading data from UniProtKB..."
tempdbname=`mktemp uniprot-$$-XXXXXXXX`
rm -f $tempdbname
echo "Temporary filename: "$tempdbname".tab.gz"
tempzipname=$tempdbname".tab.gz"
temptabname=$tempdbname".tab"
echo "Uniprot URL is: "$uniprot_url
wget -O $tempzipname $uniprot_url
gunzip $tempzipname
echo "Building BEMs for PseAAC-Builder 3.0..."
echo "Calling GPALIST for first round compile..."
perl ./gpalist.pl $temptabname 1 $option_col
echo "Calling Binary Module Compiler (BMC) program for second round compile..."
../bmc/bmc -o $temptabname-tmp -f $temptabname-gol -p $temptabname-prl -m $temptabname-mas
echo "Compressing BEM..."
gzip $temptabname-tmp
mv $temptabname-tmp.gz $bemfilename.bem
rm $temptabname-gol $temptabname-prl $temptabname-mas # $temptabname
mv $temptabname "uniprot-"$datestr".tab"
tar -czvf "uniprot-"$datestr".tar.gz" "uniprot-"$datestr".tab"
rm "uniprot-"$datestr".tab"

