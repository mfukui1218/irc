#!/bin/bash

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
