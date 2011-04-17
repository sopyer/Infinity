#pragma once

#include <vi.h>

namespace ml
{
	void transpose(vec4* transposed/*[4]*/, vec4* mat/*[4]*/);

	void mul(vec4* r, vec4* m/*[4]*/, vec4 v);
	void mul(vec4* r/*[4]*/, vec4* a/*[4]*/, vec4* b/*[4]*/);
}