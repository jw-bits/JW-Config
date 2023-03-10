#include "JWParser.h"
#include <fstream>
using namespace std;

// Test app...
int main()
{
	// Load test file
	ifstream fs("JWTest.txt", ios::binary);

	if (fs.fail())
		return -1;

	fs.seekg(0, ios::end);	
	u32 totalFileSize = fs.tellg();

	printf("Total File Size = %d\n\n", totalFileSize); 

	Buffer buffer;
	buffer.init(totalFileSize);
	
	char* b = NULL;
	buffer.grab(b);

	fs.seekg(0, ios::beg);
	fs.read(b, totalFileSize);
	fs.close();

	JWParser::Map jwMap;
	JWParser::Parse(buffer, jwMap);

	for (JWParser::Iter it = jwMap.begin(); it != jwMap.end(); it++)
	{
		const JWParser::Value& v = it->second;

		// Print key
		printf("Key = %s ", it->first.c_str());
		
		// Print value
		switch (v.getType())
		{
			case JWParser::Value::EType::eBool: 
			{
				if (v.asBool() == true)
					printf("Bool = TRUE");
				else
					printf("Bool = FALSE");
			}
			break;

			case JWParser::Value::EType::eFloat:
			{
				printf("Float = %f", v.asFloat());
			}
			break;

			case JWParser::Value::EType::eInt:
			{
				printf("Int = %d", v.asInt());
			}
			break;

			case JWParser::Value::EType::eString:
			{
				printf("String = %s", v.asString().c_str());
			}
			break;
		}

		printf("\n");
	}

	return 0;
}


