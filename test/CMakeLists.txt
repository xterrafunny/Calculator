add_executable(CalculatorUT main.cpp CalculatorUT.cpp)
add_test(CalculatorUT CalculatorUT)
target_link_libraries(CalculatorUT gtest CalculatorLib)