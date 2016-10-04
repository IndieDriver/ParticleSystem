# - Try to find OpenCL
# This module tries to find an OpenCL implementation on your system. It supports
# AMD / ATI, Apple and NVIDIA implementations, but should work, too.
#
# To set manually the paths, define these environment variables:
# OpenCL_INCPATH    - Include path (e.g. OpenCL_INCPATH=/opt/cuda/4.0/cuda/include)
# OpenCL_LIBPATH    - Library path (e.h. OpenCL_LIBPATH=/usr/lib64/nvidia)
#
# Once done this will define
#  OPENCL_FOUND        - system has OpenCL
#  OPENCL_INCLUDE_DIRS  - the OpenCL include directory
#  OPENCL_LIBRARIES    - link these to use OpenCL
#
# WIN32 should work, but is untested

FIND_PACKAGE(PackageHandleStandardArgs)

SET (OPENCL_VERSION_STRING "0.1.0")
SET (OPENCL_VERSION_MAJOR 0)
SET (OPENCL_VERSION_MINOR 1)
SET (OPENCL_VERSION_PATCH 0)

IF (APPLE)

	FIND_LIBRARY(OPENCL_LIBRARIES OpenCL DOC "OpenCL lib for OSX")
	FIND_PATH(OPENCL_INCLUDE_DIRS OpenCL/cl.h DOC "Include for OpenCL on OSX")
	FIND_PATH(_OPENCL_CPP_INCLUDE_DIRS OpenCL/cl.hpp DOC "Include for OpenCL CPP bindings on OSX")

ELSE (APPLE)

	IF (WIN32)

		FIND_PATH(OPENCL_INCLUDE_DIRS CL/cl.h)
		FIND_PATH(_OPENCL_CPP_INCLUDE_DIRS CL/cl.hpp)

		# The AMD SDK currently installs both x86 and x86_64 libraries
		# This is only a hack to find out architecture
		IF( ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64" )
			SET(OPENCL_LIB_DIR "$ENV{AMDAPPSDKROOT}/lib/x86_64")
		ELSE (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64")
			SET(OPENCL_LIB_DIR "$ENV{AMDAPPSDKROOT}/lib/x86")
		ENDIF( ${CMAKE_SYSTEM_PROCESSOR} STREQUAL "AMD64" )
		FIND_LIBRARY(OPENCL_LIBRARIES OpenCL.lib PATHS ${OPENCL_LIB_DIR} ENV OpenCL_LIBPATH)

		GET_FILENAME_COMPONENT(_OPENCL_INC_CAND ${OPENCL_LIB_DIR}/../../include ABSOLUTE)

		# On Win32 search relative to the library
		FIND_PATH(OPENCL_INCLUDE_DIRS CL/cl.h PATHS "${_OPENCL_INC_CAND}" ENV OpenCL_INCPATH)
		FIND_PATH(_OPENCL_CPP_INCLUDE_DIRS CL/cl.hpp PATHS "${_OPENCL_INC_CAND}" ENV OpenCL_INCPATH)

	ELSE (WIN32)

		# Unix style platforms

		# Guess the library path for ARM
		IF (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
			option(USE_MGD OFF "Use Mali Graphics debugger libraries")
			
			if (USE_MGD)
				set(ARM_LIBRARY_PATH_GUESS "/usr/local/lib/mgd")
			else()
				set(ARM_LIBRARY_PATH_GUESS "/usr/local/lib/mali/fbdev")
			endif()

			# This is a hack.
			# Normally find_library() only executes once.
			# However because USE_MGD might change we need
			# force detection everytime. Clearing the cache
			# variables does this.
			set(OPENCL_LIBRARIES "OPENCL_LIBRARIES-NOTFOUND" CACHE PATH "" FORCE)
			set(MALI_LIBRARY "MALI_LIBRARY-NOTFOUND" CACHE PATH "" FORCE)
		ENDIF()

		FIND_LIBRARY(OPENCL_LIBRARIES OpenCL
			PATHS ENV LD_LIBRARY_PATH 
			      ENV OpenCL_LIBPATH
			      "${ARM_LIBRARY_PATH_GUESS}"
		)

		# OPENCL_LIBRARIES might be a list so just use the first
		list(GET OPENCL_LIBRARIES 0 first_library)
		GET_FILENAME_COMPONENT(OPENCL_LIB_DIR ${first_library} PATH)
		GET_FILENAME_COMPONENT(_OPENCL_INC_CAND ${OPENCL_LIB_DIR}/../../include ABSOLUTE)

		if (CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
			# For ARM's Mali libOpenCL.so does not have the
			# OpenCL symbols. These are actually in libmali

			# FIXME: This is gross!
			if (NOT MALI_LIBRARY)
				set(IS_FIRST_RUN true)
			else()
				set(IS_FIRST_RUN false)
			endif()

			FIND_LIBRARY(MALI_LIBRARY mali
			             PATH "${ARM_LIBRARY_PATH_GUESS}" 
				    )
			IF( MALI_LIBRARY )
				# Found Mali library so append to OpenCL library list

				# FIXME: This is gross!
                                # OPENCL_LIBRARIES is a cache variable so we need to be careful
				# to not use list(append ) which will create a non-cache variable which
				# will hide the cache variable.
				# We use set with FORCE here. So we need to be really careful to not keep
				# appending to the list everytime cmake is run
				if (IS_FIRST_RUN)
					set(OPENCL_LIBRARIES "${OPENCL_LIBRARIES};${MALI_LIBRARY}" CACHE PATH "" FORCE)
				endif()

				message(STATUS "Found ARM Mali library ${MALI_LIBRARY}")
			ELSE()
				message(WARNING "ARM target detected but Mali library was not found.")
			ENDIF()
		endif()

		# The AMD SDK currently does not place its headers
		# in /usr/include, therefore also search relative
		# to the library
		FIND_PATH(OPENCL_INCLUDE_DIRS CL/cl.h PATHS ${_OPENCL_INC_CAND} "/usr/local/cuda/include" "/opt/AMDAPP/include" ENV OpenCL_INCPATH)
		FIND_PATH(_OPENCL_CPP_INCLUDE_DIRS CL/cl.hpp PATHS ${_OPENCL_INC_CAND} "/usr/local/cuda/include" "/opt/AMDAPP/include" ENV OpenCL_INCPATH)
	ENDIF (WIN32)

ENDIF (APPLE)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(OpenCL DEFAULT_MSG OPENCL_LIBRARIES OPENCL_INCLUDE_DIRS)

IF(_OPENCL_CPP_INCLUDE_DIRS)
	SET( OPENCL_HAS_CPP_BINDINGS TRUE )
	LIST( APPEND OPENCL_INCLUDE_DIRS ${_OPENCL_CPP_INCLUDE_DIRS} )
	# This is often the same, so clean up
	LIST( REMOVE_DUPLICATES OPENCL_INCLUDE_DIRS )
ENDIF(_OPENCL_CPP_INCLUDE_DIRS)

MARK_AS_ADVANCED(
  OPENCL_INCLUDE_DIRS
)

