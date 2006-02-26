clear
g++ -DNDEBUG -I misc -O3 -lmysqlclient -o forum.cgi *.cpp misc/*.cpp misc/sql/*.cpp && strip forum.cgi
