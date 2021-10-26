program:
	g++ tarea1.cpp -o tarea1 -Wall
.PHONY: run
.PHONY: clean
run: program
	./tarea1
clean:
	rm ./tarea1