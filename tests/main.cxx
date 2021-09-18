#include "bencode_parser.hxx"

int main(){
	std::cout << "--- big-buck-bunny.torrent taken from https://webtorrent.io/free-torrents ---\n\n";
	bencode::experimental::print(bencode::parse_file("../tests/big-buck-bunny.torrent"));
	std::cout << "--- sintel.torrent taken from https://webtorrent.io/free-torrents ---\n\n";
	bencode::experimental::print(bencode::parse_file("../tests/sintel.torrent"));
}