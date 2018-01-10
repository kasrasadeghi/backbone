### >= vs ==: the subtlety of pushSexp

##### Intro

So as the project has grown, I have started using Sexp as a kind of general purpose STL construct.

It can be a linked-list, it can be a tree, it can be a vector, and it can be a deque. All with a little overhead, but
not that much.

I've added a good number of STL style functions, but the most important to its use is pushSexp, which is analogous to
std::vector::push_back.

I originally had the invariant, when working in the parser, than the length would always be less than the capacity. That
invariant held true while using the original interface, just using sexp() and pushSexp(). But when doing more complex
generation, especially in `normalize`, I opted to use makeSexp. Perhaps this was a mistake, but it allowed for 
a larger control over the structure of the S-expression.

makeSexp has the postcondition that cap == length, thus breaking the invariant.

### 

pushSexp originally had the code:

```c
void pushSexp(Sexp* const s, Sexp* child) {
  s->list[s->length] = child;
  ++s->length;
  
  if (s->length == s->cap) {
    s->cap *= 2;
    s->list = realloc(s->list, s->cap * sizeof(Sexp*));
  }
}
``` 

I changed it to 
```c
void pushSexp(Sexp* const s, Sexp* child) {
  s->list[s->length] = child;
  ++s->length;
  
  if (s->length >= s->cap) { // this line
    s->cap *= 2;
    s->list = realloc(s->list, s->cap * sizeof(Sexp*));
  }
}
``` 

because I believed there was a problem with it if length == capacity before the function ran, in that it wouldn't resize.
But in reality there was an even bigger problem, because you'd be putting the child at the length of the list, and
if length == capacity that was right after the end of the list because zero-indexing.

So the current code is
```c
void pushSexp(Sexp* const s, Sexp* child) {
  if (s->length == s->cap) { // this line
    s->cap *= 2;
    s->list = realloc(s->list, s->cap * sizeof(Sexp*));
  }
  
  s->list[s->length] = child;
  ++s->length;
}
```

which is the opposite of the first one.

##### len <= cap vs len < cap: Battle of the Invariants

Should we have the invariant that len < cap and have all sexp's start off with more capacity, even 0 length ones?
or should we have the invariant that len <= cap?
The code for pushSexp does not change drastically either way.

Maybe len < cap and starting off with initial capacity is a good idea, but I think that allowing for len <= cap is a
good idea because it allows for 0 length Sexps that have no capacity, which is common with all Atoms. It also allows
for pre-allocation and growing of different rates. The weaker of the two invariants is the way to go.