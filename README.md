"Compiler" for the Weird-S Language.<br>

<br>

Upcoming:
- save strings
- print strings
- (string comparisons?)

<br>

Possible operations so far:
<table>
 <tr>
  <th>Operation</th>
  <th>Incode Symbol</th>
  <th>
   Parameters<br>
   [Optional]
  </th>
  <th>Description</th>
 </tr>
 <tr>
  <th>END</th>
  <th>^</th>
  <th>[string]</th>
  <th>If no argument is given, end of program. Otherwise endpoint/jump-to-point for given ID.</th>
 </tr>
 <tr>
  <th>PUSH</th>
  <th>/</th>
  <th>int</th>
  <th>Pushes argument onto the stack.</th>
 </tr>
 <tr>
  <th>ADD</th>
  <th>+</th>
  <th>END or POP</th>
  <th>
   Adds the second to last to the last value on the stack.<br>
   If the argument is POP, removes both operands from the stack, before pushing the result.<br>
   If the argument is END, leaves the stack and pushes the result.<br>
  </th>
 </tr>
 <tr>
  <th>SUB</th>
  <th>-</th>
  <th>END or POP</th>
  <th>
   Subtracts the last from the second to last value on the stack.<br>
   If the argument is POP, removes both operands from the stack, before pushing the result.<br>
   If the argument is END, leaves the stack and pushes the result.<br>
  </th>
 </tr>
 <tr>
  <th>DUMP</th>
  <th>.</th>
  <th>END or POP</th>
  <th>Prints the last entry on the stack and either discards it (POP) or keeps it (END).</th>
 </tr>
 <tr>
  <th>POP</th>
  <th>\</th>
  <th>[string]</th>
  <th>Removes the last entry from the stack. If a reserved variable ID is given as argument, saves the popped value into it.</th>
 </tr>
 <tr>
  <th>GT</th>
  <th>></th>
  <th>string</th>
  <th>If second to last > last stack value, jump to endpoint/jump-to-point given in the argument.</th>
 </tr>
 <tr>
  <th>EQ</th>
  <th>=</th>
  <th>string</th>
  <th>If second to last == last stack value, jump to endpoint/jump-to-point given in the argument.</th>
 </tr>
 <tr>
  <th>LT</th>
  <th><</th>
  <th>string</th>
  <th>If second to last < last stack value, jump to endpoint/jump-to-point given in the argument.</th>
 </tr>
 <tr>
  <th>JMP</th>
  <th>!</th>
  <th>string</th>
  <th>Unconditional jump to endpoint/jump-to-point given in the argument.</th>
 </tr>
 <tr>
  <th>RSRV</th>
  <th>(</th>
  <th>string</th>
  <th>Reserves place on the stack for the variable with a name given in the argument.</th>
 </tr>
 <tr>
  <th>LOAD</th>
  <th>)</th>
  <th>string</th>
  <th>Pushes value in the variable of the (arguments) given name on the stack.</th>
 </tr>
</table>

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
^ FibLoop      # Endpoint of FibLoop jump.
\ LoopCounter  # Load prepared value (9) into the LoopCounter variable (removing it from the stack).
+ ^
. ^
) LoopCounter  # Pushes value stored at/in LoopCounter on the stack.
/ 1
- \
/ 0
> FibLoop      # If second to last stack-value (LoopCounter) > last stack-value (0) jump to FibLoop jump endpoint (removes 0 from the stack).
^
```
