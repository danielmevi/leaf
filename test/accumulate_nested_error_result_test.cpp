// Copyright (c) 2018-2019 Emil Dotchevski
// Copyright (c) 2018-2019 Second Spectrum, Inc.

// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/leaf/preload.hpp>
#include <boost/leaf/handle_all.hpp>
#include <boost/leaf/result.hpp>
#include "boost/core/lightweight_test.hpp"

namespace leaf = boost::leaf;

template <int>
struct info
{
	int value;
};

leaf::error_id f0()
{
	auto propagate = leaf::accumulate( [ ]( info<0> & ) { } );
	return leaf::new_error( info<2>{2} );
}

leaf::error_id f1()
{
	auto propagate1 = leaf::preload( info<0>{-1}, info<2>{-1} );
	auto propagate2 = leaf::accumulate( [ ]( info<1> & x ) {++x.value;} );
	return f0();
}

leaf::error_id f2()
{
	return f1().propagate( info<3>{3} );
}

int main()
{
	int r = leaf::handle_all(
		[ ]() -> leaf::result<int>
		{
			return f2();
		},
		[ ]( info<0> i0, info<1> i1, info<2> i2, info<3> i3 )
		{
			BOOST_TEST_EQ(i0.value, 0);
			BOOST_TEST_EQ(i1.value, 1);
			BOOST_TEST_EQ(i2.value, 2);
			BOOST_TEST_EQ(i3.value, 3);
			return 1;
		},
		[ ]
		{
			return 2;
		} );
	BOOST_TEST_EQ(r, 1);

	return boost::report_errors();
}