#!/bin/bash

# includes

# main

main () {
	setup
	run_test
	cleanup
}

setup() {
	# chdir
	cd $(dirname ${BASH_SOURCE[0]})

	# includes
	. ./global.sh
	. ./test_functions.sh

	if ! test -x $SERVER_PROC; then
		echo "You have to compile '"$(basename "$SERVER_PROC")"' program"
		exit 1
	fi
	valgrind --leak-check=full -q ./$SERVER_PROC $PORT $PASS > server.log &
	SERVER_PID=$!
	sleep 1
}

cleanup() {
	kill $SERVER_PID
}

run_test() {
	print_test_report_header
	bash commands/test_pass_command.sh
	bash commands/test_nick_command.sh
	print_test_report_footer
}

##### main #####
main
