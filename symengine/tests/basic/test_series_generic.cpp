#include "catch.hpp"
#include <iostream>
#include <chrono>

#include <symengine/series_generic.h>
#include <symengine/mul.h>
#include <symengine/pow.h>
#include <symengine/dict.h>

using SymEngine::UnivariatePolynomial;
using SymEngine::UnivariateExprPolynomial;
using SymEngine::UnivariateSeries;
using SymEngine::univariate_series;
using SymEngine::Symbol;
using SymEngine::symbol;
using SymEngine::Pow;
using SymEngine::RCP;
using SymEngine::make_rcp;
using SymEngine::print_stack_on_segfault;
using SymEngine::map_int_Expr;
using SymEngine::Basic;
using SymEngine::one;
using SymEngine::zero;
using SymEngine::integer;
using SymEngine::integer_class;
using SymEngine::vec_basic_eq_perm;
using SymEngine::Expression;

using namespace SymEngine::literals;

TEST_CASE("Constructor of UnivariateSeries", "[UnivariateSeries]")
{
    RCP<const Symbol> x  = symbol("x");
    map_int_Expr adict_ = {{0, 1}, {1, 2}, {2, 0}};
    UnivariateExprPolynomial apoly_(univariate_polynomial(x, 2, std::move(adict_)));
    RCP<const UnivariateSeries> P = univariate_series(x, 2, apoly_);
    REQUIRE(P->__str__() == "1 + 2*x + O(x**2)");

    map_int_Expr bdict_ = {{0, 1}, {1, 0}, {2, 2}, {3, 1}};
    UnivariateExprPolynomial bpoly_(univariate_polynomial(x, 3, std::move(bdict_)));
    RCP<const UnivariateSeries> Q = UnivariateSeries::create(x, 5, bpoly_);
    REQUIRE(Q->__str__() == "1 + 2*x**2 + x**3 + O(x**5)");
}

TEST_CASE("Adding two UnivariateSeries", "[UnivariateSeries]")
{
    RCP<const Symbol> x  = symbol("x");
    map_int_Expr adict_ = {{0, 1}, {1, 2}, {2, 1}};
    UnivariateExprPolynomial apoly_(univariate_polynomial(x, 2, std::move(adict_)));
    map_int_Expr bdict_ = {{0, 2}, {1, 3}, {2, 4}};
    UnivariateExprPolynomial bpoly_(univariate_polynomial(x, 2, std::move(bdict_)));
    map_int_Expr ddict_ = {{0, 3}, {1, 5}, {2, 5}};
    UnivariateExprPolynomial dpoly_(univariate_polynomial(x, 2, std::move(ddict_)));

    const UnivariateSeries a(apoly_, x->get_name(), 5);
    const UnivariateSeries b(bpoly_, x->get_name(), 4);
    RCP<const UnivariateSeries> c = add_uni_series(a, b);
    const UnivariateSeries d(dpoly_, x->get_name(), 4);
    REQUIRE(c->__str__() == d.__str__());
    // REQUIRE(*c == d);

    const UnivariateSeries e(bpoly_, x->get_name(), 2);
    RCP<const UnivariateSeries> f = add_uni_series(a, e);
    const UnivariateSeries g(dpoly_, x->get_name(), 2);
    REQUIRE(f->__str__() == g.__str__());
    // REQUIRE(*f == g);
}

TEST_CASE("Negative of a UnivariateSeries", "[UnivariateSeries]")
{
    RCP<const Symbol> x  = symbol("x");
    map_int_Expr adict_ = {{0, 1}, {1, 2}, {2, 1}};
    UnivariateExprPolynomial apoly_(univariate_polynomial(x, 2, std::move(adict_)));
    map_int_Expr bdict_ = {{0, -1}, {1, -2}, {2, -1}};
    UnivariateExprPolynomial bpoly_(univariate_polynomial(x, 2, std::move(bdict_)));

    const UnivariateSeries a(apoly_, x->get_name(), 5);
    RCP<const UnivariateSeries> b = neg_uni_series(a);
    const UnivariateSeries c(bpoly_, x->get_name(), 5);
    REQUIRE(b->__str__() == c.__str__());
    // REQUIRE(*b == c);
}

TEST_CASE("Subtracting two UnivariateSeries", "[UnivariateSeries]")
{
    RCP<const Symbol> x  = symbol("x");
    map_int_Expr adict_ = {{0, 1}, {1, 2}, {2, 1}};
    UnivariateExprPolynomial apoly_(univariate_polynomial(x, 2, std::move(adict_)));
    map_int_Expr bdict_ = {{0, 2}, {1, 3}, {2, 4}};
    UnivariateExprPolynomial bpoly_(univariate_polynomial(x, 2, std::move(bdict_)));
    map_int_Expr fdict_ = {{0, -1}, {1, -1}, {2, -3}};
    UnivariateExprPolynomial fpoly_(univariate_polynomial(x, 2, std::move(fdict_)));
    map_int_Expr gdict_ = {{0, -1}, {1, -1}};
    UnivariateExprPolynomial gpoly_(univariate_polynomial(x, 1, std::move(gdict_)));

    UnivariateSeries a(apoly_, x->get_name(), 5);
    UnivariateSeries b(bpoly_, x->get_name(), 4);
    RCP<const UnivariateSeries> c = sub_uni_series(a, b);
    UnivariateSeries f(fpoly_, x->get_name(), 4);
    REQUIRE(c->__str__() == f.__str__());
    // REQUIRE(*c == f);

    UnivariateSeries d(bpoly_, x->get_name(), 2);
    RCP<const UnivariateSeries> e = sub_uni_series(a, d);
    UnivariateSeries g(gpoly_, x->get_name(), 2);
    REQUIRE(e->__str__() == g.__str__());
    // REQUIRE(*e == f);
}

TEST_CASE("Multiplication of two UnivariateSeries", "[UnivariateSeries]")
{
    RCP<const Symbol> x  = symbol("x");
    UnivariateExprPolynomial apoly_(univariate_polynomial(x, 2, {{0, 1}, {1, 2}, {2, 1}}));
    UnivariateExprPolynomial bpoly_(univariate_polynomial(x, 2, {{0, -1}, {1, -2}, {2, -1}}));
    UnivariateExprPolynomial epoly_(univariate_polynomial(x, 4, {{0, 1}, {1, 4}, {2, 6}, {3, 4}, {4, 1}}));
    UnivariateExprPolynomial fpoly_(univariate_polynomial(x, 2, {{0, -1}, {1, -4}, {2, -6}}));

    RCP<const UnivariateSeries> a = univariate_series(x, 5, apoly_);
    RCP<const UnivariateSeries> b = univariate_series(x, 3, bpoly_);
    RCP<const UnivariateSeries> e = univariate_series(x, 5, epoly_);
    RCP<const UnivariateSeries> f = univariate_series(x, 3, fpoly_);

    RCP<const UnivariateSeries> c = mul_uni_series(*a, *a);
    RCP<const UnivariateSeries> d = mul_uni_series(*a, *b);

    REQUIRE(c->__str__() == e->__str__());
    REQUIRE(d->__str__() == f->__str__());
    // REQUIRE(*c == *e);
    // REQUIRE(*d == *f);
}

//TEST_CASE("Differentiation of UnivariateSeries", "[UnivariateSeries]")
//{
  //  RCP<const Symbol> x  = symbol("x");
    //RCP<const UnivariateSeries> var = univariate_series(x, 1, {{0, 1}});
