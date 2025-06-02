#!/bin/bash

##### variables #####
TEST_TOTAL_CNT=0
TEST_SUCCESS_CNT=0
TEST_FAILURE_CNT=0

##### utils #####
append_text_with_space() {
	local now_text="$1" append_text="$2"
	if [ -z "$now_text" ]; then
		echo "$append_text"
	elif [ -n "$append_text" ] && [ "${append_text:0:1}" = " " ]; then
		echo "$now_text$append_text"
	elif [ -n "$append_text" ]; then
		echo "$now_text $append_text"
	else
		echo "$now_text"
	fi
}

append_prefix() {
	local prefix="$1" text="$2"
	if [ -n "$text" ] && [ "${text:0:1}" != "$prefix" ]; then
		echo "$prefix$text"
	elif [ -n "$text" ]; then
		echo "$text"
	fi
}

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

##### irc commands #####
pass_command() {
	local password="$1"
	echo "PASS $password"
}

nick_command() {
	local nickname="$1"
	echo "NICK $nickname"
}

user_command() {
	local user="$1" mode="$2" unused="$3" realname="$4"
	echo "USER $user $mode $unused :$realname"
}

simple_connect_command() { # command flow: pass -> nick -> user
	local password="$1" nickname="$2"
	pass_command "$password"
	nick_command "$nickname"
	user_command "u-$nickname" "localhost" "*" "r-$nickname"
}

join_command() {
	local channels="$1" keys="$2"
	echo "JOIN $channels"$([ -n "$keys" ] && echo -n " ")"$keys"
}

privmsg_command() {
	local target="$1" text_to_sent="$2"
	local msg="PRIVMSG"
	msg="$(append_text_with_space "$msg" "$target")"
	msg="$(append_text_with_space "$msg" "$(append_prefix ":" "$text_to_sent")")"
	echo "$msg"
}

channel_mode_command() {
	local msg="MODE"
	for arg in "$@"; do
	    msg="$(append_text_with_space "$msg" "$arg")" 
	done
	echo "$msg"
}

##### expected correct reply #####

simple_connect_msg() {
	local nickname="$1"
	welcome_msg
	reply_001_welcome_to_server "$nickname"
}

reply_001_welcome_to_server() {
	local nickname="$1"
	echo ":irc.localhost 001 $nickname :Welcome to the server!^M$"
}

##### none numeric reply #####
welcome_msg() {
	echo "Welcome to IRC server!$"
}

join_reply() {
	local nickname="$1" host="${2:-localhost}" channel="$3"
	none_numeric_reply "$nickname" "$host" "JOIN :$channel"
}

privmsg_reply() {
	local nickname="$1" host="${2:-localhost}" target="$3" msg="$4"
	none_numeric_reply "$nickname" "$host" "PRIVMSG $target :$msg"
}

none_numeric_reply() {
	local nickname="$1" host="${2:-localhost}" msg="$3"
	echo ":$nickname!u-$nickname@$host $msg^M$"
}

##### expected error reply #####
reply_error_401_no_such_nick_or_channel() {
	local nickname="$1" target="$2"
	echo ":irc.localhost 401 $nickname $target :No such nick/channel^M$"
}

reply_error_403_no_such_channel() {
	local nickname="$1" channel="$2"
	echo ":irc.localhost 403 $nickname $channel :No such channel^M$"
}

reply_error_411_no_recipient_given() {
	local nickname="$1"
	echo ":irc.localhost 411 $nickname :No recipient given^M$"
}

reply_error_412_no_text_to_send() {
	local nickname="$1"
	echo ":irc.localhost 412 $nickname :No text to send^M$"
}

reply_error_433_nickname_is_already_in_use() {
	local nickname="$1"
	echo ':irc.localhost 433  '"$nickname"' :Nickname is already in use^M$'
}

reply_error_431_no_nickname_given() {
	echo ':irc.localhost 431  :No nickname given^M$'
}

reply_error_432_erroneous_nickname() {
	local nickname="$1"
	echo ':irc.localhost 432  '"$nickname"' :Erroneous nickname^M$'
}

reply_error_461_not_enough_parameter() {
	local nickname="$1" command="$2"
	echo ":irc.localhost 461 $nickname $command :Not enough parameters^M$"
}

reply_error_451_have_not_registered() {
	local nickname="$1"
	echo ":irc.localhost 451 $nickname :You have not registered^M$"
}

reply_error_462_may_not_reregister() {
	local nickname="$1"
	echo ":irc.localhost 462 $nickname :You may not reregister^M$"
}

reply_error_464_password_incorrect() {
	echo ':irc.localhost 464  :Password incorrect^M$'
}

reply_error_476_bad_channel_mask() {
	echo -n '';
}

##### environment check #####
if [ -z "$HOST" ]; then
	echo "You should set HOST variable"
	exit 1
fi

if [ -z "$PORT" ]; then
	echo "You should set PORT variable"
	exit 1
fi
