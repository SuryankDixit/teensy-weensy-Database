# TARGET: exe

# # -L flag is used to link external libraries( here libcalc.a and -lm links <math.h>)
# exe:main.o mylibmath.a
# 	gcc  main.o -o exe -L . mylibmath.a -lm


# mylibmath.a:trig_math/trig_math.o common_math/common_math.o complex_math/complex_math.o
# 	ar rs mylibmath.a common_math/common_math.o complex_math/complex_math.o trig_math/trig_math.o
# main.o:main.c
# 	gcc  -c -I common_math -I complex_math -I trig_math main.c -o main.o

# # -I flag is used to specify directory name in which the used header files are present
# common_math.o:common_math/common_math.c
# 	gcc  -c -I common_math common_math/common_math.c -o common_math/common_math.o
# complex_math.o:complex_math/complex_math.c
# 	gcc  -c -I complex_math complex_math/complex_math.c -o complex_math/complex_math.o
# trig_math.o:trig_math/trig_math.c
# 	gcc  -c -I trig_math trig_math/trig_math.c -o trig_math/trig_math.o

# clean:
# 	rm common_math/common_math.o
# 	rm complex_math/complex_math.o
# 	rm trig_math/trig_math.o
# 	rm main.o
# 	rm mylibmath.a
# 	rm exe

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