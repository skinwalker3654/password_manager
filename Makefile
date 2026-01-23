SOURCE = main.c 
CC = gcc
TAR = main
DBFILE = .passwords

$(TAR): $(SOURCE)
	@echo "compiling the project..."
	$(CC) $< -o $@

run:
	@echo "Running the program..."
	./$(TAR)

clean:
	@echo "Reseting the program..."
	rm -rf $(TAR) $(DBFILE)
