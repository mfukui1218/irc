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
	success_nick_command ; stack_status
	fail_nick_command ; stack_status
}

##### tests #####

success_nick_command() {
	local expected_output=$(welcome_msg;)
	(
		pass_command "$PASS";
		nick_command "test_user";
	) | test_with_silent "$expected_output"
}

fail_nick_command() {
	local expected_output=$(
		welcome_msg;
		reply_error_433_nickname_is_already_in_use dup_nick
	)
	(
		pass_command "$PASS";
		nick_command "dup_nick";
		sleep 1;
	) | nc_connect > /dev/null &
	(
		pass_command "$PASS";
		nick_command "dup_nick";
	) | test_with_silent "$expected_output"
}

main
