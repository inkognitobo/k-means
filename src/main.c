#include "config.h"
#include "draw.h"
#include "kmeans.h"
#include "raylib.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Configuration ---

// Configurations of sample generation
#define MAX_DEVIATION 15.0f
#define MAX_X         200
#define MIN_X         -200
#define X_RANGE       (MAX_X - MIN_X)
#define MAX_Y         100
#define MIN_Y         -100
#define Y_RANGE       (MAX_Y - MIN_Y)

// Configurations for cluster setup
#define N_CLUSTERS   4                           // Number of clusters
#define CLUSTER_SIZE 150                         // Number of samples per clusters
#define SAMPLE_SIZE  (N_CLUSTERS * CLUSTER_SIZE) // Total number of data points
#define THRESHOLD    ((X_RANGE + Y_RANGE) * 0.5 * 0.001) // For centroid delta

// Configurations for visualisation
#define N_COLORS 6 // Number of colors, is cycled if necessary
const Color COLORS[N_COLORS] = {BLUE, PURPLE, ORANGE, GREEN, PINK, VIOLET};

// Generate a random cluster of n data points.
void generate_cluster(Vector2 *cluster, int n_samples) {
    Vector2 origin = {randomf(MIN_X + MAX_DEVIATION, MAX_X - MAX_DEVIATION),
                      randomf(MIN_Y + MAX_DEVIATION, MAX_Y - MAX_DEVIATION)};
    for (int i = 0; i < CLUSTER_SIZE; i++) {
        float x_offset = randomf(-MAX_DEVIATION, MAX_DEVIATION);
        float y_offset = randomf(-MAX_DEVIATION, MAX_DEVIATION);
        Vector2 point = {origin.x + x_offset, origin.y + y_offset};
        cluster[i] = point;
    }
}

// Run a random instance of k-means clustering
void run_random_kmeans_instance(Vector2List *clusters, Vector2 *centroids,
                                size_t n_clusters, Vector2 *samples,
                                size_t n_samples) {
    // Generate samples
    for (int i = 0; i < N_CLUSTERS; i++) {
        generate_cluster(&samples[i * CLUSTER_SIZE], CLUSTER_SIZE);
    }

    // Initiate clusters
    for (int i = 0; i < N_CLUSTERS; i++) {
        Vector2List cluster = {0};
        clusters[i] = cluster;
    }

    // Perform k-means clustering
    int n_iterations = 50;
    float threshold = (((X_RANGE + Y_RANGE) / 2.0f) * 0.0001);
    kmeans_n_cluster(clusters, centroids, n_clusters, samples, n_samples, threshold,
                     n_iterations);
}

int main(void) {
    // Seed randomness
    srand((unsigned)time(NULL));

    // Allocate array of samples, centroids and clusters around said centroids
    Vector2 samples[SAMPLE_SIZE] = {0};
    Vector2 centroids[N_CLUSTERS] = {0};
    Vector2List clusters[N_CLUSTERS] = {0};
    bool initialized = false;

    // Render
    Camera2D camera = {
        .target = {0.0,          0.0         },
        .offset = {WIDTH / 2.0f, HEIGHT / 2.0},
        .rotation = 0.0,
        .zoom = 2.0,
    };
    InitWindow(WIDTH, HEIGHT, "k-means");
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_SPACE)) {
            run_random_kmeans_instance(clusters, centroids, N_CLUSTERS, samples,
                                       SAMPLE_SIZE);
            initialized = true;
        }

        BeginDrawing();
        BeginMode2D(camera);

        ClearBackground(WHITE);

        draw_axes();

        if (initialized) {
            scatter(samples, SAMPLE_SIZE, RED);
            for (int i = 0; i < N_CLUSTERS; i++) {
                Vector2 centroid = centroids[i];
                DrawRectangle(centroid.x, centroid.y, 5.0f, 5.0f,
                              COLORS[i % N_COLORS]);
            }
        }

        EndMode2D();
        EndDrawing();
    }
    CloseWindow();

    // Free dynamic lists
    for (int i = 0; i < N_CLUSTERS; i++) {
        vector2list_clear(&clusters[i]);
    }

    return 0;
}
