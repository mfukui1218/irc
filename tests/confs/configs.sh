#!/bin/bash

# Usage: . <(bash configs.sh)
main() {
	# set configs
	set_configs
	print_configs
	print_functions
}

set_configs() {
	declare -gA configs;

	# mandatory config
	local relational_path=../
	configs["ROOT"]="$(cd "$(dirname ${BASH_SOURCE[0]})"; cd "$relational_path"; pwd)"
	
	# your project config
	configs["PORT"]=6667
	configs["HOST"]=localhost
	configs["PASS"]=pass
	configs["SERVER_PROC"]=${configs["ROOT"]}/../ircserv
}

print_configs() {
	# print config
	if [ -n "$1" ]; then
		echo "${configs["$1"]}"
	else
		for key in "${!configs[@]}"; do
			echo $key="${configs["$key"]}"
		done
	fi
}

print_functions() {
	print_function_include_all
	print_function_include
}

print_function_include_all() {
	echo '
	include_all() {
		local directory="$1"
		local modules="$(find '${configs["ROOT"]}'/"$directory" -name "*.module.sh")"
		for module in $modules; do
			. "$module"
		done
	}'
}

print_function_include() {
	echo '
	include() {
		local module_name="$1"
		local module="$(find '${configs["ROOT"]}'/modules -name "$module_name")"
		. "$module"
	}'
}

main "$@"
