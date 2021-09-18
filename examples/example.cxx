#include "bencode_parser.hxx"

int main(){
	const auto result = bencode::parse_file("../examples/sample-input/big-buck-bunny.torrent");

	if(auto itr = result.find("name");itr != result.end()){
		const auto & [dictionary_title,value] = *itr;
		[[maybe_unused]] const auto name = std::any_cast<std::string>(value);
		// ...
	}

	if(auto itr = result.find("creation date");itr != result.end()){
		const auto & [dictionary_title,value] = *itr;
		[[maybe_unused]] const auto creation_date = std::any_cast<std::int64_t>(value);
		// ...
	}

	bencode::dump_content(result);
}