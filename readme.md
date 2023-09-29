# constexpr pool allocator

C++20's constexpr dynamic memory allocation allows pointers assigned using
`new` or `std::allocator::allocate` to compare against one another *only*
if they come from a single allocation. For example, constant evaluation of
the following is fine:

```cpp
int* p = new int[2];
assert(&p[0] < &p[1]);
delete [] p;
```

So too, using `std::allocator` and its `allocate` and `deallocate` member
functions will work out nicely. However, comparing two pointers derived from
separate allocations will fail, and produce a compilation error.

```cpp
int* p1 = new int, * p2 = new int;
assert(p1 <= p2 || p1 > p2); // error
delete p1;
delete p2;
```

Included here in `constexpr-pool-allocator.hpp` is the template class
`pool_alloc`.  Thanks to Corentin Jabot and David Ledger's C++ proposal P2738,
`void` pointers can now be cast to a pointer of the type of the pointed to
object. This allows a simple pool allocator class to de defined; initialising
its void pointer member by the typed pointer argument given to the constructor.
Using two of these as the allocator arguments for corresponding containers,
allows the element addresses of two corresponding containers to be
compared, with no compilation error.
