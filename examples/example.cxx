#include "bencode_parser.hxx"

int main(){
	const auto result = bencode::parse_file("../examples/sample-input/big-buck-bunny.torrent");
	const auto metadata = bencode::extract_metadata(result);
	std::cout << bencode::convert_to_string(metadata);
	// uncomment to dump all information
	// bencode::dump_content(result);
}