# pact

Give pact several PIDs. When one PID dies, pact will kill all provided PIDs.

Example usage:

    
    cmd1 &
    PID1=$!
    cmd2 --some-arg &
    PID2=$!
    
    pact $PID1 $PID2
