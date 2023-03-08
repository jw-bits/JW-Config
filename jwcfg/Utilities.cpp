#include "Utilities.h"

void MakeUpperCase(c8 *str)
{
	if(str)
	{
		u32 l = StrLen(str);
		c8 *s = str;

		LOOP(i, l)
		{
			if(__isascii(*s))
				*s = toupper(*s);

			++s;
		}
	}
}

void MakeLowerCase(c8 *str)
{
	if(str)
	{
		u32 l = StrLen(str);
		c8 *s = str;

		LOOP(i, l)
		{
			if(__isascii(*s)) //doesnt exist in android linux
				*s = tolower(*s);

			++s;
		}
	}
}

int CharaterCount(const c8* str, u32 strLen, c8 ch)
{
	if (str == NULL || strLen == 0)
		return 0;

	int count = 0;

	LOOP(i,strLen)
	{
		if(str[i] == ch)
			++count;
	}

	return count;
}

int FindFirstOf(const c8* str, u32 strLen, c8 ch)
{
	if (str == NULL || strLen == 0)
		return -1;

	int idx = 0;

	for(; idx < (int)strLen; ++idx)
	{
		if (str[idx] == ch)
			return (int)idx;
	}

	return -1;
}



// *************** //
// *** Buffer *** //
// ************* //

bool Buffer::init(u32 size)
{
	if (size == 0)
		return false;

	return allocate(size);
}

bool Buffer::init(const u8* buf, u32 size)
{
	if ((size == 0) || (buf == NULL))
		return false;

	if (allocate(size))
	{
		memcpy(mBuff, buf, mSize);
			return true;
	}

	return false;
}

void Buffer::kill()
{
	if (mBuff) 
	{	
		delete[] mBuff; 
		mBuff = NULL; 
	}

	mSize = 0;
}

// Lazy allocate.  Only do so if more room needed
bool Buffer::allocate(u32 size)
{
	if (size > mSize) // Need more space, destroy and rebuild
	{
		kill();

		mBuff = new u8[size];

		if (mBuff == NULL)
			return false;
	}
	
	mSize = size;
	return true;
}




