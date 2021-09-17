#ifndef BENCODE_PARSER_HXX
#define BENCODE_PARSER_HXX

#include <fstream>
#include <string>
#include <map>
#include <cassert>
#include <utility>
#include <any>
#include <optional>
#include <vector>

namespace bencode {

namespace impl {

template<typename T>
std::optional<std::pair<int64_t,size_t>> extract_integer(T && content,const size_t content_length,size_t idx) noexcept {
	assert(idx < content_length);

	if(!std::isdigit(content[idx]) && content[idx] != '-' && content[idx] != 'i'){
		return {};
	}
 
	bool is_negative = false;

	if(content[idx] == 'i'){
		if(++idx == content_length){
			return {};
		}

		is_negative |= content[idx] == '-';
	}else if(content[idx] == '-'){
		//! check if its possible according to the standard
		return {};
	}

	int64_t result = 0;

	for(++idx;idx < content_length && content[idx] != 'e' && content[idx] != ':';++idx){
		if(!std::isdigit(content[idx])){
			return {};
		}

		result += content[idx] - '0';
		result *= 10;
	}

	if(idx == content_length || (content[idx] != ':' && content[idx] != 'e')){
		return {};
	}

	return is_negative ? std::make_pair(-result,idx) : std::make_pair(result,idx);
}

template<typename T>
std::optional<std::pair<std::string,size_t>> extract_label(T && content,const size_t content_length,size_t idx) noexcept {
	assert(idx < content_length);

	if(content[idx] == '-'){
		return {};
	}
	
	//! cannot be const ig
	if(const auto label_length_opt = extract_integer(std::forward<T>(content),idx,content_length)){

		auto [label_length,forwarded_idx] = label_length_opt.value();

		assert(content[forwarded_idx] == ':');
		idx = forwarded_idx + 1;

		std::string result;

		while(label_length--){
			if(idx >= content_length){
				return {};
			}

			result += content[idx++];
		}

		return std::make_pair(result,idx);
	}

	return {};
}

template<typename T>
std::optional<std::pair<std::map<std::string,std::any>,size_t>> extract_dict(T && content,size_t content_length,size_t idx) noexcept;

template<typename T>
std::optional<std::pair<std::vector<std::any>,size_t>> extract_list(T && content,const size_t content_length,size_t idx) noexcept {
	assert(idx < content_length);

	if(content[idx] != 'l'){
		return {};
	}

	std::vector<std::any> result;

	for(++idx;idx < content_length;){
		if(const auto integer_opt = extract_integer(std::forward<T>(content),content_length,idx)){
			const auto [integer,forward_idx] = integer_opt.value();

			result.push_back(integer);
			idx = forward_idx;
		}else if(const auto label_opt = extract_label(std::forward<T>(content),content_length,idx)){
			const auto & [label,forward_idx] = label_opt.value();

			result.push_back(std::move(label));
			idx = forward_idx;
		}else if(const auto list_opt = extract_list(std::forward<T>(content),content_length,idx)){
			const auto & [list,forward_idx] = list_opt.value();

			result.push_back(std::move(list));
			idx = forward_idx;
		}else if(const auto dict_opt = extract_dict(std::forward<T>(content),content_length,idx)){
			const auto & [dict,forward_idx] = dict_opt.value();
			
			result.push_back(std::move(dict));
			idx = forward_idx;
		}else{
			return {};
		}
	}

	return std::make_pair(result,idx);
}

template<typename T>
std::optional<std::pair<std::map<std::string,std::any>,size_t>> extract_dict(T && content,size_t content_length,size_t idx) noexcept {
	assert(idx < content_length);

	if(content[idx] != 'd'){
		return {};
	}

	std::map<std::string,std::any> result;

	for(++idx;idx < content_length;){
		const auto key_opt = extract_label(std::forward<T>(content),content_length,idx);

		if(!key_opt.has_value()){
			break;
		}

		const auto & [key,key_forward_idx] = key_opt.value();

		idx = key_forward_idx;
		
		if(const auto integer_opt = extract_integer(std::forward<T>(content),content_length,idx)){
			const auto [integer,forward_idx] = integer_opt.value();

			result.emplace(key,integer);
			idx = forward_idx;
		}else if(const auto label_opt = extract_label(std::forward<T>(content),content_length,idx)){
			const auto & [label,forward_idx] = label_opt.value();

			result.emplace(key,std::move(label));
			idx = forward_idx;
		}else if(const auto list_opt = extract_list(std::forward<T>(content),content_length,idx)){
			const auto & [list,forward_idx] = list_opt.value();

			result.emplace(key,std::move(list));
			idx = forward_idx;
		}else if(const auto dict_opt = extract_dict(std::forward<T>(content),content_length,idx)){
			const auto & [dict,forward_idx] = dict_opt.value();
			
			result.emplace(key,std::move(dict));
			idx = forward_idx;
		}else{
			return {};
		}
	}

	return std::make_pair(result,idx);
}

} // namespace impl

template<typename T>
inline auto parse_from_content(T && content){
	const auto content_length = std::size(content);
	return impl::extract_dict(std::forward<T>(content),content_length,0);
}

template<typename T>
inline auto parse_from_file(T && file_path){
	std::ifstream in_fstream(std::forward<T>(file_path));

	if(!in_fstream.is_open()){
		//todo find appropriate thing
		throw std::invalid_argument("File could not be opened");
	}

	std::string content;

	for(std::string temp;std::getline(in_fstream,temp);content += temp){}

	return parse_from_content(std::move(content));
}

} // namespace bencode

#endif // BENCODE_PARSER_HXX