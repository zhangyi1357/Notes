FixedCapacityStackOfStrings: FixedCapacityStackOfStrings.cpp
	c++ -o FixedCapacityStackOfStrings.out FixedCapacityStackOfStrings.cpp

FixedCapacityStack: FixedCapacityStack.cpp
	c++ -o FixedCapacityStack.out FixedCapacityStack.cpp

ResizingArrayStack: ResizingArrayStack.cpp
	c++ -o ResizingArrayStack.out ResizingArrayStack.cpp

Stack: Stack.cpp
	c++ -o Stack.out Stack.cpp

Queue: Queue.cpp
	c++ -o Queue.out Queue.cpp

Bag: Bag.cpp
	c++ -o Bag.out Bag.cpp

run: FixedCapacityStackOfStrings FixedCapacityStack ResizingArrayStack Stack Queue Bag
	./FixedCapacityStackOfStrings.out < tobe.txt
	./FixedCapacityStack.out < tobe.txt
	./ResizingArrayStack.out < tobe.txt
	./Stack.out < tobe.txt
	./Queue.out < tobe.txt
	./Bag.out < tobe.txt

clean:
	rm *.out