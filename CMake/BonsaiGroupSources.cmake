function(bonsai_group_sources _current_source_dir _source_root_path _group_prefix _source_list)
    foreach(_source IN ITEMS ${_source_list})
        get_filename_component(_source_path "${_source}" PATH)
        file(RELATIVE_PATH _source_path_rel "${_current_source_dir}${_source_root_path}" "${_current_source_dir}/${_source_path}")
        string(REPLACE "/" "\\" _group_path "${_group_prefix}${_source_path_rel}")
        source_group("${_group_path}" FILES "${_source}")
    endforeach()
endfunction()