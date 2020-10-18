objects= vmemfuncts.o main.o

VMEMSimulator :$(objects)
	gcc -o VMEMSimulator $(objects)

vmemfuncts.o :vmemfuncts.c
	gcc -c vmemfuncts.c vmemfuncts.h vtypes.h 

main.o :main.c
	gcc -c main.c vmemfuncts.h vtypes.h 

clean  :
	rm VMEMSimulator $(objects) vmemfuncts.h.gch vtypes.h.gch entries.txt

