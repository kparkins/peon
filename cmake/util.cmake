function(SetTargetOutputLocations targetName outputDirPrefix)
		set_target_properties(${targetName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${${outputDirPrefix}})
		set_target_properties(${targetName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${${outputDirPrefix}})
		set_target_properties(${targetName} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${${outputDirPrefix}})

		foreach(OUTPUT_CONFIG ${CMAKE_CONFIGURATION_TYPES} )
		    string( TOUPPER ${OUTPUT_CONFIG} OUTPUT_CONFIG )
		    string( TOLOWER ${OUTPUT_CONFIG} OUTPUT_DIRECTORY_NAME )
		    
		    set_target_properties(${targetName} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} 
		    	${${outputDirPrefix}}/${OUTPUT_DIRECTORY_NAME})
		    
		    set_target_properties(${targetName} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} 
		    	${${outputDirPrefix}}/${OUTPUT_DIRECTORY_NAME})
		    
		    set_target_properties(${targetName} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_${OUTPUT_CONFIG} 
		    	${${outputDirPrefix}}/${OUTPUT_DIRECTORY_NAME})
		endforeach()
endfunction()

macro(peon_set_option variable default type docstring)
	if(NOT DEFINED ${variable})
		set(${variable} ${default})
	endif()
	set(${variable} ${${variable}} CACHE ${type} ${docstring} FORCE)
endmacro()