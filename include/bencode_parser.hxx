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
#include <iostream>

namespace bencode {

class bencode_error : std::exception {
public:
	using exception::exception;

	template<typename T>
	explicit bencode_error(T && error) : what_(error){}

	const char * what [[nodiscard]] () const noexcept override {
		return what_.data();
	}
private:
	std::string_view what_;
};

namespace impl {

using integer_result_type = std::optional<std::pair<int64_t,size_t>>;
using label_result_type = std::optional<std::pair<std::string,size_t>>;
using list_result_type = std::optional<std::pair<std::vector<std::any>,size_t>>;
using dict_result_type = std::optional<std::pair<std::map<std::string,std::any>,size_t>>;

template<typename T>
integer_result_type extract_integer(T && content,const size_t content_length,size_t idx){
	assert(idx < content_length);

	if(content[idx] != 'i'){
		return {};
	}

	constexpr std::string_view ending_not_found("Ending character ('e') not found for integral value");

	if(++idx == content_length){
		throw bencode_error(ending_not_found.data());
	}

	int64_t result = 0;
	const bool negative = content[idx] == '-';

	idx += negative;

	for(;idx < content_length && content[idx] != 'e';++idx){
		if(!std::isdigit(content[idx])){
			throw bencode_error("Non-digits between 'i' and 'e'");
		}

		result *= 10;
		result += content[idx] - '0';
	}

	if(idx == content_length){ // could not spot 'e'
		throw bencode_error(ending_not_found.data());
	}

	assert(content[idx] == 'e');
	return negative ? std::make_pair(-result,idx + 1) : std::make_pair(result,idx + 1);
}

template<typename T>
label_result_type extract_label(T && content,const size_t content_length,size_t idx){
	assert(idx < content_length);

	if(!std::isdigit(content[idx])){
		return {};
	}

	size_t label_length = 0;

	for(;idx < content_length && content[idx] != ':';idx++){
		if(!std::isdigit(content[idx])){
			throw bencode_error("Invalid character inside label length");
		}

		label_length *= 10;
		label_length += static_cast<size_t>(content[idx] - '0');
	}

	if(idx >= content_length){
		throw bencode_error("Label separator (:) was not found");
	}

	assert(content[idx] == ':');
	++idx;

	std::string result;
	result.reserve(label_length);

	while(label_length--){
		if(idx >= content_length){
			throw bencode_error("Label doesn't have enough characters as the associated length");
		}

		result += content[idx++];
	}

	return std::make_pair(result,idx);
}

template<typename T>
dict_result_type extract_dict(T && content,size_t content_length,size_t idx);

template<typename T>
list_result_type extract_list(T && content,const size_t content_length,size_t idx){
	assert(idx < content_length);

	if(content[idx] != 'l'){
		return {};
	}

	std::vector<std::any> result;

	for(++idx;idx < content_length;){
		if(content[idx] == 'e'){
			break;
		}
		
		if(const auto integer_opt = extract_integer(std::forward<T>(content),content_length,idx)){
			const auto [integer,forward_idx] = integer_opt.value();
			result.emplace_back(integer);
			idx = forward_idx;
		}else if(const auto label_opt = extract_label(std::forward<T>(content),content_length,idx)){
			auto & [label,forward_idx] = label_opt.value();
			result.emplace_back(std::move(label));
			idx = forward_idx;
		}else if(const auto list_opt = extract_list(std::forward<T>(content),content_length,idx)){
			auto & [list,forward_idx] = list_opt.value();
			result.emplace_back(std::move(list));
			idx = forward_idx;
		}else if(const auto dict_opt = extract_dict(std::forward<T>(content),content_length,idx)){
			auto & [dict,forward_idx] = dict_opt.value();
			result.emplace_back(std::move(dict));
			idx = forward_idx;
		}else{
			return {};
		}
	}

	return std::make_pair(result,idx + 1);
}

template<typename T>
dict_result_type extract_dict(T && content,size_t content_length,size_t idx){
	assert(idx < content_length);

	if(content[idx] != 'd'){
		return {};
	}

	std::map<std::string,std::any> result;

	for(++idx;idx < content_length;){
		if(content[idx] == 'e'){
			break;
		}
		
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
			auto & [label,forward_idx] = label_opt.value();
			result.emplace(key,std::move(label));
			idx = forward_idx;
		}else if(const auto list_opt = extract_list(std::forward<T>(content),content_length,idx)){
			auto & [list,forward_idx] = list_opt.value();
			result.emplace(key,std::move(list));
			idx = forward_idx;
		}else if(const auto dict_opt = extract_dict(std::forward<T>(content),content_length,idx)){
			auto & [dict,forward_idx] = dict_opt.value();
			result.emplace(key,std::move(dict));
			idx = forward_idx;
		}else{
			return {};
		}
	}

	return std::make_pair(result,idx + 1);
}

} // namespace impl

template<typename T>
inline std::map<std::string,std::any> parse_from_content(T && content){
	const auto content_length = std::size(content);

	if(const auto dict_opt = impl::extract_dict(std::forward<T>(content),content_length,0)){
		const auto & [dict,forward_idx] = dict_opt.value();
		return dict;
	}

	return {};
}

template<typename T>
auto parse_from_file(T && file_path){
	std::ifstream in_fstream(std::forward<T>(file_path));

	if(!in_fstream.is_open()){
		throw bencode_error("File doesn't exist or could not be opened for writing");
	}

	std::string content;

	for(std::string temp;std::getline(in_fstream,temp);content += temp){}

	return parse_from_content(std::move(content));
}

} // namespace bencode

#endif // BENCODE_PARSER_HXX