/**
 * @file x_murmur32.cpp
 *
 * xCore Hash functions
 */

// x_murmur32.cpp - xCore Hash functions 
#include "xbase/x_target.h"
#include "xhash/x_murmur32.h"
#include "xhash/private/x_digest_murmur32.h"

namespace xcore
{
	void		xdigest_engine_murmur32::update(xcbuffer const& buffer)
	{
		mHash = xdigest_murmur32::buf(buffer, mHash);
	}
}
