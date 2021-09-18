#include "bencode_parser.hxx"

int main(){
	std::cout << "--- big-buck-bunny.torrent taken from https://webtorrent.io/free-torrents ---\n\n";

	const auto result = bencode::parse_file("../tests/big-buck-bunny.torrent");
	
	if(auto itr = result.find("created by");itr != result.end()){
		const auto & [dictionary_title,value] = *itr;
		const auto created_by = std::any_cast<std::string>(value);
		// ...
	}

	bencode::dump_content(result);
}