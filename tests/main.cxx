#include "bencode_parser.hxx"
#include <array>
#include <string_view>

int main(){
	std::cout << "--- big-buck-bunny.torrent ---\n\n";
	bencode::print(bencode::parse_file("../tests/big-buck-bunny.torrent"));
	std::cout << "--- sintel.torrent ---\n\n";
	bencode::print(bencode::parse_file("../tests/sintel.torrent"));
}