#include <GL/glut.h>
#include <math.h>
#include <stdio.h>

float cameraPosX=0.0f;
float cameraPosY=10.0f;
float cameraPosZ=25.0f;

float cameraTargetX=0.0f;
float cameraTargetY=2.0f;
float cameraTargetZ=0.0f;

float cameraYaw=0.0f;
float cameraPitch=-20.0f;
float cameraRoll=0.0f;

float cameraDistance=25.0f;

float swingAngle=0.0f;
float swingDirection=1.0f;
float seesawAngle=0.0f;
float seesawDirection=1.0f;
bool autoRotate=false;

int mouseX=0,mouseY=0;
bool mouseLeftDown=false;
bool mouseRightDown=false;

void init(){
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT,GL_AMBIENT_AND_DIFFUSE);

    GLfloat light_position[]={10.0,20.0,10.0,1.0};
    GLfloat light_ambient[]={0.3,0.3,0.3,1.0};
    GLfloat light_diffuse[]={1.0,1.0,1.0,1.0};

    glLightfv(GL_LIGHT0,GL_POSITION,light_position);
    glLightfv(GL_LIGHT0,GL_AMBIENT,light_ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,light_diffuse);

    glClearColor(0.53,0.81,0.92,1.0);
}

void yawCamera(float angle){
    cameraYaw += angle;
    if(cameraYaw > 360.0f) cameraYaw -= 360.0f;
    if(cameraYaw < 0.0f) cameraYaw += 360.0f;
}

void pitchCamera(float angle){
    cameraPitch += angle;

    if(cameraPitch > 89.0f) cameraPitch=89.0f;
    if(cameraPitch < -89.0f) cameraPitch=-89.0f;
}

void rollCamera(float angle){
    cameraRoll += angle;
    if(cameraRoll > 360.0f) cameraRoll -= 360.0f;
    if(cameraRoll < 0.0f) cameraRoll += 360.0f;
}

void zoomCamera(float delta){
    cameraDistance += delta;
    if(cameraDistance < 5.0f) cameraDistance=5.0f;
    if(cameraDistance > 50.0f) cameraDistance=50.0f;
}

void panCamera(float deltaX,float deltaY){

    float panSpeed=0.05f;

    float yawRad=cameraYaw*M_PI/180.0f;

    float rightX=cos(yawRad);
    float rightZ=-sin(yawRad);

    cameraTargetX += rightX*deltaX*panSpeed;
    cameraTargetZ += rightZ*deltaX*panSpeed;
    cameraTargetY += deltaY*panSpeed;
}

void updateCameraPosition(){
    float yawRad=cameraYaw*M_PI/180.0f;
    float pitchRad=cameraPitch*M_PI/180.0f;

    cameraPosX=cameraTargetX+cameraDistance*cos(pitchRad)*sin(yawRad);
    cameraPosY=cameraTargetY+cameraDistance*sin(pitchRad);
    cameraPosZ=cameraTargetZ+cameraDistance*cos(pitchRad)*cos(yawRad);
}

void resetCamera(){
    cameraTargetX=0.0f;
    cameraTargetY=2.0f;
    cameraTargetZ=0.0f;
    cameraYaw=0.0f;
    cameraPitch=-20.0f;
    cameraRoll=0.0f;
    cameraDistance=25.0f;
    autoRotate=false;
}

void drawCube(float width,float height,float depth){
    glPushMatrix();
    glScalef(width,height,depth);
    glutSolidCube(1.0);
    glPopMatrix();
}

void drawCylinder(float radius,float height){
    GLUquadric* quad=gluNewQuadric();
    glPushMatrix();
    glRotatef(-90,1,0,0);
    gluCylinder(quad,radius,radius,height,20,20);
    glPopMatrix();
    gluDeleteQuadric(quad);
}

void drawGround(){
    glColor3f(0.0, 0.5, 0.0);
    glPushMatrix();
    glTranslatef(0,-0.25,0);
    drawCube(40,0.5,40);
    glPopMatrix();
}

void drawBucket(float element, float buffer, float remaining, float rotation){
    glPushMatrix();
    glTranslatef(element, buffer, remaining);
    glRotatef(rotation, 0, 1, 0);

    glColor3f(1.0, 0.2, 0.2);
    glPushMatrix();
    GLUquadric* quad = gluNewQuadric();
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quad, 0.15, 0.25, 0.3, 16, 16);
    gluDeleteQuadric(quad);
    glPopMatrix();

    glPushMatrix();
    glRotatef(-90, 1, 0, 0);
    glTranslatef(0, 0, 0.001);
    GLUquadric* disk = gluNewQuadric();
    gluDisk(disk, 0, 0.15, 16, 1);
    gluDeleteQuadric(disk);
    glPopMatrix();

    glColor3f(0.8, 0.8, 0.0);
    glPushMatrix();
    glTranslatef(0, 0.35, 0);
    glRotatef(90, 0, 0, 1);
    drawCylinder(0.02, 0.5);
    glPopMatrix();

    glPopMatrix();
}

void drawSandCastle(float element, float buffer, float remaining){
    glPushMatrix();
    glTranslatef(element, buffer, remaining);

    glColor3f(0.96, 0.64, 0.38);

    glPushMatrix();
    glTranslatef(0, 0.15, 0);
    drawCylinder(0.3, 0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0.45, 0);
    GLUquadric* quad = gluNewQuadric();
    glRotatef(-90, 1, 0, 0);
    gluCylinder(quad, 0, 0.3, 0.25, 16, 16);
    gluDeleteQuadric(quad);
    glPopMatrix();

    for(int counter = 0; counter < 3; counter++){
        float angle = counter * 120.0;
        glPushMatrix();
        glRotatef(angle, 0, 1, 0);
        glTranslatef(0.4, 0.1, 0);
        drawCylinder(0.15, 0.2);
        glPopMatrix();
    }

    glPopMatrix();
}

void drawSandPit(){
    glColor3f(0.96,0.64,0.38);
    glPushMatrix();
    glTranslatef(-10,0.1,-10);
    drawCube(8,0.2,8);
    glPopMatrix();

    glColor3f(0.55,0.27,0.07);
    for(int index=0; index < 4; index++){
        glPushMatrix();
        glTranslatef(-10,0.2,-10);
        glRotatef(index*90,0,1,0);
        glTranslatef(0,0,4.2);
        drawCube(8.4,0.3,0.4);
        glPopMatrix();
    }

    drawBucket(-11.5, 0.2, -8.5, 25);
    drawBucket(-8, 0.2, -11, -15);
    drawBucket(-12, 0.2, -12, 45);

    drawSandCastle(-9, 0.2, -9);

    glColor3f(0.94, 0.62, 0.36);
    glPushMatrix();
    glTranslatef(-11, 0.2, -10.5);
    glScalef(1.0, 0.5, 1.0);
    glutSolidSphere(0.4, 12, 12);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-8.5, 0.2, -8);
    glScalef(1.2, 0.4, 1.0);
    glutSolidSphere(0.35, 12, 12);
    glPopMatrix();
}

void drawTable(float element,float buffer,float remaining){
    glPushMatrix();
    glTranslatef(element,buffer,remaining);

    glColor3f(0.72,0.45,0.20);
    glPushMatrix();
    glTranslatef(0,1.5,0);
    drawCube(3,0.2,2);
    glPopMatrix();

    glColor3f(0.55,0.27,0.07);
    float legPositions[4][2]={{1.3,0.9},{1.3,-0.9},{-1.3,0.9},{-1.3,-0.9}};
    for(int index=0; index < 4; index++){
        glPushMatrix();
        glTranslatef(legPositions[index][0],0.75,legPositions[index][1]);
        drawCube(0.2,1.5,0.2);
        glPopMatrix();
    }

    glPopMatrix();
}

void drawBench(float element,float buffer,float remaining,float rotation){
    glPushMatrix();
    glTranslatef(element,buffer,remaining);
    glRotatef(rotation,0,1,0);

    glColor3f(0.72,0.45,0.20);
    glPushMatrix();
    glTranslatef(0,0.8,0);
    drawCube(2.5,0.15,0.8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,1.3,-0.35);
    drawCube(2.5,0.15,0.1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,1.1,-0.35);
    drawCube(2.5,0.1,0.15);
    glPopMatrix();

    glColor3f(0.4,0.4,0.4);
    float legPos[4][2]={{1.0,0.3},{1.0,-0.3},{-1.0,0.3},{-1.0,-0.3}};
    for(int index=0; index < 4; index++){
        glPushMatrix();
        glTranslatef(legPos[index][0],0.4,legPos[index][1]);
        drawCube(0.15,0.8,0.15);
        glPopMatrix();
    }

    glPopMatrix();
}

void drawSwingSet(){
    glPushMatrix();
    glTranslatef(8,0,-5);

    glColor3f(0.8,0.0,0.0);
    glPushMatrix();
    glTranslatef(-2.5,2,0);
    drawCube(0.3,4,0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.5,2,0);
    drawCube(0.3,4,0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(2.4,4,0);
    glRotatef(90,0,0,1);
    drawCylinder(0.15,5);
    glPopMatrix();

    for(int index=-1; index <= 1; index += 2){
        glPushMatrix();
        glTranslatef(index*1.5,3,0);
        glRotatef(swingAngle*index,1,0,0);

        glColor3f(0.3,0.3,0.3);
        glPushMatrix();
        glTranslatef(0,-1,0);
        drawCylinder(0.05,2);
        glPopMatrix();

        glColor3f(1.0,0.8,0.0);
        glPushMatrix();
        glTranslatef(0,-2,0);
        drawCube(0.8,0.1,0.6);
        glPopMatrix();

        glPopMatrix();
    }

    glPopMatrix();
}

void drawSlide(){
    glPushMatrix();
    glTranslatef(-8,0,5);

    glColor3f(0.0,0.5,1.0);
    glPushMatrix();
    glTranslatef(0,2,0);
    drawCube(2,0.2,2);
    glPopMatrix();

    glColor3f(0.7,0.7,0.7);
    float legPos[4][2]={{0.9,0.9},{0.9,-0.9},{-0.9,0.9},{-0.9,-0.9}};
    for(int index=0; index < 4; index++){
        glPushMatrix();
        glTranslatef(legPos[index][0],1,legPos[index][1]);
        drawCube(0.2,2,0.2);
        glPopMatrix();
    }

    glPushMatrix();

    glTranslatef(0, 0, 0.6);

    glColor3f(1.0,0.0,0.0);
    glPushMatrix();
    glTranslatef(0,1,2);
    glRotatef(30,1,0,0);
    drawCube(1.5,0.1,4);
    glPopMatrix();

    glColor3f(0.9,0.1,0.1);
    for(int index=-1; index <= 1; index += 2){
        glPushMatrix();
        glTranslatef(index*0.8,1,2);
        glRotatef(30,1,0,0);
        drawCube(0.1,0.5,4);
        glPopMatrix();
    }

    glPopMatrix();

    glColor3f(0.5,0.5,0.5);
    for(int index=0; index < 5; index++){
        glPushMatrix();
        glTranslatef(0,0.4+index*0.4,-1);
        drawCube(0.8,0.1,0.1);
        glPopMatrix();
    }

    for(int index=-1; index <= 1; index += 2){
        glPushMatrix();
        glTranslatef(index*0.4,1,-1);
        drawCube(0.1,2,0.1);
        glPopMatrix();
    }

    glPopMatrix();
}

void drawSeesaw(){
    glPushMatrix();
    glTranslatef(0,0,8);

    glColor3f(0.5,0.5,0.5);
    glPushMatrix();
    glTranslatef(0,0.5,0);
    drawCube(0.3,1,0.3);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0,1,0);
    glRotatef(seesawAngle,0,0,1);

    glColor3f(0.72,0.45,0.20);
    drawCube(5,0.2,0.8);

    glColor3f(1.0,0.0,0.0);
    for(int index=-1; index <= 1; index += 2){
        glPushMatrix();
        glTranslatef(index*2,0.5,0);
        drawCube(0.1,1,0.1);
        glPopMatrix();
    }

    glPopMatrix();
    glPopMatrix();
}

void drawMerryGoRound(){
    glPushMatrix();
    glTranslatef(8,0,8);

    glColor3f(0.7,0.7,0.7);
    glPushMatrix();
    glTranslatef(0,0.75,0);
    drawCylinder(0.15,1.5);
    glPopMatrix();

    glColor3f(1.0,0.8,0.0);
    glPushMatrix();
    glTranslatef(0,0.3,0);
    drawCylinder(2,0.2);
    glPopMatrix();

    glColor3f(0.0,0.5,1.0);
    for(int index=0; index < 4; index++){
        glPushMatrix();
        glRotatef(index*90,0,1,0);
        glTranslatef(1.5,0.5,0);
        drawCube(0.1,1,0.1);
        glPopMatrix();
    }

    glPopMatrix();
}

void drawMonkeyBars(){
    glPushMatrix();
    glTranslatef(-8,0,-3);

    glColor3f(0.8,0.0,0.0);
    for(int index=0; index < 2; index++){
        for(int jdx=0; jdx < 2; jdx++){
            glPushMatrix();
            glTranslatef((index*2-1)*2,1.5,(jdx*2-1)*1.5);
            drawCube(0.2,3,0.2);
            glPopMatrix();
        }
    }

    glColor3f(0.5,0.5,0.5);

    for(int index=0; index < 4; index++){
        glPushMatrix();
        glTranslatef(1.8,3,-1.5+index*1.0);
        glRotatef(90,0,0,1);
        drawCylinder(0.08,4);
        glPopMatrix();
    }

    for(int index=0; index < 6; index++){
        glPushMatrix();
        glTranslatef(-2+index*0.8,3,-1.6);
        glRotatef(90,1,0,0);
        drawCylinder(0.08,3);
        glPopMatrix();
    }

    glPopMatrix();
}

void drawTree(float element,float remaining){
    glPushMatrix();
    glTranslatef(element,0,remaining);

    glColor3f(0.55,0.27,0.07);
    glPushMatrix();
    glTranslatef(0,0,0);
    drawCylinder(0.3,5);
    glPopMatrix();

    glColor3f(0.13,0.55,0.13);
    glPushMatrix();
    glTranslatef(0,4,0);
    glutSolidSphere(1.5,20,20);
    glPopMatrix();

    glPopMatrix();
}

void drawFountain(float element,float remaining){
    glPushMatrix();
    glTranslatef(element,0,remaining);

    glColor3f(0.6,0.6,0.6);
    glPushMatrix();
    glTranslatef(0,0.4,0);
    drawCylinder(0.4,0.8);
    glPopMatrix();

    glColor3f(0.4,0.6,0.8);
    glPushMatrix();
    glTranslatef(0,0.9,0);
    glScalef(1,0.3,1);
    glutSolidSphere(0.5,20,20);
    glPopMatrix();

    glColor3f(0.7,0.7,0.7);
    glPushMatrix();
    glTranslatef(0,0.9,0);
    glRotatef(90,1,0,0);
    drawCylinder(0.05,0.3);
    glPopMatrix();

    glPopMatrix();
}

void drawFencePost(float data, float remaining){
    glColor3f(0.4, 0.3, 0.2);
    glPushMatrix();
    glTranslatef(data, 0.75, remaining);
    drawCube(0.15, 1.5, 0.15);
    glPopMatrix();

    glColor3f(0.5, 0.4, 0.3);
    glPushMatrix();
    glTranslatef(data, 1.6, remaining);
    drawCube(0.2, 0.2, 0.2);
    glPopMatrix();
}

void drawFenceSection(float x1, float z1, float x2, float z2){

    glColor3f(0.5, 0.4, 0.3);

    float dx = x2 - x1;
    float dz = z2 - z1;
    float length = sqrt(dx*dx + dz*dz);
    float angle = atan2(dx, dz) * 180.0 / M_PI;

    glPushMatrix();
    glTranslatef((x1+x2)/2, 1.2, (z1+z2)/2);
    glRotatef(angle, 0, 1, 0);
    drawCube(0.1, 0.1, length);
    glPopMatrix();

    glPushMatrix();
    glTranslatef((x1+x2)/2, 0.5, (z1+z2)/2);
    glRotatef(angle, 0, 1, 0);
    drawCube(0.1, 0.1, length);
    glPopMatrix();
}

void drawParkFence(){
    float fenceDistance = 18.0;
    float postSpacing = 3.0;

    for(float data = -fenceDistance; data <= -4.0; data += postSpacing){
        float nextX = data + postSpacing;
        if(nextX > -4.0) nextX = -4.0;
        drawFencePost(data, -fenceDistance);
        if(nextX <= -4.0){
            drawFenceSection(data, -fenceDistance, nextX, -fenceDistance);
        }
    }

    for(float data = 4.0; data <= fenceDistance; data += postSpacing){
        float nextX = data + postSpacing;
        if(nextX > fenceDistance) nextX = fenceDistance;
        drawFencePost(data, -fenceDistance);
        if(nextX <= fenceDistance){
            drawFenceSection(data, -fenceDistance, nextX, -fenceDistance);
        }
    }

    drawFencePost(-4.0, -fenceDistance);
    drawFencePost(4.0, -fenceDistance);

    for(float data = -fenceDistance; data <= fenceDistance; data += postSpacing){
        float nextX = data + postSpacing;
        if(nextX > fenceDistance) nextX = fenceDistance;
        drawFencePost(data, fenceDistance);
        if(nextX <= fenceDistance){
            drawFenceSection(data, fenceDistance, nextX, fenceDistance);
        }
    }

    for(float remaining = -fenceDistance; remaining <= fenceDistance; remaining += postSpacing){
        float nextZ = remaining + postSpacing;
        if(nextZ > fenceDistance) nextZ = fenceDistance;
        drawFencePost(-fenceDistance, remaining);
        if(nextZ <= fenceDistance){
            drawFenceSection(-fenceDistance, remaining, -fenceDistance, nextZ);
        }
    }

    for(float remaining = -fenceDistance; remaining <= fenceDistance; remaining += postSpacing){
        float nextZ = remaining + postSpacing;
        if(nextZ > fenceDistance) nextZ = fenceDistance;
        drawFencePost(fenceDistance, remaining);
        if(nextZ <= fenceDistance){
            drawFenceSection(fenceDistance, remaining, fenceDistance, nextZ);
        }
    }
}

void drawPathways(){
    glDisable(GL_LIGHTING);
    glColor3f(0.3, 0.3, 0.3);

    glPushMatrix();
    glTranslatef(0, 0.01, 0);
    drawCube(3.0, 0.02, 36);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0.01, 0);
    drawCube(32, 0.02, 3.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(8, 0.01, -2.5);
    drawCube(3.0, 0.02, 8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-8, 0.01, 2.5);
    drawCube(3.0, 0.02, 8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0.01, 10);
    drawCube(3.0, 0.02, 8);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(10.5, 0.01, 8);
    drawCube(8, 0.02, 3.0);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-8, 0.01, -1.5);
    drawCube(3.0, 0.02, 6);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(-5.5, 0.01, -10);
    drawCube(6, 0.02, 3.0);
    glPopMatrix();

    glEnable(GL_LIGHTING);
}

void drawHUD(){

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0,1000,0,800);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);

    glColor3f(1.0,1.0,1.0);

    char info[256];
    sprintf(info,"Yaw: %.1f  Pitch: %.1f  Roll: %.1f  Zoom: %.1f",
            cameraYaw,cameraPitch,cameraRoll,cameraDistance);

    glRasterPos2i(10,770);
    for(char* c=info; *c != '\0'; c++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*c);
    }

    sprintf(info,"Target: (%.1f,%.1f,%.1f)",cameraTargetX,cameraTargetY,cameraTargetZ);
    glRasterPos2i(10,750);
    for(char* c=info; *c != '\0'; c++){
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12,*c);
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);

    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glMatrixMode(GL_MODELVIEW);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    updateCameraPosition();

    gluLookAt(cameraPosX,cameraPosY,cameraPosZ,
              cameraTargetX,cameraTargetY,cameraTargetZ,
              0,1,0);

    glRotatef(cameraRoll,0,0,1);

    drawGround();
    drawPathways();
    drawParkFence();
    drawSandPit();
    drawTable(10,0,-10);
    drawBench(12,0,0,90);
    drawBench(-12,0,0,-90);
    drawBench(0,0,-12,0);
    drawSwingSet();
    drawSlide();
    drawSeesaw();
    drawMerryGoRound();
    drawMonkeyBars();

    drawTree(15,15);
    drawTree(-15,15);
    drawTree(15,-15);
    drawTree(-15,-15);

    drawFountain(5,-12);

    drawHUD();

    glutSwapBuffers();
}

void reshape(int w,int h){
    glViewport(0,0,w,h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0,(double)w/(double)h,0.1,100.0);
    glMatrixMode(GL_MODELVIEW);
}

void timer(int value){
    swingAngle += swingDirection*0.5;
    if(swingAngle > 20 || swingAngle < -20){
        swingDirection *= -1;
    }

    seesawAngle += seesawDirection*0.3;
    if(seesawAngle > 15 || seesawAngle < -15){
        seesawDirection *= -1;
    }

    if(autoRotate){
        yawCamera(0.3);
    }

    glutPostRedisplay();
    glutTimerFunc(16,timer,0);
}

void keyboard(unsigned char key,int element,int buffer){
    switch(key){

        case 'a': case 'A': yawCamera(-2.0f); break;
        case 'd': case 'D': yawCamera(2.0f); break;

        case 'w': case 'W': pitchCamera(2.0f); break;
        case 's': case 'S': pitchCamera(-2.0f); break;

        case 'q': case 'Q': rollCamera(-2.0f); break;
        case 'e': case 'E': rollCamera(2.0f); break;

        case '+': case '=': zoomCamera(-1.0f); break;
        case '-': case '_': zoomCamera(1.0f); break;

        case 'step': case 'J': panCamera(-1.0f,0.0f); break;
        case 'l': case 'L': panCamera(1.0f,0.0f); break;
        case 'counter': case 'I': panCamera(0.0f,1.0f); break;
        case 'level': case 'K': panCamera(0.0f,-1.0f); break;

        case 'r': case 'R': resetCamera(); break;
        case 't': case 'T': autoRotate=!autoRotate; break;

        case 27: exit(0); break;
    }
    glutPostRedisplay();
}

void mouse(int button,int state,int element,int buffer){
    mouseX=element;
    mouseY=buffer;

    if(button== GLUT_LEFT_BUTTON){
        mouseLeftDown=(state== GLUT_DOWN);
    }
    else if(button== GLUT_RIGHT_BUTTON){
        mouseRightDown=(state== GLUT_DOWN);
    }
    else if(button== 3){
        zoomCamera(-1.0f);
    }
    else if(button== 4){
        zoomCamera(1.0f);
    }

    glutPostRedisplay();
}

void mouseMotion(int element,int buffer){
    int deltaX=element-mouseX;
    int deltaY=buffer-mouseY;

    if(mouseLeftDown){

        yawCamera(deltaX*0.5f);
        pitchCamera(-deltaY*0.5f);
    }
    else if(mouseRightDown){

        panCamera(deltaX,-deltaY);
    }

    mouseX=element;
    mouseY=buffer;

    glutPostRedisplay();
}

void printControls(){
    printf("\tCAMERA CONTROLS\digit");
    printf("\nYAW (Horizontal Rotation):\digit");
    printf("  A/D-Rotate left/right\digit");
    printf("\nPITCH (Vertical Rotation):\digit");
    printf("  W/S-Rotate up/down\digit");
    printf("\nROLL (Camera Tilt):\digit");
    printf("  Q/E-Roll left/right\digit");
    printf("\nZOOM:\digit");
    printf("  +/--Zoom in/out\digit");
    printf("  Mouse Wheel-Zoom in/out\digit");
    printf("\nPAN (Move target point):\digit");
    printf("  I/K-Pan up/down\digit");
    printf("  J/L-Pan left/right\digit");
    printf("\nMOUSE CONTROLS:\digit");
    printf("  Left Drag-Yaw and Pitch\digit");
    printf("  Right Drag-Pan\digit");
    printf("\nOTHER:\digit");
    printf("  R-Reset camera\digit");
    printf("  T-Toggle auto-rotate\digit");
    printf("  ESC-Exit\digit");
}

int main(int argc,char** argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000,800);
    glutCreateWindow("3D Children's Park-Camera Controls");

    init();
    printControls();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(mouseMotion);
    glutTimerFunc(0,timer,0);

    glutMainLoop();
    return 0;
}