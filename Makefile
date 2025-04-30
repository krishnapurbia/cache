CXX = g++
CXXFLAGS = -std=c++17 -O2


SIM_EXEC = L1simulate
TEST_EXEC = test_runner


SIM_SRC = main.cpp
TEST_SRC = ex.cpp


SIM_OBJ = $(SIM_SRC:.cpp=.o)
TEST_OBJ = $(TEST_SRC:.cpp=.o)


all: $(SIM_EXEC) $(TEST_EXEC)

$(SIM_EXEC): $(SIM_OBJ)
	$(CXX) $(CXXFLAGS) -o $(SIM_EXEC) $(SIM_OBJ)

$(TEST_EXEC): $(TEST_OBJ)
	$(CXX) $(CXXFLAGS) -o $(TEST_EXEC) $(TEST_OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<


run: all
	./$(TEST_EXEC) ./$(SIM_EXEC) app1 plot.csv


clean:
	rm -f *.o $(SIM_EXEC) $(TEST_EXEC) output.txt plot.csv

