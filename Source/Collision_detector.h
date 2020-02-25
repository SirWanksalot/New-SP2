#pragma once
#include <Vector3.h>
#include <TRS.h>
#include "cornercoord.h"

bool collision_detector(Vector3 pos_obj1, Vector3 dimension_obj1, Vector3 pos_obj2, Vector3 dimension_obj2); //rectangle vs rectangle
bool collision_detector(Vector3 pos_obj1, Vector3 dimension_obj1, Vector3 pos_obj2, float radius); //rectangle vs circle
bool collision_detector(Vector3 pos_obj1, float radius1, Vector3 pos_obj2, float radius2); //circle vs circle

bool collision_detector(TRS obj1, cornercoord pos_obj1, TRS obj2, cornercoord pos_obj2);
bool collision_detector(TRS obj1, cornercoord dimensions_obj1, TRS obj2, float radius);