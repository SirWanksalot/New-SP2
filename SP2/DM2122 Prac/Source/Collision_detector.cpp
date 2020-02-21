#include "Collision_detector.h"
//rectangle vs rectangle
bool collision_detector(Vector3 pos_obj1, Vector3 dimension_obj1, Vector3 pos_obj2, Vector3 dimension_obj2)
{
	return pos_obj1.x + dimension_obj1.x / 2 >= pos_obj1.x - dimension_obj2.x / 2
		&& pos_obj1.x - dimension_obj1.x / 2 <= pos_obj2.x + dimension_obj2.x
		&& pos_obj1.z + dimension_obj1.z / 2 >= pos_obj1.z - dimension_obj2.z / 2
		&& pos_obj1.z - dimension_obj1.z / 2 <= pos_obj2.z + dimension_obj2.z;
}
//rectangle vs circle
bool collision_detector(Vector3 pos_obj1, Vector3 dimension_obj1, Vector3 pos_obj2, float radius)
{
	float x = Math::Max(pos_obj1.x - dimension_obj1.x / 2, Math::Min(pos_obj2.x, pos_obj1.x - dimension_obj1.x));
	float z = Math::Max(pos_obj1.z - dimension_obj1.z / 2, Math::Min(pos_obj2.z, pos_obj1.z - dimension_obj1.z));

	float shortestdist = sqrt((x - pos_obj2.x) * (x - pos_obj2.x) + (z - pos_obj2.z) * (z - pos_obj2.z));
	return shortestdist < radius;
}
//circle vs circle
bool collision_detector(Vector3 pos_obj1, float radius1, Vector3 pos_obj2, float radius2)
{
	float shortestdist = sqrt((pos_obj1.x - pos_obj2.x) * (pos_obj1.x - pos_obj2.x)
		+ (pos_obj1.y - pos_obj2.y) * (pos_obj1.y - pos_obj2.y)
		+ (pos_obj1.z - pos_obj2.z) * (pos_obj1.z - pos_obj2.z));
	return shortestdist < (radius1 + radius2);
}

bool collision_detector(TRS obj1, cornercoord dimensions_obj1, TRS obj2, cornercoord dimensions_obj2)
{
	return obj1.translate.x + dimensions_obj1.maxX * obj1.Scale.x >= obj2.translate.x + dimensions_obj2.minX * obj2.Scale.x 
		&& obj1.translate.x + dimensions_obj1.minX * obj1.Scale.x <= obj2.translate.x + dimensions_obj2.maxX * obj2.Scale.x
		&& obj1.translate.z + dimensions_obj1.maxZ * obj1.Scale.z >= obj2.translate.z + dimensions_obj2.minZ * obj2.Scale.z
		&& obj1.translate.z + dimensions_obj1.minZ * obj1.Scale.z <= obj2.translate.z + dimensions_obj2.maxZ * obj2.Scale.z;
}

bool collision_detector(TRS obj1, cornercoord dimensions_obj1, TRS obj2, float radius)
{

	return false;
}