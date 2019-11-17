kill $(pgrep serv)
rm serv
rm cl
gcc server.c -lpthread -o serv
gcc client.c -lpthread -o cl
./serv --port 20001 --tnum 2 &
./serv --port 20002 --tnum 5 &
./cl --k 20 --mod 47 --servers ./servers.txt