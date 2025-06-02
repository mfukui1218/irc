#!/bin/bash

main () {
	setup
	run_test
	cleanup
	is_all_ok
}

setup() {
	# chdir
	cd $(dirname ${BASH_SOURCE[0]})

	# includes
	. ../global.sh
	. ../test_functions.sh
}

cleanup() {
	print_test_report
	echo > /dev/null
}

run_test() {
	fail_pass_command ; stack_status
	success_pass_command ; stack_status
}

##### tests #####

fail_pass_command() {
	local expected_output=$(
		welcome_msg;
		reply_error_464_password_incorrect;
	)
	(
		pass_command "hoge";
	) | test_with_silent "$expected_output"
}

success_pass_command() {
		local expected_output=$(
			welcome_msg;
		)
	(
		pass_command "$PASS";
	) | test_with_silent "$expected_output"
}

main
