#include "bencode_parser.hxx"

int main(){
	const auto result = bencode::parse_file("../examples/sample-input/big-buck-bunny.torrent");
	const auto metadata = bencode::extract_metadata(result);

	// possibly containing non-standard compliant keys and values
	std::cout << bencode::convert_to_string(result) << '\n';

	// only evaluate dictionary standard dictionary keys
	std::cout << bencode::convert_to_string(metadata) << '\n';

	std::cout << metadata.raw_info_dict << '\n';
}