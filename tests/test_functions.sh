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
	local result=$(nc -q $NC_TIMEOUT $HOST $PORT | cat -A)
	echo "$result"
	if diff <(echo "$expected_output") <(echo "$result") > /dev/stderr; then
		return 0
	else
		return 1
	fi
}

stack_status() {
	tmpstat=$?
	STAT=$((STAT + tmpstat))
	if [ $tmpstat -ne 0 ]; then
		fail "${BASH_SOURCE[1]}:${BASH_LINENO[0]}"
	fi
}

is_all_ok() {
	test "$STAT" = "0"
}

fail() {
	printf "%b" "\e[31mfail: \e[m"
	echo "$@"
}

if [ -z "$NC_TIMEOUT" ]; then
	echo "You should set NC_TIMEOUT variable"
	exit 1
fi

if [ -z "$HOST" ]; then
	echo "You should set HOST variable"
	exit 1
fi

if [ -z "$PORT" ]; then
	echo "You should set PORT variable"
	exit 1
fi
