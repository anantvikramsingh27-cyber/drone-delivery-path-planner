
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    double weight;
    int priority;
    int selected;
    int delivered;
} Package;

// Function to calculate square root
double squareRoot(double value) {
    double x, y;

    if (value <= 0)
        return 0;

    x = value;
    y = 1.0;

    while ((x > y ? x-y : y-x) > 0.0000000001 * x) {
        x = (x + y) / 2.0;
        y = value / x;
    }

    return x;
}

// Function to calculate distance
double calculateDistance(double x1, double y1,
                         double x2, double y2) {
    double dx = x2 - x1;
    double dy = y2 - y1;

    return squareRoot(dx * dx + dy * dy);
}

// Function to select packages according to priority and weight
void selectPackages(Package *p, int *order, int n,
                    double capacity,
                    double *totalWeight,
                    int *selectedCount) {

    int i, j, temp;

    *totalWeight = 0;
    *selectedCount = 0;

    for (i = 0; i < n; i++)
        order[i] = i;

    // Higher priority packages are selected first
    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {

            int a = order[i];
            int b = order[j];

            if (p[b].priority > p[a].priority ||
               (p[b].priority == p[a].priority &&
                p[b].weight < p[a].weight)) {

                temp = order[i];
                order[i] = order[j];
                order[j] = temp;
            }
        }
    }

    for (i = 0; i < n; i++) {
        int index = order[i];

        if (p[index].weight <= capacity - *totalWeight + 0.000001) {

            p[index].selected = 1;
            *totalWeight += p[index].weight;
            (*selectedCount)++;
        }
    }
}

// Function to plan delivery route
void planRoute(Package *p, double (*coordinates)[2],
               int n, int *route,
               int *routeCount, double *totalDistance) {

    double currentX = 0;
    double currentY = 0;

    int i;

    *routeCount = 0;
    *totalDistance = 0;

    // Nearest neighbour route planning
    while (1) {

        int best = -1;
        double bestDistance = 1.0e100;

        for (i = 0; i < n; i++) {

            if (p[i].selected && !p[i].delivered) {

                double d = calculateDistance(
                    currentX, currentY,
                    coordinates[i][0],
                    coordinates[i][1]
                );

                if (best == -1 ||
                    d < bestDistance - 0.000001 ||
                    ((d >= bestDistance - 0.000001 &&
                      d <= bestDistance + 0.000001) &&
                     p[i].priority > p[best].priority)) {

                    best = i;
                    bestDistance = d;
                }
            }
        }

        if (best == -1)
            break;

        route[*routeCount] = best;
        (*routeCount)++;

        p[best].delivered = 1;

        *totalDistance += bestDistance;

        currentX = coordinates[best][0];
        currentY = coordinates[best][1];
    }

    // Return drone to depot
    if (*routeCount > 0) {
        *totalDistance += calculateDistance(
            currentX, currentY, 0, 0
        );
    }
}

// Function to generate delivery report
void printReport(Package *p, double (*coordinates)[2],
                 int n, int *route, int routeCount,
                 int selectedCount, double totalWeight,
                 double totalDistance, double capacity,
                 double speed, double fuelRate) {

    int i;

    double timeHours;
    double fuelUsed;

    printf("\n========== DRONE DELIVERY REPORT ==========\n");

    printf("Packages entered : %d\n", n);
    printf("Packages selected: %d\n", selectedCount);
    printf("Packages skipped : %d\n", n - selectedCount);

    if (routeCount == 0) {
        printf("No package could be loaded within weight limit.\n");
        return;
    }

    printf("\nDelivery Sequence:\n");

    printf("DEPOT (0,0)");

    for (i = 0; i < routeCount; i++) {

        int index = route[i];

        printf(" -> P%d (%.2f, %.2f)",
               p[index].id,
               coordinates[index][0],
               coordinates[index][1]);
    }

    printf(" -> DEPOT (0,0)\n");

    printf("\nDelivered Packages:\n");

    for (i = 0; i < routeCount; i++) {

        int index = route[i];

        printf("Package %d | Weight: %.2f kg | Priority: %d\n",
               p[index].id,
               p[index].weight,
               p[index].priority);
    }

    printf("\nSkipped Packages:\n");

    for (i = 0; i < n; i++) {

        if (!p[i].selected) {

            printf("Package %d | Weight: %.2f kg | Priority: %d\n",
                   p[i].id,
                   p[i].weight,
                   p[i].priority);
        }
    }

    timeHours = totalDistance / speed;
    fuelUsed = totalDistance * fuelRate;

    printf("\nTotal Payload: %.2f kg\n", totalWeight);
    printf("Maximum Capacity: %.2f kg\n", capacity);

    printf("Total Flight Distance: %.2f km\n", totalDistance);

    printf("Estimated Flight Time: %.2f hours\n", timeHours);

    printf("Flight Time in Minutes: %.2f minutes\n",
           timeHours * 60);

    printf("Estimated Fuel Usage: %.2f ml\n", fuelUsed);

    printf("============================================\n");
}

int main() {

    int n, i;

    double capacity, speed, fuelRate;

    double totalWeight = 0;
    double totalDistance = 0;

    int selectedCount = 0;
    int routeCount = 0;

    Package *packages;

    // Dynamically allocated 2D coordinate array
    double (*coordinates)[2];

    int *order;
    int *route;

    printf("===== DRONE DELIVERY PATH PLANNER =====\n");

    printf("Depot location: (0,0)\n");
    printf("Coordinates in km and weight in kg.\n");

    printf("\nEnter number of packages (1-100): ");

    if (scanf("%d", &n) != 1 || n < 1 || n > 100) {
        printf("Invalid number of packages.\n");
        return 1;
    }

    // Dynamic memory allocation
    packages = (Package *)calloc(n, sizeof(Package));

    coordinates = (double (*)[2])
                  malloc(n * sizeof(*coordinates));

    order = (int *)malloc(n * sizeof(int));

    route = (int *)malloc(n * sizeof(int));

    if (packages == NULL || coordinates == NULL ||
        order == NULL || route == NULL) {

        printf("Memory allocation failed.\n");

        free(packages);
        free(coordinates);
        free(order);
        free(route);

        return 1;
    }

    printf("Enter maximum drone capacity (kg): ");

    if (scanf("%lf", &capacity) != 1 || capacity <= 0) {
        printf("Invalid capacity.\n");
        free(packages);
        free(coordinates);
        free(order);
        free(route);
        return 1;
    }

    printf("Enter drone speed (km/hour): ");

    if (scanf("%lf", &speed) != 1 || speed <= 0) {
        printf("Invalid speed.\n");
        free(packages);
        free(coordinates);
        free(order);
        free(route);
        return 1;
    }

    printf("Enter fuel consumption (ml/km): ");

    if (scanf("%lf", &fuelRate) != 1 || fuelRate < 0) {
        printf("Invalid fuel rate.\n");
        free(packages);
        free(coordinates);
        free(order);
        free(route);
        return 1;
    }

    // Input package information
    for (i = 0; i < n; i++) {

        packages[i].id = i + 1;

        printf("\n--- Package %d ---\n", i + 1);

        printf("Enter X coordinate: ");
        if (scanf("%lf", &coordinates[i][0]) != 1) {
            printf("Invalid coordinate.\n");
            free(packages);
            free(coordinates);
            free(order);
            free(route);
            return 1;
        }

        printf("Enter Y coordinate: ");
        if (scanf("%lf", &coordinates[i][1]) != 1) {
            printf("Invalid coordinate.\n");
            free(packages);
            free(coordinates);
            free(order);
            free(route);
            return 1;
        }

        printf("Enter package weight (kg): ");
        if (scanf("%lf", &packages[i].weight) != 1 ||
            packages[i].weight <= 0) {

            printf("Invalid weight.\n");
            free(packages);
            free(coordinates);
            free(order);
            free(route);
            return 1;
        }

        printf("Enter priority (1=Low, 5=High): ");

        if (scanf("%d", &packages[i].priority) != 1 ||
            packages[i].priority < 1 ||
            packages[i].priority > 5) {

            printf("Invalid priority.\n");
            free(packages);
            free(coordinates);
            free(order);
            free(route);
            return 1;
        }
    }

    // Package selection
    selectPackages(packages, order, n, capacity,
                   &totalWeight, &selectedCount);

    // Route planning
    planRoute(packages, coordinates, n,
              route, &routeCount, &totalDistance);

    // Generate report
    printReport(packages, coordinates, n,
                route, routeCount, selectedCount,
                totalWeight, totalDistance,
                capacity, speed, fuelRate);

    // Free dynamically allocated memory
    free(packages);
    free(coordinates);
    free(order);
    free(route);

    return 0;
}
