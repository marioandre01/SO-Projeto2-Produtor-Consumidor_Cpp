#!/bin/bash
 
export myrun_DIR=`pwd`
export myrun_TEST=/tmp/test
 
rm -rf $myrun_TEST
mkdir $myrun_TEST
cd $myrun_TEST
$myrun_DIR/main
cd $myrun_DIR
