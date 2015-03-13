all: help
c_release:
	gcc server.c util.c -o server
c_debug:
	gcc server.c util.c -o server -g
cpp_release:
	g++ server.cpp util.c -o server -lboost_system -lpthread -std=c++11
	g++ client.cpp util.c -o client -lboost_system -lpthread -std=c++11
cpp_debug:
	g++ server.cpp util.c -o server -g -lboost_system -lpthread -std=c++11
	g++ client.cpp util.c -o client -g -lboost_system -lpthread -std=c++11
wc_release:
	i586-mingw32msvc-gcc server.c util.c -o server.exe
wc_debug:
	i586-mingw32msvc-gcc server.c util.c -o server.exe -g
wcpp_release:
	i586-mingw32msvc-g++ server.cpp util.c -o server.exe
wcpp_debug:
	i586-mingw32msvc-g++ server.cpp util.c -o server.exe -g
debug:
	gdb ./server
run:
	gnome-terminal -e "./server 1337"
	gnome-terminal -e "./client 127.0.0.1 1337"
clear:
	rm ./server
	rm ./client
commit:
	git commit -a
	git push GL
help:
	@echo "Options:"
	@echo "	c_release"
	@echo "	cpp_release"
	@echo "	c_debug"
	@echo "	cpp_debug"
	@echo "	wc_release"
	@echo "	wc_debug"
	@echo "	wcpp_release"
	@echo "	wcpp_debug"
	@echo "	run"
	@echo "	debug"
	@echo "	clear"
	@echo "	commit"
	@echo "	help"