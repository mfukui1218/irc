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
	fail_channel_mode_command_unauthorized ; stack_status
	fail_channel_mode_command ; stack_status
	success_channel_mode_command_mode_invite_only ; stack_status
	fail_channel_mode_command_mode_invite_only ; stack_status
}

##### tests #####
fail_channel_mode_command_unauthorized() {
	local expected_output=$(
		welcome_msg;
		reply_error_451_have_not_registered_with_command "JOIN";
	)
	(
		channel_mode_command "unauth";
	) | test_with_silent "$expected_output"
}

fail_channel_mode_command() {
	local nickname_1="nick1" nickname_2="nick2"
	local channel="#havent_permission" modes="+i"
	
	local expected_output_1=$(
		simple_connect_msg "$nickname_1";
		join_reply "$nickname_1" "" "$channel"
	)
	(
		simple_connect_command "$PASS" "$nickname_1"
		join_command "$channel"
		sleep 0.5;
	) | test_with_silent "$expected_output_1" &
	local pid1="$!"
	local expected_output_2=$(
		simple_connect_msg "$nickname_2";
		join_reply "$nickname_2" "" "$channel"
		__482_not_channel_operator "$channel";
	)
	(
		sleep 0.5;
		simple_connect_command "$PASS" "$nickname_2"
		join_command "$channel";
		channel_mode_command "$channel" "$modes"
	)| test_with_silent "$expected_output_2"
	local stat=$?
	wait "$pid1"
	return "$stat"
}

success_channel_mode_command_mode_invite_only() {
	local nickname="nick"
	local channel="#invite_only_channel" modes="+i"
	
	local expected_output=$(
		simple_connect_msg "$nickname";
		join_reply "$nickname" "" "$channel"
		mode_reply "$nickname" "" "$channel" "$modes"
	)
	(
		simple_connect_command "$PASS" "$nickname"
		join_command "$channel"
		channel_mode_command "$channel" "$modes";
	) | test_with_silent "$expected_output"
}

fail_channel_mode_command_mode_invite_only() {
	local nickname_1="nick1" nickname_2="nick2"
	local channel="#invite_only_channel2" modes="+i"
	
	local expected_output_1=$(
		simple_connect_msg "$nickname_1";
		join_reply "$nickname_1" "" "$channel"
		mode_reply "$nickname_1" "" "$channel" "$modes"
	)
	(
		simple_connect_command "$PASS" "$nickname_1"
		join_command "$channel"
		channel_mode_command "$channel" "$modes";
		sleep 0.5;
	) | test_with_silent "$expected_output_1" &
	local pid1="$!"
	local expected_output_2=$(
		simple_connect_msg "$nickname_2";
		reply_error_400_unknown_error "$nickname_2";
	)
	(
		sleep 0.5;
		simple_connect_command "$PASS" "$nickname_2"
		join_command "$channel";
	) | test_with_silent "$expected_output_2"
	local stat=$?
	wait "$pid1"
	return "$stat"
}

main
