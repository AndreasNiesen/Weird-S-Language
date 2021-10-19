"Compiler" for the Weird-S Language.<br>

<br>

Upcoming:
- save strings
- print strings
- (string comparisons?)

<br>

Possible operations so far:
 - END ("^"): End of the program. End(point) of a jump-based instruction, if second argument is given.
 - PUSH ("/"): Pushes second argument onto the stack. (currently only ints, will add support for characters later on).
 - ADD ("+"): Adds (and removes if POP is second argument) the last two entries on the stack and pushes the result.
 - SUB ("-"): Subtracts (and removes if POP is second argument) the last from the second to last entry on the stack and pushes the result.
 - DUMP ("."): Prints last entry on stack and either discards entry (POP) or keeps it (END).
 - POP ("\\"): Removes last entry from stack. If second argument is ID of a variable, saves popped value to variable.
 - GT (">"): Compares the last two stack entries (removing the last one) and jumping to ID given by second argument, if second to last > last.
 - EQ ("="): Compares the last two stack entries (removing the last one) and jumping to ID given by second argument, if second to last == last.
 - LT ("<") Compares the last two stack entries (removing the last one) and jumping to ID given by second argument, if second to last < last.
 - JMP ("!"): Unconditional jump to ID given by second argument.
 - RSRV ("("): Reserves place on stack for a variable with name given by second argument. (currently only saves a single int per ID - will be of variable size)
 - LOAD (")"): Pushes value in variable on the stack.

Single line comments are supported, marked by "#".

<br>

Simple example:
```
/ 35    # Pushes 35.
/ 34    # Pushes 34.
+ ^     # Adds 35 and 34. Pushes result.
/ 512   # Pushes 512.
/ 92    # Pushes 92.
- \     # Subtracts 92 from 512 (512 - 92), while removing both from the stack. Pushes result.
+ ^     # Adds 69 and 420 (having removed 512 and 92, second to last stack value is the result of the first addition). Pushes result.
. ^     # Prints 489 (result of previous addition).
^       # Ends Program.
```

<br>

Looping example (Fibonacci Numbers):
```
# Reserve variable(s)
( LoopCounter

# Push and Print initial Fibonacci-Values
/ 0
. ^
/ 1
. ^

# Prepare LoopCounter
/ 9

# Print + Calculate Fibonacci
^ FibLoop     # Endpoint of FibLoop jump.
\ LoopCounter # Load prepared value (9) into the LoopCounter variable (removing it from the stack).
+ ^
. ^
) LoopCounter # Pushes value stored at/in LoopCounter on the stack.
/ 1
- \
/ 0
> FibLoop     # If second to last stack-value (LoopCounter) > last stack-value (0) jump to FibLoop jump endpoint (removes 0 from the stack).
^
```
