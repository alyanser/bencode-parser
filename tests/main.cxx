#include "bencode_parser.hxx"

int main(){
	std::cout << "--- big-buck-bunny.torrent taken from https://webtorrent.io/free-torrents ---\n\n";
	bencode::print(bencode::parse_file("../tests/big-buck-bunny.torrent"));
}