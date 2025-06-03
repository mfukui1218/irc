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
	. <(bash ../confs/configs.sh)
	include_all "modules"
}

cleanup() {
	print_test_report
	echo > /dev/null
}

run_test() {
	success_nick_command ; stack_status
	fail_nick_command_already_in_use ; stack_status
	fail_nick_command_missing_argument ; stack_status
	fail_nick_command_invalid_format ; stack_status
	nick_command_change_name ; stack_status
	nick_command_re_change_name ; stack_status
}

##### tests #####

success_nick_command() {
	local expected_output=$(welcome_msg;)
	(
		pass_command "$PASS";
		nick_command "test_user";
	) | test_with_silent "$expected_output"
}

fail_nick_command_already_in_use() {
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

fail_nick_command_missing_argument() {
	local expected_output=$(
		welcome_msg;
		reply_error_431_no_nickname_given
	)
	(
		pass_command "$PASS";
		nick_command "";
	) | test_with_silent "$expected_output"
}

fail_nick_command_invalid_format() {
	local expected_output=$(
		welcome_msg;
		reply_error_432_erroneous_nickname 'nick!name'
	)
	(
		pass_command "$PASS";
		nick_command "nick!name";
	) | test_with_silent "$expected_output"
}

nick_command_change_name() {
	local expected_output=$(
		welcome_msg;
	)
	(
		pass_command "$PASS";
		nick_command "test_user";
		nick_command "new_name";
	) | test_with_silent "$expected_output"
}

nick_command_re_change_name() {
	local expected_output=$(
		welcome_msg;
	)
	(
		pass_command "$PASS";
		nick_command "test_user";
		nick_command "new_name";
		nick_command "test_user";
	) | test_with_silent "$expected_output"
}

main
