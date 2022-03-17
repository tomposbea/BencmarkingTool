project(BenchmarkingTool)
cmake_minimum_required(VERSION 3.16.3)

set(COMPILER_OPTIONS)
if(UNIX)
        message( STATUS "Linux")
        EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
        message( STATUS "Architecture: ${ARCHITECTURE}" )
        if( ${ARCHITECTURE} STREQUAL "x86_64" )
		message( STATUS "dut38")
		set(COMPILER_OPTIONS ${COMPILER_OPTIONS} -O3 -Wl,--no-as-needed -lm)
		#[[add_definitions("-O3 -DNDEBUG -Wl,--no-as-needed -lm")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG -Wl,--no-as-needed -lm")
                set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O3 -DNDEBUG  -Wl,--no-as-needed -lm")
		]]
       	else()
		set(COMPILER_OPTIONS ${COMPILER_OPTIONS} -O3 -march=armv8.2-a -Wl,--no-as-needed -lm)
        endif()
endif(UNIX)

if(WIN32)
        message( STATUS "Windows")
endif(WIN32)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -O3 -Wl,--no-as-needed -lm")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg -O3 -Wl,--no-as-needed -lm")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg -O3 -Wl,--no-as-needed -lm")

add_executable(Run 
	zsiga.c
	print.c
	generate.c
	read_conf.c
	conversions.c
	lcm.c)

target_link_libraries(Run Threads::Threads)
target_compile_options(Run PUBLIC ${COMPILER_OPTIONS})
