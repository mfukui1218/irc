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

	if ! kill -0 "$SERVER_PID" 2>/dev/null; then
		echo "[ERROR] couldn't start server"
		exit 1
	fi
}

cleanup() {
	sleep 1
	kill $SERVER_PID

	echo "wait for TIME-WAIT port $PORT"
	while ss -tan | grep ":$PORT" > /dev/null; do
		sleep 3;
		echo -n ".";
	done
	echo "done"
}

run_test() {
	print_test_report_header
	bash commands/test_pass_command.sh
	bash commands/test_nick_command.sh
	bash commands/test_user_command.sh
	bash commands/test_join_command.sh
	bash commands/test_privmsg_command.sh
	print_test_report_footer
}

##### main #####
main
