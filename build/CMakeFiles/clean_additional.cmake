# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Proyecto-program-IV_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Proyecto-program-IV_autogen.dir\\ParseCache.txt"
  "Proyecto-program-IV_autogen"
  )
endif()
