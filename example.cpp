#include "constexpr-pool-allocator.hpp"
#include <cassert>
#include <vector>

// Copyright (c) 2023 Paul Keir, University of the West of Scotland.
//   https://github.com/SCT4SP/constexpr-pool-allocator

constexpr bool test_constexpr_simple_allocator_compare()
{
  bool b{true};
  const std::size_t sz = 2;
  int* p1 = new int[sz];
  b = b && &p1[0] < &p1[sz-1];
  delete [] p1;

  std::allocator<float> alloc;
  float* p2 = alloc.allocate(sz);
  b = b && (&p2[0] < &p2[sz-1]);
  alloc.deallocate(p2, sz);

  return b;
}

constexpr bool test_constexpr_pool_allocator()
{
  bool b{true};

  const std::size_t sz   = 1024;
  const std::size_t half = sz/2;
  std::allocator<float> alloc;
  float* p = alloc.allocate(sz);
  pool_alloc<float> a1{p};
  pool_alloc<float> a2{p + half};

  {
    std::vector v1(half, 42.0f, a1);
    std::vector v2(half, 43.0f, a2);
    using expected_t = std::vector<float, pool_alloc<float>>;
    static_assert(std::is_same_v<decltype(v1), expected_t>);
    static_assert(std::is_same_v<decltype(v2), expected_t>);
    b = b && v1[0] == 42.0f && v2[0] == 43.0f;
    b = b && (p == &v1[0]) && (&v1[half-1] < &v2[0]) && (&v2[0] < &v2[half-1]);
  }

  alloc.deallocate(p, sz);

  return b;
}

int main(int argc, char* argv[])
{
  static_assert(test_constexpr_simple_allocator_compare());
         assert(test_constexpr_simple_allocator_compare());

  static_assert(test_constexpr_pool_allocator());
         assert(test_constexpr_pool_allocator());

  return 0;
}
