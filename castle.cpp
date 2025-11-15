#include <vector>
#include <GL/glut.h>

// Control point structure
struct Point2D {
    float x, y;
};

// List of control points (example)
std::vector<Point2D> ctrlPoints = {
    {100, 100},
    {150, 300},
    {300, 300},
    {400, 100}
};

int draggedPoint=-1;

Point2D windowToWorld(int x, int y) {
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    return { (float)x, (float)(h - y) };
}

// de Casteljau algorithm
Point2D deCasteljau(const std::vector<Point2D>& points, float t) {
    std::vector<Point2D> tmp = points;
    int n = tmp.size();
    for (int k = 1; k < n; ++k) {
        for (int i = 0; i < n - k; ++i) {
            tmp[i].x = (1 - t) * tmp[i].x + t * tmp[i + 1].x;
            tmp[i].y = (1 - t) * tmp[i].y + t * tmp[i + 1].y;
        }
    }
    return tmp[0];
}

void motion(int x, int y) {
    if (draggedPoint != -1) {
        ctrlPoints[draggedPoint] = windowToWorld(x, y);
        glutPostRedisplay();
    }
}

void mouse(int button, int state, int x, int y) {
    Point2D m = windowToWorld(x, y);
    const float threshold = 10.0f; // Pixel tolerance for point selection

    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Check if clicking near a point for deletion or drag
        for (int i = 0; i < ctrlPoints.size(); ++i) {
            float dx = ctrlPoints[i].x - m.x;
            float dy = ctrlPoints[i].y - m.y;
            if (dx*dx + dy*dy < threshold*threshold) {
                draggedPoint = i; // Begin drag
                return;
            }
        }
        // Add new point
        ctrlPoints.push_back(m);
        glutPostRedisplay();
    }
    else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        if (draggedPoint != -1) draggedPoint = -1; // End drag
    }
    else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        // Right click to delete point
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

// Render function for OpenGL
void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw control polygon
    glColor3f(0.9f, 0.9f, 0.9f);
    glBegin(GL_LINE_STRIP);
    for (auto& p : ctrlPoints)
        glVertex2f(p.x, p.y);
    glEnd();

    // Draw control points
    glPointSize(8);
    glColor3f(1.f, 0.f, 0.f);
    glBegin(GL_POINTS);
    for (auto& p : ctrlPoints)
        glVertex2f(p.x, p.y);
    glEnd();

    // Draw Bézier curve
    glColor3f(0.1f, 0.2f, 1.f);
    glBegin(GL_LINE_STRIP);
    for (float t = 0; t <= 1.0f; t += 0.01f) {
        Point2D pt = deCasteljau(ctrlPoints, t);
        glVertex2f(pt.x, pt.y);
    }
    glEnd();

    glFlush();
}

void init() {
    glClearColor(1, 1, 1, 1);
    gluOrtho2D(0, 500, 0, 400); // Set window size
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 400);
    glutCreateWindow("de Casteljau Bezier Curve Newlol");
    init();
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
