"Compiler" for the Weird-S Language.<br>

<br>

Upcoming:
- Comments
- Substraction
- (Branching)
- (Loops)

<br>

Possible operations so far:
 - END ("-"): End of the program. (No Operation as second argument).<br>
 - PUSH ("/"): Pushes second argument onto the stack. (currently only ints, will add support for characters later on).<br>
 - ADD ("+"): Adds (and removes if POP is second argument) the last two entries on the stack and pushes the result.<br>
 - DUMP ("."): Prints last entry on stack and either discards entry (POP) or keeps it (END).<br>
 - POP ("\\"): Removes last entry from stack.<br>

<br>

Quick example program:
```
/ 35
/ 34
\+ -
/ 100
/ 320
\+ \
\+ -
. -
-
```

Explained:
```
/ 35    # Pushes 35.
/ 34    # Pushes 34.
\+ -    # Adds 35 and 34. Pushes result.
/ 100   # Pushes 100.
/ 320   # Pushes 320.
\+ \    # Adds 100 and 320, while removing them from the stack. Pushes result.
\+ -    # Adds 69 and 420 (having removed 100 and 320, second to last stack value is the result of the first addition). Pushes result.
. -     # Prints 489 (result of previous addition).
\-      # Ends Program.
```
