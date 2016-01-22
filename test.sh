#!/bin/bash

echo "shell at $$"

(sleep 24h)&
PID1=$!
echo "sleep1 at $PID1"
(sleep 1)&
PID2=$!
echo "sleep2 at $PID2"

./pact $PID1 $PID2
PACT_PID=$!
echo "pact at $PACT_PID"


ps -A | grep $PID1
ps -A | grep $PID2
ps -A | grep $PACT_PID

#read

#ps -A | grep $PID1
#ps -A | grep $PID2
#ps -A | grep $PACT_PID
