# Working-part2
# Memory-Leak-Detector
In this project, we try to implement a garbage collector for C programs which work on the principle of reachability of objects to detect memory leaks. Through this project, we understand the limitation of such a garbage collector for C like programming languages (which have direct access to underlying memory addresses, unlike Java/python) and analyze its limitations and cost for being an inbuilt feature of C-like language.


## Compile
  * make           
  * g++ -c -I db_lib main.cpp -o main.o
  * g++ main.o -o exe -L . mydblib.a
**make** This command will build the 

## Run the program as :
  * ./exe

## Documentation:
Start writing your code from test-app.c file and implement your code. Compile memory-leak.c file and add memory-leak.h file where-ever needed.

  * **Step 1** : Initialize a new structure database :
    * struct_db_t *struct_db = calloc(1, sizeof(struct_db_t));
    * mld_init_primitive_data_types_support(struct_db);

  * **Step 2** : Create structure record for structure that you have declared:
    * static field_info_t emp_fields[] = {
    *    FIELD_INFO(emp_t, emp_name, CHAR,    0),
    *    FIELD_INFO(emp_t, emp_id,   UINT32,  0),
    *    FIELD_INFO(emp_t, age,      UINT32,  0),
    *    FIELD_INFO(emp_t, mgr,      OBJ_PTR, emp_t),
    *    FIELD_INFO(emp_t, salary,   FLOAT, 0)};

  * **Step 3** : Register the structure in structure database
    * REGISTER_STRUCTURE(struct_db, emp_t, emp_fields);

  * **Step 4** : Print structure database
    * print_structure_db(struct_db);

  * **Step 5** : Initialize a new Object database :
    * object_db_t *object_db = calloc(1, sizeof(object_db_t));
    * object_db->struct_db = struct_db;
    
  * **Step 6** : Create objects, equivalent to standard 
    * Use **xcalloc** instead of calloc and malloc.
    * void *ptr = xcalloc(object_db_t *object_db, char *struct_name, int units);

  * **Step 7** :
    * Choose Root objects in the hierarchy of your data structures.
    * mld_set_dynamic_object_as_root(object_db, abhishek);

  * **Step 8** :
    * print_object_db(object database pointer);
    
  * **Step 9** :
    * run_mld_algorithm(object_db);
    * report_leaked_objects(object_db);

## Implementation Details :
<p>
  <img src="Screenshots/s2.jpg" width="700" height="450">
</p>
<p>
  <img src="Screenshots/s1.jpg" width="700" height="450">
</p>

## Limitations :
  * Storing Pointers to Non-Pointer data types will is not handled.
  * Indirect Reference to objects is not valid.
  * Does not allow Embedded Object Declaration.
  * This library cannot handle unions because unions don't have fixed size.
  * **This library is equivalent to Java Garbage Collector if we write programs in Java Style**

## Output :
<p>
  <img src="Screenshots/s3.png" width="700" height="450">
</p>
<p>
  <img src="Screenshots/s4.png" width="700" height="450">
</p>