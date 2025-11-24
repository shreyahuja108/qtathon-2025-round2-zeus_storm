# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\surveillance_panel_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\surveillance_panel_autogen.dir\\ParseCache.txt"
  "surveillance_panel_autogen"
  )
endif()
