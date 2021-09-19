#include "bencode_parser.hxx"

int main(){
	// const auto result = bencode::parse_file("../examples/sample-input/sintel.torrent");
	const auto result = bencode::parse_file("/home/ali/Downloads/wired-cd.torrent");
	const auto metadata = bencode::extract_metadata(result);
	const auto metadata_str = bencode::convert_to_string(metadata);

	// uncomment to dump all information
	bencode::dump_content(result);
}