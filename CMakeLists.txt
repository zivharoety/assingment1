cmake_minimum_required(VERSION 3.12)
project(untitled)


set(CMAKE_CXX_STANDARD 14)

add_executable(untitled src/Main.cpp src/Table.cpp src/Action.cpp src/Customer.cpp src/Restaurant.cpp src/dish.cpp include/Restaurant.h include/Table.h include/Dish.h include/Action.h include/Customer.h)
