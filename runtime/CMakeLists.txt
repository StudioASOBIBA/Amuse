# ----------------------
# Runtime configuration
# ----------------------

# Root directory for runtime modules.
set(AMUSE_RUNTIME_DIR "${CMAKE_CURRENT_LIST_DIR}")

# Per-module build toggles.
option(AMUSE_BUILD_MODULE_CORE "Build Amuse core module" ON)
option(AMUSE_BUILD_MODULE_RENDERING "Build Amuse rendering module" ON)

# Generated include directory for alias headers.
set(AMUSE_RUNTIME_GENERATED_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/include")

# Accumulator for modules that successfully define targets.
set(AMUSE_RUNTIME_MODULE_TARGETS "")

# Adds a runtime module if it exists and collects its target.
function(amuse_add_runtime_module module_name module_dir module_target)
	if (EXISTS "${module_dir}/CMakeLists.txt")
		# Keep each module's binary output in its own subfolder.
		add_subdirectory(
			"${module_dir}"
			"${CMAKE_CURRENT_BINARY_DIR}/amuse_${module_name}"
		)

		if (TARGET "${module_target}")
			list(APPEND AMUSE_RUNTIME_MODULE_TARGETS "${module_target}")
			set(AMUSE_RUNTIME_MODULE_TARGETS "${AMUSE_RUNTIME_MODULE_TARGETS}" PARENT_SCOPE)
		else()
			message(
				STATUS
				"[Amuse] Module '${module_name}' did not define target '${module_target}'"
			)
		endif()
	else()
		message(STATUS "[Amuse] Skipping module '${module_name}' (no CMakeLists.txt)")
	endif()
endfunction()

# Core runtime module (base types, misc utilities, etc.).
if (AMUSE_BUILD_MODULE_CORE)
	amuse_add_runtime_module("core" "${AMUSE_RUNTIME_DIR}/core" "amuse_core")
endif()

# Rendering runtime module (optional; may be absent).
if (AMUSE_BUILD_MODULE_RENDERING)
	amuse_add_runtime_module("rendering" "${AMUSE_RUNTIME_DIR}/rendering" "amuse_rendering")
endif()

# Aggregate runtime library that links all enabled modules.
add_library(amuse_runtime)

foreach(module_target IN LISTS AMUSE_RUNTIME_MODULE_TARGETS)
	target_link_libraries(amuse_runtime PUBLIC "${module_target}")
endforeach()

# Expose the generated include directory to dependents.
target_include_directories(amuse_runtime
	PUBLIC
		$<BUILD_INTERFACE:${AMUSE_RUNTIME_GENERATED_INCLUDE_DIR}>
)

# Friendly output name for the runtime library.
set_target_properties(amuse_runtime
	PROPERTIES
		OUTPUT_NAME "libamuse"
)
