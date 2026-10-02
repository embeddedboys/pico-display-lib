# Panel configuration helpers, shared by the driver CMakeLists files.
#
# A panel config (configs/<name>.cmake) sets CMake variables; these helpers turn
# the ones it set into the -D macros the C sources read.  The point of routing
# everything through here is that "how a config variable becomes a macro" lives in
# one place instead of being re-spelled in six driver CMakeLists files, and that
# the two ways that can go silently wrong are handled once:
#
#   1. Emitting a variable nobody set produces `-DNAME=`, i.e. a macro defined
#      *empty*.  That beats the `#ifndef NAME` defaults in include/config.h (they
#      no longer fire), and `#elif NAME` becomes `#elif` with no expression, which
#      is a hard error.  panel_emit_value() therefore skips undefined variables.
#
#   2. A 0/1 switch that is only *sometimes* defined makes `#if X` and `#ifdef X`
#      disagree.  panel_emit_flag() always defines its flags.

# panel_emit_value(<VAR> [<VAR>=<MACRO>]...)
#
#   Emit -D<VAR>=<value>, or -D<MACRO>=<value> to emit under another name.
#   Variables the selected config did not set are skipped, so whatever default
#   include/config.h carries for them still applies.
function(panel_emit_value)
    foreach(spec IN LISTS ARGN)
        string(REPLACE "=" ";" _parts "${spec}")
        list(GET _parts 0 _var)
        if(DEFINED ${_var} AND NOT "${${_var}}" STREQUAL "")
            list(LENGTH _parts _n)
            if(_n GREATER 1)
                list(GET _parts 1 _macro)
            else()
                set(_macro "${_var}")
            endif()
            target_compile_definitions(${LIBRARY_NAME} PUBLIC ${_macro}=${${_var}})
        endif()
    endforeach()
endfunction()

# panel_emit_flag(<VAR>...)
#
#   Emit -D<VAR>=1 when the config set it truthy, -D<VAR>=0 otherwise.  Always
#   defined, so a flag means the same thing whether it is tested with #if or
#   #ifdef.
function(panel_emit_flag)
    foreach(_var IN LISTS ARGN)
        if(${_var})
            target_compile_definitions(${LIBRARY_NAME} PUBLIC ${_var}=1)
        else()
            target_compile_definitions(${LIBRARY_NAME} PUBLIC ${_var}=0)
        endif()
    endforeach()
endfunction()

# panel_require(<VAR>...)
#
#   Fail at configure time when the selected panel config is missing something the
#   build cannot invent.  This is what turns "typo in a config, or a config copied
#   from a panel with a different bus, compiles anyway and misbehaves on hardware"
#   into an error naming the variable and the config.
function(panel_require)
    foreach(_var IN LISTS ARGN)
        if(NOT DEFINED ${_var})
            message(FATAL_ERROR
                "panel config '${PUD_CONFIG}' does not set ${_var}")
        endif()
    endforeach()
endfunction()
