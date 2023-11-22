#include "../headers/map.h"

//init: initialise map
Map map_init(int capacity){
    Map mymap = malloc(sizeof(map));
    mymap->capacity = capacity;
    mymap->size = 0;
    mymap->array = malloc(capacity*sizeof(Map_node));
    for(int i = 0; i < capacity; i++){
        mymap->array[i] = malloc(sizeof(map_node));
        mymap->array[i]->next = NULL;
        mymap->array[i]->key = -1;
        mymap->array[i]->weight = -1;
    }
    return mymap;
}

//hash: hash value for string s
unsigned int hash(int key, int capacity){
    return key % capacity;
}

void mapify(Map map, int* array, float* weights, int size){
    for(int i = 0; i < size; i++){
        map_add(map, array[i], weights[i]);
    }
}

//if key_remove is in map and key_add is not in map, update map
//if key_add is in map with different weight, map is NOT updated
bool map_update(Map map, int key_add, float weight, int key_remove){
    if(map_get(map, key_remove) != -1){
        if(map_get(map, key_add) == -1){
            map_remove(map, key_remove);
            map_add(map, key_add, weight);
            return 1;
        }
    }
    return 0;
}

//add: put (key, value) in map 
bool map_add(Map map, int key, float weight){
    //check if key is already in map
    unsigned hashval = hash(key, map->capacity);
    Map_node mynode;
    for(mynode = map->array[hashval]; mynode->next != NULL; mynode = mynode->next){
        if(mynode->key == key){
            return 0;
        }
    }
    Map_node node = malloc(sizeof(map_node));
    node->key = key;
    node->weight = weight;
    node->next = NULL;
    // put node at end of list
    mynode->next = node;
    map->size++;  
    map_rehash(map);
}

void map_rehash(Map map){
    if(map->size > map->capacity){
        Map_node *old_array = map->array;   // save old array for free later
        map->capacity = map->capacity*2;
        Map_node *new_array = malloc(map->capacity*sizeof(Map_node));
        for(int i = 0; i < map->capacity; i++){
            new_array[i] = malloc(sizeof(map_node));
            new_array[i]->next = NULL;
            new_array[i]->key = -1;
            new_array[i]->weight = -1;
        }
        for(int i = 0; i < map->capacity/2; i++){
            Map_node node;
            Map_node next = NULL;
            for (node = map->array[i]->next; node != NULL; node = next){
                next = node->next;
                unsigned hashval = hash(node->key, map->capacity);
                Map_node mynode = new_array[hashval];
                // put node at end of list
                for(mynode = new_array[hashval]; mynode->next != NULL; mynode = mynode->next);
                mynode->next = malloc(sizeof(map_node));
                mynode->next->key = node->key;
                mynode->next->weight = node->weight;
                mynode->next->next = NULL;
            }
        }
        for(int i = 0; i < map->capacity/2; i++){
            Map_node node;
            Map_node next = NULL;
            for (node = old_array[i]->next; node != NULL; node = next){
                next = node->next;
                free(node);
            }
        }
        free(old_array);
        map->array = new_array;

        for(int i = 0; i < map->capacity; i++){
            Map_node node;
            Map_node next = NULL;
            for (node = map->array[i]->next; node != NULL; node = next){
                next = node->next;
            }
        }
    }
}

//remove: remove node corresponding to key from map
bool map_remove(Map map, int key){
    map_node *node;
    map_node *prev = NULL;
    map_node *next = NULL;
    for (node = map->array[hash(key, map->capacity)]; node != NULL; node = next){
        next = node->next;
        if (key == node->key){
            //printf("Removed key: %s value: %s\n", node->key, node->value);
            free(node);
            map->size--;
            if(prev == NULL){
                map->array[hash(key, map->capacity)] = next;
                return 1;
            }
            else{
                prev->next = next;
                return 1;
            }
        }
        prev = node;
    }
}

//destroy: free map
void map_destroy(Map map){

    // free all nodes, not the ones in the array (dummy nodes)
    for(int i = 0; i < map->capacity; i++){
        map_node *node;
        map_node *next = NULL;
        for (node = map->array[i]; node != NULL; node = next){
            next = node->next;
            free(node);
        }
    }
    free(map->array);
    free(map);
}

float map_get(Map map, int key){ // returns -1 if not found
    unsigned hashval = hash(key, map->capacity);
    Map_node mynode;
    for(mynode = map->array[hashval]->next; mynode != NULL; mynode = mynode->next){
        if(mynode->key == key){
            return mynode->weight;
        }
    }
    // if not found
    return -1;
}

// Map to array
int *map_to_array(Map map, int* size){
    int *array = malloc(map->size*sizeof(int));
    *size = map->size;
    int i = 0;
    for(int j = 0; j < map->capacity; j++){
        Map_node mynode;
        for(mynode = map->array[j]->next; mynode->next != NULL; mynode = mynode->next){
            array[i] = mynode->key;
            i++;
        }
    }
    return array;
}