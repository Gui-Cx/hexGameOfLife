CPPC = g++-10
# CPPFLAGS = -Wall -Werror -I/usr/include/SFML/ -I/usr/include -L/usr/lib/x86_64-linux-gnu/lib -lsfml-graphics -lsfml-window -lsfml-system -g
CPPFLAGS = -Wall -Werror -g
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
CPPFILES = $(wildcard *.cpp)
OFILES = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

main : $(OFILES) Makefile
	$(CPPC) -o $@ $(OFILES) $(CPPFLAGS) $(LIBS)

# main.o : main.cpp
# 	echo "Compiling main.o"
# 	$(CPPC) $(CPPFLAGS) -c $^

# readFile.o : readFile.cpp
# 	echo "Compiline readFile.o"
# 	$(CPPC) $(CPPFLAGS) -c $^

%.o : %.cpp
	echo "Creating $@"
	$(CPPC) $(CPPFLAGS) -c $^

clean : 
	rm -rf $(OFILES) main
