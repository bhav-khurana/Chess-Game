#include <GL/glut.h>
#include <math.h>
#include "chess.cpp"
#include <string.h>

using namespace std;

// Defining the size of the window
int w = 1366, h = 685, d, offset = 2;

// Declaring the function prototypes
void rectangle(int a, int b, int c, int d);
void triangle(int a, int b, int c, int d, int e, int f);
void circle(int x, int y, int r);
void myinit();
void mreshape(int wx, int hx);
void board_layout();
void skeleton_box(int x, int y);
void clearbox(int x, int y);
void highlight(int x, int y);
void message(char *);
void display();

Chessboard c1(skeleton_box, clearbox, highlight, display, message);

void myinit()
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, 0, h);

	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_POINT_SMOOTH);
}

void mreshape(int wx, int hx)
{
	// Check if the window size matches the desired size
	if (wx == w && hx == h)
	{
		board_layout();
		c1.redisplay();
		return;
	}

	// If the window size doesn't match, set it to the desired size
	if (wx != w || hx != h)
		glutReshapeWindow(w, h);
}

// Function to draw an unfilled rectangle border
void rect_box(int a, int b, int c, int d);

// Function to define the message box space
void message_box()
{
	glColor3f(0, 0, 0);
	rectangle(750, 400, 1300, 650);
	glColor3f(1, 1, 1);
	rect_box(750, 400, 1300, 650);
}

// Function to display a message in the message box after clearing it
void message(char *msg)
{
	message_box();

	glRasterPos2f(770, 600);
	glColor3f(1, 1, 1);
	for (int i = 0; i < strlen(msg); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, msg[i]);
}

// Function to display a King at position (x, y) with a specified color
void king(int x, int y, int color)
{
	if (color == 0)
		glColor3ub(57, 94, 144); // Blue color for white King
	else
		glColor3ub(30, 5, 34); // Dark color for black King

	// Calculate the center for the king
	x = x * d + offset + d / 2;
	y = y * d + offset + d / 2;

	glLineWidth(2);
	glPointSize(1);

	// Draw the king piece
	rectangle(x - d / 14.285, y + d / 2.702, x + d / 14.285, y + d / 3.125);
	rectangle(x - d / 33.33, y + d / 2.439, x + d / 50, y + d / 3.33);
	circle(x, y + d / 5, d / 5.555);
	rectangle(x - d / 25, y + d / 10, x + d / 25, y + d / 20);
	rectangle(x - d / 11.11, y + d / 20, x + d / 11.11, y);
	rectangle(x - d / 16.667, y, x + d / 16.667, y - d / 4.167);
	rectangle(x - d / 11.11, y - d / 4.167, x + d / 11.11, y - d / 3.448);
	rectangle(x - d / 9.09, y - d / 3.448, x + d / 9.09, y - d / 2.564);
}

// Function to display a Knight at position (x, y) with a specified color
void knight(int x, int y, int color)
{
	if (color == 0)
		glColor3ub(57, 94, 144); // Blue color for white Knight
	else
		glColor3ub(30, 5, 34); // Dark color for black Knight

	// Calculate the center of the piece
	x = x * d + offset + d / 2;
	y = y * d + offset + d / 2;

	glLineWidth(2);
	glPointSize(1);
	glBegin(GL_LINES);

	// Draw the piece
	glVertex2i(x, y + d / 4.347);
	glVertex2i(x - d / 9.09, y + d / 5);
	glVertex2i(x - d / 9.09, y + d / 5);
	glVertex2i(x - d / 14.285, y - d / 14.285);
	glVertex2i(x, y + d / 4.3478);
	glVertex2i(x + d / 4, y + d / 14.285);
	glVertex2i(x + d / 4, y + d / 14.285);
	glVertex2i(x + d / 5, y);
	glVertex2i(x + d / 5, y);
	glVertex2i(x + d / 12.5, y + d / 25);
	glVertex2i(x + d / 12.5, y + d / 25);
	glVertex2i(x + d / 16.667, y - d / 14.285);
	glEnd();
	glLineWidth(1);
	rectangle(x - d / 16.667, y - d / 14.285, x + d / 16.667, y - d / 4.167);
	rectangle(x - d / 11.11, y - d / 4.167, x + d / 11.11, y - d / 3.448);
	rectangle(x - d / 9.09, y - d / 3.448, x + d / 9.09, y - d / 2.564);
}

// Function to display a Queen at position (x, y) with a specified color
void queen(int x, int y, int color)
{
	if (color == 0)
		glColor3ub(57, 94, 144); // Blue color for white Queen
	else
		glColor3ub(30, 5, 34); // Dark color for black Queen

	x = x * d + offset + d / 2;
	y = y * d + offset + d / 2;

	glLineWidth(2);
	glPointSize(1);

	triangle(x - d / 25, y + d / 2.778, x + d / 25, y + d / 2.778, x, y + d / 2.439);
	rectangle(x - d / 11.11, y + d / 2.778, x + d / 11.11, y + d / 3.333);
	circle(x, y + d / 5, d / 5.555);
	rectangle(x - d / 25, y + d / 10, x + d / 25, y + d / 20);
	rectangle(x - d / 11.11, y + d / 20, x + d / 11.11, y);
	rectangle(x - d / 16.667, y, x + d / 16.667, y - d / 4.166);
	rectangle(x - d / 11.11, y - d / 4.167, x + d / 11.111, y - d / 3.448);
	rectangle(x - d / 9.09, y - d / 3.448, x + d / 9.09, y - d / 2.564);
}

// Function to display a Bishop at position (x, y) with a specified color
void bishop(int x, int y, int color)
{
	if (color == 0)
		glColor3ub(57, 94, 144); // Blue color for white Bishop
	else
		glColor3ub(30, 5, 34); // Dark color for black Bishop

	x = x * d + offset + d / 2;
	y = y * d + offset + d / 2;

	glLineWidth(1);
	glPointSize(1);

	circle(x, y + d / 3.33, d / 7.692);
	circle(x, y + d / 6.667, d / 5.555);
	rectangle(x - d / 25, y + d / 20, x + d / 25, y - d / 50);
	rectangle(x - d / 11.11, y - d / 50, x + d / 11.11, y - d / 14.285);
	rectangle(x - d / 16.667, y - d / 14.285, x + d / 16.667, y - d / 4.167);
	rectangle(x - d / 11.11, y - d / 4.167, x + d / 11.11, y - d / 3.448);
	rectangle(x - d / 9.09, y - d / 3.448, x + d / 9.09, y - d / 2.564);
}

// Function to display a Rook at position (x, y) with a specified color
// (x, y) is the center of the Rook
void rook(int x, int y, int color)
{
	if (color == 0)
		glColor3ub(57, 94, 144); // Blue color for white Rook
	else
		glColor3ub(30, 5, 34); // Dark color for black Rook

	x = x * d + offset + d / 2;
	y = y * d + offset + d / 2;

	glLineWidth(2);
	glPointSize(1);

	rectangle(x - d / 8.333, y + d / 5, x + d / 8.333, y + d / 20);
	rectangle(x - d / 11.11, y + d / 20, x + d / 11.11, y - d / 3.448);
	rectangle(x - d / 11.11, y - d / 4.167, x + d / 11.11, y - d / 3.448);
	rectangle(x - d / 9.09, y - d / 3.448, x + d / 9.09, y - d / 2.564);
}

// Function to display a Pawn at position (x, y) with a specified color
void pawn(int x, int y, int color)
{
	if (color == 0)
		glColor3ub(57, 94, 144); // Blue color for white Pawn
	else
		glColor3ub(30, 5, 34); // Dark color for black Pawn

	x = x * d + offset + d / 2;
	y = y * d + offset + d / 2;

	glLineWidth(2);
	glPointSize(1);

	circle(x, y + d / 20, d / 5);
	rectangle(x - d / 16.667, y - d / 14.285, x + d / 16.667, y - d / 4.167);
	rectangle(x - d / 11.11, y - d / 4.167, x + d / 11.11, y - d / 3.448);
	rectangle(x - d / 9.09, y - d / 3.448, x + d / 9.09, y - d / 2.564);
}

// Utility functions to draw the shapes

// Display an unfilled triangle with vertices at (a, b), (c, d), and (e, f)
void triangle(int a, int b, int c, int d, int e, int f)
{
	glLineWidth(2);
	glBegin(GL_LINE_LOOP);
	glVertex2i(a, b);
	glVertex2i(c, d);
	glVertex2i(e, f);
	glEnd();
}

// Display a filled rectangle with corners at (a, b) and (c, d)
void rectangle(int a, int b, int c, int d)
{
	glBegin(GL_POLYGON);
	glVertex2i(a, b);
	glVertex2i(c, b);
	glVertex2i(c, d);
	glVertex2i(a, d);
	glEnd();
}

// Display a filled circle with center at (x, y) and radius 'r'
void circle(int x, int y, int r)
{
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glPointSize(r);
	glBegin(GL_POINTS);
	glVertex2f(x, y);
	glEnd();
}

void board_layout()
{
	glLineWidth(2);
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			clearbox(i, j);
}

// Display an unfilled rectangle with corners at (a, b) and (c, d)
void rect_box(int a, int b, int c, int d)
{
	glBegin(GL_LINE_LOOP);
	glVertex2i(a, b);
	glVertex2i(c, b);
	glVertex2i(c, d);
	glVertex2i(a, d);
	glEnd();
}

void skeleton_box(int x, int y)
{
	glColor3f(0.6078, 0.3019, 0.0745);
	glLineWidth(3);
	rect_box(x * d + offset, y * d + offset, (x + 1) * d + offset, (y + 1) * d + offset);
}

void clearbox(int x, int y)
{
	if ((x + y) % 2 == 0)
		glColor3ub(195, 127, 10);
	else
		glColor3ub(255, 255, 255);

	glBegin(GL_POLYGON);
	glVertex2i(x * d + offset, y * d + offset);
	glVertex2i(x * d + offset, (y + 1) * d + offset);
	glVertex2i((x + 1) * d + offset, (y + 1) * d + offset);
	glVertex2i((x + 1) * d + offset, y * d + offset);
	glEnd();

	skeleton_box(x, y);
}

// Highlight the box at (x, y) with a specific color
// (x, y) is the position of the box to highlight
void highlight(int x, int y)
{
	glColor3ub(30, 144, 255); // Highlight color
	glLineWidth(3);
	rect_box(x * d + offset, y * d + offset, (x + 1) * d + offset, (y + 1) * d + offset);
}

void display()
{
	glFlush();
}

// Initialize the Chessboard layout and chess engine
void initboard()
{
	d = h;
	if (w < d)
		d = w;
	d = d / 8;
	board_layout();

	c1.setKing(king);
	c1.setQueen(queen);
	c1.setBishop(bishop);
	c1.setRook(rook);
	c1.setKnight(knight);
	c1.setPawn(pawn);
}

int m = 0;

// Enable/disable mouse control for piece selection
void reset(int v)
{
	m = 0;
}

void mouse(int b, int s, int x, int y)
{
	if (m == 1)
		return; // Mouse control already in progress

	m = 1;
	y = h - y; // Adjust the y-coordinate to match the coordinate system
	if (b != GLUT_LEFT_BUTTON || s != GLUT_DOWN)
		return; // Only handle left mouse button press events

	if (x > (8 * d + offset) || y > (8 * d + offset))
		return; // Clicked outside the Chessboard

	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			if (j * d + offset < x && i * d + offset < y && (j + 1) * d + offset >= x && (i + 1) * d + offset >= y)
			{
				x = j;
				y = i;
			}

	c1.select(x, y);			  // Handle piece selection
	glutTimerFunc(185, reset, 0); // Re-enable mouse control after 185ms
}

void mainmenu(int id)
{
	if (id == 1)
		c1.undo_move(); // Handle undo move option from the menu
}

void initmenu()
{
	// Create a right-click menu with an "UNDO" option
	glutCreateMenu(mainmenu);
	glutAddMenuEntry("UNDO", 1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(w, h);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("CHESS");

	myinit();
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	initboard();
	initmenu();
	message_box();

	glutDisplayFunc(display);
	glutReshapeFunc(mreshape);
	glutMouseFunc(mouse);

	glutMainLoop();
	return 0;
}