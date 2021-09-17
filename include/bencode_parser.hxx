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

/**
 * @brief Can be used to specify parsing mode strictness.
 *
 * Strict : Do not tolerate any error. Reliable output.
 * Relaxed : Tolerate all possible errors. Might produce unrealible output.
 */
enum class Parsing_Mode { Strict, Relaxed };

/**
 * @brief Exception class thrown when parsing fails.
 */
class bencode_error : public std::exception {
public:
	using exception::exception;

	template<typename T>
	explicit bencode_error(T && error) : what_(std::forward<T>(error)){}

	const char * what [[nodiscard]] () const noexcept override {
		return what_.data();
	}
private:
	std::string_view what_;
};

namespace impl {

using integer_result_type = std::optional<std::pair<std::int64_t,std::size_t>>;
using label_result_type = std::optional<std::pair<std::string,std::size_t>>;
using list_result_type = std::optional<std::pair<std::vector<std::any>,std::size_t>>;
using dict_result_type = std::optional<std::pair<std::map<std::string,std::any>,std::size_t>>;

template<typename T>
integer_result_type extract_integer(T && content,const std::size_t content_length,const Parsing_Mode mode,std::size_t idx){
	assert(idx < content_length);

	if(content[idx] != 'i'){
		return {};
	}

	constexpr std::string_view ending_not_found("Ending character ('e') not found for integral value");

	if(++idx == content_length){
		throw bencode_error(ending_not_found.data());
	}

	const bool negative = content[idx] == '-';
	idx += negative;
	std::int64_t result = 0;

	for(;idx < content_length && content[idx] != 'e';++idx){
		if(std::isdigit(content[idx])){
			result *= 10;
			result += content[idx] - '0';
		}else if(mode == Parsing_Mode::Strict){
			throw bencode_error("Non-digits between 'i' and 'e'");
		}
	}

	if(idx == content_length){
		throw bencode_error(ending_not_found.data());
	}

	assert(content[idx] == 'e');
	return negative ? std::make_pair(-result,idx + 1) : std::make_pair(result,idx + 1);
}

template<typename T>
label_result_type extract_label(T && content,const std::size_t content_length,const Parsing_Mode mode,std::size_t idx){
	assert(idx < content_length);

	if(!std::isdigit(content[idx])){
		return {};
	}

	std::size_t label_length = 0;

	for(;idx < content_length && content[idx] != ':';idx++){
		if(std::isdigit(content[idx])){
			label_length *= 10;
			label_length += static_cast<std::size_t>(content[idx] - '0');
		}else if(mode == Parsing_Mode::Strict){
			throw bencode_error("Invalid character inside label length");
		}
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
			break;
		}

		result += content[idx++];
	}

	return std::make_pair(result,idx);
}

template<typename T>
dict_result_type extract_dictionary(T && content,std::size_t content_length,Parsing_Mode mode,std::size_t idx);

template<typename T>
list_result_type extract_list(T && content,const std::size_t content_length,const Parsing_Mode mode,std::size_t idx){
	assert(idx < content_length);

	if(content[idx] != 'l'){
		return {};
	}

	std::vector<std::any> result;

	for(++idx;idx < content_length;){
		if(content[idx] == 'e'){
			break;
		}
		
		if(const auto integer_opt = extract_integer(std::forward<T>(content),content_length,mode,idx)){
			const auto [integer,forward_idx] = integer_opt.value();
			result.emplace_back(integer);
			idx = forward_idx;
		}else if(const auto label_opt = extract_label(std::forward<T>(content),content_length,mode,idx)){
			auto & [label,forward_idx] = label_opt.value();
			result.emplace_back(std::move(label));
			idx = forward_idx;
		}else if(const auto list_opt = extract_list(std::forward<T>(content),content_length,mode,idx)){
			auto & [list,forward_idx] = list_opt.value();
			result.emplace_back(std::move(list));
			idx = forward_idx;
		}else if(const auto dict_opt = extract_dictionary(std::forward<T>(content),content_length,mode,idx)){
			auto & [dict,forward_idx] = dict_opt.value();
			result.emplace_back(std::move(dict));
			idx = forward_idx;
		}else{
			if(mode == Parsing_Mode::Relaxed){
				break;
			}

			return {};
		}
	}

	return std::make_pair(result,idx + 1);
}

template<typename T>
dict_result_type extract_dictionary(T && content,std::size_t content_length,const Parsing_Mode mode,std::size_t idx){
	assert(idx < content_length);

	if(content[idx] != 'd'){
		return {};
	}

	std::map<std::string,std::any> result;

	for(++idx;idx < content_length;){
		if(content[idx] == 'e'){
			break;
		}
		
		const auto key_opt = extract_label(std::forward<T>(content),content_length,mode,idx);

		if(!key_opt.has_value()){
			if(mode == Parsing_Mode::Relaxed){
				break;
			}

			throw bencode_error("Invalid or non-existent dictionary key");
		}

		auto & [key,key_forward_idx] = key_opt.value();
		idx = key_forward_idx;

		if(const auto integer_opt = extract_integer(std::forward<T>(content),content_length,mode,idx)){
			const auto [integer,forward_idx] = integer_opt.value();
			result.emplace(std::move(key),integer);
			idx = forward_idx;
		}else if(const auto label_opt = extract_label(std::forward<T>(content),content_length,mode,idx)){
			auto & [label,forward_idx] = label_opt.value();
			result.emplace(std::move(key),std::move(label));
			idx = forward_idx;
		}else if(const auto list_opt = extract_list(std::forward<T>(content),content_length,mode,idx)){
			auto & [list,forward_idx] = list_opt.value();
			result.emplace(std::move(key),std::move(list));
			idx = forward_idx;
		}else if(const auto dict_opt = extract_dictionary(std::forward<T>(content),content_length,mode,idx)){
			auto & [dict,forward_idx] = dict_opt.value();
			result.emplace(std::move(key),std::move(dict));
			idx = forward_idx;
		}else{
			if(mode == Parsing_Mode::Relaxed){
				break;
			}

			return {};
		}
	}

	return std::make_pair(result,idx + 1);
}

} // namespace impl

/**
 * @brief Parses the torrent file contents and returns torrent keys mapped to corresponding values.
 *
 * @param content Contents of the torrent file.
 * @param parsing_mode Parsing strictness specifier.. 
 * @return auto :- std::map<std::string,std::any> :- [dictionary_titles,values].
 */
template<typename T>
auto parse_content(T && content,const Parsing_Mode parsing_mode = Parsing_Mode::Strict){
	const auto content_length = std::size(content);

	if(const auto dict_opt = impl::extract_dictionary(std::forward<T>(content),content_length,parsing_mode,0)){
		const auto & [dict,forward_idx] = dict_opt.value();
		return dict;
	}

	return std::map<std::string,std::any>{};
}

/**
 * @brief Parses the torrent file and returns torrent keys mapped to corresponding values.
 * 
 * @param file_path Absolute path of the torrent file.
 * @param parsing_mode Parsing strictness specifier.
 * @return auto :- std::map<std::string,std::any> :- [dictionary_titles,values].
 */
template<typename T>
auto parse_file(T && file_path,const Parsing_Mode parsing_mode = Parsing_Mode::Strict){
	std::ifstream in_fstream(std::forward<T>(file_path));

	if(!in_fstream.is_open()){
		throw bencode_error("File doesn't exist or could not be opened for reading");
	}

	std::string content;

	for(std::string temp;std::getline(in_fstream,temp);content += temp){}

	return parse_content(std::move(content),parsing_mode);
}

namespace experimental {

void print(const std::map<std::string,std::any> & parsed_dict) noexcept;

namespace impl {

inline void print(const std::vector<std::any> & parsed_list) noexcept {
	
	for(const auto & value : parsed_list){
		try{
			std::cout << std::any_cast<std::string>(value) << ' ';
		}catch(const std::bad_any_cast & bad_string_cast){
			try{
				std::cout << std::any_cast<std::int64_t>(value) << ' ';
			}catch(const std::bad_any_cast & bad_int_cast){
				try{
					impl::print(std::any_cast<decltype(parsed_list)>(value));
				}catch(const std::bad_any_cast & bad_vector_cast){
					experimental::print(std::any_cast<std::map<std::string,std::any>>(value));
				}
			}
		}
	}
}

} // namespace impl

/**
 * @warning This function is just for illustration purposes only.
 * @brief Prints the contents of parsed_dictionary by trying all possible types.
 * 
 * @param parsed_dict Parsed torrent file conents returned by bencode::parse_file or bencode::parse_content.
 */
inline void print(const std::map<std::string,std::any> & parsed_dictionary) noexcept {
		
	for(const auto & [dict_key,value] : parsed_dictionary){
		std::cout << dict_key << "  :  ";

		if(dict_key == "pieces"){
			std::cout << "long non-ascii characters (present in actual dict but not being printed)\n";
			continue;
		}

		try{
			std::cout << std::any_cast<std::string>(value) << '\n';
		}catch(const std::bad_any_cast & bad_string_cast){
			try{
				std::cout << std::any_cast<std::int64_t>(value) << '\n';
			}catch(const std::bad_any_cast & bad_int_cast){
				try{
					impl::print(std::any_cast<std::vector<std::any>>(value));
					std::cout << '\n';
				}catch(const std::bad_any_cast & bad_vector_cast){
					print(std::any_cast<decltype(parsed_dictionary)>(value));
					std::cout << '\n';
				}
			}
		}
	}
}

} // namespace experimental

} // namespace bencode

#endif // BENCODE_PARSER_HXX