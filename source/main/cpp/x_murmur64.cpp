/**
 * @file x_murmur64.cpp
 *
 * xCore Hash functions
 */

// x_murmur64.cpp - xCore Hash functions 
#include "xbase\x_types.h"
#include "xbase\x_endian.h"
#include "xbase\x_string.h"
#include "xbase\x_string_std.h"
#include "xhash\x_murmur64.h"
#include "xhash\private\x_digest_murmur64.h"

namespace xcore
{
	void		xdigest_engine_murmur64::update(void const* inBuffer, s32 inLength)
	{
		mHash = xdigest_murmur64::buf(inBuffer, inLength, mHash);
	}
}