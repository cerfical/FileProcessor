cmake_minimum_required(VERSION 3.5)

# Basic project setup
project(FileProcessor VERSION 0.2 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

list(APPEND CMAKE_AUTOUIC_SEARCH_PATHS "ui/")
qt_standard_project_setup()

qt_add_executable(FileProcessor WIN32
	"app/main.cpp"

	"include/MainWindow.hpp"
	"src/MainWindow.cpp"

	"include/RelativeTime.hpp"
	"src/RelativeTime.cpp"

	"include/RelativeDistance.hpp"
	"src/RelativeDistance.cpp"

	"include/ObjectInfo.hpp"
	"src/ObjectInfo.cpp"

	"ui/MainWindow.ui"
)

target_compile_features(FileProcessor PRIVATE cxx_std_20)
target_include_directories(FileProcessor PRIVATE
	"include/"
)

target_link_libraries(FileProcessor PRIVATE Qt6::Widgets)
