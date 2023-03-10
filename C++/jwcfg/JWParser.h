#ifndef __JWPARSER_H__
#define __JWPARSER_H__

#include "Utilities.h"
#include <list>
#include <map>
#include <string>

/*
	File format is as follows:

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
*/

class JWParser
{
public:

	class Value
	{
		public:
		
			enum class EType
			{
				eEmpty = 0,
				eInt,
				eFloat,
				eBool,
				eString
			};

			Value() : mType(EType::eEmpty), mInt(0), mFloat(0.0f), mBool(false) {}

			void set(int i) { mType = EType::eInt; mInt = i; }
			void set(float f) { mType = EType::eFloat; mFloat = f; }
			void set(bool b) { mType = EType::eBool; mBool = b; }
			void set(const std::string &s) { mType = EType::eString; mString = s; }

			EType getType() const { return mType; }
			int asInt() const { return mInt; }
			float asFloat() const { return mFloat; }
			bool asBool() const { return mBool; }
			const std::string& asString() const { return mString; }

		private:

			EType mType;
			int mInt;
			float mFloat;
			bool mBool;
			std::string mString;
	};

	struct KeyComparer
	{
		bool operator()(const std::string &a, const std::string &b) const
		{
			if (a.length() == b.length())
			{
				return (a.compare(b) < 0);
			}
			else
			{
				return (a.length() < b.length());
			}
		}
	};

	enum class EReturnCode
	{
		eSuccess = 0,
		eInvalidKey,
		eMalformedData,
		eNoValidData,
		eUnexpectedValue
	};
	
	typedef std::pair<std::string, Value> Pair;
	typedef std::map<std::string, Value, KeyComparer> Map;
	typedef Map::iterator Iter;
	typedef Map::const_iterator ConstIter;

	static EReturnCode Parse(const Buffer &buffer, Map &jwMap);

private:

	static EReturnCode ParseKeyValue(const std::string &str, Map &jwMap);
	static EReturnCode ParseValue(const std::string &str, Value &value);
};

#endif
