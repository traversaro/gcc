// PR libstdc++/111327 - std::bind_front (and std::not_fn) doesn't always
// perfectly forward according to value category of the call wrapper object
// { dg-options "-std=gnu++20" }
// { dg-do compile { target c++20 } }

#include <functional>
#include <utility>

struct F {
  void operator()(...) & = delete;
  void operator()(...) const &;
};

struct G {
  void operator()(...) && = delete;
  void operator()(...) const &&;
};

int main() {
  auto f0 = std::bind_front(F{});
  f0(); // { dg-error "deleted" }
  std::move(f0)();
  std::as_const(f0)();
  std::move(std::as_const(f0))();

  auto g0 = std::bind_front(G{});
  g0(); // { dg-error "deleted" }
  std::move(g0)(); // { dg-error "deleted" }
  std::move(std::as_const(g0))();

  auto f1 = std::bind_front(F{}, 42);
  f1(); // { dg-error "deleted" }
  std::move(f1)();
  std::as_const(f1)();
  std::move(std::as_const(f1))();

  auto g1 = std::bind_front(G{}, 42);
  g1(); // { dg-error "deleted" }
  std::move(g1)(); // { dg-error "deleted" }
  std::move(std::as_const(g1))();
}
