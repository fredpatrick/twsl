#!/bin/bash
cd /home/fredpatrick/wrk/twsl/bin
echo $LD_LIBRARY_PATH
source ./setup_twsl.sh
ps
./twslclient
