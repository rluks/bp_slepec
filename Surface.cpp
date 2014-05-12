#include "Surface.h"

Surface::Surface()
{
}

Surface::Surface(glm::vec3 c, float f, float a, ShaderSurface s, float temp, bool kill) 
:color(c), frequency(f), amplitude(a), surfaceShader(s), temperature(temp), kills(kill)
{
}

/**
 *	@brief pretizeni << operatoru
 */
std::ostream& operator<<(std::ostream&os, const Surface& a)
{
  os << "surface: color(" << a.color.r << ", " << a.color.g << ", " << a.color.b << 
		"), frequency(" << a.frequency <<
		"), amplitude(" << a.amplitude << 
		"), temperature(" << a.temperature <<
		"), kills(" << a.kills <<
		").";
  return os;
}