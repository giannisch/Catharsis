

all:
	g++ directory.cpp catharsis.cpp filesystem.cpp -o catharsis

debug:
	g++ -g directory.cpp catharsis.cpp filesystem.cpp -o catharsis
	
	
clean:
	rm *~ catharsis
