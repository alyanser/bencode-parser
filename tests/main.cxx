#include "bencode_parser.hxx"
#include <iostream>

int main(){
	bencode::parse_from_file("sample.torrent");
}