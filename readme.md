# constexpr pool allocator

C++20's constexpr dynamic memory allocation allows pointers assigned using
`new` or `std::allocator::allocate` to compare against one another *only*
if they come from a single allocation. For example, constant evaluation of
the following is fine:

```cpp
int* p1 = new int[2];
assert(&p[0] < &p[1]);
delete [] p;
```

So too, using `std::allocator` and its `allocate` and `deallocate` member
functions will work out nicely. However, comparing two pointers derived from
separate allocations will fail, and produce a compilation error.

Included in constexpr-pool-allocator.hpp is the template class `pool_alloc`.
Thanks to Corentin Jabot and David Ledger C++ proposal P2738, `void` pointers
can now be cast to a pointer of the type of the pointed to object.
