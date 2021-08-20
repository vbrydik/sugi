#include <stdio.h>
#include "sugi/core.h"
#include "hash.h"
#include "array.h"
#include "ecs.h"



void init_fn(void)
{
    printf("init\n");
}

void update_fn(void)
{}

void draw_fn(void)
{}


struct Test;
typedef struct Test{
    int x;
    int y;
} Test;


// Components
struct Position;
typedef struct Position {
    int x;
    int y;
} Position;

struct Velocity;
typedef struct Velocity {
    int dx;
    int dy;
} Velocity;


void movement_system(ecs_world_t* world, entity_id_t entity_id)
{
    // get references to those components
    Position* pos = ecs_get_component(world, entity_id, "Position");
    Velocity* vel = ecs_get_component(world, entity_id, "Velocity");

    // do some operations on those components
    pos->x += vel->dx;
    pos->y += vel->dy;
}


void construct_entity(ecs_world_t* world, entity_id_t id, int x, int y)
{
    Position* pos = malloc(sizeof (Position));
    pos->x = x;
    pos->y = y;

    Velocity* vel = malloc(sizeof (Velocity));
    vel->dx = 1;
    vel->dy = 2;

    ecs_add_component(world, id, "Position", pos);
    ecs_add_component(world, id, "Velocity", vel);
}

void print_entities(ecs_world_t* world)
{
    Position* pos = ecs_get_component(&world, 1, "Position");
    printf("entity.1 x: %d\n", pos->x);
    printf("entity.1 y: %d\n", pos->y);

    pos = ecs_get_component(&world, 2, "Position");
    printf("entity.2 x: %d\n", pos->x);
    printf("entity.2 y: %d\n", pos->y);
}


int main(void)
{
    ecs_world_t world = ecs_init_world();

    //Create entity
    {
        // Create entity and get its id
        int entity_id = ecs_create_entity(&world);
        int entity_id_2 = ecs_create_entity(&world);
        construct_entity(&world, entity_id, 10, 20);
        construct_entity(&world, entity_id_2, 30, 40);
    }

    // Register a system
    {
        // Here we register a system (just a function)
        // with a list required components for it.
        ecs_register_system(&world, movement_system, 2, "Position", "Velocity");
    }

    {
        Position* pos = ecs_get_component(&world, 1, "Position");
        printf("entity.1 x: %d\n", pos->x);
        printf("entity.1 y: %d\n", pos->y);

        pos = ecs_get_component(&world, 2, "Position");
        printf("entity.2 x: %d\n", pos->x);
        printf("entity.2 y: %d\n", pos->y);
    }

    ecs_update(&world);

    {
        Position* pos = ecs_get_component(&world, 1, "Position");
        printf("entity.1 x: %d\n", pos->x);
        printf("entity.1 y: %d\n", pos->y);

        pos = ecs_get_component(&world, 2, "Position");
        printf("entity.2 x: %d\n", pos->x);
        printf("entity.2 y: %d\n", pos->y);
    }

    {
        Position* pos = malloc(sizeof (Position));
        pos->x = 1337;
        pos->y = 420;
        ecs_set_component(&world, 2, "Position", pos);
    }

    {
        Position* pos = ecs_get_component(&world, 1, "Position");
        printf("entity.1 x: %d\n", pos->x);
        printf("entity.1 y: %d\n", pos->y);

        pos = ecs_get_component(&world, 2, "Position");
        printf("entity.2 x: %d\n", pos->x);
        printf("entity.2 y: %d\n", pos->y);
    }

    sugi_core_set_init(init_fn);
    sugi_core_set_update(update_fn);
    sugi_core_set_draw(draw_fn);

    sugi_core_run();

    // sugi_core_set_init(sugi_core_null_fn);    // call_lua_init);
    // sugi_core_set_update(sugi_core_null_fn);  // call_lua_tick);
    // sugi_core_set_draw(sugi_core_null_fn);
    // // initialize terminal
    // sugi_term_init();
    // // start engine loop
    // sugi_core_run();
    // // close lua interpreter
    // lua_close(L);

    printf(".\n");
    ecs_free(&world);

    return 0;
}
