include(CheckCXXCompilerFlag)
include(CheckCCompilerFlag)

macro(make_flag_for_check flag outVar)
	# We cannot check for -Wno-foo as this won't throw a warning so we must check for the -Wfoo option directly
	# http://stackoverflow.com/questions/38785168/cc1plus-unrecognized-command-line-option-warning-on-any-other-warning
	string(REGEX REPLACE "^-Wno-(error=)?" "-W" ${outVar} ${flag})
endmacro()

function(check_warning lang warningFlag var)
	make_flag_for_check(${warningFlag} escapedFlag)
    # Remove special chars
	string(MAKE_C_IDENTIFIER ${lang}_WARNING_${escapedFlag}_SUPPORTED VarName)
	if(lang STREQUAL "C")
		check_c_compiler_flag(${escapedFlag} ${VarName})
	elseif(lang STREQUAL "CXX")
		check_cxx_compiler_flag(${escapedFlag} ${VarName})
	else()
		message(FATAL_ERROR "Invalid language: ${lang}")
	endif()
	set(${var} ${${VarName}} PARENT_SCOPE)
endfunction()

function(check_and_add_warnings)
	include(ParseArguments)
	set(options)
	set(one_value_options TARGET VISIBILITY)
	set(multi_value_options ALL C CXX)
	set(required_options TARGET VISIBILITY)
	parse_arguments("${options}" "${one_value_options}" "${multi_value_options}" "${required_options}" ${ARGN})

	set(warningFlagsALL "")
	set(warningFlagsC "")
	set(warningFlagsCXX "")
	foreach(lang IN LISTS multi_value_options)
		foreach(warningFlag IN LISTS ARG_${lang})
  		set(supportedC OFF)
			set(supportedCXX OFF)
			if(lang MATCHES "^(C|ALL)$")
				check_warning(C ${warningFlag} supportedC)
			endif()
			if(lang MATCHES "^(CXX|ALL)$")
				check_warning(CXX ${warningFlag} supportedCXX)
			endif()
			if(supportedC AND supportedCXX)
				list(APPEND warningFlagsALL ${warningFlag})
			elseif(supportedC)
				list(APPEND warningFlagsC ${warningFlag})
				if(lang STREQUAL "ALL")
					message(AUTHOR_WARNING "${warningFlag} addes in ALL only supported in C. Put it to C section?")
				endif()
			elseif(supportedCXX)
				list(APPEND warningFlagsCXX ${warningFlag})
				if(lang STREQUAL "ALL")
					message(AUTHOR_WARNING "${warningFlag} addws in ALL only supported in CXX. Put it to CXX section?")
				endif()
			endif()
		endforeach()
	endforeach()
	# Create a single generator expression or ommit if for all
	foreach(lang IN LISTS multi_value_options)
		if(warningFlags${lang})
			if(NOT lang STREQUAL "ALL")
				set(flags "$<$<COMPILE_LANGUAGE:${lang}>:${warningFlags${lang}}>")
			else()
				set(flags "${warningFlags${lang}}")
			endif()
			target_compile_options(${ARG_TARGET} ${ARG_VISIBILITY} ${flags})
		endif()
	endforeach()
endfunction()
