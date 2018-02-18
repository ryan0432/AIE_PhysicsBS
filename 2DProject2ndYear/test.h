#pragma once
//how to use a namespace correctly

namespace MathHelp
{
	float Square(const float a)
	{
		return a*a;//do something
	}

	float Power(const float a, const int power)
	{
		float result = a;
		for (size_t i = 0; i < power; i++)
		{
			result *= a;
		}

		return result;
	}
};

