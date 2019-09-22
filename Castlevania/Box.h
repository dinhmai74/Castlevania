#pragma once

struct Box
{
	float l;
	float t;
	float r;
	float b;
	Box() { l = t = r = b = 0; }
	Box(float left, float top, float right, float bottom) {
		l = left;
		t = top;
		r = right;
		b = bottom;
	}
	Box& operator =(const Box& a)
	{
		l = a.l;
		t = a.t;
		r = a.r;
		b = a.b;
		return *this;
	}
};