add_executable(amuse_example
	basic_example.cpp
)

target_link_libraries(amuse_example
	PUBLIC
		amuse_runtime
		amuse_core
)

set_target_properties(amuse_example
	PROPERTIES
		OUTPUT_NAME "basic-example"
)
