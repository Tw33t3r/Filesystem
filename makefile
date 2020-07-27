CC=g++
#CFLAGS = -ggdb -Wall
CFLAGS = 

all: driver1 driver2 driver3 driver4 driver5 driver6

 
bitvector.o: bitvector.cpp  bitvector.h
	$(CC) $(CFLAGS) -c bitvector.cpp

bitvector_driver: bitvector.o
	$(CC) $(CFLAGS) bitvector_driver.cpp bitvector.o -o bitvector_driver

disk.o: disk.cpp  disk.h
	$(CC) $(CFLAGS) -c disk.cpp

diskmanager.o: diskmanager.cpp  diskmanager.h
	$(CC) $(CFLAGS) -c diskmanager.cpp

partitionmanager.o: partitionmanager.cpp  partitionmanager.h
	$(CC) $(CFLAGS) -c partitionmanager.cpp

filesystem.o: filesystem.cpp  filesystem.h
	$(CC) $(CFLAGS) -c filesystem.cpp

driver1: driver1.cpp client.h bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o
	$(CC) $(CFLAGS)  -o driver1 driver1.cpp bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o

driver2: driver2.cpp client.h bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o
	$(CC) $(CFLAGS)  -o driver2 driver2.cpp bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o

driver3: driver3.cpp client.h bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o
	$(CC) $(CFLAGS)  -o driver3 driver3.cpp bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o

driver4: driver4.cpp client.h bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o
	$(CC) $(CFLAGS)  -o driver4 driver4.cpp bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o

driver5: driver5.cpp client.h bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o
	$(CC) $(CFLAGS)  -o driver5 driver5.cpp bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o

driver6: driver6.cpp client.h bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o
	$(CC) $(CFLAGS)  -o driver6 driver6.cpp bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o

driver: driver.cpp client.h bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o
	$(CC)  -o driver driver.cpp bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o

clean: 
	rm -f bitvector.o disk.o diskmanager.o partitionmanager.o filesystem.o driver1 driver2 driver3 driver4 driver5 driver6 driver bitvector_driver DISK1
