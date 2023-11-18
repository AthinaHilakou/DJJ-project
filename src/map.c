#include "../headers/map.h"

//init: initialise map
Map map_init(int capacity){
    Map mymap = malloc(sizeof(map));
    mymap->capacity = capacity;
    mymap->size = 0;
    mymap->array = malloc(capacity*sizeof(map_node));
    for(int i = 0; i < capacity; i++){
        mymap->array[i]->next = NULL;
        mymap->array[i]->key = -1;
    }
}

//hash: hash value for string s
unsigned int hash(int key, int capacity){
    return key % capacity;
}

void mapify(Map map, int* array, int size){
    for(int i = 0; i < size; i++){
        map_add(map, array[i]);
    }
}

//find: look for node corresponding to s in map
bool map_update(Map map, int key_add, int key_remove){
    if(map_remove(map, key_remove)){
        if(map_add(map, key_add))
            return 1;
        else
            return 0;
    } else
        return 0;
}

//add: put (key, value) in map 
bool map_add(Map map, int key){
    Map_node node = malloc(sizeof(map_node));
    node->key = key;
    node->next = NULL;
    unsigned hashval = hash(key, map->capacity);
    Map_node mynode;
    for(mynode = map->array[hashval]; mynode->next != NULL; mynode = mynode->next){
        if(mynode->key == key){
            free(node);
            return 0;
        }
    }
    mynode->next = node;
    map->size++;  
    
}
// {
//     struct map_node *node;
//     unsigned hashval;
//     if ((node = find(key, map)) == NULL) { //not found 
//         node = (struct map_node *) malloc(sizeof(*node));
//         if (node == NULL || ((node->key = strdup(key)) == NULL)){
//           fprintf(stderr, "Could not allocate memory");
//           exit(EXIT_FAILURE);
//         }
//         hashval = hash(key);
//         node->next = map[hashval];
//         map[hashval] = node;
//     } else //already there
//         free(node->value); //free previous value
//     if ((node->value= strdup(value)) == NULL){
//         fprintf(stderr, "Could not allocate memory");
//         exit(EXIT_FAILURE); 
//     }
//     return node;
// }


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
            if(prev == NULL){
                map->array[hash(key, map->capacity)] = next;
                return;
            }
            else{
                prev->next = next;
                return;
            }
        }
        prev = node;
    }
    printf("%d is not an index\n", key);
}

//destroy: free map
void map_destroy(Map map){

    for(int i = 0; i < map.capacity; i++){
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

int *map_get(Map map){
    int *array = malloc(map->size*sizeof(int));
    int i = 0;
    for(int j = 0; j < map->capacity; j++){
        map_node *node;
        for (node = map->array[j]; node != NULL; node = node->next){
            array[i] = node->key;
            i++;
        }
    }
}
