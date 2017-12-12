### Auto: the way it was meant to be

The C Programming Language has a feature that goes fairly unused, and in fact most C programmers 
have probably never heard of it. The `auto` keyword in C denotes automatic allocation of a variable
on the stackframe of the current function call and automatically freeing it on return.

Backbone does not have a way to express modification of a name that has been `let` to be some
result because that's misleading. You said `let a be 5`. `a` should always be `5`. Because of this,
you cannot get the address of a value that has been let and you cannot store a value to be some
primitive directly. `let` statements must be the result of some computation in order for them to 
actually be used. In mathematics, there is also a `let` statement for aliasing a value, but that
has not (yet) been implemented as we prioritized `auto`.

`Auto` represents stack allocation of a variable of a certain size, and it sets the name assigned
to be the pointer to the current stack allocation. This allows passing by reference more 
naturally, gives semantics to getting the address of a stack variable, and distinguishes between
values you can reassign and values you can't.

The only way to mutate a value is to call some heap allocator (`malloc` family) and then store 
different values into it or get a pointer using `auto`.

### Usage: store and load

To get the value of a automatic variable, just like every pointer, you `load` it.
To change the value you `store` something. Seems pretty straightforward.

  