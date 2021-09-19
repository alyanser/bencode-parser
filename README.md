# bencode-parser

<b>C++ Bencode parser</b>

<b>Api</b>
<pre>
bencode::result_type result = bencode::parse_file(path_to_bencoded_file)
or
bencode::result_type result = bencode::parse_content(bencoded_content)

bencode::Metadata metadata = bencode::extract_metadata(result)

// utility
std::string decoded_str = bencode::convert_to_string(metadata) // useful for regex
bencode::dump_content(result)

See the header file for more details
</pre>

<b>Usage</b><br>
<pre>
See examples/example.cxx
</pre>

<b>Installation</b>
<pre>
Just include bencode_parser.hxx
</pre>
</pre>
<b>Parsing Mode</b>
<pre>
Pass any of these enum constants to the functions mentioned in Api.<Br>
<b>bencode::Parsing_Mode::Strict</b> (default) to consider any syntax error as fatal and throw.
<b>bencode::Parsing_Mode::Relaxed</b> to ignore as many errors as possible.
</pre>
<b>Testing</b>
<pre>
git clone https://github.com/alyanser/bencode-parser
cd bencode-parser && mkdir build && cd build 
cmake .. && make && ./bencode-parser 
</pre>

<b>Sample metadata result returned by bencode::convert_to_string</b>
<pre>
--- big-buck-bunny.torrent taken from https://webtorrent.io/free-torrents ---

- Name : 

Big Buck Bunny

- Content type : 

Directory

- Total Size 

276445467

- Announce URL : 

udp://tracker.leechers-paradise.org:6969

- Created by : 

WebTorrent <https://webtorrent.io>

- Creation date : 

1490916601

- Comment : 

WebTorrent <https://webtorrent.io>

- Encoding : 

UTF-8

- Piece length : 

262144

- Announce list : 

udp://tracker.leechers-paradise.org:6969 udp://tracker.coppersurfer.tk:6969 udp://tracker.opentrackr.org:1337

Files information:

	Path : Big Buck Bunny.en.srt	Size : 140
	Path : Big Buck Bunny.mp4	Size : 276134947
	Path : poster.jpg	Size : 310380

</pre>
