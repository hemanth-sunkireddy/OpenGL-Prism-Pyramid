#include <GL/glut.h>
#include <iostream>
#include <cmath>    

/* Global variables */
char title[] = "3D Shapes";
GLfloat angle = 0.5f;  // Initial rotation angle
int sides = 3; // Default number of sides

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);               // Set the type of depth-test
    glShadeModel(GL_SMOOTH);              // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}


void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);



    glBegin(GL_TRIANGLES);

    // Base face
    glColor3f(0.3f, 0.5f, 0.4f); // Random color
    glVertex3f(0.0f, -1.0f, 0.0f);  // Center of the base
    glVertex3f(cos(2.0 * M_PI * 0 / sides), -1.0f, sin(2.0 * M_PI * 0 / sides));
    glVertex3f(cos(2.0 * M_PI * 1 / sides), -1.0f, sin(2.0 * M_PI * 1 / sides));

    // Side faces
    for (int i = 0; i < sides; ++i) {
        // Front face of the pyramid
        if ( i %2 == 0)
            glColor3f((GLfloat)i / sides, 0.0f, 1.0f - (GLfloat)i / sides);
        else
            glColor3f(0.9f, 0.9f, 0.1f);
        glVertex3f(0.0f, 1.0f, 0.0f);  // Apex
        glVertex3f(cos(2.0 * M_PI * i / sides), -1.0f, sin(2.0 * M_PI * i / sides));
        glVertex3f(cos(2.0 * M_PI * (i + 1) / sides), -1.0f, sin(2.0 * M_PI * (i + 1) / sides));
    }

    glEnd();

    glutSwapBuffers();
}




/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number_of_sides>\n";
        return -1;
    }

    // Convert the command-line argument to an integer
    sides = std::atoi(argv[1]);

    if (sides < 3) {
        std::cerr << "Error: Number of sides should be at least 3.\n";
        return -1;
    }

    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); // Enable double buffered mode and depth testing
    glutInitWindowSize(640, 480);     // Set the window's initial width & height
    glutInitWindowPosition(50, 50);    // Position the window's initial top-left corner
    glutCreateWindow(title);          // Create window with the given title
    glutDisplayFunc(display);         // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);         // Register callback handler for window re-size event
    initGL();                         // Our own OpenGL initialization
    glutMainLoop();                   // Enter the infinite event-processing loop
    return 0;
}
