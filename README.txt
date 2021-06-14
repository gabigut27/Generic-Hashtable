README


===Description ===

Program files:
GenericHashTable.h - file that has declarations of functions and two strucs of Table and Object.
GenericHashTable.c - file that implementaion a Generic Hashtable and its Actions as needed.
funcation:

	createTable - recieves size of table, type and length of list and creates a table that will 
    serve us as hash table.
    freeTable - receives pointer for a Struct of table and frees all allocated members.
    add - receives a pointer to table and data and adds it and handle with differnet cases.
    removeObj - receives pointer to table and data and removes the data from the table.
    search - receives pointer to table and data and checkes if it exists in the table.
    printTable - prints the table.
    createObject - recevies a pointer for data and creating an Object.
    freeObject - receives a pointer for an object and type and realsee the memory from the allocate it took
    in the create.
    isEqual - receives type and two pointer of data and checkes if the two data are equal.
    intHashFun - receives pointer to key(data) and original size of the table and calculates the hash value of integer.
    StrHashFun - same as intHashFun but instead of calculate for int hash it calcualtes for strings.
    handleTable - receives pointer to table, object and idx and checks if there is an empty space where
    we can push the object in the ratio we can run on for the idx value hash we got.
   This is a help func for add func.
    multiplyTable -  receives pointer to table, object and id and doubles the size of the table
    and transfer the objects to thre right place.
    This is a help func for handleTable.
    sumascii - receives a string and the original size of the table and help to the strHushFun to calculate the hash value.
    freeList - receives a pointer to Object * which contains a list and realse the allocated memory.





