# bencode-parser

<b>C++ Bencode parser</b>

<b>Api</b>
<pre>
bencode::result_type result = bencode::parse_file(file_path);
or
bencode::result_type result = bencode::parse_content(content);

bencode::Metadata metadata = bencode::extract_metadata(result);
bencode::dump_content(result);
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

<b>Sample metadata result returned by bencode::extract_metadata</b>
<pre>
--- big-buck-bunny.torrent taken from https://webtorrent.io/free-torrents ---

Name : Big Buck Bunny
Announce : udp://tracker.leechers-paradise.org:6969
Created by : WebTorrent <https://webtorrent.io>
Creation date : 1490916601
Comment : WebTorrent <https://webtorrent.io>
Encoding : UTF-8
Piece length : 262144
Announce list : udp://tracker.leechers-paradise.org:6969 udp://tracker.coppersurfer.tk:6969 ...
Files information:
	Path : Big Buck Bunny.en.srt	Size : 140
	Path : Big Buck Bunny.mp4	Size : 276134947
	Path : poster.jpg	Size : 310380
  
...
</pre>
