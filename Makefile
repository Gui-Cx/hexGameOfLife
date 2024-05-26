CPPC = g++-10
CPPFLAGS = -Wall -Werror -g
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
CPPFILES = $(wildcard *.cpp)
OFILES = $(patsubst %.cpp, %.o, $(wildcard *.cpp))

main : $(OFILES) Makefile
	$(CPPC) -o $@ $(OFILES) $(CPPFLAGS) $(LIBS)

%.o : %.cpp
	echo "Creating $@"
	$(CPPC) $(CPPFLAGS) -c $^

clean : 
	rm -rf $(OFILES) main
