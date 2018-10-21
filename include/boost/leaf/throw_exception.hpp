//Copyright (c) 2018 Emil Dotchevski
//Copyright (c) 2018 Second Spectrum, Inc.

//Distributed under the Boost Software License, Version 1.0. (See accompanying
//file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef UUID_0DBBFB5AD3FA11E89F9D20E3C82C3C47
#define UUID_0DBBFB5AD3FA11E89F9D20E3C82C3C47

#include <boost/leaf/error.hpp>
#include <exception>

#define LEAF_THROW(e) ::boost::leaf::throw_exception(e,LEAF_SOURCE_LOCATION)

namespace
boost
	{
	namespace
	leaf
		{
		class
		mismatch_error:
			public std::exception
			{
			};
		namespace
		leaf_detail
			{
			inline void enforce_std_exception( std::exception const & ) { }
			template <class Ex>
			class
			exception:
				public Ex,
				public error
				{
				exception( exception const & )=delete;
				exception & operator=( exception const & )=delete;
				bool moved_;
				public:
				exception( Ex && ex, error && e ):
					Ex(std::move(ex)),
					error(std::move(e)),
					moved_(false)
					{
					enforce_std_exception(*this);
					}
				exception( exception && x ) noexcept:
					Ex(std::move(static_cast<Ex &&>(x))),
					error(std::move(static_cast<error &&>(x))),
					moved_(false)
					{
					x.moved_ = true;
					}
				~exception()
					{
					if( !moved_ )
						clear_current_error_(*this);
					}
				};
			}
		template <class... E,class Ex>
		[[noreturn]]
		void
		throw_exception( Ex && ex, E && ... e )
			{
			throw leaf_detail::exception<Ex>(std::move(ex),error(std::move(e)...));
			}
		}
	}

#endif
