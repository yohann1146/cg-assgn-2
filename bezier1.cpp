//TASKS 1 AND 2: implementing the Bezier Curve algorithm and adding editable inputs

#include <cstdio>
#include <vector>
#include <GL/glut.h>

#define HEIGHT 400
#define WIDTH 500

//structure for 2d coordinate
struct Point {
    float x, y;
};

//initial list of control points
std::vector<Point> ctrlPoints = {
    {100, 100},
    {150, 300},
    {300, 300},
    {400, 100}
};

//list of samples (every point on the curve)
std::vector<Point> samples;

//index of the point in the list which is currently being dragged
//initially set to -1 (no point being dragged)
int draggedPoint=-1;

Point windowToWorld(int x, int y) {
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    return {(float)x, (float)(h - y)};
}

// de Casteljau algorithm
Point deCasteljau(const std::vector<Point>& points, float t) {
    std::vector<Point> tmp = points;
    int n = tmp.size();
    for (int k=1; k<n; ++k) {
        for (int i=0; i<n-k; ++i) {
            tmp[i].x = (1-t)*tmp[i].x + t*tmp[i + 1].x;
            tmp[i].y = (1-t)*tmp[i].y + t*tmp[i + 1].y;
        }
    }
    return tmp[0];
}

//function to find list of points on curve at a fixed sample rate
std::vector<Point> sampleCurve(const std::vector<Point>& ctrlPoints, float interval) {
    std::vector<Point> samples;
    FILE* f = fopen("points.txt", "w");
    if(!f) return {};

    for (double t = 0; t <= 1.0; t += interval) {
        samples.push_back(deCasteljau(ctrlPoints, t));
        fprintf(f, "%f %f\n", samples.back().x, samples.back().y);
    }
    fclose(f);
    return samples;
}

//glut motion func for moving while pressing
void motion(int x, int y) {
    if (draggedPoint != -1) {
        ctrlPoints[draggedPoint] = windowToWorld(x, y);
        glutPostRedisplay();
    }
}

//glut func for mouseclick
void mouse(int button, int state, int x, int y) {
    Point m = windowToWorld(x, y);
    const float threshold = 10.0f; //how near the mouse needs to be to the point

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        //drag
        //iteratively finding which point is close to mouse
        for (int i = 0; i < ctrlPoints.size(); ++i) {
            float dx = ctrlPoints[i].x - m.x;
            float dy = ctrlPoints[i].y - m.y;
            if (dx*dx + dy*dy < threshold*threshold) {
                draggedPoint = i;
                return;
            }
        }
        
        ctrlPoints.push_back(m);
        glutPostRedisplay();
    }

    //release
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (draggedPoint != -1) draggedPoint = -1;      //set index to default now
    }

    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        //delete
        for (int i = 0; i < ctrlPoints.size(); ++i) {
            float dx = ctrlPoints[i].x - m.x;
            float dy = ctrlPoints[i].y - m.y;
            if (dx*dx + dy*dy < threshold*threshold) {
                ctrlPoints.erase(ctrlPoints.begin() + i);
                glutPostRedisplay();
                return;
            }
        }
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    //draw polygon enclosing the pts
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_LINE_STRIP);
    for (auto& p : ctrlPoints)
        glVertex2f(p.x, p.y);
    glEnd();

    //draw points
    glPointSize(8);
    glColor3f(1.f, 0.f, 0.f);
    glBegin(GL_POINTS);
    for (auto& p : ctrlPoints)
        glVertex2f(p.x, p.y);
    glEnd();

    //draw curve
    glColor3f(0.1f, 0.2f, 1.f);
    glBegin(GL_LINE_STRIP);
    //sampling at interval of 0.01
    samples=sampleCurve(ctrlPoints, 0.01);
    
    for (auto &pt: samples)
        glVertex2f(pt.x, pt.y);
    
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, WIDTH, 0, HEIGHT);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("2D Bezier Curve");
    init();
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
