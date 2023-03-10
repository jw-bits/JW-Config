# JW-Config

A "just works" simple config file for standard atomic types

File format:

	key0=value0
	key1=value1
	key2=value2
	...

	key = Only valid characters are ASCII letters and numbers
	
	All key, value pairs MUST contain an equal sign (=)
	Spacing around (=) is valid
	Example:  key0 = value0

	value = integers, floats, booleans, strings

	Valid integers
		key0 = 5555
		key0 = -4321

		Commas are NOT allowed

	Valid floats
		key0 = 3.142
		key1 = -3.142
		key2 = .456789
		key3 = -.895

		Commas are NOT allowed
		MUST contain a period

	Valid booleans
		key0=true
		key1=false
		key2=TRUE
		key3=FALSE

		Only true or false allowed (upper-lowercase doesn't matter)

	Valid strings
		key0="This is a string"
		key1="This is a \"string\" with quotes in it"

		MUST begin and end with quotes.
