# bencode-parser

<b>C++ Bencode parser</b>

<b>Api</b>
<pre>
bencode::result_type result = bencode::parse_file(file_path);
or
bencode::result_type result = bencode::parse_content(content);

bencode::dump_content(result);
</pre>

<b>Usage</b><br>
<pre>
See examples/example.cxx
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

<b>Sample output of bencode::dump_content for a torrent file</b>
<pre>
--- big-buck-bunny.torrent taken from https://webtorrent.io/free-torrents ---

announce  :  udp://tracker.leechers-paradise.org:6969
announce-list  :  udp://tracker.leechers-paradise.org:6969 udp://tracker.coppersurfer.tk:6969 ...
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
pieces  :  possibly long non-ascii characters (present in actual dict but not being printed)
</pre>
