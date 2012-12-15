#include <LTBL/Utils.h>

#include <sstream>
#include <cstdlib>

namespace ltbl
{
	float GetFloatVal(std::string strConvert)
	{
		return static_cast<float>(atof(strConvert.c_str()));
	}
}
