#ifndef UTILITIES_MP_HXX_
#define UTILITIES_MP_HXX_

#include <tuple>
#include <type_traits>
#include <functional>

template <template<class> class Cond, class Tuple>
struct find_type;

//所有元素均不满足条件
template <template<class> class Cond>
struct find_type<Cond, std::tuple<>> {
    using type = std::void_t<>;
};

template <template<class> class Cond, class Tuple>
using find_type_t = typename find_type<Cond, Tuple>::type;

//找到了满足条件的元素
template <template<class> class Cond, class T, class... Ts>
struct find_type<Cond, std::tuple<T, Ts...>>: std::conditional<Cond<T>::value, T, find_type_t<Cond, std::tuple<Ts...>>> {};

template<size_t N>
using index_t = std::integral_constant<size_t, N>; // C++14

template<size_t M>
struct magic_switch {
  template<class F, class...Args>
  using R=std::result_of_t<F(index_t<0>, Args...)>;
  template<class F, class...Args>
  R<F, Args...> operator()(F&& f, size_t i, Args&&...args)const{
    if (i >= M)
      throw i; // make a better way to return an error
    return invoke(std::make_index_sequence<M>{}, std::forward<F>(f), i, std::forward<Args>(args)...);
  }
private:
  template<size_t...Is, class F, class...Args>
  R<F, Args...> invoke(std::index_sequence<Is...>, F&&f, size_t i, Args&&...args)const {
    using pF=decltype(std::addressof(f));
    using call_func = R<F, Args...>(*)(pF pf, Args&&...args);
    static const call_func table[M]={
      [](pF pf, Args&&...args)->R<F, Args...>{
        return std::forward<F>(*pf)(index_t<Is>{}, std::forward<Args>(args)...);
      }...
    };
    return table[i](std::addressof(f), std::forward<Args>(args)...);
  }
};

#endif /* UTILITIES_MP_HXX_ */
