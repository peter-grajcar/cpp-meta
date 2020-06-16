/*
 * Introduction:
 *
 * Chebyshev polynomial (of first kind) are orthogonal polynomials. Chebyshev polynomial
 * of degree N is defined recursively:
 * T_0(x) = 1
 * T_1(x) = x
 * T_{n+1}(x) = 2xT_n(x) - T_{n-1}(x)
 *
 * Let a_i be i-th coefficient of T_{n-1}(x),
 *     b_i be i-th coefficient of T_n(x),
 *     c_i be i-th coefficient of T_{n+1}(x)
 * Then c_i = 2*b_i - a_i
 *
 */
#include <iostream>
#include <type_traits>

template <size_t Deg, size_t N>
struct ChebyshevCoefficient;

template <>
struct ChebyshevCoefficient<0, 0>
{
    static constexpr int value = 1 ;
};

template <size_t N>
struct ChebyshevCoefficient<0, N>
{
    static constexpr int value = 0;
};

template <>
struct ChebyshevCoefficient<1, 1>
{
    static constexpr int value = 1;
};

template <size_t N>
struct ChebyshevCoefficient<1, N>
{ static constexpr int value = 0; };

template <size_t Deg>
struct ChebyshevCoefficient<Deg, Deg>
{
    static constexpr int value = 2 * ChebyshevCoefficient<Deg - 1, Deg - 1>::value;
};

template <size_t Deg, size_t N>
struct ChebyshevCoefficient
{
    static constexpr int value = N > Deg ? 0 : 2 * ChebyshevCoefficient<Deg - 1, N - 1>::value - ChebyshevCoefficient<Deg - 2, N>::value;
};


template <int... args>
struct Array
{
    static constexpr int data[sizeof...(args)] = { args... };
};

template <size_t N, template <size_t> class F, int... args>
struct GenerateArray
{
    using array = typename GenerateArray<N - 1, F, F<N>::value, args...>::array;
};

template <template <size_t> class F, int... args>
struct GenerateArray<0, F, args...>
{
    using array = Array<F<0>::value, args...>;
};

template <size_t N, template <size_t> class F>
struct FillArray
{
    using array = typename GenerateArray<N - 1, F>::array;
};

template<size_t index> struct MetaFunc
{
    static constexpr int value = index + 1;
};

template <size_t Deg>
struct ChebyshevPolynomial
{
    template <size_t N>
    struct CoefFunc {
        static constexpr int value = ChebyshevCoefficient<Deg, N>::value;
    };
    static constexpr auto coef = FillArray<Deg + 1, CoefFunc>::array::data;
};

int main()
{
    static_assert(ChebyshevCoefficient<3, 3>::value == 4);
    static_assert(ChebyshevCoefficient<3, 2>::value == 0);
    static_assert(ChebyshevCoefficient<3, 1>::value == -3);
    static_assert(ChebyshevCoefficient<3, 0>::value == 0);

    using P = ChebyshevPolynomial<3>;
    for(size_t i = 0; i < 4; ++i)
        std::cout << P::coef[i] << ' ';
    std::cout << std::endl;

    return 0;
}
