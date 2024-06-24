#include <GL/glut.h>
#include <cmath>
#include <iostream>

/* Global variables */
char title[] = "3D Shapes";
GLfloat angle = 0.0f;  // Initial rotation angle
int sides = 3; // Default number of sides

int toggle_shape = 0;


/* Initialize OpenGL Graphics */
void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);              // Set background color to black and opaque
    glClearDepth(1.0f);                                // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);                           // Enable depth testing for z-culling
    glDepthFunc(GL_LEQUAL);                            // Set the type of depth-test
    glShadeModel(GL_SMOOTH);                           // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Nice perspective corrections
}

void prismDraw(int sides)
{
    glBegin(GL_TRIANGLES); // Begin drawing the prism with triangles
    for (int i = 0; i < sides; ++i)
    {
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
        for (int j = 0; j <= 1; ++j)
        {
            if(j==0)
            glColor3f(0.9f, 0.1f, 0.2f); 
            else
                glColor3f(0.2f, 0.4f, 0.3f); 
            glVertex3f(x1, y1, z);
            glVertex3f(x2, y2, z);
            glVertex3f(x2, y2, z - 1.0f);

            glVertex3f(x1, y1, z);
            glVertex3f(x2, y2, z - 1.0f);
            glVertex3f(x1, y1, z - 1.0f);

            z = -1.0f; // Update Z-coordinate for the back face
        }
    }
}

void pyramidDraw(int sides){
    glBegin(GL_TRIANGLES);            // Begin drawing the pyramid with triangles
    // Base face
    glColor3f(1.0f, 0.0f, 0.0f); // Red
    for (int i = 0; i < sides; ++i) {
        glVertex3f(cos(2.0 * M_PI * i / sides), -1.0f, sin(2.0 * M_PI * i / sides));
        glVertex3f(cos(2.0 * M_PI * (i + 1) / sides), -1.0f, sin(2.0 * M_PI * (i + 1) / sides));
        glVertex3f(0.0f, -1.0f, 0.0f);  // Center of the base
    }

    // Side faces
    for (int i = 0; i < sides; ++i) {
        glColor3f(0.0f, 1.0f, 0.0f); // Green
        glVertex3f(0.0f, 1.0f, 0.0f);  // Apex
        glVertex3f(cos(2.0 * M_PI * i / sides), -1.0f, sin(2.0 * M_PI * i / sides));
        glVertex3f(cos(2.0 * M_PI * (i + 1) / sides), -1.0f, sin(2.0 * M_PI * (i + 1) / sides));
    }
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix

    glLoadIdentity();                 // Reset the model-view matrix
    glTranslatef(0.0f, 0.0f, -5.0f);  // Move into the screen
    
    // To change the angle of the prism.
    gluLookAt(0.0, 0.0, 0.0, 2.0, 2.0, 2.0, 0.0, 1.0, 0.0);

    if(toggle_shape == 1)
        pyramidDraw(sides);
    else
        prismDraw(sides);

    glEnd();  // End of drawing the pyramid

    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)

}


void keyboard(unsigned char key, int x, int y) {
    switch (key) {
        case 't':
            if(toggle_shape == 1)
            {
                toggle_shape = 0;
            }
            else if(toggle_shape == 0)
            {
                toggle_shape = 1;
            }
            break;
    }
    glutPostRedisplay();    // Change this
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
    glutKeyboardFunc(keyboard);
    initGL();                         // Our own OpenGL initialization
    glutMainLoop();                   // Enter the infinite event-processing loop
    return 0;
}
