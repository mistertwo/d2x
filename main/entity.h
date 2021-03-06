#ifndef _ENTITY_H
#define _ENTITY_H


#include "object.h"
#include "robot.h"


typedef struct entity
{
	char   *name;
	ubyte  object_type;
	int    object_number;
} entity;

void entity_init(void);
void entity_add(ubyte object_type, int object_id, char object_name[16]);

#endif
