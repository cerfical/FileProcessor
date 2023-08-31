cmake_minimum_required(VERSION 3.5)

# Basic project setup
project(FileProcessor VERSION 0.1 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(FileProcessor WIN32
	"app/main.cpp"

	"include/MainWindow.hpp"
	"src/MainWindow.cpp"
)

target_include_directories(FileProcessor PRIVATE
	"include/"
)

target_link_libraries(FileProcessor PRIVATE Qt6::Widgets)
