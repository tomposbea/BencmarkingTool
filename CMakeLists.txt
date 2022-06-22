project(BenchmarkingTool)
cmake_minimum_required(VERSION 3.12)

set(COMPILER_OPTIONS)
if(UNIX)
        message( STATUS "Linux")
        EXECUTE_PROCESS( COMMAND uname -m COMMAND tr -d '\n' OUTPUT_VARIABLE ARCHITECTURE )
        message( STATUS "Architecture: ${ARCHITECTURE}" )
        if( ${ARCHITECTURE} STREQUAL "x86_64" )
		set(COMPILER_OPTIONS ${COMPILER_OPTIONS} -pg -O3 -DNDEBUG -Wl,--no-as-needed -lprofiler -lm -g)
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
	#src/fifo.c
	#src/init/read_conf.c
	src/init/init.c
	#src/init/error.c
	#src/init/usage.c
	src/report/write_xml.c
	src/report/write_csv.c
	src/report/system_data.c
	src/report/print.c
	#src/process/redblacktree.c
	#src/process/hash_map.c
	#src/process/binary_search_tree.c
)

add_library(Lib SHARED src/generate/generate.c src/process/lcm.c src/generate/conversions.c src/init/usage.c src/init/error.c src/init/read_conf.c src/process/hash_map.c src/fifo.c src/process/binary_search_tree.c src/process/redblacktree.c)

SET_TARGET_PROPERTIES(Lib PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(Run Lib Threads::Threads)
target_compile_options(Run PUBLIC ${COMPILER_OPTIONS})

enable_testing()
add_subdirectory(eric-test)
add_subdirectory(test)
