# Properties->C/C++->General->Additional Include Directories
# include_directories ("${PROJECT_SOURCE_DIR}/utils")

# Set Properties->General->Configuration Type to Application(.exe)
# Creates app.exe with the listed sources (main.cxx)
# Adds sources to the Solution Explorer
add_executable (app main.cpp)

# Properties->Linker->Input->Additional Dependencies
# target_link_libraries (app  utils)

# Creates a folder "executables" and adds target
# project (app.vcproj) under it
set_property(TARGET app PROPERTY FOLDER "executables")

# Properties->General->Output Directory
set_target_properties(app PROPERTIES
                      RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# add_dependencies(app)

target_link_libraries(app)

# Adds logic to INSTALL.vcproj to copy app.exe to destination directory
# install (TARGETS app
#         RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/_install)
