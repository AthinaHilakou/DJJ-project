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
        // mymap->array[i]->count = 1; //count how many times key has been added
    }
    return mymap;
}

//hash: hash value for string s
unsigned int hash(int key, int capacity){
    return key % capacity;
}

int get_map_size(Map map){
    return map->size;
}

void mapify(Map map, int* array, float* weights, int size){
    for(int i = 0; i < size; i++){
        map_add(map, array[i], weights[i]);
    }
}

//if key_remove is in map and key_add is not in map, update map
//if key_add is in map with different weight, map is NOT updated
bool map_update(Map map, int key_add, float weight, int key_remove){
    if(map_get(map, key_remove) != -1){     // if key_remove is in map
        if(map_get(map, key_add) == -1){    // if key_add is not in map
            if(map_remove(map, key_remove) == false){
                printf("Error in map_update\n");
                exit(1);
            }    // remove key_remove
            map_add(map, key_add, weight);  // add key_add
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
    // printf("Key: %d\n", mynode->key);
    // printf("Key to add: %d\n", key);
    if(mynode->key == key){
        return 0;
    }

    Map_node node = malloc(sizeof(map_node));
    node->key = key;
    node->weight = weight;
    node->next = NULL;
    // node->count = 1;
    // put node at end of list
    mynode->next = node;
    map->size++;  
    map_rehash(map);
    return 1;
}

void map_rehash(Map map){
    if(map->size > map->capacity/2){
        Map_node *old_array = map->array;   // save old array for free later
        int old_capacity = map->capacity;   // save old capacity for rehash loop later
        map->capacity = map->capacity*2;
        Map_node *new_array = malloc(map->capacity*sizeof(Map_node));
        for(int i = 0; i < map->capacity; i++){
            new_array[i] = malloc(sizeof(map_node));
            new_array[i]->next = NULL;
            new_array[i]->key = -1;
            new_array[i]->weight = -1;
            // new_array[i]->count = 1; //count how many times key has been added
        }
        // rehash
        for(int i = 0; i < old_capacity; i++){
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
                // mynode->next->count = 1;
            }
        }
        // free old array
        for(int i = 0; i < old_capacity; i++){
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
            printf("Key: %d\n", node->key);
            free(node);
            map->size--;
            if(prev == NULL){
                map->array[hash(key, map->capacity)] = next;
                return 1;
            } else{
                printf("Prev: %d\n", prev->key);
                prev->next = next;
                return 1;
            }
        }
        prev = node;
    }
    return 0;
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
        for(mynode = map->array[j]->next; mynode != NULL; mynode = mynode->next){
            array[i] = mynode->key;
            i++;
        }
    }
    return array;
}

void map_print(Map map){
    int *array;
    int size;
    array = map_to_array(map, &size);
    for(int i = 0; i < size; i++){
        printf("%d ", array[i]);
    }
    printf("\n");
    free(array);
}