mydblib.a:db_lib/b-plus-tree.o db_lib/commands.o db_lib/table.o db_lib/UserInput.o
	ar rs mydblib.a db_lib/b-plus-tree.o db_lib/commands.o db_lib/table.o db_lib/UserInput.o

b-plus-tree.o:db_lib/b-plus-tree.cpp
	g++ -c -I db_lib db_lib/b-plus-tree.cpp db_lib/b-plus-tree.o

commands.o:db_lib/commands.cpp
	g++ -c -I db_lib db_lib/commands.cpp db_lib/commands.o

table.o:db_lib/table.cpp
	g++ -c -I db_lib db_lib/table.cpp db_lib/table.o

UserInput.o:db_lib/UserInput.cpp
	g++ -c -I db_lib db_lib/UserInput.cpp db_lib/UserInput.o

clean:
	rm db_lib/*.o
	rm mydblib.a