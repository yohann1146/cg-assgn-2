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
void generateSurfaceOfRevolution(const std::vector<Point2D>& curve, int rotationSteps, std::vector<Point3D>& vertices,
    std::vector<std::vector<int>>& faces) {
    double dTheta = 2.0*M_PI/rotationSteps;

    //generating vertices
    for (int j=0; j<rotationSteps; ++j) {
        double theta = j*dTheta;
        double cosTheta = cos(theta);
        double sinTheta = sin(theta);
        for (auto& p:curve) {
            vertices.push_back({ p.x*cosTheta, p.y, p.x*sinTheta });
        }
    }

    int curveSize = curve.size();
    //generating quad face
    for (int j=0; j <rotationSteps; ++j) {
        int nextJ=(j+1) % rotationSteps;
        for (int i=0; i <curveSize-1; ++i) {
            int v0 = j*curveSize + i;
            int v1 = nextJ*curveSize + i;
            int v2 = nextJ*curveSize + (i+1);
            int v3 = j*curveSize + (i+1);
            faces.push_back({4, v0, v1, v2, v3});
        }
    }
}

//generating OFF file from faces and vertices
void writeOFF(const std::string& filename,
              const std::vector<Point3D>& vertices,
              const std::vector<std::vector<int>>& faces) {
    std::ofstream out(filename);
    out << "OFF\n";
    out << vertices.size() << " " <<faces.size()<< " 0\n";
    for (auto& v:vertices) {
        out << v.x << " " << v.y << " " << v.z << "\n";
    }
    for (auto& f:faces) {
        for (int vi:f) out << vi << " ";
        out << "\n";
    }
    out.close();
}

int main(){
    
    //reading sampled points from txt
    std::ifstream in("points.txt");
    std::vector<Point2D> samples;
    Point2D temp;
    while (in >> temp.x >> temp.y)
        samples.push_back(temp);

    //generating mesh from 
    std::vector<Point3D> vertices;
    std::vector<std::vector<int>> faces;
    int rotationSteps = 36;  // 360/36 = 10 degrees increment for each face
    generateSurfaceOfRevolution(samples, rotationSteps, vertices, faces);

    //writing to surface.off
    std::string outputFilename = "surface.off";
    writeOFF(outputFilename, vertices, faces);

    std::cout << "OFF surface mesh written to " << outputFilename << std::endl;
    std::cout << std::endl;

    return 0;
}