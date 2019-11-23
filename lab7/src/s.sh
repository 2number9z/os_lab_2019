kill $(pgrep tcpserver)
rm tcpserver
rm tcpclient
gcc tcpserver.c -o tcpserver
gcc tcpclient.c -o tcpclient
./tcpserver 10050 5 &
./tcpclient 127.0.0.1 10050 8