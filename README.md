# pact

Give pact several process IDs. When one process ID dies, pact will kill all
provided PIDs.

Example usage:

    #!/bin/bash
    
    cmd1 &
    PID1=$!
    cmd2 --some-arg &
    PID2=$!
    
    ./pact $PID1 $PID2

##Advanced usage

pact also accepts a one character modifier before the process ID.
It can be either M or K. M monitors the process without killing it, and 
K will not monitor the process, but it will kill the process if another
monitored process dies.

Example:

    #!/bin/bash
    
    (sleep 5)&
    PID1=$!
    (sleep 24h)&
    PID2=$!

    # If the shell dies, kill the other processes. If one of the
    # processes die, don't kill the shell.
    ./pact $PID1 $PID2 M$$
    
    # Pause, because otherwise the shell will immediately die and pact
    # will kill the subprocesses.
    pause
