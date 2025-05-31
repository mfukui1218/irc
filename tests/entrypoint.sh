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

	if ! test -x $SERVER_PROC; then
		echo "You have to compile '"$(basename "$SERVER_PROC")"' program"
		exit 1
	fi
	valgrind --leak-check=full -q ./$SERVER_PROC $PORT $PASS > /dev/null &
	SERVER_PID=$!
	sleep 1
}

cleanup() {
	kill $SERVER_PID
}

run_test() {
	bash commands/test_pass_command.sh
}

##### main #####
main
