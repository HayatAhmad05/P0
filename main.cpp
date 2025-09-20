#include <iostream>
#include <string>
#include <cmath>

struct PotentialVector {
    double px;
    double py;
};

struct PointOfInterest {
    int x;
    int y;
    char type; // 'G' for goal and 'O' for obstacle
};

class Map {
private:
    PotentialVector** grid;
    PointOfInterest* pointList; // A dynamic array to store goals/obstacles
    int pointCount; // Number of points in pointList
    int maxPoints; // Maximum capacity of pointList
    int mapN, mapM;
    double k;
    bool isInitialized;

    // private methods
    void allocateMemory(int N, int M);
    void deallocateMemory(int N);
    void initialize(int N, int M);
    void resizePointList();
    void updatePotentials();

public:
    Map();
    ~Map();
    void CREATE(int N, int M);
    void POINT(char T, int X, int Y);
    void MOVE(int X, int Y);
    void CLEAR();
    void UPDATE(double K);
};

// constructor
Map::Map() {
    grid = nullptr;
    mapN = 0;
    mapM = 0;
    k = 0.0;
    isInitialized = false;
    pointCount = 0;
    maxPoints = 0;
    pointList = new PointOfInterest[maxPoints];
}

// destructor
Map::~Map() {
    if (grid != nullptr) {
        deallocateMemory(mapN);
    }
    delete[] pointList;
    pointList = nullptr;
}

// private methods
void Map::allocateMemory(int N, int M) {
    mapN = N;
    mapM = M;
    grid = new PotentialVector*[N];
    for (int i = 0; i < N; i++) {
        grid[i] = new PotentialVector[M];
    }
    pointCount = 0;  
}

void Map::deallocateMemory(int N) {
    for (int i = 0; i < N; i++) {
        delete[] grid[i];
    }
    delete[] grid;
    grid = nullptr;
    isInitialized = false;
}

void Map::initialize(int N, int M) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            grid[i][j].px = 0;
            grid[i][j].py = 0;
        }
    }
}

void Map::resizePointList() {
    int newCapacity = 0;

    if (maxPoints == 0){

        newCapacity = 10;

    } else{

        newCapacity = maxPoints * 2;
    }

    PointOfInterest* newArray = new PointOfInterest[newCapacity]; 

    for (int i = 0; i < pointCount; i++) {
        newArray[i] = pointList[i];
    }


    delete[] pointList;

    pointList = newArray;
    maxPoints = newCapacity;
}

void Map::updatePotentials() {
    for (int i = 0; i < mapN; i++) {
        for (int j = 0; j < mapM; j++) {
            grid[i][j].px = 0.0;
            grid[i][j].py = 0.0;
            for (int z = 0; z < pointCount; z++) {
                PotentialVector P;
                int point_x = pointList[z].x;
                int point_y = pointList[z].y;
                char point_char = pointList[z].type;
                double distance = std::sqrt(std::pow(j - point_x, 2) + std::pow(i - point_y, 2));
                if (distance == 0) {
                    continue;
                }
                if (point_char == 'G') {
                    P.px = -k / distance;
                    P.py = -k / distance;
                } else {
                    P.px = k / distance;
                    P.py = k / distance;
                }
                grid[i][j].px = grid[i][j].px + P.px;
                grid[i][j].py = grid[i][j].py + P.py;
            }
        }
    }
}

// public methods
void Map::CREATE(int N, int M) {
    if (isInitialized) {
        deallocateMemory(mapN);
    }
    allocateMemory(N, M);
    initialize(N, M);
    k = 1;
    isInitialized = true;

    std::cout << "success" << std::endl;
}

void Map::POINT(char T, int X, int Y) {
    if (!isInitialized) {
        std::cout << "failure" << std::endl;
        return;
    }

    if (X < 0 || X >= mapN || Y < 0 || Y >= mapM) {
        std::cout << "failure" << std::endl;
        return;
    }

    if (T != 'G' && T != 'O') {
        std::cout << "failure" << std::endl;
        return;
    }

    if (pointCount >= maxPoints) {
        resizePointList();
    }
    pointList[pointCount].x = X;
    pointList[pointCount].y = Y;
    pointList[pointCount].type = T;
    pointCount++;

    // Update potential vectors in the grid
    updatePotentials();
    std::cout << "success" << std::endl;
}

void Map::MOVE(int X, int Y) {
    if (isInitialized == false) {
        std::cout << "failure" << std::endl;
        return;
    }

    if (X < 0 || X >= mapM || Y < 0 || Y >= mapN) {
        std::cout << "failure" << std::endl;
        return;
    }

    PotentialVector point = grid[Y][X];

    std::cout << point.px << " " << point.py << std::endl;
}

void Map::CLEAR() {
    if (isInitialized == false) {
        std::cout << "failure" << std::endl;
        return;
    }

    pointCount = 0;

    initialize(mapN, mapM);

    std::cout << "success" << std::endl;
}

void Map::UPDATE(double K) {
    if (isInitialized == false) {
        std::cout << "failure" << std::endl;
        return;
    }

    if (K <= 0) {
        std::cout << "failure" << std::endl;
        return;
    }

    k = K;

    updatePotentials();
    std::cout << "success" << std::endl;
}

int main() {
    Map myMap;
    std::string command;

    while (std::cin >> command) {
        if (command == "EXIT") {
            break;
        } else if (command == "CREATE") {
            int N, M;
            std::cin >> N >> M;
            myMap.CREATE(N, M);
        } else if (command == "POINT") {
            char T;
            int X, Y;
            std::cin >> T >> X >> Y;
            myMap.POINT(T, X, Y);
        } else if (command == "MOVE") {
            int X, Y;
            std::cin >> X >> Y;
            myMap.MOVE(X, Y);
        } else if (command == "CLEAR") {
            myMap.CLEAR();
        } else if (command == "UPDATE") {
            double K;
            std::cin >> K;
            myMap.UPDATE(K);
        }
    }

    return 0;
}