// Copyright (c) 2018-2020 Emil Dotchevski and Reverge Studios, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/leaf/config.hpp>
#ifdef BOOST_LEAF_NO_EXCEPTIONS

#include <iostream>

int main()
{
	std::cout << "Unit test not applicable." << std::endl;
	return 0;
}

#else

#include <boost/leaf/handle_exception.hpp>
#include <boost/leaf/result.hpp>
#include "lightweight_test.hpp"

namespace leaf = boost::leaf;

template <int> struct info { int value; };

struct my_exception: std::exception { };

int main()
{
	{
		leaf::result<int> r = leaf::try_handle_some(
			[]() -> leaf::result<int>
			{
				return 42;
			},
			[]
			{
				return 1;
			} );
		BOOST_TEST(r);
		BOOST_TEST_EQ(r.value(), 42);
	}
	{
		leaf::result<int> r = leaf::try_handle_some(
			[]() -> leaf::result<int>
			{
				throw leaf::exception( my_exception(), info<1>{1} );
			},
			[]( my_exception const &, info<1> const & x )
			{
				BOOST_TEST_EQ(x.value, 1);
				return 42;
			} );
		BOOST_TEST(r);
		BOOST_TEST_EQ(r.value(), 42);
	}
	{
		leaf::result<int> r = leaf::try_handle_some(
			[]() -> leaf::result<int>
			{
				return leaf::new_error( info<1>{1} );
			},
			[]( info<1> const & x )
			{
				BOOST_TEST_EQ(x.value, 1);
				return -42;
			} );
		BOOST_TEST(r);
		BOOST_TEST_EQ(r.value(), -42);
	}

	{
		auto error_handlers = std::make_tuple(
			[]( my_exception const &, info<1> const & x ) -> leaf::result<int>
			{
				BOOST_TEST_EQ(x.value, 1);
				return 42;
			},
			[]( info<1> const & x ) -> leaf::result<int>
			{
				BOOST_TEST_EQ(x.value, 1);
				return -42;
			} );
		{
			leaf::result<int> r = leaf::try_handle_some(
				[]() -> leaf::result<int>
				{
					return 42;
				},
				error_handlers );
			BOOST_TEST(r);
			BOOST_TEST_EQ(r.value(), 42);
		}
		{
			leaf::result<int> r = leaf::try_handle_some(
				[]() -> leaf::result<int>
				{
					throw leaf::exception( my_exception(), info<1>{1} );
				},
				error_handlers );
			BOOST_TEST(r);
			BOOST_TEST_EQ(r.value(), 42);
		}
		{
			leaf::result<int> r = leaf::try_handle_some(
				[]() -> leaf::result<int>
				{
					return leaf::new_error( info<1>{1} );
				},
				error_handlers );
			BOOST_TEST(r);
			BOOST_TEST_EQ(r.value(), -42);
		}
	}

	{
		int r = leaf::try_handle_all(
			[]() -> leaf::result<int>
			{
				return 42;
			},
			[]
			{
				return 1;
			} );
		BOOST_TEST_EQ(r, 42);
	}
	{
		int r = leaf::try_handle_all(
			[]() -> leaf::result<int>
			{
				throw leaf::exception( my_exception(), info<1>{1} );
			},
			[]( my_exception const &, info<1> const & x )
			{
				BOOST_TEST_EQ(x.value, 1);
				return 42;
			},
			[]
			{
				return 1;
			} );
		BOOST_TEST_EQ(r, 42);
	}
	{
		int r = leaf::try_handle_all(
			[]() -> leaf::result<int>
			{
				return leaf::new_error( info<1>{1} );
			},
			[]( info<1> const & x )
			{
				BOOST_TEST_EQ(x.value, 1);
				return 42;
			},
			[]
			{
				return 1;
			} );
		BOOST_TEST_EQ(r, 42);
	}

	{
		auto error_handlers = std::make_tuple(
			[]( my_exception const &, info<1> const & x )
			{
				BOOST_TEST_EQ(x.value, 1);
				return 42;
			},
			[]( info<1> const & x )
			{
				BOOST_TEST_EQ(x.value, 1);
				return -42;
			},
			[]
			{
				return 1;
			} );
		{
			int r = leaf::try_handle_all(
				[]() -> leaf::result<int>
				{
					return 42;
				},
				error_handlers );
			BOOST_TEST_EQ(r, 42);
		}
		{
			int r = leaf::try_handle_all(
				[]() -> leaf::result<int>
				{
					throw leaf::exception( my_exception(), info<1>{1} );
				},
				error_handlers );
			BOOST_TEST_EQ(r, 42);
		}
		{
			int r = leaf::try_handle_all(
				[]() -> leaf::result<int>
				{
					return leaf::new_error( info<1>{1} );
				},
				error_handlers );
			BOOST_TEST_EQ(r, -42);
		}
	}

	{
		int r = leaf::try_handle_all(
			[]() -> leaf::result<int>
			{
				return leaf::try_handle_all(
					[]() -> leaf::result<int>
					{
						return leaf::new_error( info<1>{1} );
					},
					[]( info<1> const & ) -> int
					{
						BOOST_LEAF_THROW_EXCEPTION(my_exception());
					},
					[]( leaf::catch_<> )
					{
						return 1;
					},
					[]
					{
						return 2;
					} );
			},
			[]( leaf::catch_<> )
			{
				return 3;
			},
			[]
			{
				return 4;
			} );

		BOOST_TEST_EQ(r, 3);
	}

	{
		auto error_handlers = std::make_tuple(
			[]( info<1> const & ) -> int
			{
				BOOST_LEAF_THROW_EXCEPTION(my_exception());
			},
			[]( leaf::catch_<> )
			{
				return 1;
			},
			[]
			{
				return 2;
			} );
		int r = leaf::try_handle_all(
			[&]() -> leaf::result<int>
			{
				return leaf::try_handle_all(
					[&]() -> leaf::result<int>
					{
						return leaf::new_error( info<1>{1} );
					},
					error_handlers );
			},
			[]( leaf::catch_<> )
			{
				return 3;
			},
			[]
			{
				return 4;
			} );

		BOOST_TEST_EQ(r, 3);
	}

	{
		int r = leaf::try_handle_all(
			[]() -> leaf::result<int>
			{
				return leaf::try_handle_some(
					[]() -> leaf::result<int>
					{
						return leaf::new_error( info<1>{1} );
					},
					[]( info<1> const & ) -> int
					{
						BOOST_LEAF_THROW_EXCEPTION(my_exception());
					},
					[]( leaf::catch_<> )
					{
						return 1;
					} );
			},
			[]( leaf::catch_<> )
			{
				return 3;
			},
			[]
			{
				return 4;
			} );

		BOOST_TEST_EQ(r, 3);
	}

	{
		auto error_handlers = std::make_tuple(
			[]( info<1> const & ) -> leaf::result<int>
			{
				BOOST_LEAF_THROW_EXCEPTION(my_exception());
			},
			[]( leaf::catch_<> ) -> leaf::result<int>
			{
				return 1;
			} );
		int r = leaf::try_handle_all(
			[&]() -> leaf::result<int>
			{
				return leaf::try_handle_some(
					[&]() -> leaf::result<int>
					{
						return leaf::new_error( info<1>{1} );
					},
					error_handlers );
			},
			[]( leaf::catch_<> )
			{
				return 3;
			},
			[]
			{
				return 4;
			} );

		BOOST_TEST_EQ(r, 3);
	}

	return boost::report_errors();
}

#endif
