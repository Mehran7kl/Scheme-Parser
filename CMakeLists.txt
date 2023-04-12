cmake_minimum_required(VERSION 3.6.0)
project(Interpreter)
include_directories(
	${PROJECT_SOURCE_DIR}/include
	${PROJECT_SOURCE_DIR}/src
)
file(GLOB SRCS
	"${PROJECT_SOURCE_DIR}/include/*"
	"${PROJECT_SOURCE_DIR}/src/*"
)
set(RELEASE 0)
set(SAFE 1)
set(DEBUG 2)
set(LOG 3)
set(LOG2 4)

set(BUILD ${LOG})


add_compile_definitions(
			_BUILD_=${BUILD} 
			_RELEASE_=${RELEASE}
			_SAFE_=${SAFE}
			_DEBUG_=${DEBUG}
			_LOG_=${LOG}
			_LOG2_=${LOG2}
			SMODE=_BUILD_>=_SAFE_
			DMODE=_BUILD_>=_DEBUG_
			LMODE=_BUILD_>=_LOG_
			
			PROJECT_DIR="${PROJECT_SOURCE_DIR}"
	
)

if(NOT BUILD)	
	add_compile_definitions(NDEBUG)
	add_compile_options(-Ofast)
endif()
add_executable(Interpreter ${SRCS})