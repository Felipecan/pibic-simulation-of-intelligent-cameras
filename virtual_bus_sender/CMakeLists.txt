INCLUDE_DIRECTORIES(BEFORE
  ${CMAKE_SOURCE_DIR}/libCERTI 
  ${CMAKE_SOURCE_DIR}/test/libgraphc
  ${CMAKE_SOURCE_DIR}/include/
  ${CMAKE_SOURCE_DIR}/include/hla-1_3
  ${CMAKE_BINARY_DIR}/include/hla-1_3)
include_directories(${CERTI_SOURCE_DIR}/libHLA)

IF (WIN32)
   ADD_DEFINITIONS(-DHAVE_STRING_H)
ENDIF (WIN32)

find_program(GENGETOPT_PROGRAM NAMES gengetopt)
if (GENGETOPT_PROGRAM)
   set(GGOFILE ${CMAKE_CURRENT_SOURCE_DIR}/sender.ggo)
   add_custom_target(GensenderCmdline
                    COMMAND ${GENGETOPT_PROGRAM} --no-handle-error --show-required -i ${GGOFILE}
                    DEPENDS ${GGOFILE}
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    COMMENT "Regenerate sender command line C files -- (from ${GGOFILE})"
                    )
endif(GENGETOPT_PROGRAM)
set(sender_SRCS_generated
    sender_cmdline.c sender_cmdline.h)

IF (CMAKE_COMPILER_IS_GNUCC) 
   message(STATUS "INHIBIT Compiler (GCC) warning for gengetopt generated files: ${sender_SRCS_generated}.")
   SET_SOURCE_FILES_PROPERTIES(${sender_SRCS_generated}
                               PROPERTIES COMPILE_FLAGS "-w")
ENDIF(CMAKE_COMPILER_IS_GNUCC)

if (MSVC)
   message(STATUS "INHIBIT Compiler (MSVC) warning for gengetopt generated files: ${sender_SRCS_generated}.")
   SET_SOURCE_FILES_PROPERTIES(${rtig_SRCS_generated}
                               PROPERTIES COMPILE_FLAGS "-D_CRT_SECURE_NO_WARNINGS")
endif(MSVC)

########### next target ###############

SET(sender_SRCS
  VirtualBusFederate.cpp
  main.cpp
  FederateAmbassador.cpp
  #${sender_SRCS_generated} not ready [yet]
  )
  
IF(X_DISPLAY_MISSING)
  MESSAGE(STATUS "Some X11 header were not found: only arm Test Application with no GUI will be built.")
ELSE(X_DISPLAY_MISSING)
  ADD_EXECUTABLE(sender ${sender_SRCS})
  TARGET_LINK_LIBRARIES(sender graph RTI FedTime)  
  
  INSTALL(TARGETS sender
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)  
ENDIF(X_DISPLAY_MISSING)

ADD_EXECUTABLE(sender-nogui ${sender_SRCS})
set_target_properties(sender-nogui PROPERTIES COMPILE_FLAGS "-DX_DISPLAY_MISSING=1")
TARGET_LINK_LIBRARIES(sender-nogui graph-nogui RTI FedTime)
INSTALL(TARGETS sender-nogui
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)  
    
########### install files ###############
INSTALL(FILES  
  Test.fed Test.xml 
  DESTINATION share/federations)
  
INSTALL(FILES  
  dtest_sender.py 
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE WORLD_READ WORLD_EXECUTE
  DESTINATION share/scripts)

