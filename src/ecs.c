#include "ecs.h"


ecs_world_t ecs_init_world()
{
    ecs_world_t world;
    world.counter = 0;
    world.entity_ids = array_create(ECS_INIT_ENT);
    world.entity_components = array_create(ECS_INIT_ENT);
    world.system_functions = array_create(ECS_INIT_SYS);
    world.system_components = array_create(ECS_INIT_SYS);
    return world;
}


void ecs_free(ecs_world_t* world)
{
    for (int i = 0; i < array_len(&world->entity_ids); i++)
    {
        hash_table_t* ht = world->entity_components.array[i];
        ht_free(ht);
    }
}


void ecs_register_system(ecs_world_t* world, void (*fn)(ecs_world_t*, entity_id_t), int n, ...)
{
    va_list args;
    array_t* a = array_create_ref(10);

    va_start(args, n);
    for (int i = 0; i < n; i++)
    {
        char *str = va_arg(args, char *);
        array_append(a, str);
    }
    va_end(args);

    array_append(&world->system_functions, fn);
    array_append(&world->system_components, a);
}


entity_id_t ecs_create_entity(ecs_world_t* world)
{
    // Update counter
    world->counter++;

    // Add entity id to id array
    array_append(&world->entity_ids, (void *)world->counter);

    // Add entity components hash table to components array
    hash_table_t* components = ht_create_ref(ECS_MAX_COMP_PER_ENT);
    array_append(&world->entity_components, components);

    return world->counter;
}


void ecs_add_component(ecs_world_t* world, entity_id_t entity_id, char* name, void* component)
{
    int ht_index = array_find(&world->entity_ids, (void *)entity_id);
    hash_table_t* ht = array_get(&world->entity_components, ht_index);
    if (!ht_has_key(ht, name))
        ht_insert(ht, name, component);
}


void* ecs_get_component(ecs_world_t* world, entity_id_t entity_id, char* name)
{
    int ht_index = array_find(&world->entity_ids, (void *)entity_id);
    hash_table_t* ht = array_get(&world->entity_components, ht_index);
    return ht_read(ht, name);
}


void ecs_set_component(ecs_world_t* world, entity_id_t entity_id, char* name, void* component)
{
    int ht_index = array_find(&world->entity_ids, (void *)entity_id);
    hash_table_t* ht = array_get(&world->entity_components, ht_index);
    ht_write(ht, name, component);
}


void ecs_update(ecs_world_t* world)
{
    // Loop over every system
    for (int sys_i = 0; sys_i < array_len(&world->system_functions); sys_i++)
    {
        // Get the system function
        void (*fn)(ecs_world_t*, entity_id_t) = array_get(&world->system_functions, sys_i);
        // Get components needed for this function
        array_t* components = array_get(&world->system_components, sys_i);

        // Loop over every entity
        for (int ent_i = 0; ent_i < array_len(&world->entity_components); ent_i++)
        {
            // Get entity id and components hash table
            entity_id_t eid = (entity_id_t) array_get(&world->entity_ids, ent_i);
            hash_table_t* ent_components = array_get(&world->entity_components, ent_i);

            // This flag represents if entity is suitable for this system
            int suitable = 1;

            // Check if each of the required components are in the entity
            for (int comp_i = 0; comp_i < array_len(&world->entity_components); comp_i++)
            {
                if (!ht_has_key(ent_components, (char *)array_get(components, comp_i)))
                {
                    suitable = 0;
                    break;
                }
            }

            // If entity is suitable for the system, run system function on it
            if (suitable)
                fn(world, eid);
        }
    }
}
