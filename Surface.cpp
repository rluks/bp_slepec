#include "Surface.h"

Surface::Surface(){
}

Surface::Surface(glm::vec3 c, float f, float a, ShaderSurface s) : color(c), frequency(f), amplitude(a), surfaceShader(s)
{

}