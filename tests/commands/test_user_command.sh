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
	local modules="$(find ../modules -name "*.sh")"
	for module in $modules; do
		. "$module"
	done
}

cleanup() {
	print_test_report
	echo > /dev/null
}

run_test() {
	success_user_command_unauthorized ; stack_status
	success_user_command ; stack_status
	success_user_command_after_nick ; stack_status
	fail_user_command_missing_args ; stack_status
	fail_user_command_duplicate ; stack_status
	success_user_command_before_nick ; stack_status
}

##### tests #####
success_user_command_unauthorized() {
	local expected_output=$(
		welcome_msg;
	)
	(
		user_command "hoge" "localhost" "*" "suge";
	) | test_with_silent "$expected_output"
}

success_user_command() {
	local expected_output=$(
		welcome_msg;
		reply_001_welcome_to_server "nick"
	)
	(
		pass_command "$PASS";
		nick_command "nick";
		user_command "hoge" "localhost" "*" "suge";
	) | test_with_silent "$expected_output"
}

success_user_command_after_nick() {
	local expected_output=$(
		welcome_msg;
		reply_001_welcome_to_server "nick"
	)
	(
		pass_command "$PASS";
		nick_command "nick";
		user_command "realname" "localhost" "*" "description";
	) | test_with_silent "$expected_output"
}

fail_user_command_missing_args() {
	local expected_output=$(
		welcome_msg;
		reply_error_461_not_enough_parameter "nick" "USER"
	)
	(
		pass_command "$PASS";
		nick_command "nick";
		user_command "onlyonearg";
	) | test_with_silent "$expected_output"
}

fail_user_command_duplicate() {
	local expected_output=$(
		welcome_msg;
		reply_001_welcome_to_server "nick";
		reply_error_462_may_not_reregister "nick"
	)
	(
		pass_command "$PASS";
		nick_command "nick";
		user_command "realname" "localhost" "*" "desc";
		user_command "again" "host" "*" "desc";
	) | test_with_silent "$expected_output"
}

success_user_command_before_nick() {
	local expected_output=$(
		welcome_msg;
	)
	(
		pass_command "$PASS";
		user_command "name" "host" "*" "desc";
	) | test_with_silent "$expected_output"
}

main
