# Fox instruction set

## General syntax

```assembly
<op> <register1> <register2> <destination>
```
### Instructions

#### BACK
Fox ir
```assembly
back <register1> <destination> ;$destination = $register1.back()
```
Same as

```cpp
destination = register1.back()//get the last element of the array
```


#### AT

```assembly
at <register1> <register2> <destination>;$destination = $register2.at($register1)
```
Same as

```cpp
destination = register2[register1]//get the element at the index register1 of register2
```
#### ASPTR_VAL
    
```assembly
asptr_val <register1> <destination>;*$destination = $register1
```
Same as
```cpp
*destination = register1//assign the value of the register to the pointer destination
```

#### LDPTR_VAL
```assembly
ldptr_val <register1> <destination>;$destination = *$register1
```
Same as
```cpp
destination = *register1//Dereference pointer and assign to destination
```

#### GETPTR
```assembly
getptr <register1> <destination>;$destination =&$register1
```
Same as
```cpp
destination = &register1//get the address of the register
```

#### LEN
```assembly
len <register1> <destination>;$destination = $register1.length()
```
Same as
```cpp
destination = register1.length()//get the length of array
```
####  FREE
```assembly
free <register1>;free($register1)
```
Same as
```cpp
free(register1)//free up memory used by register1
```
#### MALLOC
```assembly
malloc <register1> <destination>;$destination = malloc($register1)
```
Same as
```cpp
destination = malloc(register1)//allocate register1 bytes of memory to destination
```

#### REALLOC
```assembly
realloc <register1> <destination>;$destination = realloc($register1)
```
Same as
```cpp
destination = realloc(register1)//reallocate register1 bytes of memory to destination
```

#### INSERT
```assembly
insert <register1> <register2> <destination>;$destination[$register2]=$register1
```
Same as
```cpp
destination[register2]=register1
```

#### APPEND
```assembly
append <register1> <destination>;$destination.push_back($register1)
```
Same as
```cpp
destination.push_back(register1)//append register1 to the end of the array
```

#### POP
```assembly
pop <register>;Pop the last element of the array
```
Same as
```cpp
array.pop_back()//pop the last element of the array
```

#### EQ_ARRAY
```assembly
eq_array <register1> <register2> <destination>;$destination = $register1 == $register2
```
Same as
```cpp
destination = register1 == register2//check if the arrays are equal
```

#### IF_NEQ
```assembly
if_neq <register1> <register2> <destination>;if($register1 != $register2): jmp $destination
```
Same as
```cpp
if(register1 != register2){
    destination()
}
```

#### IF_EQ
```assembly
if_eq <register1> <register2> <destination>;if($register1 == $register2): jmp $destination
```
Same as
```cpp
if(register1 == register2){
    destination()
}
```
#### IF_GT
```assembly
if_gt <register1> <register2> <destination>;if($register1 > $register2): jmp $destination
```
Same as
```cpp
if(register1 > register2){
    destination()
}
```
#### IF_LT
```assembly
if_lt <register1> <register2> <destination>;if($register1 < $register2): jmp $destination
```
Same as
```cpp
if(register1 < register2){
    destination()
}
```
#### IF_GE
```assembly
if_ge <register1> <register2> <destination>;if($register1 >= $register2): jmp $destination
```
Same as
```cpp
if(register1 >= register2){
    destination()
}
```
#### IF_LE
```assembly
if_le <register1> <register2> <destination>;if($register1 <= $register2): jmp $destination
```
Same as
```cpp
if(register1 <= register2){
    destination()
}
```
#### IF_AND
```assembly
if_and <register1> <register2> <destination>;if($register1 && $register2): jmp $destination
```
Same as
```cpp
if(register1 && register2){
    destination()
}
```
#### IF_OR
```assembly
if_or <register1> <register2> <destination>;if($register1 || $register2): jmp $destination
```
Same as
```cpp
if(register1 || register2){
    destination()
}
```
#### OP_IF
```assembly
op_if <register1> <destination>;if($register1==True): jmp $destination
```
Same as
```cpp
if(register1){
    destination()
}
```
#### OP_ELSE
```assembly
op_else <destination>;else: jmp $destination
```
Same as
```cpp
else{
    destination()
}
```

#### LOAD
```assembly
op_load <value> <destination>;$destination = value
```
Same as
```cpp
destination = value//value is a const expression
```
#### MOV
```assembly
mov <register1> <destination>;$destination = $register1
```
Same as
```cpp
destination = register1//assign the value of register1 to destination
```

#### ADD
```assembly
add <register1> <register2> <destination>;$destination = $register1 + $register2
```
Same as
```cpp
destination = register1 + register2//add register1 and register2 and assign to destination
```
#### SUB
```assembly
sub <register1> <register2> <destination>;$destination = $register1 - $register2
```
Same as
```cpp
destination = register1 - register2//subtract register2 from register1 and assign to destination
```

#### MUL
```assembly
mul <register1> <register2> <destination>;$destination = $register1 * $register2
```
Same as
```cpp
destination = register1 * register2//multiply register1 and register2 and assign to destination
```

#### DIV
```assembly
div <register1> <register2> <destination>;$destination = $register1 / $register2
```
Same as
```cpp
destination = register1 / register2//divide register1 by register2 and assign to destination
```

#### MOD
```assembly
mod <register1> <register2> <destination>;$destination = $register1 % $register2
```
Same as
```cpp
destination = register1 % register2//modulo register1 by register2 and assign to destination
```

#### BIT_XOR
```assembly
bit_xor <register1> <register2> <destination>;$destination = $register1 ^ $register2
```
Same as
```cpp
destination = register1 ^ register2//bitwise xor register1 and register2 and assign to destination
```

#### EQ
```assembly
eq <register1> <register2> <destination>;$destination = $register1 == $register2
```
Same as
```cpp
destination = register1 == register2//check if register1 is equal to register2 and assign to destination
```
#### NEQ
```assembly
neq <register1> <register2> <destination>;$destination = $register1 != $register2
```
Same as
```cpp
destination = register1 != register2//check if register1 is not equal to register2 and assign to destination
```
#### GT
```assembly
gt <register1> <register2> <destination>;$destination = $register1 > $register2
```
Same as
```cpp
destination = register1 > register2//check if register1 is greater than register2 and assign to destination
```
#### LT
```assembly
lt <register1> <register2> <destination>;$destination = $register1 < $register2
```
Same as
```cpp
destination = register1 < register2//check if register1 is less than register2 and assign to destination
```
#### GE
```assembly
ge <register1> <register2> <destination>;$destination = $register1 >= $register2
```
Same as
```cpp
destination = register1 >= register2//check if register1 is greater than or equal to register2 and assign to destination
```
#### LE
```assembly
le <register1> <register2> <destination>;$destination = $register1 <= $register2
```
Same as
```cpp
destination = register1 <= register2//check if register1 is less than or equal to register2 and assign to destination
```
#### AND
```assembly
and <register1> <register2> <destination>;$destination = $register1 && $register2
```
Same as
```cpp
destination = register1 && register2//check if register1 and register2 are true and assign to destination
```
#### OR
```assembly
or <register1> <register2> <destination>;$destination = $register1 || $register2
```
Same as
```cpp
destination = register1 || register2//check if register1 or register2 is true and assign to destination
```
#### BIT_OR
```assembly
bit_or <register1> <register2> <destination>;$destination = $register1 | $register2
```
Same as
```cpp
destination = register1 | register2//bitwise or register1 and register2 and assign to destination
```
#### BIT_AND
```assembly
bit_and <register1> <register2> <destination>;$destination = $register1 & $register2
```
Same as
```cpp
destination = register1 & register2//bitwise and register1 and register2 and assign to destination
```
#### SHL
```assembly
shl <register1> <register2> <destination>;$destination = $register1 << $register2
```
Same as
```cpp
destination = register1 << register2//shift register1 left by register2 and assign to destination
```
#### SHR
```assembly
shr <register1> <register2> <destination>;$destination = $register1 >> $register2
```
Same as
```cpp
destination = register1 >> register2//shift register1 right by register2 and assign to destination
```
#### NEG
```assembly
neg <register1> <destination>;$destination = -$register1
```
Same as
```cpp
destination = -register1//negate register1 and assign to destination
```
#### NOT
```assembly
not <register1> <destination>;$destination = !$register1
```
Same as
```cpp
destination = !register1//logical not register1 and assign to destination
```
#### BIT_NOT
```assembly
bit_not <register1> <destination>;$destination = ~$register1
```
Same as
```cpp
destination = ~register1//bitwise not register1 and assign to destination
```

#### JMP
```assembly
jmp <label>;label()
```
Same as
```cpp
label()//call label
```

#### DL_OPEN
```assembly
dl_open <register1> <destination>;$destination = dlopen($register1)
```
Same as
```cpp
destination = dlopen(register1)//load dynamic library register1 and assign to destination
```

#### DL_CLOSE
```assembly
dl_close <register1> ;dlclose($register1)
```
Same as
```cpp
dlclose(register1)//close dynamic library register1
```

#### DL_CALL
```assembly
dl_call <register1> <register2> <register3>;temp = dlsym($register1, $register2); temp($register3)
```
Same as
```cpp
temp = dlsym(register1, register2);//load the function register2 from dynamic library register1 and assign it to temp
temp(register3,memory); //then call temp argument register3 and memory
```
#### RET
```assembly
ret
```
Same as
```cpp
return//return from label or stop the execution of the vm. It should always be present at the end of the program and any label
```