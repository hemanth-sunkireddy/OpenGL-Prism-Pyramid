#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <random>

/* Global variables */
char title[] = "3D Shapes";
GLfloat angle = 0.0f; // Initial rotation angle
int sides;            // Number of sides

float Xaxis_angle = 0.0;
float Yaxis_angle = 0.0;
float Zaxis_angle = 0.0;
float second_axis_motion = 0.0;
float first_axis_motion = 0.0;
float third_axis_motion = 0.0;

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

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    glMatrixMode(GL_MODELVIEW);                         // To operate on model-view matrix

    glLoadIdentity();                   // Reset the model-view matrix
    glTranslatef(0.0f, 0.0f, -5.0f);    // Move into the screen
    glRotatef(angle, 1.0f, 1.0f, 1.0f); // Rotate the prism
                                        // To change the angle of the prism.
    gluLookAt(3.0, 2.0, 3.0,  // Eye position (x, y, z)
          0.0, 0.0, 0.0,  // Center of the view (look-at point)
          2.0, 1.0, 2.0);

    glBegin(GL_TRIANGLES); // Begin drawing the prism with triangles
    for (int i = 0; i < sides; ++i)
    {
        // Calculate vertices for each triangular face
        GLfloat x1 = sin(2 * M_PI * i / sides);
        GLfloat y1 = cos(2 * M_PI * i / sides);

        GLfloat x2 = sin(2 * M_PI * (i + 1) / sides);
        GLfloat y2 = cos(2 * M_PI * (i + 1) / sides);

        // Generate random colors for each face
        GLfloat colorR = static_cast<GLfloat>(rand()) / RAND_MAX;
        GLfloat colorG = static_cast<GLfloat>(rand()) / RAND_MAX;
        GLfloat colorB = static_cast<GLfloat>(rand()) / RAND_MAX;

        // Front face
        // Front face
        glColor3f(0.5f, 0.4f, 0.3f);
        glVertex3f(second_axis_motion, first_axis_motion, third_axis_motion);
        glVertex3f(x1 + second_axis_motion, y1 + first_axis_motion, third_axis_motion);
        glVertex3f(x2 + second_axis_motion, y2 + first_axis_motion, third_axis_motion);

        // Back face
        glVertex3f(second_axis_motion, first_axis_motion, -1.0f + third_axis_motion);
        glVertex3f(x2 + second_axis_motion, y2 + first_axis_motion, -1.0f + third_axis_motion);
        glVertex3f(x1 + second_axis_motion, y1 + first_axis_motion, -1.0f + third_axis_motion);

        // Connect opposite vertices with cylindrical structure
        GLfloat z = 0.0f; // Z-coordinate for the cylindrical structure
        for (int j = 0; j <= 1; ++j)
        {
            // Different colors for each cylindrical structure
            glColor3f(colorR, colorG, colorB);
            glVertex3f(x1 + second_axis_motion, y1 + first_axis_motion, z + third_axis_motion);
            glVertex3f(x2 + second_axis_motion, y2 + first_axis_motion, z + third_axis_motion);
            glVertex3f(x2 + second_axis_motion, y2 + first_axis_motion, z - 1.0f + third_axis_motion);

            glVertex3f(x1 + second_axis_motion, y1 + first_axis_motion, z + third_axis_motion);
            glVertex3f(x2 + second_axis_motion, y2 + first_axis_motion, z - 1.0f + third_axis_motion);
            glVertex3f(x1 + second_axis_motion, y1 + first_axis_motion, z - 1.0f + third_axis_motion);

            z = -1.0f; // Update Z-coordinate for the back face
        }
    }
    glEnd(); // End of drawing the prism

    glutSwapBuffers(); // Swap the front and back frame buffers (double buffering)
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshape(GLsizei width, GLsizei height)
{ // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0)
        height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity();            // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Timer function to update the rotation angle */
void update(int value)
{
    angle += 2.0f; // Increment the rotation angle
    if (angle > 360)
    {
        angle -= 360;
    }
    glutPostRedisplay();          // Request a redraw
    glutTimerFunc(16, update, 0); // Call update again after 16 milliseconds
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'p':
        first_axis_motion += 0.5;
        break;
    case 'o':
        first_axis_motion -= 0.5;
        break;
    case 'i':
        second_axis_motion -= 0.5;
        break;
    case 'u':
        second_axis_motion += 0.5;
        break;
    case 'l':
        third_axis_motion -= 0.5;
        break;
    case 'k':
        third_axis_motion += 0.5;
        break;
    }
    glutPostRedisplay(); // Change this
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <number_of_sides>\n";
        return -1;
    }

    // Convert the command-line argument to an integer
    sides = std::atoi(argv[1]);

    if (sides < 3)
    {
        std::cerr << "Error: Number of sides should be at least 3.\n";
        return -1;
    }

    glutInit(&argc, argv);                         // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); // Enable double buffered mode and depth testing
    glutInitWindowSize(640, 480);                  // Set the window's initial width & height
    glutInitWindowPosition(50, 50);                // Position the window's initial top-left corner
    glutCreateWindow(title);                       // Create window with the given title
    glutDisplayFunc(display);                      // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);                      // Register callback handler for window re-size event
    glutKeyboardFunc(keyboard);
    initGL();       // Our own OpenGL initialization
    glutMainLoop(); // Enter the infinite event-processing loop/
    return 0;
}
