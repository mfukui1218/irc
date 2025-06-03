#!/bin/bash

##### test base function ######
test_with_logging() {
	local expected_output="$1"
	local log_filename="$2"
	test_with_stdout "$expected_output" > "$log_filename"
}

test_with_silent() {
	local expected_output="$1"
	test_with_stdout "$expected_output" > /dev/null
}

test_with_stdout() {
	local expected_output="$1"
	local result=$(nc_connect | cat -A)
	echo "$result"
	if diff <(echo "$expected_output") <(echo "$result") > /dev/stderr; then
		return 0
	else
		return 1
	fi
}

nc_connect() {
	nc -q 0 $HOST $PORT
}

##### global variable check #####
if [ -z "$HOST" ]; then
	echo "You should set HOST variable"
	exit 1
fi

if [ -z "$PORT" ]; then
	echo "You should set PORT variable"
	exit 1
fi
