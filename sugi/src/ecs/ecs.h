#ifndef ECS_H
#define ECS_H

#define ECS_INIT_ENT 			1024
#define ECS_INIT_SYS 			128
#define ECS_MAX_COMP_PER_ENT 	32

#include <stdarg.h>
#include "hash/hash.h"
#include "array/array.h"

typedef uint64_t entity_id_t;

typedef struct {
    entity_id_t counter;
    array_t entity_ids;
    array_t entity_components;
    array_t system_functions;
    array_t system_components;
} ecs_world_t;

ecs_world_t ecs_init_world();
entity_id_t ecs_create_entity(ecs_world_t* world);
void  ecs_register_system(ecs_world_t* world, void (*fn)(ecs_world_t*, entity_id_t), int n, ...);
void  ecs_add_component(ecs_world_t* world, entity_id_t entity_id, char* name, void* component);
void* ecs_get_component(ecs_world_t* world, entity_id_t entity_id, char* name);
void  ecs_set_component(ecs_world_t* world, entity_id_t entity_id, char* name, void* component);
void  ecs_update(ecs_world_t* world);
void  ecs_free(ecs_world_t* world);

#endif // ECS_H
