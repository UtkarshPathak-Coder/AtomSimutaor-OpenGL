#include <cmath>
#include <vector>
#include <iostream>
#include <GL/glut.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int M_PI = 3.1418;
const double ELECTRON_ORBIT_RADIUS[] = { 0.5, 0.8, 1.1 };
const int ORBITAL_ENERGY[] = { 2, 8 };
const int MAX_ELECTRONS = 10;
//const int ORBITAL_ENERGY[] = { 2, 4, 4 };


struct Element {
    std::string name;
    int atomicNumber;
    int numProtons;
    int numNeutrons;
};

std::vector<Element> elements = {
    {"Hydrogen", 1, 1, 0},
    {"Helium", 2, 2, 2},
    {"Lithium", 3, 3, 4},
    {"Beryllium", 4, 4, 5},
    {"Boron", 5, 5, 6},
    {"Carbon", 6, 6, 6},
    {"Nitrogen", 7, 7, 7},
    {"Oxygen", 8, 8, 8},
    {"Fluorine", 9, 9, 10},
    {"Neon", 10, 10, 10}
};

int currentAtomicNumber = 1;
int lastMouseX = 0;
int lastMouseY = 0;
float cameraRotationX = 0.0f;
float cameraRotationY = 0.0f;

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
}

void drawNucleus(int numProtons, int numNeutrons) {
    glColor3f(0.5f, 0.5f, 0.5f);

    glColor3f(1.0f, 0.0f, 0.0f);
    for (int i = 0; i < numProtons; i++) {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);
        glutSolidSphere(0.2, 10, 10);
        glPopMatrix();
    }

    glColor3f(0.0f, 0.0f, 1.0f);
    for (int i = 0; i < numNeutrons; i++) {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, 0.0f);
        glutSolidSphere(0.2, 10, 10);
        glPopMatrix();
    }
}

void drawElectrons(int atomicNumber) {
    glColor3f(0.7f, 0.7f, 0.7f); // Set a light shade for the orbits

    int numElectrons = atomicNumber <= MAX_ELECTRONS ? atomicNumber : MAX_ELECTRONS;
    int numOrbits = sizeof(ELECTRON_ORBIT_RADIUS) / sizeof(ELECTRON_ORBIT_RADIUS[0]);

    int electronCount = 0;

    for (int i = 0; i < numOrbits; i++) {
        int maxElectronsInOrbit = ORBITAL_ENERGY[i];
        double orbitRadius = ELECTRON_ORBIT_RADIUS[i];
        double orbitSpeed = 0.001 * (i  + 1);

        glPushMatrix();
        glRotatef(cameraRotationX, 1.0f, 0.0f, 0.0f); // Apply camera rotation to orbits
        glRotatef(cameraRotationY, 0.0f, 1.0f, 0.0f);
        glColor3f(0.7f, 0.7f, 0.7f);
        glBegin(GL_LINE_LOOP); // Draw orbit path as a line loop
        for (int j = 0; j < 360; j++) {
            double angle = j * M_PI / 180.0;
            double x = orbitRadius * cos(angle);
            double y = orbitRadius * sin(angle);
            double z = 0.0f;
            glVertex3f(x, y, z);
        }
        glEnd();
        glPopMatrix();

        glColor3f(0.0f, 0.0f, 0.0f); // Set color for electrons

        for (int j = 0; j < maxElectronsInOrbit; j++) {
            if (electronCount >= numElectrons)
                break;

            double angle = j * 2 *M_PI / maxElectronsInOrbit;
            double x = orbitRadius * cos(angle + glutGet(GLUT_ELAPSED_TIME) * orbitSpeed);
            double y = orbitRadius * sin(angle + glutGet(GLUT_ELAPSED_TIME) * orbitSpeed);
            double z = 0.0f;

            glPushMatrix();
            glRotatef(cameraRotationX, 1.0f, 0.0f, 0.0f); // Apply camera rotation to electrons
            glRotatef(cameraRotationY, 0.0f, 1.0f, 0.0f);
            glTranslatef(x, y, z);
            glutSolidSphere(0.05, 10, 10);
            glPopMatrix();

            electronCount++;
        }
    }
}






void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glTranslatef(0.0f, 0.0f, -5.0f);
    glRotatef(cameraRotationX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraRotationY, 0.0f, 1.0f, 0.0f);

    drawNucleus(elements[currentAtomicNumber - 1].numProtons, elements[currentAtomicNumber - 1].numNeutrons);
    drawElectrons(currentAtomicNumber);

    glFlush();
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)width / (float)height, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        lastMouseX = x;
        lastMouseY = y;
    }
}

void motion(int x, int y) {
    int deltaX = x - lastMouseX;
    int deltaY = y - lastMouseY;

    cameraRotationX += deltaY * 0.2f;
    cameraRotationY += deltaX * 0.2f;

    lastMouseX = x;
    lastMouseY = y;

    glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y) {
    if (key >= '1' && key <= '9') {
        int atomicNum = key - '0';
        if (atomicNum <= elements.size()) {
            currentAtomicNumber = atomicNum;
            std::cout << "Selected Element: " << elements[currentAtomicNumber - 1].name << std::endl;
            glutPostRedisplay();
        }
    }
}

//void setupBackground() {
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//}

void setupBackground() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -10.0, 10.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void drawBackground() {
    glBegin(GL_QUADS);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 0.0f);
    glVertex3f(1.0f, -1.0f, 0.0f);
    glColor3f(0.8f, 0.8f, 0.8f);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glVertex3f(-1.0f, 1.0f, 0.0f);
    glEnd();
}


//void drawBackground() {
//    glBegin(GL_QUADS);
//    glColor3f(1.0f, 1.0f, 1.0f);
//    glVertex2f(0.0f, 0.0f);
//    glVertex2f(1.0f, 0.0f);
//    glColor3f(0.8f, 0.8f, 0.8f);
//    glVertex2f(1.0f, 1.0f);
//    glVertex2f(0.0f, 1.0f);
//    glEnd();
//}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    glutCreateWindow("Atom Simulation");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutKeyboardFunc(keyboard);

    setupBackground();
    glutIdleFunc(display);

    glutMainLoop();
   // return 0;
}