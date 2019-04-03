#ifndef __BBOX_H__
#define __BBOX_H__

#include <limits>
#include <string> 

/** 
* @class BBox
* @brief The class that describe BBoux of object
*/
class Bbox {

	private:
		float xMin, yMin, zMin;
		float xMax, yMax, zMax;
				
	public:

		Bbox();
		std::string toString();
		void update (float x, float y, float z);

		
};
#endif
