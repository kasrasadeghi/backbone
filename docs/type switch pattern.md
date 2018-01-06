### Type Switch Pattern

A pattern for switching through Sexp Types using is*SexpName*().

#### Delegation

If you're switching through each type and delegating to a single function, do

```c
void typeSwitch(Sexp* s) {
  if (isType0(s)) { delegateType0(s); } else
  if (isType1(s)) { delegateType1(s); } else
  if (isType2(s)) { delegateType2(s); } else
  if (isType3(s)) { delegateType3(s); } else
  if (isType4(s)) { delegateType4(s); } else
  if (isType5(s)) { delegateType5(s); } 
  else { catchAll(s); } // might be an error case
}
```

This is fairly easy to read and check to see if each case lines up.
These parts of the code are also fairly easy for me to make mistakes on 
because I tend to not think very hard about copy-pasting my lines of code.

This is also fairly concise.

#### Complicated Switch

If you're handling the condition with more than one statement, consider

```c
void typeSwitch(Sexp* s) {
  if (isType0(s)) { 
    beforeType0(s);
    afterType0(s);
    return; 
  } else if (isType1(s)) { 
    beforeType1(s);
    afterType1(s);
    return;
  } else if (isType2(s)) { 
    beforeType2(s);
    afterType2(s);
    return;
  } else if (isType3(s)) { 
    beforeType3(s);
    afterType3(s);
    return;
  } else if (isType4(s)) { 
    beforeType4(s);
    afterType4(s);
    return;
  } else {
    catchAll(s);  
  }
  error(s); // might be an error case
  assert(0);
}
```

This is a lot more verbose and vertically spaced, but allows for more helpful
error handling.