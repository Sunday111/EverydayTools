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