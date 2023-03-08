#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <stdlib.h>
#include <stdio.h>	
#include <string>

// Defines
typedef unsigned char u8;
typedef char c8;
typedef unsigned int u32;

#define LOOP(i, num) for(u32 i = 0; i < (num); ++i)

//					 
// Functions
//

__inline u32 StrLen(const c8* s) { return (s == NULL) ? 0 : (u32)strlen(s); }
__inline bool IsWhiteSpace(c8 c) {	return (c == ' ' || c == '\r' || c == '\n' || c == '\t'); }
__inline bool IsNewLine(c8 c) { return (c == '\r' || c == '\n'); }

// True if c == any of:		! " # $ % & ' ( ) * + , - . / : ; < = > ? @ [ \ ] ^ _ `
__inline bool IsPunctuation(c8 c) { return ((c >= 33 && c <= 47) || (c >= 58 && c <= 64) || (c >= 91 && c <= 96)); }

__inline bool IsDigit(c8 c) { return (c >= '0' && c <= '9'); }
__inline bool IsUpperCase(c8 c) {	return (c >= 'A' && c <= 'Z'); }
__inline bool IsLowerCase(c8 c) { return (c >= 'a' && c <= 'z'); }
__inline bool IsEOF(c8 c) { return (c == EOF); }

void MakeUpperCase(c8 *str);
void MakeLowerCase(c8 *str);
int CharaterCount(const c8* str, u32 strLen, c8 ch); // Returns number of times 'ch' appears in str
int FindFirstOf(const c8* str, u32 strLen, c8 ch); // Returns index of first 'ch' or -1

//
// Classes
//

class Buffer
{
public:

	Buffer() { mBuff = 0; mSize = 0; }
	~Buffer() { kill(); }

	bool init(u32 size);
	bool init(const u8* buf, u32 size);
	void kill();	

	void grab(u8* &buff) const { buff = mBuff; }
	void grab(char*& buff) const { buff = (char*)mBuff; }
	
	const u8* get() const { return mBuff; } 
	u32 getSize() const { return mSize; }

private:
	u8 *mBuff;
	u32 mSize;

	// No copies
	Buffer(const Buffer &buffer) : mBuff(0), mSize(0) {}
	Buffer& operator =(const Buffer &buffer) { return *this; }

	bool allocate(u32 size);
};

#endif
