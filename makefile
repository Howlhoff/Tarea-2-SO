program:
	g++ tarea2.cpp -o tarea2 -Wall
.PHONY: run
.PHONY: clean
run: program
	./tarea2
clean:
	rm ./tarea2