#include "bencode_parser.h"
#include <iostream>

int main() {
	constexpr std::string_view sample_input_path = "../examples/sample-input/big-buck-bunny.torrent";

	const auto result = bencode::parse_file(sample_input_path);
	const auto metadata = bencode::extract_metadata(result);

	// only evaluate dictionary standard dictionary keys
	std::cout << bencode::convert_to_string(metadata) << '\n';

	for(const auto & [file_path, size] : metadata.file_info) {
		std::cout << file_path << ' ' << size << '\n';
	}

	// possibly containing non-standard compliant keys and values
	// std::cout << bencode::convert_to_string(result) << '\n';

	// std::cout << metadata.raw_info_dict << '\n';
}