"Compiler" for the Weird-S Language.<br>

<br>

Upcoming:
- Comments
- (Branching)
- (Loops)

<br>

Possible operations so far:
 - END ("^"): End of the program. (No Operation as second argument).<br>
 - PUSH ("/"): Pushes second argument onto the stack. (currently only ints, will add support for characters later on).<br>
 - ADD ("+"): Adds (and removes if POP is second argument) the last two entries on the stack and pushes the result.<br>
 - SUB ("-"): Subtracts (and removes if POP is second argument) the last from the second to last entry on the stack and pushes the result.<br>
 - DUMP ("."): Prints last entry on stack and either discards entry (POP) or keeps it (END).<br>
 - POP ("\\"): Removes last entry from stack.<br>

<br>

Quick example program:
```
/ 35
/ 34
+ ^
/ 512
/ 92
- \
+ ^
. ^
^
```

Explained:
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
