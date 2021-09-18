#include "bencode_parser.hxx"

int main(){
	const auto result = bencode::parse_file("../examples/sample-input/big-buck-bunny.torrent");
	const auto metadata = bencode::extract_metadata(result);

	std::cout << "Name : " << metadata.name << '\n';
	std::cout << "Announce : " << metadata.announce << '\n';
	std::cout << "Created by : " << metadata.created_by << '\n';
	std::cout << "Creation date : " << metadata.creation_date << '\n';
	std::cout << "Comment : " << metadata.comment << '\n';
	std::cout << "Encoding : " << metadata.encoding << '\n';
	std::cout << "Piece length : " << metadata.piece_length << '\n';
	std::cout << "Announce list : ";

	for(const auto & announce : metadata.announce_list){
		std::cout << announce << ' ';
	}

	std::cout << "\nFiles information:\n";

	for(const auto & [file_path,file_size] : metadata.file_info){
		std::cout << "\tPath : " << file_path << "\tSize : " << file_size << '\n';
	}

	// uncomment to dump all information
	// bencode::dump_content(result);
}