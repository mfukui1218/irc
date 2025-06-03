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
	fail_privmsg_command_unauthorized ; stack_status
	success_privmsg_command_user2user ; stack_status
	success_privmsg_command_user2channel ; stack_status
	fail_privmsg_command_no_such_nick ; stack_status
	fail_privmsg_command_no_such_channel ; stack_status
	fail_privmsg_command_no_text_to_send ; stack_status
	fail_privmsg_command_not_enough_parameter ; stack_status
	fail_privmsg_command_no_recipient_given ; stack_status
}

##### tests #####
fail_privmsg_command_unauthorized() {
	local expected_output=$(
		welcome_msg;
		reply_error_451_have_not_registered;
	)
	(
		privmsg_command "unused-target" "unused-msg";
	) | test_with_silent "$expected_output"
}

success_privmsg_command_user2user() {
	local nickname_1="nick1" nickname_2="nick2"
	local msg="hogehoge"
	
	local expected_output_1=$(
		simple_connect_msg "$nickname_1";
	)
	(
		simple_connect_command "$PASS" "$nickname_1"
		sleep 0.5;
		privmsg_command "$nickname_2" "$msg";
	) | test_with_silent "$expected_output_1" &
	local pid1="$!"
	local expected_output_2=$(
		simple_connect_msg "$nickname_2";
		privmsg_reply "$nickname_1" "" "$nickname_2" "$msg"
	)
	(
		simple_connect_command "$PASS" "$nickname_2"
		sleep 1;
	) | test_with_silent "$expected_output_2"
	local stat=$?
	wait "$pid1"
	return "$stat"
}

success_privmsg_command_user2channel() {
	local nickname_1="nick1" nickname_2="nick2"
	local msg="user2channel message" channel="#channel"
	
	local expected_output_1=$(
		simple_connect_msg "$nickname_1";
		join_reply "$nickname_1" "" "$channel"
		join_reply "$nickname_2" "" "$channel"
	)
	(
		simple_connect_command "$PASS" "$nickname_1"
		join_command "$channel"
		sleep 0.5;
		privmsg_command "$channel" "$msg";
		sleep 0.5;
	) | test_with_silent "$expected_output_1" &
	local pid1="$!"
	local expected_output_2=$(
		simple_connect_msg "$nickname_2";
		join_reply "$nickname_2" "" "$channel"
		privmsg_reply "$nickname_1" "" "$channel" "$msg"
	)
	(
		simple_connect_command "$PASS" "$nickname_2"
		join_command "$channel"
		sleep 1.5;
	) | test_with_silent "$expected_output_2"
	local stat=$?
	wait "$pid1"
	return "$stat"
}

fail_privmsg_command_no_such_nick() {
	local nickname="nick"
	local target="no_user"
	local msg="can not send"
	local expected_output=$(
		simple_connect_msg "$nickname";
		reply_error_401_no_such_nick_or_channel "$nickname" "$target"
	)
	(
		simple_connect_command "$PASS" "$nickname"
		privmsg_command "$target" "$msg";
	) | test_with_silent "$expected_output"
}

fail_privmsg_command_no_such_channel() {
	local nickname="nick"
	local target="#no_channel"
	local msg="can not send"
	local expected_output=$(
		simple_connect_msg "$nickname";
		reply_error_401_no_such_nick_or_channel "$nickname" "$target"
	)
	(
		simple_connect_command "$PASS" "$nickname"
		privmsg_command "$target" "$msg";
	) | test_with_silent "$expected_output"
}

fail_privmsg_command_no_text_to_send() {
	local nickname="nick"
	local target="#no_channel"
	local msg=":"
	local expected_output=$(
		simple_connect_msg "$nickname";
		reply_error_412_no_text_to_send "$nickname"
	)
	(
		simple_connect_command "$PASS" "$nickname"
		privmsg_command "$target" "$msg";
	) | test_with_silent "$expected_output"
}

fail_privmsg_command_not_enough_parameter() {
	local nickname="nick"
	local target="#no_channel"
	local msg=""
	local expected_output=$(
		simple_connect_msg "$nickname";
		reply_error_461_not_enough_parameter "$nickname" "PRIVMSG"
	)
	(
		simple_connect_command "$PASS" "$nickname";
		privmsg_command " ";
	) | test_with_silent "$expected_output"
}

fail_privmsg_command_no_recipient_given() {
	local nickname="nick"
	local target="#no_channel"
	local msg=""
	local expected_output=$(
		simple_connect_msg "$nickname";
		reply_error_411_no_recipient_given "$nickname"
	)
	(
		simple_connect_command "$PASS" "$nickname";
		privmsg_command "  ";
	) | test_with_silent "$expected_output"
}

main
