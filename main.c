#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MaxBuildingsNum 50
#define MaxCoursesNum 50

#define INFINITY 100000

typedef struct QueueRecord *Queue;

struct QueueRecord
{

    int Capacity;
    int Front;
    int Rear;
    int Size;
    struct Course *Array;
};

typedef struct BuildingGraphRecord *BuildingGraph;

struct Building
{

    char Name[50];
    int Weight;
    struct Building *Next;
};

struct BuildingGraphRecord
{

    struct Building *Buildings[MaxBuildingsNum];
    int Pointer; // Points to the first empty index in the array
};

typedef struct CoursesGraphRecord *CoursesGraph;

struct CoursesGraphRecord
{

    struct Course *Courses[MaxCoursesNum];
    int Pointer; // Points to the first empty index in the array
};

struct Course
{

    char Name[50];
    struct Course *Next;
};

int IsBuildingEmpty(struct Building *);
int IsCourseEmpty(struct Course *);

BuildingGraph CreateBuildingGraph();
CoursesGraph CreateCoursesGraph();

void InsertLastToBuildingsArray(char *, BuildingGraph);
void InsertLastToCoursesArray(char *, CoursesGraph);

void InsertBuildingLast(char *, int, struct Building *);
void InsertCourseLast(char *, struct Course *);

void PrintBuildingList(struct Building *);
void PrintCourseList(struct Course *);

int getBuildingIndex(char *, BuildingGraph);

void Dijkstra(BuildingGraph, int, int, int);

int isEmpty(Queue);
int isFull(Queue);
Queue CreateQueue(int);
void MakeEmpty(Queue);
int Succ(int, Queue);
void Enqueue(struct Course, Queue);
void Dequeue(Queue);
struct Course FrontAndDequeue(Queue);

int getInDegree(struct Course *);
void removeCourse(char *, struct Course *);
struct Course *InsertCourseNodeLast(char *, struct Course *);
struct Course *TopologicalSort(CoursesGraph);

int main()
{
    BuildingGraph buildingsGraph = CreateBuildingGraph();
    CoursesGraph courseGraph = CreateCoursesGraph();
    struct Course *sortedCoursesList;

    int IsLoaded = 0;
    int IsSorted = 0;

    FILE *input;

    char line[100];

    char BuildingName1[50];
    char BuildingName2[50];
    int Weight;

    char CourseName[50];

    int choice;

    do
    {
        printf("Please Select one of the options you want:\n");
        printf("1. Load the two files\n");
        printf("2. Calculate the shortest distance between two buildings\n");
        printf("3. Print the shortest route between two buildings and the total distance\n");
        printf("4. Sort the courses using the topological sort\n");
        printf("5. Print the sorted courses\n");
        printf("6. Exit the application\n\n");
        printf("Enter the option: ");

        scanf("%d", &choice);

        switch (choice)
        {

        case 1:

            input = fopen("input_buildings.txt", "r");

            if (input == NULL)
            {
                printf("\nUnable to read the text from the file!\n\n");
                exit(1);
            }

            // First making the Buildings array
            while (fgets(line, sizeof(line), input))
            {
                char *token;
                token = strtok(line, "#");

                if (token != NULL)
                {
                    strcpy(BuildingName1, token);
                    int isFound = 0;
                    for (int i = 0; i < buildingsGraph->Pointer; i++)
                    {
                        if (strcmp(buildingsGraph->Buildings[i]->Name, BuildingName1) == 0)
                        {
                            isFound = 1;
                            break;
                        }
                    }
                    if (!isFound)
                    {
                        InsertLastToBuildingsArray(BuildingName1, buildingsGraph);
                    }
                }
            }

            fclose(input);

            input = fopen("input_buildings.txt", "r");

            if (input == NULL)
            {
                printf("\nUnable to read the text from the file!\n\n");
                exit(1);
            }

            // Second making the graph
            while (fgets(line, sizeof(line), input))
            {
                char *token = strtok(line, "#");

                if (token != NULL)
                {
                    strcpy(BuildingName1, token);
                    token = strtok(NULL, "#");
                }
                if (token != NULL)
                {
                    strcpy(BuildingName2, token);
                    token = strtok(NULL, "#");
                }

                if (token != NULL)
                {
                    Weight = atoi(token);
                }

                for (int i = 0; i < buildingsGraph->Pointer; i++)
                {
                    if (strcmp(buildingsGraph->Buildings[i]->Name, BuildingName1) == 0)
                    {
                        InsertBuildingLast(BuildingName2, Weight, buildingsGraph->Buildings[i]);
                        break;
                    }
                }
            }

            fclose(input);

            // Prints the graph
            /*printf("\n");
            for (int i = 0; i < buildingsGraph->Pointer; i++)
            {
                printf("%s |-> ", buildingsGraph->Buildings[i]->Name);
                PrintBuildingList(buildingsGraph->Buildings[i]);
            }*/

            input = fopen("input_courses.txt", "r");

            if (input == NULL)
            {
                printf("\nUnable to read the text from the file!\n\n");
                exit(1);
            }

            // First making the Courses array
            while (fgets(line, sizeof(line), input))
            {
                char *token;
                token = strtok(line, "#");

                if (token != NULL)
                {
                    strcpy(CourseName, token);
                    int isFound = 0;
                    for (int i = 0; i < courseGraph->Pointer; i++)
                    {
                        if (strcmp(courseGraph->Courses[i]->Name, CourseName) == 0)
                        {
                            isFound = 1;
                            break;
                        }
                    }
                    if (!isFound)
                    {
                        CourseName[strcspn(CourseName, "\n")] = '\0';
                        InsertLastToCoursesArray(CourseName, courseGraph);
                    }
                }
            }

            fclose(input);

            input = fopen("input_courses.txt", "r");

            if (input == NULL)
            {
                printf("\nUnable to read the text from the file!\n\n");
                exit(1);
            }

            // Second making the courses graph
            while (fgets(line, sizeof(line), input))
            {
                char *token = strtok(line, "#");

                char firstCourseName[50];
                strcpy(firstCourseName, token);

                token = strtok(NULL, "#");

                while (token != NULL)
                {
                    strcpy(CourseName, token);

                    for (int i = 0; i < courseGraph->Pointer; i++)
                    {
                        if (strcmp(courseGraph->Courses[i]->Name, firstCourseName) == 0)
                        {
                            CourseName[strcspn(CourseName, "\n")] = '\0';
                            InsertCourseLast(CourseName, courseGraph->Courses[i]);
                            break;
                        }
                    }

                    token = strtok(NULL, "#");
                }
            }

            fclose(input);

            // Prints the graph
            /*printf("\n");
            for (int i = 0; i < courseGraph->Pointer; i++)
            {
                printf("%s |-> ", courseGraph->Courses[i]->Name);
                PrintCourseList(courseGraph->Courses[i]);
            }*/

            printf("\nThe information loaded successfully\n\n");
            IsLoaded = 1;
            break;

        case 2:

            if (IsLoaded == 1)
            {

                printf("Enter the name of the first building: ");
                scanf("%s", BuildingName1);
                printf("Enter the name of the second building: ");
                scanf("%s", BuildingName2);

                int sourceBuilding = getBuildingIndex(BuildingName1, buildingsGraph);
                int destinationBuilding = getBuildingIndex(BuildingName2, buildingsGraph);

                if (sourceBuilding != -1)
                {
                    if (destinationBuilding != -1)
                    {
                        Dijkstra(buildingsGraph, sourceBuilding, destinationBuilding, 0);
                    }
                    else
                    {
                        printf("\nThe second Building you entered not found\n\n");
                    }
                }
                else
                {
                    printf("\nThe first Building you entered not found\n\n");
                }
            }
            else
            {

                printf("\nYou have to load the files first\n\n");
            }
            break;

        case 3:

            if (IsLoaded == 1)
            {

                printf("Enter the name of the first building: ");
                scanf("%s", BuildingName1);
                printf("Enter the name of the second building: ");
                scanf("%s", BuildingName2);

                int sourceBuilding = getBuildingIndex(BuildingName1, buildingsGraph);
                int destinationBuilding = getBuildingIndex(BuildingName2, buildingsGraph);

                if (sourceBuilding != -1)
                {
                    if (destinationBuilding != -1)
                    {
                        Dijkstra(buildingsGraph, sourceBuilding, destinationBuilding, 1);
                    }
                    else
                    {
                        printf("\nThe second Building you entered not found\n\n");
                    }
                }
                else
                {
                    printf("\nThe first Building you entered not found\n\n");
                }
            }
            else
            {

                printf("\nYou have to load the files first\n\n");
            }

            break;

        case 4:

            if (IsLoaded == 1)
            {
                sortedCoursesList = TopologicalSort(courseGraph);
                IsSorted = 1;
            }
            else
            {

                printf("\nYou have to load the files first\n\n");
            }

            break;

        case 5:

            if (IsSorted == 1)
            {

                int num = 1;
                printf("\nThe sorted courses is:\n\n");
                while (sortedCoursesList != NULL)
                {
                    printf("%d.%s\n", num, sortedCoursesList->Name);
                    sortedCoursesList = sortedCoursesList->Next;
                    num++;
                }
                printf("\n");
            }
            else
            {

                printf("\nYou have to sort the courses first\n\n");
            }
            break;

        case 6:

            printf("\nWe are gonna miss you <3\n");
            break;

        default:

            printf("\nAre you sure you entered a number between 1 - 6 :/\n\n");
        }

    } while (choice != 6);

    return 0;
}
int IsBuildingEmpty(struct Building *L)
{
    return L->Next == NULL;
}

int IsCourseEmpty(struct Course *L)
{
    return L->Next == NULL;
}

BuildingGraph CreateBuildingGraph()
{
    BuildingGraph G = (BuildingGraph)malloc(sizeof(struct BuildingGraphRecord));

    if (G == NULL)
    {
        printf("Out of memory!\n");
        exit(1);
    }

    G->Pointer = 0;

    for (int i = 0; i < MaxBuildingsNum; i++)
    {
        G->Buildings[i] = NULL;
    }

    return G;
}

CoursesGraph CreateCoursesGraph()
{
    CoursesGraph G = (CoursesGraph)malloc(sizeof(struct CoursesGraphRecord));

    if (G == NULL)
    {
        printf("Out of memory!\n");
        exit(1);
    }

    G->Pointer = 0;

    for (int i = 0; i < MaxCoursesNum; i++)
    {
        G->Courses[i] = NULL;
    }

    return G;
}

void InsertLastToBuildingsArray(char *buildingName, BuildingGraph G)
{
    if (G->Pointer < MaxBuildingsNum)
    {
        G->Buildings[G->Pointer] = (struct Building *)malloc(sizeof(struct Building));
        if (G->Buildings[G->Pointer] == NULL)
        {
            printf("Out of memory!\n");
            exit(1);
        }
        strcpy(G->Buildings[G->Pointer]->Name, buildingName);
        G->Buildings[G->Pointer]->Next = NULL;
        G->Pointer++;
    }
    else
    {
        printf("Full Graph!\n");
    }
}

void InsertLastToCoursesArray(char *courseName, CoursesGraph G)
{
    if (G->Pointer < MaxCoursesNum)
    {
        G->Courses[G->Pointer] = (struct Course *)malloc(sizeof(struct Course));
        if (G->Courses[G->Pointer] == NULL)
        {
            printf("Out of memory!\n");
            exit(1);
        }
        strcpy(G->Courses[G->Pointer]->Name, courseName);
        G->Courses[G->Pointer]->Next = NULL;
        G->Pointer++;
    }
    else
    {
        printf("Full Graph!\n");
    }
}

void InsertBuildingLast(char *buildingName, int weight, struct Building *L)
{
    struct Building *temp;
    struct Building *P = L;

    temp = (struct Building *)malloc(sizeof(struct Building));

    if (temp == NULL)
    {
        printf("Out of memory!\n");
        exit(1);
    }

    strcpy(temp->Name, buildingName);
    temp->Weight = weight;
    temp->Next = NULL;

    while (P->Next != NULL)
    {
        P = P->Next;
    }

    P->Next = temp;
}

void InsertCourseLast(char *courseName, struct Course *L)
{
    struct Course *temp;
    struct Course *P = L;

    temp = (struct Course *)malloc(sizeof(struct Course));

    if (temp == NULL)
    {
        printf("Out of memory!\n");
        exit(1);
    }

    strcpy(temp->Name, courseName);
    temp->Next = NULL;

    while (P->Next != NULL)
    {
        P = P->Next;
    }

    P->Next = temp;
}

struct Course *InsertCourseNodeLast(char *courseName, struct Course *L)
{

    struct Course *temp;
    struct Course *P = L;

    temp = (struct Course *)malloc(sizeof(struct Course));

    if (temp == NULL)
    {
        printf("Out of memory!\n");
        exit(1);
    }

    strcpy(temp->Name, courseName);
    temp->Next = NULL;

    if (L == NULL)
    {
        return temp;
    }
    else
    {

        while (P->Next != NULL)
        {
            P = P->Next;
        }

        P->Next = temp;

        return L;
    }
}

void PrintBuildingList(struct Building *L)
{
    struct Building *P = L;

    if (IsBuildingEmpty(L))
    {
        printf("Empty list\n");
    }
    else
    {
        while (P->Next != NULL)
        {
            P = P->Next;
            printf("%s (%d) -> ", P->Name, P->Weight);
        }
        printf("\n");
    }
}

void PrintCourseList(struct Course *L)
{
    struct Course *P = L;

    if (IsCourseEmpty(L))
    {
        printf("Empty list\n");
    }
    else
    {
        while (P->Next != NULL)
        {
            P = P->Next;
            printf("%s -> ", P->Name);
        }
        printf("\n");
    }
}

int getBuildingIndex(char *buildingName, BuildingGraph G)
{
    for (int i = 0; i < G->Pointer; i++)
    {
        if (strcasecmp(G->Buildings[i]->Name, buildingName) == 0)
        {
            return i;
        }
    }
    return -1;
}

void Dijkstra(BuildingGraph graph, int source, int destination, int printPath)
{
    // First: making the parallel arrays
    int buildingsNum = graph->Pointer;
    int distance[buildingsNum];
    int neighbour[buildingsNum];
    int isVisited[buildingsNum];

    // Second: initialize the array
    for (int i = 0; i < buildingsNum; i++)
    {
        distance[i] = INFINITY;
        neighbour[i] = -1;
        isVisited[i] = 0;
    }

    // Third: setting the source vertex
    distance[source] = 0;

    // Fourth: Loop through all buildings and calculate their distance from the source
    for (int i = 0; i < buildingsNum - 1; i++)
    {
        int minDistance = INFINITY;
        int minIndex = -1;

        // Part one: find the lowest unvisited vertex distance
        for (int j = 0; j < buildingsNum; j++)
        {
            if (isVisited[j] == 0 && distance[j] <= minDistance)
            {
                minIndex = j;
                minDistance = distance[j];
            }
        }

        // Part Two: set the vertex that it got visited and save its index
        int u = minIndex;
        isVisited[u] = 1;

        // Part Three: calculate the next vertex distance from source and update the neighbour
        struct Building *temp = graph->Buildings[u]->Next;
        while (temp != NULL)
        {
            int buildingIndex = getBuildingIndex(temp->Name, graph);

            if (isVisited[buildingIndex] == 0 && distance[u] != INFINITY)
            {
                if (distance[u] + temp->Weight < distance[buildingIndex])
                {
                    distance[buildingIndex] = distance[u] + temp->Weight;
                    neighbour[buildingIndex] = u;
                }
            }
            temp = temp->Next;
        }
    }

    if (distance[destination] == INFINITY)
    {
        printf("\nThere is no path between the buildings\n\n");
    }
    else
    {
        if (printPath == 1)
        {
            printf("\nThe shortest distance between the buildings is %d\n", distance[destination]);
            printf("The shortest Path is: ");

            int destinationBuilding = destination;
            int path[MaxBuildingsNum];
            int counter = 0;

            // Traverse from destination to source and store the path
            while (destinationBuilding != -1)
            {
                path[counter] = destinationBuilding;
                destinationBuilding = neighbour[destinationBuilding];
                counter++;
            }

            // Print the path in reverse order
            for (int i = counter - 1; i >= 0; i--)
            {
                printf("%s", graph->Buildings[path[i]]->Name);
                if (i > 0)
                {
                    printf(" -> ");
                }
            }

            printf("\n\n");
        }
        else
        {
            printf("\nThe shortest distance between the buildings is %d\n\n", distance[destination]);
        }
    }
}

int getInDegree(struct Course *L)
{
    struct Course *P = L->Next;
    int count = 0;

    while (P != NULL)
    {
        count += 1;
        P = P->Next;
    }

    return count;
}

void removeCourse(char *courseName, struct Course *L)
{
    struct Course *P;
    struct Course *temp;

    P = L;
    while (P->Next != NULL && strcmp(P->Next->Name, courseName) != 0)
        P = P->Next;

    if (!(P->Next == NULL))
    {
        temp = P->Next;
        P->Next = temp->Next;
    }
}

int isEmpty(Queue Q)
{
    return Q->Size == 0;
}

int isFull(Queue Q)
{
    return Q->Size == Q->Capacity;
}

Queue CreateQueue(int Capacity)
{
    Queue Q;

    Q = (Queue)malloc(sizeof(struct QueueRecord));

    if (Q == NULL)
    {
        printf("Out of memory!\n");
        exit(1);
    }

    Q->Array = (struct Course *)malloc(sizeof(struct Course) * Capacity);

    if (Q->Array == NULL)
    {
        printf("Out of memory!\n");
        exit(1);
    }

    Q->Capacity = Capacity;
    MakeEmpty(Q);

    return Q;
}

void MakeEmpty(Queue Q)
{
    Q->Size = 0;
    Q->Front = 1;
    Q->Rear = 0;
}

void DisposeQueue(Queue Q)
{

    if (Q != NULL)
    {
        free(Q->Array);
        free(Q);
    }
}

int Succ(int Value, Queue Q)
{

    if (++Value == Q->Capacity)
        Value = 0;

    return Value;
}

void Enqueue(struct Course courseNode, Queue Q)
{
    if (isFull(Q))
    {
        printf("Full Queue!\n");
    }
    else
    {
        Q->Size++;
        Q->Rear = Succ(Q->Rear, Q);
        Q->Array[Q->Rear] = courseNode;
    }
}

struct Course FrontAndDequeue(Queue Q)
{
    struct Course courseNode;
    if (isEmpty(Q))
    {
        printf("Empty Queue!\n");
    }
    else
    {
        Q->Size--;
        courseNode = Q->Array[Q->Front];
        Q->Front = Succ(Q->Front, Q);
    }

    return courseNode;
}

void Dequeue(Queue Q)
{

    if (isEmpty(Q))
    {
        printf("Empty Queue!\n");
    }
    else
    {
        Q->Size--;
        Q->Front = Succ(Q->Front, Q);
    }
}

struct Course *TopologicalSort(CoursesGraph graph)
{
    // First: Making the inDegree array, Queue, SortedLinkedList
    int inDegree[MaxCoursesNum];
    Queue Q = CreateQueue(MaxCoursesNum);
    struct Course *sortedCourses = NULL;

    // Second: Initialize inDegree array
    for (int i = 0; i < graph->Pointer; i++)
    {
        inDegree[i] = getInDegree(graph->Courses[i]);
    }

    // Third: Enqueue the Courses with zero inDegree
    for (int i = 0; i < graph->Pointer; i++)
    {
        if (inDegree[i] == 0)
        {
            Enqueue(*(graph->Courses[i]), Q);
        }
    }

    // Fourth: Topological sort (save the courses name in the list and repeat the previous steps(second and third) while the queue not empty)
    while (!isEmpty(Q))
    {
        struct Course frontCourse = FrontAndDequeue(Q);
        sortedCourses = InsertCourseNodeLast(frontCourse.Name, sortedCourses);

        for (int i = 0; i < graph->Pointer; i++)
        {
            struct Course *courseNode = graph->Courses[i]->Next;

            while (courseNode != NULL)
            {
                if (strcmp(courseNode->Name, frontCourse.Name) == 0)
                {
                    removeCourse(frontCourse.Name, graph->Courses[i]);
                    inDegree[i]--;

                    if (inDegree[i] == 0)
                    {
                        Enqueue(*(graph->Courses[i]), Q);
                    }

                    break;
                }
                courseNode = courseNode->Next;
            }
        }
    }

    printf("\nThe courses have been sorted\n\n");
    DisposeQueue(Q);

    return sortedCourses;
}
