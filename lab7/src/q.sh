make udpserver
./udpserver 20001 100 &
sleep 1
./udpclient 127.0.0.1 20001 100