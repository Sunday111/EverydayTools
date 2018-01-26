#pragma once

#include <exception>
#include <iterator>
#include <ostream>

void PrintException(std::ostream& output, const std::exception& e, int level = 0) {
	auto print_level = [&output, level]() -> std::ostream& {
		std::fill_n(std::ostream_iterator<char>(output), level, ' ');
		return output;
	};

	print_level() << "exception: " << e.what() << '\n';

	try {
		std::rethrow_if_nested(e);
	} catch (const std::exception& e) {
		PrintException(output, e, level + 1);
	} catch (...) {
		print_level() << "exception: unknown\n";
	}
}