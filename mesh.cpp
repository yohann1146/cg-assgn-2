//TASK 3 - sampling a bezier curve to create surface of revolution

#include <vector>
#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iostream>

struct Point2D {
    double x, y;
};
struct Point3D {
    double x, y, z;
};

//Generate surface of revolution
void generateSurfaceOfRevolution(const std::vector<Point2D>& curve,
                                 int rotationSteps,
                                 std::vector<Point3D>& vertices,
                                 std::vector<std::vector<int>>& faces) {
    double dTheta = 2.0 * M_PI / rotationSteps;

    // Generate vertices
    for (int j = 0; j < rotationSteps; ++j) {
        double theta = j * dTheta;
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);
        for (const auto& p : curve) {
            vertices.push_back({ p.x * cosTheta, p.y, p.x * sinTheta });
        }
    }

    int curveSize = curve.size();
    // Generate faces (quads)
    for (int j = 0; j < rotationSteps; ++j) {
        int nextJ = (j + 1) % rotationSteps;
        for (int i = 0; i < curveSize - 1; ++i) {
            int v0 = j * curveSize + i;
            int v1 = nextJ * curveSize + i;
            int v2 = nextJ * curveSize + (i + 1);
            int v3 = j * curveSize + (i + 1);
            // Quad face
            faces.push_back({4, v0, v1, v2, v3});
        }
    }
}

// Write OFF file
void writeOFF(const std::string& filename,
              const std::vector<Point3D>& vertices,
              const std::vector<std::vector<int>>& faces) {
    std::ofstream out(filename);
    out << "OFF\n";
    out << vertices.size() << " " << faces.size() << " 0\n";
    for (const auto& v : vertices) {
        out << v.x << " " << v.y << " " << v.z << "\n";
    }
    for (const auto& f : faces) {
        for (int vi : f) out << vi << " ";
        out << "\n";
    }
    out.close();
}

int main(){

    // 1. Sample the Bézier curve at uniform intervals 0.0 to 1.0 step 0.1
    std::ifstream in("points.txt");
    std::vector<Point2D> samples;
    Point2D temp;
    while (in >> temp.x >> temp.y)
        samples.push_back(temp);

    // 2. Generate the surface of revolution vertices and faces
    std::vector<Point3D> vertices;
    std::vector<std::vector<int>> faces;
    int rotationSteps = 36;  // Revolve in 36 steps (10 degrees each)
    generateSurfaceOfRevolution(samples, rotationSteps, vertices, faces);

    // 3. Write to OFF file
    std::string outputFilename = "surface.off";
    writeOFF(outputFilename, vertices, faces);

    std::cout << "OFF surface mesh written to " << outputFilename << std::endl;
    std::cout << std::endl;

    return 0;
}