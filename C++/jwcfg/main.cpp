#include "JWParser.h"
#include <fstream>
#include <ctime>
using namespace std;

// Test app...
int main()
{
	// Start the file reading
	clock_t startTime = clock();
	clock_t totalTime;

	// Load test file
	ifstream fs("JWTest.txt", ios::binary);

	if (fs.fail())
		return -1;

	fs.seekg(0, ios::end);	
	u32 totalFileSize = fs.tellg();

	printf("Total file size in bytes  = %d\n", totalFileSize); 

	Buffer buffer;
	buffer.init(totalFileSize);
	
	char* b = NULL;
	buffer.grab(b);

	fs.seekg(0, ios::beg);
	fs.read(b, totalFileSize);
	fs.close();

	totalTime = clock() - startTime;
	printf("Reading the file took %f seconds.\n\n", ((float)totalTime) / CLOCKS_PER_SEC);

	// Start the parsing
	startTime = clock();

	JWParser::Map jwMap;
	JWParser::Parse(buffer, jwMap);

	totalTime = clock() - startTime;
	printf("Parsing took %f seconds.\n\n", ((float)totalTime) / CLOCKS_PER_SEC);

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


