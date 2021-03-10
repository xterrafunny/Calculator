add_library(CalculatorLib Calculator.h Calculator.cpp)
add_executable(CalculatorRun main.cpp)
target_link_libraries(CalculatorRun CalculatorLib)