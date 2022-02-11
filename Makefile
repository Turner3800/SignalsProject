all: aggregator.c producer.c
	gcc -o aggregator aggregator.c 
	gcc -o producer producer.c

run1:
	./aggregator 1
run2:
	./aggregator 2
run3:
	./aggregator 3
run4:
	./aggregator 4
run5:
	./aggregator 5

clean:
	rm producer
	rm aggregator
	rm log.txt
	