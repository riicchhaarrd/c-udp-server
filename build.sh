compiler="gcc -std=c99"
params="-w -I. -m32 -fPIC -Wno-write-strings -g"

mkdir obj

echo "COMPILING"
$compiler $params -c sv.c -o obj/sv.o
$compiler $params -c net.c -o obj/net.o
obj="$(ls obj/*.o)"
$compiler -m32 -L/lib32 -Wall $obj -o server
#$compiler -m32 -L/lib32 `mysql_config --libs` -lmysqlclient -I/usr/include/mysql -ldl -lm -lssl -Wall $obj -o server
rm -rf ./obj
echo "DONE"
