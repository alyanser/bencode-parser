# bencode-parser

<b>C++ Header only Bencode Parser.</b>

<b>Api:</b>
<pre>
bencode::result_type result = bencode::parse_file(file_path);
or
bencode::result_type result = bencode::parse_content(content);

bencode::experimental::print(result);
</pre>

<b>Usage</b>
<pre>
std::any_cast the following types to extract the result from bencode::result_type

std::string for labels
std::int64_t for integers
bencode::list_type for nested lists
bencode::dictionary_type for nested dictionaries

See the header file for more details
</pre>

<b>Testing</b>
<pre>
git clone https://github.com/alyanser/bencode-parser
cd bencode-parser && mkdir build && cd build 
cmake .. && make && ./bencode-parser
</pre>

<b>Sample output for a torrent file</b>
<pre>
--- big-buck-bunny.torrent taken from https://webtorrent.io/free-torrents ---

announce  :  udp://tracker.leechers-paradise.org:6969
announce-list  :  udp://tracker.leechers-paradise.org:6969 udp://tracker.coppersurfer.tk:6969
comment  :  WebTorrent <https://webtorrent.io>
created by  :  WebTorrent <https://webtorrent.io>
creation date  :  1490916601
encoding  :  UTF-8
info  :  files  :  length  :  140
path  :  Big Buck Bunny.en.srt 
length  :  276134947
path  :  Big Buck Bunny.mp4 
length  :  310380
path  :  poster.jpg 
name  :  Big Buck Bunny
piece length  :  262144
pieces  :  long non-ascii characters (present in actual dict but not being printed)
</pre>
