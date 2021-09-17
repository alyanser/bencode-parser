#include "bencode_parser.hxx"

int main(){
	std::cout << "--- big-buck-bunny.torrent ---\n\n";
	bencode::experimental::print(bencode::parse_file("../tests/big-buck-bunny.torrent"));
	std::cout << "--- sintel.torrent ---\n\n";
	bencode::experimental::print(bencode::parse_file("../tests/sintel.torrent"));
}