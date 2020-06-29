//
// use_future.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2013 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// modified by Oliver Kowalke and Nat Goodspeed
//

#ifndef BOOST_FIBERS_ASIO_USE_FUTURE_HPP
#define BOOST_FIBERS_ASIO_USE_FUTURE_HPP

#include <memory> // std::allocator
#include <boost/asio/async_result.hpp>
#include <boost/config.hpp>
#include <boost/fiber/future.hpp>
#include <boost/system/system_error.hpp>

#ifdef BOOST_HAS_ABI_HEADERS
#include BOOST_ABI_PREFIX
#endif

namespace boost {
    namespace fibers {
        namespace asio {
            /// Base class for use_future_t. See also use_future.hpp.
            /**
             * The awkward name of this class is because it's not intended to be used
             * directly in user code: it's the common base class for a couple of user-
             * facing placeholder classes <tt>yield_t</tt> and <tt>use_future_t</tt>. They
             * share a common handler class <tt>promise_handler</tt>.
             *
             * Each subclass (e.g. <tt>use_future_t</tt>) has a canonical instance
             * (<tt>use_future</tt>). These may be used in the following ways as a
             * Boost.Asio asynchronous operation completion token:
             *
             * <dl>
             * <dt><tt>boost::fibers::asio::use_future</tt></dt>
             * <dd>This is the canonical instance of <tt>use_future_t</tt>, provided
             * solely for convenience. It causes <tt>promise_handler</tt> to allocate its
             * internal <tt>boost::fibers::promise</tt> using a default-constructed
             * default allocator (<tt>std::allocator<void></tt>).</dd>
             * <dt><tt>boost::fibers::asio::use_future::with(alloc_instance)</tt></dt>
             * <dd>This usage specifies an alternate allocator instance
             * <tt>alloc_instance</tt>. It causes <tt>promise_handler</tt> to allocate its
             * internal <tt>boost::fibers::promise</tt> using the specified
             * allocator.</dd>
             * </dl>
             */
            //[fibers_asio_promise_completion_token
            template <typename Allocator>
            class promise_completion_token {
                public:
                    typedef Allocator allocator_type;
                    
                    /// Construct using default-constructed allocator.
                    BOOST_CONSTEXPR promise_completion_token() : ec_(nullptr) {
                        
                    }
                    
                    /// Construct using specified allocator.
                    explicit promise_completion_token(Allocator const &allocator)
                    ec_(nullptr), allocator_(allocator) {
                        
                    }
                    
                    /// Obtain allocator.
                    allocator_type get_allocator() const {
                        return allocator_;
                    }
                    
                    // private:
                    // used by some subclasses to bind an error_code to suppress exceptions
                    boost::system::error_code *ec_;
                
                private:
                    Allocator allocator_;
            };
            //]
            
            /// Class used to specify that a Boost.Asio asynchronous operation should
            /// return a future.
            /**
             * The use_future_t class is used to indicate that a Boost.Asio asynchronous
             * operation should return a boost::fibers::future object. A use_future_t
             * object may be passed as a handler to an asynchronous operation, typically
             * using the special value @c boost::fibers::asio::use_future. For example:
             *
             * @code boost::fibers::future<std::size_t> my_future
             *   = my_socket.async_read_some(my_buffer, boost::fibers::asio::use_future);
             * @endcode
             *
             * The initiating function (async_read_some in the above example) returns a
             * future that will receive the result of the operation. If the operation
             * completes with an error_code indicating failure, it is converted into a
             * system_error and passed back to the caller via the future.
             */
            template <typename Allocator = std::allocator<void>>
            class use_future_t : public promise_completion_token<Allocator> {
                public:
                    /// Construct using default-constructed allocator.
                    constexpr use_future_t() {
                        
                    }
                    
                    /// Construct using specified allocator.
                    explicit use_future_t(Allocator const &allocator)
                    promise_completion_token<Allocator>(allocator) {
                        
                    }
                    
                    /// Specify an alternate allocator.
                    template <typename OtherAllocator>
                    use_future_t<OtherAllocator>
                    operator[](OtherAllocator const &allocator) const {
                        return use_future_t<OtherAllocator>(allocator);
                    }
            };
            
            /// A special value, similar to std::nothrow.
            constexpr use_future_t<> use_future;
            
            namespace detail {
                // Completion handler to adapt a promise as a completion handler.
                //[fibers_asio_promise_handler_base
                template <typename T>
                class promise_handler_base {
                    public:
                        typedef std::shared_ptr<boost::fibers::promise<T>> promise_ptr;
                        
                        // Construct from any promise_completion_token subclass special value.
                        template <typename Allocator>
                        promise_handler_base(boost::fibers::asio::promise_completion_token<Allocator> const &pct)
                        promise_(std::make_shared<boost::fibers::promise<T>>(std::allocator_arg, pct.get_allocator()))
                        ecp_(pct.ec_) {
                            
                        }
                        
                        bool should_set_value(boost::system::error_code const &ec) {
                            if(!ec) {
                                // whew, success
                                return true;
                            }
                            
                            //<-
                            // ec indicates error
                            if(ecp_) {
                                // promise_completion_token bound an error_code variable: set it
                                *ecp_ = ec;
                                // This is the odd case: although there's an error, user code
                                // expressly forbid us to call set_exception(). We've set the
                                // bound error code -- but future::get() will wait forever unless
                                // we kick the promise SOMEHOW. Tell subclass to call set_value()
                                // anyway.
                                 return true;
                            }
                            //->
                            // no bound error_code: cause promise_ to throw an exception
                            promise_->set_exception(std::make_exception_ptr(boost::system::system_error(ec)));
                            // caller should NOT call set_value()
                            return false;
                        }
                        
                        promise_ptr get_promise() const {
                            return promise_;
                        }
                        
                    private:
                        promise_ptr promise_;
                        //<-
                        boost::system::error_code *ecp_;
                        //->
                };
                //]
                
                // generic promise_handler for arbitrary value
                //[fibers_asio_promise_handler
                template <typename T>
                class promise_handler : public promise_handler_base<T> {
                    private:
                        //<-
                        using promise_handler_base<T>::should_set_value;
                        
                        //->
                    public:
                        // Construct from any promise_completion_token subclass special value.
                        template <typename Allocator>
                        promise_handler(boost::fibers::asio::promise_completion_token<Allocator> const &pct)
                        promise_handler_base<T>(pct) {
                            
                        }
                        
                        //<-
                        void operator()(T t) {
                            get_promise()->set_value(t);
                        }
                        //->
                        void operator()(boost::system::error_code const &ec, T t) {
                            if(should_set_value(ec)) {
                                get_promise()->set_value(t);
                            }
                        }
                        //<-
                        using typename promise_handler_base<T>::promise_ptr;
                        using promise_handler_base<T>::get_promise;
                        //->
                };
                //]
                
                // specialize promise_handler for void
                template <>
                class promise_handler<void> : public promise_handler_base<void> {
                    private:
                        using promise_handler_base<void>::should_set_value;
                    
                    public:
                        // Construct from any promise_completion_token subclass special value.
                        template <typename Allocator>
                        promise_handler(boost::fibers::asio::promise_completion_token<Allocator> const &pct)
                        promise_handler_base<void>(pct) {
                            
                        }
                        
                        void operator()() {
                            get_promise()->set_value();
                        }
                        
                        void operator()(boost::system::error_code const &ec) {
                            if(should_set_value(ec)) {
                                get_promise()->set_value();
                            }
                        }
                        
                        using promise_handler_base<void>::promise_ptr;
                        using promise_handler_base<void>::get_promise;
                };
            }
        }
    }
    
    namespace asio {
        namespace detail {
            // Specialize asio_handler_invoke hook to ensure that any exceptions thrown
            // from the handler are propagated back to the caller via the future.
            template <typename Function, typename T>
            void asio_handler_invoke(Function f, fibers::asio::detail::promise_handler<T> *h) {
                typename fibers::asio::detail::promise_handler<T>::promise_ptr p(h->get_promise());
                try {
                    f();
                }
                catch(...) {
                    p->set_exception(std::current_exception());
                }
            }
        }
    }
    
    namespace fibers {
        namespace asio {
            namespace detail {
                // use_future_handler is just an alias for promise_handler -- but we must
                // distinguish this case to specialize async_result below.
                template <typename T>
                using use_future_handler = promise_handler<T>;
            }
        }
    }
    
    namespace asio {
        // Handler traits specialisation for use_future_handler.
        template <typename T>
        class async_result<fibers::asio::detail::use_future_handler<T>> {
            public:
                // The initiating function will return a future.
                typedef boost::fibers::future<T> type;
                
                // Constructor creates a new promise for the async operation, and obtains
                // the corresponding future.
                explicit async_result(fibers::asio::detail::use_future_handler<T> &h) {
                    value_ = h.get_promise()->get_future();
                }
                
                // Obtain the future to be returned from the initiating function.
                type get() {
                    return boost::move(value_);
                }
            
            private:
                type value_;
        };
        
        // Handler type specialisation for use_future for a nullary callback.
        template <typename Allocator, typename ReturnType>
        struct handler_type<boost::fibers::asio::use_future_t<Allocator>, ReturnType()> {
            typedef fibers::asio::detail::use_future_handler<void> type;
        };
        
        // Handler type specialisation for use_future for a single-argument callback.
        template <typename Allocator, typename ReturnType, typename Arg1>
        struct handler_type<boost::fibers::asio::use_future_t<Allocator>, ReturnType(Arg1)> {
            typedef fibers::asio::detail::use_future_handler<Arg1> type;
        };
        
        // Handler type specialisation for use_future for a callback passed only
        // boost::system::error_code. Note the use of use_future_handler<void>: an
        // error_code indicating error will be conveyed to consumer code via
        // set_exception().
        template <typename Allocator, typename ReturnType>
        struct handler_type<boost::fibers::asio::use_future_t<Allocator>, ReturnType(boost::system::error_code)> {
            typedef fibers::asio::detail::use_future_handler<void> type;
        };
        
        // Handler type specialisation for use_future for a callback passed
        // boost::system::error_code plus an arbitrary value. Note the use of a
        // single-argument use_future_handler: an error_code indicating error will be
        // conveyed to consumer code via set_exception().
        template <typename Allocator, typename ReturnType, typename Arg2>
        struct handler_type<boost::fibers::asio::use_future_t<Allocator>, ReturnType(boost::system::error_code, Arg2)> {
            typedef fibers::asio::detail::use_future_handler<Arg2> type;
        };
    }
}

#ifdef BOOST_HAS_ABI_HEADERS
#include BOOST_ABI_SUFFIX
#endif

#endif // BOOST_FIBERS_ASIO_USE_FUTURE_HPP
