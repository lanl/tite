# Tag Invoked Type Erasure (TITE)

The TITE library provides type erasure implementation utilizing the `tag_invoke` paradigm proposed for standardization here: https://open-std.org/JTC1/SC22/WG21/docs/papers/2019/p1895r0.pdf.
The implementation contained herein is largely modeled after that provided in the standardization proposal and available at https://godbolt.org/z/3TvO4f.
Significant modification have been made to the original implementation to improve its suitability to be utilized for GPU architectures.
In particular the implementation:
 - only requires a C++14 standard.
 - has been extended to obtain `vtable`s for GPU device architectures in addition to CPU host architectures.
 - provides a `gpu_allocator` class appropriate for allocation of the type-erased object to GPU memory
 - provides copy semantics omitted from the original implementation--
    - generally the copy semantics are to completely copy the type-erased object
    - when the usage of the type-erased object guarantees immutability the copy semantics are altered to reference-counted shallow copies (copies of pointers) for improved performance

# Usage

## Customization Point Objects (CPOs)

The basic usage is predicated on customization point objects (CPOs).
The client of the library defines the customization point objects they wish to utilize in the type-erasure process.
For example, consider one wishes to type-erase any container providing the `operator[]` interface; then, the CPO to do this might look like the following:
```cpp
struct get_cpo {
private:
  template<typename ContainerT, typename IntT>
  constexpr
  friend auto tag_invoke(get_cpo, ContainerT&& c, IntT i) -> decltype( std::forward<ContainerT>(c)[i] )
  {
    return std::forward<ContainerT>(c)[i];
  }

public:
  template<typename ContainerT, typename IntT>
  constexpr
  auto operator()(ContatinerT&& c, IntT i) const
  -> decltype( tite::tag_invoke{}(*this, std::forward<ContainerT>(c), i) )
  {
    return tite::tag_invoke{}(*this, std::forward<ContainerT>(c), i) );
  }
};
```
While initially a bit obscure, the CPO works as follows:
 - given an instance of the `get_cpo` it can be handed a container and an index through its public `operator()` interface passing itself as the first argument
 - the `operator()` interface then defers to the `tite::tag_invoke` callable object which in turn calls the `tag_invoke` function *unqualified* invoking argument-dependent lookup (ADL)
 - if the container implements `operator[]` then the CPO's private friend instance of `tag_invoke` will resolve the unqualified call to `tag_invoke` and the container's `operator[]` interface will be used
 - the beauty of the `tag_invoke` proposal, however, is that it the container does not provide an `operator[]` interface, the container can define its own `private friend tag_invoke` overload taking `get_cpo` as the first argument followed by the container and an index and perform the operations appropriate for it thereby providing a *simple* customization point

The TITE library provides a `cpo_base` CRTP class in an attempt to ease the "boiler plate" repetitiveness of defining the CPOs.
Using the `cpo_base` class the above `get_cpo` could be written:
```cpp
struct get_cpo : cpo_base<get_cpo> 
private:
  template<typename ContainerT, typename IntT>
  constexpr
  friend auto tag_invoke(get_cpo, ContainerT&& c, IntT i) -> decltype( std::forward<ContainerT>(c)[i] )
  {
    return std::forward<ContainerT>(c)[i];
  }
};
```
The boiler plate of providing the `operator()` interface is provided by the `cpo_base` where every argument is taken by forwarding reference and forwarded to the `tag_invoke` callable object.

## Overloads

CPOs are utilized to build overload definitions--types that serve as the interfaces to the type-erased object--which are a combination of a CPO and type-erased signature.
For example, one could define an overload for the `get_cpo` to be the following:
```cpp
  using const_get_overload = tite::overload<get_cpo, int const&(tite::self const&, size_t)>;
```
The above overload concretizes the types that will be passed using the type-erasure.
Here, `tite::self` is a special tag that will be replaced with the type-erased type as appropriate.
The index argument of `get_cpo` is concretized to a `size_t`.

The same CPO can be utilized for multiple overloads provided they satisfy the API requirements.
For example, one can also define a non-const access as follows:
```cpp
  using mutable_get_overload = tite::overload<get_cpo, int&(tite::self&, size_t)>;
```

## The `any_unique` Type Erasure

The `any_unique` class provides a type-erasure satisfying the overloads specified.
Continuing with the example of type erasing a container providing the `operator[]` interface, one can define the following:
```cpp
  using random_access_int_container = tite::any_unique<const_get_overload, mutable_get_overload>;
```
and then construct one as follows:
```
  random_access_container c = std::vector<int>{1,2,3,4,5};
```
Mutable access to the elements can then be obtained using
```
  get_cpo{}(c, 3) = 6; // container now has {1,2,3,6,5};
```
or immutable access by
```
  auto const& const_c = c;
  auto& v3 = get_cpo{}(const_c, 2); // v3 is a const reference to 3
```
(Note, had `c` been marked `const` then then `const_cast` would not be needed).

The `any_unique` class can be told how to allocate the type erased object by providing an additional allocator argument, e.g.,
```cpp
  random_access_container c2 = {std::deque<int>{1,2,3,4,5}, std::allocator<unsigned char>};
```
Note that the allocator passed will be rebound to the type being erased (here the `std::deque<int>`).
Moreover, the type erasure can be placed in GPU memory utilizing the `tite::gpu_allocator`, e.g.,
```cpp
  random_access_container c3 = {gpu_container, tite::gpu_allocator<unsigned char>};
```
(here the type `gpu_container` is used purely for expository purposes).

**IMPORTANT NOTE**: When type erasing to the GPU it is important to understand that the `gpu_allocator` only allocates the object to the GPU, but it is the clients responsibility to ensure that any contained data resides in GPU memory as required.

