# afrodite_embed_binaries(<output-sources-var> <output-include-dir-var>
#                          NAME <symbol> FILE <path> [NAME <symbol> FILE <path> ...])
#
# For each NAME/FILE pair, generates a .cc/.hh pair (via the bin2cpp host
# tool) that embeds FILE's bytes as `extern const unsigned char <NAME>_data[]`
# and `inline constexpr std::size_t <NAME>_size`. Appends the generated .cc
# paths to <output-sources-var> and the generated header directory to
# <output-include-dir-var>, both in the parent scope.
function(afrodite_embed_binaries OUT_SOURCES OUT_INCLUDE_DIR)
    cmake_parse_arguments(ARG "" "" "NAME;FILE" ${ARGN})

    list(LENGTH ARG_NAME name_count)
    list(LENGTH ARG_FILE file_count)
    if(NOT name_count EQUAL file_count)
        message(FATAL_ERROR "afrodite_embed_binaries: NAME and FILE lists must have the same length")
    endif()

    set(generated_dir "${CMAKE_BINARY_DIR}/generated/mesh_data")
    file(MAKE_DIRECTORY "${generated_dir}")

    set(sources "")
    math(EXPR last_index "${name_count} - 1")
    foreach(i RANGE ${last_index})
        list(GET ARG_NAME ${i} sym_name)
        list(GET ARG_FILE ${i} src_file)

        if(NOT EXISTS "${src_file}")
            message(FATAL_ERROR "afrodite_embed_binaries: input file does not exist: ${src_file}")
        endif()

        set(out_cc "${generated_dir}/${sym_name}.mesh.cc")
        set(out_hh "${generated_dir}/${sym_name}.mesh.hh")
        string(TOUPPER "AFRODITE_GENERATED_${sym_name}_MESH_HH" guard)

        add_custom_command(
            OUTPUT "${out_cc}" "${out_hh}"
            COMMAND bin2cpp "${src_file}" "${out_cc}" "${out_hh}" "${guard}" "${sym_name}"
            DEPENDS bin2cpp "${src_file}"
            COMMENT "Embedding ${src_file} -> ${sym_name}_data"
            VERBATIM
        )

        list(APPEND sources "${out_cc}")
    endforeach()

    set(${OUT_SOURCES} ${sources} PARENT_SCOPE)
    set(${OUT_INCLUDE_DIR} ${generated_dir} PARENT_SCOPE)
endfunction()
