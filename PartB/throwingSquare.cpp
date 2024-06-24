#include <GL/glut.h>
#include <math.h>
#include <iostream>

/* Global variables */
char title[] = "3D Shapes";
GLfloat angle = 0.0f;  // Initial rotation angle
int sides = 4;

/* Initialize OpenGL Graphics */
void initGL() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
    glClearDepth(1.0f);                   // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);               // Set the type of depth-test
    glShadeModel(GL_SMOOTH);              // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections
}

/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

    glLoadIdentity();                 // Reset the model-view matrix
    glTranslatef(0.0f, 0.0f, -5.0f);  // Move into the screen
    glRotatef(angle, 1.0f, 1.0f, 1.0f);  // Rotate the prism

     // To change the angle of the prism.
    gluLookAt(3.0, 3.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glBegin(GL_TRIANGLES);  // Begin drawing the prism with triangles
    for (int i = 0; i < sides; ++i) {
        // Calculate vertices for each triangular face
        GLfloat x1 = sin(2 * M_PI * i / sides);
        GLfloat y1 = cos(2 * M_PI * i / sides);

        GLfloat x2 = sin(2 * M_PI * (i + 1) / sides);
        GLfloat y2 = cos(2 * M_PI * (i + 1) / sides);

        // Front face
        glColor3f(1.0f, 0.0f, 0.0f); // Red
        glVertex3f(0.0f, 0.0f, 0.0f);
        glVertex3f(x1, y1, 0.0f);
        glVertex3f(x2, y2, 0.0f);

        // Back face
        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glVertex3f(0.0f, 0.0f, -1.0f);
        glVertex3f(x2, y2, -1.0f);
        glVertex3f(x1, y1, -1.0f);

        // Connect opposite vertices with cylindrical structure
        GLfloat z = 0.0f; // Z-coordinate for the cylindrical structure
        for (int j = 0; j <= 1; ++j) {
            glColor3f(0.0f, 0.0f, 1.0f); // Blue for the cylindrical structure
            glVertex3f(x1, y1, z);
            glVertex3f(x2, y2, z);
            glVertex3f(x2, y2, z - 1.0f);

            glVertex3f(x1, y1, z);
            glVertex3f(x2, y2, z - 1.0f);
            glVertex3f(x1, y1, z - 1.0f);

            z = -1.0f; // Update Z-coordinate for the back face
        }
    }

    glEnd();  // End of drawing the prism

    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)
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

void update(int value) {
    int windowWidth = glutGet(GLUT_WINDOW_WIDTH);

    angle += 2.0f;  // Increment the rotation angle
    if (angle > 45) {
        angle -= 90;
    }
    glutPostRedisplay();  // Request a redraw
    glutTimerFunc(16, update, 0);  // Call update again after 16 milliseconds
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv) {
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); // Enable double buffered mode and depth testing
    glutInitWindowSize(640, 480);     // Set the window's initial width & height
    glutInitWindowPosition(50, 50);    // Position the window's initial top-left corner
    glutCreateWindow(title);          // Create window with the given title
    glutDisplayFunc(display);         // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);         // Register callback handler for window re-size event
    initGL();                         // Our own OpenGL initialization
    glutTimerFunc(25, update, 0);     // Set up a timer-based animation
    glutMainLoop();                   // Enter the infinite event-processing loop
    return 0;
}
