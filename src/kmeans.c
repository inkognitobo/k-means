#include "kmeans.h"

#include <assert.h>
#include <float.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Empty a list of vectors, deallocating the memory in used.
void vector2list_clear(Vector2List *list) {
    free(list->items);
    list->items = NULL;
    list->capacity = 0;
    list->size = 0;
}

// Add a vector to the vector list.
// If the capacity is reached, the items are re-allocated to twice the size.
void vector2list_add(Vector2List *list, Vector2 v) {
    if (!(list->size < list->capacity)) {
        size_t capacity = (list->capacity * 2) > MIN_CAPACITY ? (list->capacity * 2)
                                                              : MIN_CAPACITY;
        list->items = realloc(list->items, capacity * sizeof(Vector2));
        list->capacity = capacity;
    }

    if (list->items == NULL) {
        fprintf(stderr, "Failed to grow dynamic array\n");
    }

    list->items[list->size++] = v;
}

// Debug a vector list to stdout.
void vector2list_print(Vector2List *list) {
    printf("[ ");
    for (int i = 0; i < list->size; i++) {
        printf("(%f, %f) ", list->items[i].x, list->items[i].y);
    }
    printf("]\n");
}

// Compute the euclidian distance (L2-norm) between two vectors v and w.
float vector2_euclidian_distance(Vector2 v, Vector2 w) {
    return sqrtf(powf(v.x - w.x, 2) + powf(v.y - w.y, 2));
}

// Assign samples to the cluster with the closest centroid.
// -> Expectation step.
void kmeans_assign(Vector2List *clusters, Vector2 *centroids, size_t n_clusters,
                   Vector2 *samples, size_t n_samples) {
    // Clear clusters
    for (int i = 0; i < n_clusters; i++) {
        vector2list_clear(&clusters[i]);
    }

    // Assign samples to cluster with closest centroid
    for (int i = 0; i < n_samples; i++) {
        Vector2 sample = samples[i];
        int min_dist = INT_MAX;
        int winner = INT_MAX;
        for (int j = 0; j < n_clusters; j++) {
            int dist = vector2_euclidian_distance(sample, centroids[j]);
            if (dist < min_dist) {
                min_dist = dist;
                winner = j;
            }
        }
        assert(winner <= n_clusters && "failed to find closest centroid");
        vector2list_add(&clusters[winner], sample);
    }
}

// Recompute cluster centroids as the mean of all assigned samples.
// -> Maximization step.
// Returns a delta that is the euclidian distance between the old and new
// cluster centroid accumulated over all centroids.
float kmeans_update(Vector2List *clusters, Vector2 *centroids, size_t n_clusters,
                    Vector2 *samples, size_t n_samples) {
    float delta = 0;
    for (int i = 0; i < n_clusters; i++) {
        Vector2List cluster = clusters[i];
        // If a center has no assigned samples then re-initialize it at random
        if (cluster.size == 0) {
            int idx = (int)randomf(0, n_samples + 1);
            Vector2 centroid = {samples[idx].x, samples[idx].y};
            centroids[i] = centroid;
            continue;
        }

        Vector2 acc = {0};
        for (int j = 0; j < cluster.size; j++) {
            acc.x += cluster.items[j].x;
            acc.y += cluster.items[j].y;
        }

        Vector2 centroid = {acc.x / cluster.size, acc.y / cluster.size};
        delta += vector2_euclidian_distance(centroid, centroids[i]);
        centroids[i].x = centroid.x;
        centroids[i].y = centroid.y;
    }
    return delta;
}

// Perform a single step of k-means clustering.
// 1. Expectation step
// 2. Maximization step
float kmeans_step(Vector2List *clusters, Vector2 *centroids, size_t n_clusters,
                  Vector2 *samples, size_t n_samples) {
    kmeans_assign(clusters, centroids, n_clusters, samples, n_samples);
    float delta = kmeans_update(clusters, centroids, n_clusters, samples, n_samples);
    return delta;
}

// Perform k-means clustering until the change in the position of cluster centroids
// fails to exceed some threshold.
void kmeans_cluster(Vector2List *clusters, Vector2 *centroids, size_t n_clusters,
                    Vector2 *samples, size_t n_samples, float threshold) {
    float delta;
    int i = 0;
    do {
        delta = kmeans_step(clusters, centroids, n_clusters, samples, n_samples);
    } while (delta > threshold);
}

// Repeat k-means clustering a number of n times and choose the cluster configuration
// that leads to the smallest cost.
void kmeans_n_cluster(Vector2List *clusters, Vector2 *centroids, size_t n_clusters,
                      Vector2 *samples, size_t n_samples, float threshold,
                      int n_iterations) {
    // Initialisation
    float min_cost = FLT_MAX;
    Vector2 final_centroids[n_clusters];
    for (int i = 0; i < n_clusters; i++) {
        final_centroids[i] = (Vector2){0.0, 0.0};
    }

    // Repeat k-means clustering
    for (int i = 0; i < n_iterations; i++) {
        // Initialise centroid positions as random data point
        for (int j = 0; j < n_clusters; j++) {
            int idx = (int)randomf(0, n_samples - 1);
            Vector2 centroid = {samples[idx].x, samples[idx].y};
            centroids[j] = centroid;
        }
        kmeans_cluster(clusters, centroids, n_clusters, samples, n_samples,
                       threshold);
        float cost = kmeans_cost(clusters, centroids, n_clusters);
        if (cost < min_cost) {
            min_cost = cost;
            for (int j = 0; j < n_clusters; j++) {
                final_centroids[j].x = centroids[j].x;
                final_centroids[j].y = centroids[j].y;
            }
        }
    }

    // Recover optimal centroid positions and assign samples
    for (int i = 0; i < n_clusters; i++) {
        centroids[i].x = final_centroids[i].x;
        centroids[i].y = final_centroids[i].y;
    }
    kmeans_assign(clusters, centroids, n_clusters, samples, n_samples);
}

// Compute the cost as the mean distance to each cluster centroid averaged over
// all assigned samples.
float kmeans_cost(Vector2List *clusters, Vector2 *centroids, size_t n_clusters) {
    float acc_dist = 0.0;
    for (int i = 0; i < n_clusters; i++) {
        float acc_centroid_dist = 0.0;
        for (int j = 0; j < clusters[i].size; j++) {
            acc_centroid_dist +=
                vector2_euclidian_distance(clusters[i].items[j], centroids[i]);
        }
        acc_dist += acc_centroid_dist / clusters[i].size;
    }
    return acc_dist / n_clusters;
}

// Generate a random floating number between `min` and `max`.
// Note: don't forget to seed the random generator **once**:
// `srand((unsigned)time(NULL))`
float randomf(float min, float max) {
    return ((float)rand() / (float)(RAND_MAX)) * (max - min) + min;
}
