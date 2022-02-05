# Assignment 1 - Turner Gravin

How to Compile: `make`

* Testcase with 1 Producer Process: `make run1`

* Testcase with 2 Producer Processes (same amount of data): `make run2`

* Testcase with 3 Producer Processes (different amount of data): `make run3`

* Testcase with 5 Producer Processes (different amount of data): `make run5`

There are 5 provided data files containing test data. Each Producer uses data from its assosiated data file (dataX.txt). In order to run alternative tests, simply change the data within the desired files and run the assosiated make runX, where X is the number of Producer Processes to spawn.

Log.txt contains all signals recieved along with their assosiated values after each run is completed.

Final Statistics are printed to Standard Output.
