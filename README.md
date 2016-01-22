# pact

Give pact several PIDs. When one PID dies, pact will kill all provided PIDs.

Example usage:

    #!/bin/bash
    
    cmd1 &
    PID1=$!
    cmd2 --some-arg &
    PID2=$!
    
    ./pact $PID1 $PID2

##Advanced usage

pact also accepts a one character modifier, either M or K. M monitors the PID
without killing it, and K will kill the PID without monitoring it.

Example:

	#!/bin/bash
	(sleep 5)&
	PID1=$!
	(sleep 24h)&
	PID2=$!
	# If the shell dies, kill the other processes. If one of the
	# processes die, don't kill the shell.
	./pact $PID1 $PID2 M$$
	pause
