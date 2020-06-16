#include <iostream>
#include <type_traits>

template <int N, int P>
struct Power
{
    enum { value = N * Power<N, P - 1>::value };
};

template <int N>
struct Power<N, 0>
{
    enum { value = 1 };
};

template <size_t Deg, size_t N>
struct ChebyshevCoefficient;

template <>
struct ChebyshevCoefficient<0, 0>
{
    static constexpr int coef = 1 ;
};

template <size_t N>
struct ChebyshevCoefficient<0, N>
{
    static constexpr int coef = 0;
};

template <>
struct ChebyshevCoefficient<1, 1>
{
    static constexpr int coef = 1;
};

template <size_t N>
struct ChebyshevCoefficient<1, N>
{ static constexpr int coef = 0; };

template <size_t Deg>
struct ChebyshevCoefficient<Deg, Deg>
{
    static constexpr int coef = 2 * ChebyshevCoefficient<Deg - 1, Deg - 1>::coef;
};

template <size_t Deg, size_t N>
struct ChebyshevCoefficient
{
    static constexpr int coef = N > Deg ? 0 : 2 * ChebyshevCoefficient<Deg - 1, N - 1>::coef - ChebyshevCoefficient<Deg - 2, N>::coef;
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
        static constexpr int value = ChebyshevCoefficient<Deg, N>::coef;
    };
    static constexpr auto coefs = FillArray<Deg + 1, CoefFunc>::array::data;
};

int main()
{
    /*const int *compilerFilledArray = Table<4>::array;
    for (int i=0; i < 5; ++i)
        std::cout << compilerFilledArray[i] << ' ';
    std::cout << std::endl;*/

    static_assert(ChebyshevCoefficient<3, 3>::coef == 4);
    static_assert(ChebyshevCoefficient<3, 1>::coef == -3);

    std::cout << ChebyshevCoefficient<3, 3>::coef << "x^3 + ";
    std::cout << ChebyshevCoefficient<3, 2>::coef << "x^2 + ";
    std::cout << ChebyshevCoefficient<3, 1>::coef << "x + ";
    std::cout << ChebyshevCoefficient<3, 0>::coef << std::endl;

    static_assert(Power<3, 4>::value == 81);

    using P = ChebyshevPolynomial<3>;
    for(size_t i = 0; i < 4; ++i)
        std::cout << P::coefs[i] << ' ';
    std::cout << std::endl;

    return 0;
}
