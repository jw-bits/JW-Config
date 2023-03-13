#include "JWParser.h"

void AddToStringList(const std::string &str, std::list<std::string> &strList)
{
	int equalCount = CharaterCount(str.c_str(), str.length(), '=');

	if (equalCount == 1)
	{
		strList.push_back(str);
	}
	else if (equalCount > 1) // Only possible if a string...
	{
		int quoteCount = CharaterCount(str.c_str(), str.length(), '"');

		if (quoteCount >= 2)
			strList.push_back(str); // Assume valid string for now
	}
	else
	{
		printf("Skipping this malformed JW key-value pair: %s\n", str.c_str());
	}
}

JWParser::EReturnCode JWParser::Parse(const Buffer &buffer, Map &jwMap)
{
	std::list<std::string> strPairs;
	std::string str;

	const u8 *buf = buffer.get();

	for (u32 i = 0; i < buffer.getSize(); ++i)
	{
		c8 ch = (c8)buf[i];

		if (IsNewLine(ch) || IsEOF(ch))
		{
			if (ch == '\r') // Skip past \r,
				continue;

			if (str.length() > 0)
			{
				AddToStringList(str, strPairs);
				str.clear(); // No matter what, clear for next go around
			} 
		}
		else
		{
			c8 s[2] = { ch, 0 };
			str.append(s);
		}
	}

	// Check to see if final string needs to be added!
	if (str.length() > 0)
	{
		AddToStringList(str, strPairs);
		str.clear(); // No matter what, clear for next go around
	} 

	if (strPairs.size() == 0)
		return EReturnCode::eNoValidData;

	jwMap.clear(); // Clear out anything in map, ready to fill with new values

	do
	{
		const std::string &s = strPairs.back();
		
		if (ParseKeyValue(s, jwMap) != EReturnCode::eSuccess)
		{
			printf("Unable to parse following key-value pair: %s\n", s.c_str());			
		}

		strPairs.pop_back();
	}
	while(strPairs.size() > 0);

	return EReturnCode::eSuccess;
}

JWParser::EReturnCode JWParser::ParseKeyValue(const std::string &str, Map &jwMap)
{
	const c8 *s = str.c_str();
	int len = (int)str.length();

	const int equalIdx = FindFirstOf(s, (u32)len, '=');

	if (equalIdx == -1)
		return EReturnCode::eInvalidKey;
	
	// Remove leading whitespace
	int sIdx = 0;

	while((sIdx < len) && (IsWhiteSpace(s[sIdx])))
	{
		++sIdx;
	}

	if (sIdx >= equalIdx)
		return EReturnCode::eInvalidKey;

	// Remove trailing whitespace
	int eIdx = equalIdx;

	while((eIdx > 1) && (IsWhiteSpace(s[eIdx - 1])))
	{
		--eIdx;
	}

	if (eIdx < sIdx)
		return EReturnCode::eInvalidKey;

	std::string key(s + sIdx, eIdx - sIdx);

	// Remove leading whitespace
	sIdx = equalIdx + 1;
	
	while((sIdx < len) && (IsWhiteSpace(s[sIdx])))
		++sIdx;

	if (sIdx >= len)
		return EReturnCode::eUnexpectedValue;

	// Remove trailing whitespace
	eIdx = len;

	while((eIdx > equalIdx) && (IsWhiteSpace(s[eIdx - 1])))
	{
		--eIdx;
	}

	if (eIdx < sIdx)
		return EReturnCode::eInvalidKey;

	std::string value(s + sIdx, eIdx - sIdx);

	Value jwVal;
	
	JWParser::EReturnCode rc = ParseValue(value, jwVal);

	if (rc != EReturnCode::eSuccess)
		return rc;
	
	jwMap.insert(Pair(key, jwVal));
		return EReturnCode::eSuccess;
}

JWParser::EReturnCode JWParser::ParseValue(const std::string &str, Value &jwValue)
{
	const c8 *s = str.c_str();
	u32 len = str.length();
	c8 ch = s[0];

	// Possibily an integer or float
	if (IsDigit(ch) || ch == '-' || ch == '.')
	{
		bool foundPeriod = (ch == '.');

		if (!foundPeriod)
		{
			int idx = FindFirstOf(s, len, '.');

			if (idx != -1)
				foundPeriod = true;
		}

		if (foundPeriod) // It's a float
		{
			float f = (float)atof(s);
			jwValue.set(f);
		}
		else // It's a int
		{
			int i = atoi(s);
			jwValue.set(i);
		}

		return EReturnCode::eSuccess;
	}
	// Possibly a bool
	else if (ch == 't' || ch == 'T' || ch == 'f' || ch == 'F')
	{
		if (len == 4) // Check for truth
		{
			bool R = (s[1] == 'r' || s[1] == 'R');
			bool U = (s[2] == 'u' || s[2] == 'U');
			bool E = (s[3] == 'e' || s[3] == 'E');

			if (R && U && E)
			{
				jwValue.set(true);
					return EReturnCode::eSuccess;
			}
			else
				return EReturnCode::eMalformedData;
		}
		else if (len == 5) // Check for falsehood
		{
			bool A = (s[1] == 'a' || s[1] == 'A');
			bool L = (s[2] == 'l' || s[2] == 'L');
			bool S = (s[3] == 's' || s[3] == 'S');
			bool E = (s[4] == 'e' || s[4] == 'E');

			if (A && L && S && E)
			{
				jwValue.set(false);
					return EReturnCode::eSuccess;
			}
			else
				return EReturnCode::eMalformedData;
		}
	}
	// Possibly a string
	else if (ch == '"')
	{
		if (str[len - 1] == '"')
		{
			std::string subStr(str, 1, len - 2); 
			jwValue.set(subStr);
				return EReturnCode::eSuccess;
		}
	}

	return EReturnCode::eMalformedData;
}