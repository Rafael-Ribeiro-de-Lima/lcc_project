CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -pedantic
TARGET = lcc_analyzer
SRC = main.cpp token.cpp errors.cpp symbol_table.cpp lexer.cpp grammar.cpp parser_ll1.cpp
OBJ = $(SRC:.cpp=.o)
VALID_PROGRAMS = programa1.lcc programa2.lcc programa3.lcc

.PHONY: all run test clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
ifndef FILE
	$(error Use: make run FILE=programa1.lcc)
endif
	./$(TARGET) $(FILE)

test: $(TARGET)
	@set -e; \
	for file in $(VALID_PROGRAMS); do \
		./$(TARGET) $$file > /tmp/lcc_cpp_$${file}.out; \
		echo "OK - $$file"; \
	done; \
	echo "Todos os programas validos foram aceitos."

clean:
	rm -f $(TARGET) $(OBJ)
