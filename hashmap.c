#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {

  Pair* new = createPair(key,value);
  new->key = key;
  new->value = value;

  int pos = hash(key,map->capacity);

  while(map->buckets[pos] != NULL){
    if(pos+1 == map->capacity){
      pos=0;
    }else  pos++;
  }

  map->buckets[pos] = new;
  map->size++;
  map->current = pos;
}

void enlarge(HashMap * map) {
  enlarge_called = 1; //no borrar (testing purposes)
  
  Pair** aux = map->buckets;
  int capacidad = map->capacity;
  map->capacity *=2;
  
  Pair** maux = (Pair**) calloc(map->capacity, sizeof(Pair*));
  map->buckets = maux;
  
  map->size=0;
  
  for(int i = 0 ; i < capacidad ; i++){
    if(aux[i] != NULL && aux[i]->key != NULL){
      insertMap(map, aux[i]->key, aux[i]->value);
    }
  }
  
}

HashMap * createMap(long capacity) {

  HashMap * map = (HashMap*) malloc(sizeof(HashMap));
  map->buckets = (Pair**) calloc(capacity,sizeof(Pair*));
  map->capacity = capacity;
  map->size = 0;
  map->current = -1;
  return map;
}

void eraseMap(HashMap * map,  char * key){    
  
  int pos = hash(key, map->capacity);

  while(map->buckets[pos] != NULL){
    map->current = pos;
    if(is_equal(map->buckets[pos]->key,key) != 0){
      map->buckets[pos]->key = NULL;
      break;
    }
    pos++;
  }
  map->size--;
}

Pair * searchMap(HashMap * map,  char * key) {   
  int pos = hash(key, map->capacity);
  while(map->buckets[pos] != NULL){
    map->current = pos;
    if(is_equal(map->buckets[pos]->key,key) != 0){
      return map->buckets[pos];
    }
    pos++;
  }
  return NULL;
}

Pair * firstMap(HashMap * map) {
  int pos = 0;

  while(map->buckets[pos] == NULL || map->buckets[pos]->key == NULL){
    pos++;
  }
  map->current = pos;
  return map->buckets[pos];
}

Pair * nextMap(HashMap * map) {
  int pos = map->current+1;
  
  while(map->buckets[pos] == NULL || map->buckets[pos]->key == NULL){

    if(pos == map->capacity){
      return NULL;
    }
    pos++;
  }
  
  map->current = pos;
  return map->buckets[pos]; 
}
