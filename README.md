# Assignment 1 - Turner Gravin

How to Compile: `make`

* Testcase with 1 Producer Process: `make run1`

* Testcase with 2 Producer Processes (same amount of data): `make run2`

* Testcase with 3 Producer Processes (different amount of data): `make run3`

* Testcase with 5 Producer Processes (different amount of data): `make run5`

There are 5 provided data files containing test data. Each Producer uses data from its assosiated data file (dataX.txt). In order to run alternative tests, simply change the data within the desired files and run the assosiated make runX, where X is the number of Producer Processes to spawn.

log.txt contains all signals recieved along with their assosiated values after each run is completed.

Final Statistics are printed to Standard Output.

### Assumptions:

* The total amount of values to be recieved from signals is set to 5000, any more values than 5000 will be invalid.

* Data in the files cannot be negative, as the producer will only be sending a negative value when it has finished sending all values.put.
