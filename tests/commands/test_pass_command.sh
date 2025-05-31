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
	echo > /dev/null
}

run_test() {
	local tmpstat
	fail_pass_command ; stack_status
	success_pass_command ; stack_status
}

##### tests #####

fail_pass_command() {
	local expected_output='Welcome to IRC server!$
:irc.localhost 464  :Password incorrect^M$'
	(
		echo "PASS hoge";
	) | test_with_silent "$expected_output"
}

success_pass_command() {
		local expected_output='Welcome to IRC server!$'
	(
		echo "PASS $PASS";
	) | test_with_silent "$expected_output"
}

main
