Target:exe

exe:main.o mydblib.a
	g++ main.o -o exe -L . mydblib.a

mydblib.a:db_lib/b-plus-tree.o db_lib/commands.o db_lib/table.o db_lib/UserInput.o db_lib/connect.o
	ar rs mydblib.a db_lib/b-plus-tree.o db_lib/commands.o db_lib/table.o db_lib/UserInput.o db_lib/connect.o

main.o:main.cpp
	g++ -c -I db_lib main.cpp -o main.o

b-plus-tree.o:db_lib/b-plus-tree.cpp
	g++ -c -I db_lib db_lib/b-plus-tree.cpp db_lib/b-plus-tree.o

commands.o:db_lib/commands.cpp
	g++ -c -I db_lib db_lib/commands.cpp db_lib/commands.o

table.o:db_lib/table.cpp
	g++ -c -I db_lib db_lib/table.cpp db_lib/table.o

UserInput.o:db_lib/UserInput.cpp
	g++ -c -I db_lib db_lib/UserInput.cpp db_lib/UserInput.o

connect.o:db_lib/connect.cpp
	g++ -c -I db_lib db_lib/connect.cpp db_lib/connect.o

clean:
	rm db_lib/*.o
	rm mydblib.a
	rm main.o
	rm exe