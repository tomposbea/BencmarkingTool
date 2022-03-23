project(BenchmarkingTool)
cmake_minimum_required(VERSION 3.13.4)

set(COMPILER_OPTIONS)
if(UNIX)
        message( STATUS "Linux")
        EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
        message( STATUS "Architecture: ${ARCHITECTURE}" )
        if( ${ARCHITECTURE} STREQUAL "x86_64" )
		set(COMPILER_OPTIONS ${COMPILER_OPTIONS} -O3 -DNDEBUG -Wl,--no-as-needed -lm)
       	else()
		set(COMPILER_OPTIONS ${COMPILER_OPTIONS} -O3 -DNDEBUG -march=armv8.2-a -Wl,--no-as-needed -lm)
	endif()
endif(UNIX)

if(WIN32)
        message( STATUS "Windows")
endif(WIN32)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

add_executable(Run 
	main.c
	print.c
	generate.c
	read_conf.c
	conversions.c
	lcm.c
	fifo.c)

target_link_libraries(Run Threads::Threads)
target_compile_options(Run PUBLIC ${COMPILER_OPTIONS})
