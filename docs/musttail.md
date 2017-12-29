### Musttail Keywords

`inalloca` seems like something useful for passing contiguous memory
from the stackframe along to an inner function.

`bitcasting` doesn't seem very useful for our present situation.

`sret` is useful for chaining calls. It means the pointer you return
is the same as the parameter you're marking with this attribute.

`byval` means that the object pointed to should really be passed 
by value. This is probably useful for const references.

`inreg` means that you should put this argument in a 
target-dependent register.

`returned` means that the argument value is the exact same as the
return value. Probably useful for method chaining.