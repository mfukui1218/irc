#!/bin/bash

##### variables #####
TEST_TOTAL_CNT=0
TEST_SUCCESS_CNT=0
TEST_FAILURE_CNT=0

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

print_test_report_header() {
    printf "\n===== 🧪 TEST REPORT SUMMARY =====\n"
    printf "%-30s | %7s | %7s | %7s | %s\n" "File" "Total" "Success" "Fail" "Status"
    printf -- "-------------------------------+---------+---------+---------+--------\n"
}

print_test_report() {
    local file
    local total success fail

	file=${BASH_SOURCE[1]}
    total=${TEST_TOTAL_CNT}
	success=${TEST_SUCCESS_CNT:-0}
	fail=${TEST_FAILURE_CNT:-0}

	local status="✅ PASS"
	[[ $fail -ne 0 ]] && status="❌ FAIL"

	printf "%-30s | %7d | %7d | %7d | %s\n" "$file" "$total" "$success" "$fail" "$status"
}

print_test_report_footer() {
    printf "======================================================================\n\n"
}

nc_connect() {
	nc -q $NC_TIMEOUT $HOST $PORT
}

stack_status() {
	tmpstat=$?
	STAT=$((STAT + tmpstat))
	((TEST_TOTAL_CNT++))
	if [ $tmpstat -ne 0 ]; then
		((TEST_FAILURE_CNT++))
		fail "${BASH_SOURCE[1]}:${BASH_LINENO[0]}"
	else
		((TEST_SUCCESS_CNT++))
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
