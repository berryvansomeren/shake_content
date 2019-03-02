#----------------------------------------------------------------
# Create the external targets that our project relies on

# Freetype 
add_subdirectory( external/freetype-2.9/ )


# a list of all include directories
set( SHAKE_INCLUDE_DIRECTORIES 
	src/
    external/freetype-2.9/include/
	external/glad/include/
    external/stb/
	C:/Users/Berry/Documents/shake3/shake_core/src/
	C:/Users/Berry/Documents/shake3/shake_graphics/src/
	C:/Users/Berry/Documents/shake3/shake_io/src/
)

link_directories( ${PROJECT_NAME} PUBLIC
	C:/Users/Berry/Documents/shake_build
)

# a list of all external libraries
set( SHAKE_EXTERNAL_LIBRARIES 
	${EXECUTABLE_NAME}
    freetype
	shake_core
	shake_graphics
	shake_io
)