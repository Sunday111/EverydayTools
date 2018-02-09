#pragma once

#include <stdexcept>

template<typename Exception = std::runtime_error, typename TChar, typename F>
decltype(auto) CallAndRethrow(const TChar* rethrowMessage, F&& f) {
	try {
		return static_cast<decltype(f.operator()())>(f());
	} catch (...) {
		std::throw_with_nested(Exception(rethrowMessage));
	}
}

namespace call_and_rethrow_helpers {
    struct Caller {
        Caller(const char* name) :
            m_name(name)
        {
        }

        const char* m_name = nullptr;
    
        template<typename Functor>
        decltype(auto) operator+(Functor&& functor) const {
            return CallAndRethrow(m_name, std::move(functor));
        }
    };
}

#ifdef _MSC_VER
#define CallAndRethrowM call_and_rethrow_helpers::Caller(__FUNCTION__)
#elif
// Need some warning here?
#define CallAndRethrowM call_and_rethrow_helpers::Caller(__func__)
#endif