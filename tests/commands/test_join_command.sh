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
	fail_join_command_unauthorized ; stack_status
	success_join_command ; stack_status
	fail_join_command_invalid_channel_name ; stack_status
	fail_join_command_no_param ; stack_status
	success_join_command_too_many_channel ; stack_status
}

##### tests #####
fail_join_command_unauthorized() {
	local expected_output=$(
		welcome_msg;
		reply_error_451_have_not_registered;
	)
	(
		join_command "#channel";
	) | test_with_silent "$expected_output"
}

success_join_command() {
	local nickname="nick"
	local expected_output=$(
		simple_connect_msg "$nickname";
		join_reply "$nickname" "" "#channel"
		join_reply "$nickname" "" "#hoge"
	)
	(
		simple_connect_command "$PASS" "$nickname";
		join_command "#channel,#hoge";
	) | test_with_silent "$expected_output"
}

fail_join_command_invalid_channel_name() {
	local nickname="nick"
	local expected_output=$(
		simple_connect_msg "nick";
		reply_error_403_no_such_channel "$nickname" "!invalid"
	)
	(
		simple_connect_command "$PASS" "nick";
		join_command "!invalid";
	) | test_with_silent "$expected_output"
}

fail_join_command_no_param() {
	local nickname="nick"
	local expected_output=$(
		simple_connect_msg "$nickname";
		reply_error_461_not_enough_parameter "$nickname" "JOIN";
	)
	(
		simple_connect_command "$PASS" "$nickname";
		join_command "";
	) | test_with_silent "$expected_output"
}

success_join_command_too_many_channel() {
	local nickname="nick"
	local expected_output=$(
		simple_connect_msg "$nickname";
		join_reply "$nickname" "" "#a";
		join_reply "$nickname" "" "#b";
		join_reply "$nickname" "" "#c";
		join_reply "$nickname" "" "#d";
		join_reply "$nickname" "" "#e";
		join_reply "$nickname" "" "#f";
		join_reply "$nickname" "" "#g";
		join_reply "$nickname" "" "#h";
		join_reply "$nickname" "" "#i";
		join_reply "$nickname" "" "#j";
		join_reply "$nickname" "" "#k";
		join_reply "$nickname" "" "#l";
	)
	(
		simple_connect_command "$PASS" "$nickname";
		join_command "#a,#b,#c,#d,#e,#f,#g,#h,#i,#j,#k,#l";
	) | test_with_silent "$expected_output"
}

main
