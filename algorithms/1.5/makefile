compile: quick-find.cpp quick-union.cpp weighted-quick-union.cpp
	c++ -o quick-find.out quick-find.cpp -O2
	c++ -o quick-union.out quick-union.cpp -O2
	c++ -o quick-union-path-compression.out quick-union-path-compression.cpp -O2
	c++ -o weighted-quick-union.out weighted-quick-union.cpp -O2
	c++ -o weighted-quick-union-by-height.out weighted-quick-union-by-height.cpp -O2
	c++ -o weighted-quick-union-path-compression.out weighted-quick-union-path-compression.cpp -O2

test-tiny: compile
	@echo "testing tinyUF.txt..."
	time ./quick-find.out < tinyUF.txt > /dev/null
	time ./quick-union.out < tinyUF.txt > /dev/null
	time ./quick-union-path-compression.out < tinyUF.txt > /dev/null
	time ./weighted-quick-union.out < tinyUF.txt > /dev/null
	time ./weighted-quick-union-by-height.out < tinyUF.txt > /dev/null
	time ./weighted-quick-union-path-compression.out < tinyUF.txt > /dev/null

test-medium: compile
	@echo "testing mediumUF.txt..."
	time ./quick-find.out < mediumUF.txt > /dev/null
	time ./quick-union.out < mediumUF.txt > /dev/null
	time ./quick-union-path-compression.out < mediumUF.txt > /dev/null
	time ./weighted-quick-union.out < mediumUF.txt > /dev/null
	time ./weighted-quick-union-by-height.out < mediumUF.txt > /dev/null
	time ./weighted-quick-union-path-compression.out < mediumUF.txt > /dev/null

test-large: compile
	@echo "testing largeUF.txt..."
	time ./weighted-quick-union-path-compression.out < largeUF.txt > /dev/null
	time ./weighted-quick-union-by-height.out < largeUF.txt > /dev/null
	time ./weighted-quick-union.out < largeUF.txt > /dev/null
	time ./quick-union-path-compression.out < largeUF.txt > /dev/null
	time ./quick-union.out < largeUF.txt > /dev/null
	# time ./quick-find.out < largeUF.txt > /dev/null

test-all: test-tiny test-medium test-large

ErdosRenyi: ErdosRenyi.cpp
	c++ -o ErdosRenyi.out ErdosRenyi.cpp -O2
	./ErdosRenyi.out 100 1000
	./ErdosRenyi.out 200 1000
	./ErdosRenyi.out 400 1000
	./ErdosRenyi.out 800 1000
	./ErdosRenyi.out 1600 1000
	./ErdosRenyi.out 3200 1000
	./ErdosRenyi.out 6400 1000
	./ErdosRenyi.out 12800 1000
	./ErdosRenyi.out 25600 1000
	./ErdosRenyi.out 51200 1000
	./ErdosRenyi.out 102400 1000
	./ErdosRenyi.out 204800 1000
	./ErdosRenyi.out 409600 1000

clean:
	rm *.out