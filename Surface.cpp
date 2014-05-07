#include "Surface.h"

Surface::Surface()
{
}

Surface::Surface(glm::vec3 c, float f, float a, ShaderSurface s, float temp, bool kill) 
:color(c), frequency(f), amplitude(a), surfaceShader(s), temperature(temp), kills(kill)
{
}