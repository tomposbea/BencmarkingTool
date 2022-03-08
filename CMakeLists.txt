project(BenchmarkingTool)
cmake_minimum_required(VERSION 3.12)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_executable(Run 
	main.c
	print.c
	generate.c
	read_conf.c
	conversions.c
	lcm.c)

target_link_libraries(Run Threads::Threads)
