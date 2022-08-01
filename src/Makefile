FLAGS = -std=c++17 -Wall -Werror -Wextra
OS = $(shell uname)
ifeq ($(OS), Linux)
	OS_FLAGS = -lm -pthread -g
else
	OS_FLAGS = 
endif
all : check gcov_report

test: s21_tests
	./s21_tests

s21_tests: s21_matrix_oop.a tests.cpp
	g++ $(FLAGS) tests.cpp $(OS_FLAGS) -lgtest -L. s21_matrix_oop.a -o s21_tests

s21_matrix_oop.a: build_s21_matrix clean_obj

build_s21_matrix: s21_matrix_oop.cpp s21_matrix_oop.h
	g++ $(FLAGS) $(OS_FLAGS) -c s21_matrix_oop.cpp
	ar rc s21_matrix_oop.a s21_matrix_oop.o
	ranlib s21_matrix_oop.a

gcov_report: gcov_report_build
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info

gcov_report_build: s21_matrix_oop.cpp tests.cpp s21_matrix_oop.h
	g++ $(FLAGS) tests.cpp -lgtest $(OS_FLAGS) --coverage s21_matrix_oop.cpp -o gcov_report
	./gcov_report
	lcov -t "./gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	open ./report/index.html


check: s21_matrix_oop.cpp s21_matrix_oop.h tests.cpp s21_tests
	valgrind --leak-check=yes ./s21_tests

clean_obj:
	rm -rf *.o

clean_lib: 
	rm -rf *.a

clean_test:
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info
	rm -rf gcov_report
	rm -rf report
	rm -rf *dSYM

clean: clean_lib clean_test clean_obj
	rm -rf s21_matrix
	rm -rf s21_tests
	rm -rf CPPLINT.cfg

