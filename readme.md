
make	-> compiles L1simulate and test_runner from main.cpp and ex.cpp
make run -> Builds everything, then  tests and writes plot.csv
make clean	-> Deletes all generated files: .o, L1simulate, test_runner, output files

./test_runner
-> the default parameters for test_runner are ( sim_path : ./L1simulate ) ( trace: app1) (csvoutput : plot.csv)
-> to run with other parameter you have execute test_runner manually
-> (eg.: ./test_runner ./L1simulate app1 plot.csv )

./L1simulate
-> commnadline arguments ( eg: ./L1simulate -t app1 -s 5 -E 2 -b 5 -o output.txt)
