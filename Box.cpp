#include "Box.h"

/**
 *	@brief pretizeni << operatoru
 */
std::ostream& operator<<(std::ostream&os, const Box& a)
{
  os << "box: vMin(" << a.vMin.x << ", " << a.vMin.y << 
		"), vMax(" << a.vMax.x << ", " << a.vMax.y <<
		"), surface(" << a.surface <<
		").";
  return os;
}