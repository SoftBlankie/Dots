dots: main.cpp brain.h dot.h population.h
	g++ -g -Wall -Werror -std=c++11 -o dots main.cpp -lsfml-graphics -lsfml-window -lsfml-system
clean:
	rm -f *.out *.o dots
