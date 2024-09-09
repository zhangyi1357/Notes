db: db.c
	gcc db.c -o db

run: db
	./db mydb.db

clean:
	rm -f db *.db

test: db
	python db_test.py

format: *.c
	clang-format -style=Google -i *.c