// Copyright (c) 2018-2020 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/leaf/config.hpp>
#ifndef BOOST_LEAF_NO_EXCEPTIONS
#	include <boost/leaf/handle_exception.hpp>
#endif
#include <boost/leaf/handle_error.hpp>
#include "_test_ec.hpp"

namespace leaf = boost::leaf;

template <class... T>
struct unwrap_tuple;

template <template <class> class S, class... E>
struct unwrap_tuple<std::tuple<S<E>...>>
{
	using type = std::tuple<E...>;
};

template <class... H>
typename unwrap_tuple<typename leaf::context_type_from_handlers<H...>::Tup>::type * expd( H && ... )
{
	return 0;
}

template <class T, class U>
void test( U * )
{
	static_assert(std::is_same<T,U>::value,"context deduction");
}

template <int> struct info { int value; };
template <int> struct info_f { int value() const; };

enum class my_error_code
{
	ok,
	error1,
	error2,
	error3
};

void not_called_on_purpose()
{
	test< std::tuple<info<1>> >( expd([]( info<1> ){ }) );
	test< std::tuple<info<1>> >( expd([]( info<1> const ){ }) );
	test< std::tuple<info<1>> >( expd([]( info<1> const & ){ }) );
	test< std::tuple<info<1>> >( expd([]( info<1>, leaf::error_info const & ){ }) );

	test< std::tuple<info<1>,info<2>> >( expd([]( info<1> ){ }, []( info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1> ){ }, []( info<1>, info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1>, info<2> ){ }, []( info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1>, info<2> ){ }, []( info<1>, info<2> ){ }) );

	test< std::tuple<info<1>,info<2>> >( expd([]( leaf::error_info const &, info<1> ){ }, []( info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( leaf::error_info const &, info<1> ){ }, []( info<1>, info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( leaf::error_info const &, info<1>, info<2> ){ }, []( info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( leaf::error_info const &, info<1>, info<2> ){ }, []( info<1>, info<2> ){ }) );

	test< std::tuple<info<1>,info<2>> >( expd([]( info<1>, leaf::error_info const & ){ }, []( info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1>, leaf::error_info const & ){ }, []( info<1>, info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1>, leaf::error_info const &, info<2> ){ }, []( info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1>, leaf::error_info const &, info<2> ){ }, []( info<1>, info<2> ){ }) );

	test< std::tuple<info<1>,info<2>> >( expd([]( info<1> ){ }, []( leaf::error_info const &, info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1> ){ }, []( leaf::error_info const &, info<1>, info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1>, info<2> ){ }, []( leaf::error_info const &, info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1>, info<2> ){ }, []( leaf::error_info const &, info<1>, info<2> ){ }) );

	test< std::tuple<info<1>,info<2>> >( expd([]( info<1> ){ }, []( info<2>, leaf::error_info const & ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1> ){ }, []( info<1>, leaf::error_info const &, info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1>, info<2> ){ }, []( info<2>, leaf::error_info const & ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1>, info<2> ){ }, []( info<1>, leaf::error_info const &, info<2> ){ }) );

	test< std::tuple<info<1>,info<2>,info<3>> >( expd([]( info<1> ){ }, []( info<2> ){ }, []( info<3> ){ }) );
	test< std::tuple<info<1>,info<2>,info<3>> >( expd([]( info<1> ){ }, []( info<1>, info<2> ){ }, []( info<1>, info<3> ){ }) );
	test< std::tuple<info<1>,info<2>,info<3>> >( expd([]( info<1> ){ }, []( info<1>, info<2> ){ }, []( info<1>, info<3> ){ }) );
	test< std::tuple<info<1>,info<2>,info<3>> >( expd([]( info<1>, info<2> ){ }, []( info<2> ){ }, []( info<3> ){ }) );
	test< std::tuple<info<1>,info<2>,info<3>> >( expd([]( info<1>, info<2> ){ }, []( info<2> ){ }, []( info<3> ){ }) );
	test< std::tuple<info<1>,info<2>,info<3>> >( expd([]( info<1> ){ }, []( info<2> ){ }, []( info<3>, info<2> ){ }) );
	test< std::tuple<info<1>,info<2>,info<3>> >( expd([]( info<1> ){ }, []( info<2> ){ }, []( info<3>, info<2> ){ }) );
	test< std::tuple<info<1>,info<3>,info<2>> >( expd([]( info<1>, info<3> ){ }, []( info<2> ){ }, []( info<3> ){ }) );
	test< std::tuple<info<1>,info<3>,info<2>> >( expd([]( info<1>, info<3> ){ }, []( info<2> ){ }, []( info<3> ){ }) );
	test< std::tuple<info<1>,info<2>,info<3>> >( expd([]( info<1> ){ }, []( info<2>, info<3> ){ }, []( info<3> ){ }) );
	test< std::tuple<info<1>,info<2>,info<3>> >( expd([]( info<1> ){ }, []( info<2>, info<3> ){ }, []( info<3> ){ }) );

	test< std::tuple<my_error_code> >( expd([]( leaf::match<my_error_code,my_error_code::error1> ){ }) );
	test< std::tuple<std::error_code> >( expd([]( leaf::match<leaf::condition<cond_x>, cond_x::x00> ){ }) );

	test< std::tuple<info<1>> >( expd([]( leaf::match<info<1>,42> ){ }) );
	test< std::tuple<info_f<1>> >( expd([]( leaf::match<info_f<1>,42> ){ }) );

#ifndef BOOST_LEAF_NO_EXCEPTIONS
	test< std::tuple<info<1>> >( expd([]( leaf::catch_<std::exception>, info<1> ){ }) );
#endif

	test< std::tuple<info<1>,info<2>,info<3>> >( expd([]( info<1> const *, info<2> ){ }, []( info<1>, info<3> const * ){ }) );
	test< std::tuple<info<1>,info<2>,info<3>> >( expd([]( info<1> const, info<2> ){ }, []( info<1> const *, info<3> ){ }) );

#if BOOST_LEAF_DIAGNOSTICS
	test< std::tuple<info<1>,info<2>,leaf::leaf_detail::e_unexpected_count> >( expd([]( info<1>, info<2>, leaf::diagnostic_info const & ){ }, []( info<1>, info<2> ){ }) );
	test< std::tuple<info<1>,info<2>,leaf::leaf_detail::e_unexpected_info> >( expd([]( info<1>, info<2> ){ }, []( info<1>, leaf::verbose_diagnostic_info const &, info<2> ){ }) );
	test< std::tuple<info<1>,info<2>,leaf::leaf_detail::e_unexpected_count,leaf::leaf_detail::e_unexpected_info> >( expd([]( info<1>, info<2>, leaf::diagnostic_info const & ){ }, []( info<1>, leaf::verbose_diagnostic_info const &, info<2> ){ }) );
#else
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1>, info<2>, leaf::diagnostic_info const & ){ }, []( info<1>, info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1>, info<2> ){ }, []( info<1>, leaf::verbose_diagnostic_info const &, info<2> ){ }) );
	test< std::tuple<info<1>,info<2>> >( expd([]( info<1>, info<2>, leaf::diagnostic_info const & ){ }, []( info<1>, leaf::verbose_diagnostic_info const &, info<2> ){ }) );
#endif

	{
		auto error_handlers = std::make_tuple(
			[]( info<2> ) { },
			[]( info<3> ) { } );
		test< std::tuple<info<1>, info<2>, info<3>, info<4>, info<5>>>( expd(
			[]( info<1> ) { },
			error_handlers,
			[]( info<4>, info<5> ) { } ) );
	}

	{
		auto error_handlers1 = std::make_tuple(
			[]( info<4> ) { },
			[]( info<5> ) { } );
		auto error_handlers2 = std::make_tuple(
			[]( info<2> ) { },
			[]( info<3> ) { },
			error_handlers1 );
		test< std::tuple<info<1>, info<2>, info<3>, info<4>, info<5>, info<6>>>( expd(
			[]( info<1> ) { },
			error_handlers2,
			[]( info<4>, info<5> ) { },
			[]( info<6> ) { } ) );
	}
}

int main()
{
	return 0;
}
