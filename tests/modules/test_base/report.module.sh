#!/bin/bash

##### variables #####
TEST_TOTAL_CNT=0
TEST_SUCCESS_CNT=0
TEST_FAILURE_CNT=0
STAT=0

##### test report function #####
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
