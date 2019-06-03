#ifndef __BBOX_H__
#define __BBOX_H__

#include <limits>
#include <string> 

/** 
* @class BBox
* @brief The class that describe BBoux of object
*/
class Bbox {
				
	public:

		Bbox();
		std::string toString();
		void update (float x, float y, float z);
		float *getMin(){return new float[3]{xMin, yMin, zMin};}
		float *getMax(){return new float[3]{xMax, yMax, zMax};}
		float *getCenter();
		float xMin, yMin, zMin;
		float xMax, yMax, zMax;
		
};
#endif
