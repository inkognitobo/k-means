#include "raylib.h"

#include <stdlib.h>

#ifndef KMEANS_H
#define KMEANS_H

static const size_t MIN_CAPACITY = 8;

// Vector2List
typedef struct {
    size_t capacity;
    size_t size;
    Vector2 *items;
} Vector2List;

void vector2list_clear(Vector2List *list);
void vector2list_add(Vector2List *list, Vector2 v);
void vector2list_print(Vector2List *list);

// Vector2
float vector2_euclidian_distance(Vector2 v, Vector2 w);

// K-means clustering
void kmeans_assign(Vector2List *clusters, Vector2 *centroids, size_t n_clusters,
                    Vector2 *samples, size_t n_samples);
float kmeans_update(Vector2List *clusters, Vector2 *centroids, size_t n_clusters,
                     Vector2 *samples, size_t n_samples);
float kmeans_step(Vector2List *clusters, Vector2 *centroids, size_t n_clusters,
                   Vector2 *samples, size_t n_samples);
void kmeans_cluster(Vector2List *clusters, Vector2 *centroids, size_t n_clusters,
                     Vector2 *samples, size_t n_samples, float threshold);
void kmeans_n_cluster(Vector2List *clusters, Vector2 *centroids, size_t n_clusters,
                       Vector2 *samples, size_t n_samples, float treshold,
                       int n_iterations);
float kmeans_cost(Vector2List *clusters, Vector2 *centroids, size_t n_clusters);

// Utility
float randomf(float min, float max);

#endif // !KMEANS_H
