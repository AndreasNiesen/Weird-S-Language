"Compiler" for the Weird-S Language.<br>

<br>

Upcoming:
- Loops

<br>

Possible operations so far:
 - END ("^"): End of the program. (No Operation as second argument).<br>
 - PUSH ("/"): Pushes second argument onto the stack. (currently only ints, will add support for characters later on).<br>
 - ADD ("+"): Adds (and removes if POP is second argument) the last two entries on the stack and pushes the result.<br>
 - SUB ("-"): Subtracts (and removes if POP is second argument) the last from the second to last entry on the stack and pushes the result.<br>
 - DUMP ("."): Prints last entry on stack and either discards entry (POP) or keeps it (END).<br>
 - POP ("\\"): Removes last entry from stack.<br>
 - GT (">"): Compares the last two stack entries (removing the last one) and jumping to ID given by second argument, if second to last > last.
 - EQ ("="): Compares the last two stack entries (removing the last one) and jumping to ID given by second argument, if second to last == last.
 - LT ("<") Compares the last two stack entries (removing the last one) and jumping to ID given by second argument, if second to last < last.

Single line comments are supported, marked by "#".

<br>

Simple example program:
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

Branching example program:
```
/ 500   # Pushes 500.
/ 50    # Pushes 50.
= OUT   # If 500 equals 50 jump to OUT, marked by "^ OUT" (removes the 50 from the stack).
/ 500   # Pushes 500.
= IN    # If 500 equals 500 jump to IN, marked by "^ IN" (removes the last 500 from the stack).
/ 1     # Pushes 1.
. ^     # Prints 1.
^       # End of Program.
^ IN    # Jump point for the IN labeled comparison.
/ 2     # Pushes 2.
. ^     # Prints 2.
^       # End of Program.
^ OUT   # Jump point for the OUT labeled comparison.
/ -1    # Pushes -1.
. ^     # Prints -1.
^       # End of program.
```