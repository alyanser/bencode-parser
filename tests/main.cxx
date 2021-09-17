#include "bencode_parser.hxx"
#include <iostream>

int main() try {
	const auto result = bencode::parse_from_file("sample.torrent");

	for(const auto & [key,value] : result){
		std::cout << key << '\n';
		// std::cout << std::any_cast<int32_t>(value) << '\n';
	}
}catch(const std::exception & exception){
	std::cout << exception.what() << '\n';
}