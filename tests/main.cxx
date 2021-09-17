#include "bencode_parser.hxx"
#include <iostream>

int main() try {
	for(const auto & [key,value] : bencode::parse_from_file("sintel.torrent",bencode::Parsing_Mode::Strict)){
		std::cout << key << ' ';
		// std::cout << std::any_cast<int32_t>(value) << '\n';
	}
}catch(const std::exception & exception){
	std::cout << exception.what() << '\n';
}