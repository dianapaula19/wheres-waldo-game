#include <GL/freeglut.h> // nu trebuie uitat freeglut.h (sau glut.h sau gl.h & glu.h)
#define LEG_W 22
#define LEG_H 94
#define ARM_W 16
#define ARM_H 72
#define HEAD 37
#define LEG_SPACE 7
#define ARM_SPACE 8
#define HEAD_SPACE_H 6
#define HEAD_SPACE_W 7

struct RGB {
	// the values for R, G and B go from 0 to 255
	double R, G, B;
	public:
		RGB(double R, double G, double B) {
			this->R = R/255.0;
			this->G = G/255.0;
			this->B = B/255.0;
		}
};

struct Coord {
	GLint x1, y1, x2, y2;
	public:
		Coord(GLint x1, GLint y1, GLint x2, GLint y2) {
			this->x1 = x1;
			this->y1 = y1;
			this->x2 = x2;
			this->y2 = y2;
		}
};

namespace Colors {

	RGB FACE = RGB(253, 224, 194);
	RGB GRASS = RGB(202, 220, 92);
	RGB SKY = RGB(133, 206, 203);
	RGB PURPLE = RGB(170, 108, 171);
	RGB YELLOW = RGB(253, 242, 126);
	RGB DARK_BLUE = RGB(101, 117, 143);
	RGB BLUE = RGB(100, 141, 197);
	RGB LIGHT_BLUE_ONE = RGB(144, 179, 173);
	RGB LIGHT_BLUE_TWO = RGB(143, 211, 220);
	RGB RED = RGB(216, 35, 42);

}

Coord GRASS_COORD = Coord(0, 0, 1200, 500);

namespace Draw {

	void person(Coord person, RGB shirt_color, RGB pants_color) {
		
		glColor3f(pants_color.R, pants_color.G, pants_color.B);
		
		// drawing the legs
		glRecti(person.x1 + ARM_W + ARM_SPACE, person.y1, person.x1 + ARM_W + ARM_SPACE + LEG_W, person.y1 + LEG_H); // left leg
		glRecti(person.x1 + ARM_W + ARM_SPACE + LEG_W + LEG_SPACE, person.y1, person.x1 + ARM_W + ARM_SPACE + LEG_W * 2 + LEG_SPACE, person.y1 + LEG_H); // right leg
		
		glColor3f(shirt_color.R, shirt_color.G, shirt_color.B);
		// drawing the upper body
		glRecti(person.x1 + ARM_W + ARM_SPACE, person.y1 + LEG_H, person.x1 + ARM_W + ARM_SPACE + LEG_W * 2 + LEG_SPACE, person.y1 + LEG_H + ARM_H);
		
		// drawing the arms
		glRecti(person.x1, person.y1 + LEG_H, person.x1 + ARM_W, person.y1 + LEG_H + ARM_H); // left arm
		glRecti(person.x1 + ARM_W + ARM_SPACE * 2 + LEG_W * 2 + LEG_SPACE, person.y1 + LEG_H, person.x1 + ARM_W * 2 + ARM_SPACE * 2 + LEG_W * 2 + LEG_SPACE, person.y1 + LEG_H + ARM_H); // right arm

		// drawing the head

		glColor3f(Colors::FACE.R, Colors::FACE.G, Colors::FACE.B);
		glRecti(person.x1 + ARM_W + ARM_SPACE + HEAD_SPACE_W, person.y1 + LEG_H + ARM_H + HEAD_SPACE_H, person.x1 + ARM_W + ARM_SPACE + HEAD_SPACE_W + HEAD, person.y1 + LEG_H + ARM_H + HEAD_SPACE_H + HEAD);

	}

	void grass() {
		glColor3f(Colors::GRASS.R, Colors::GRASS.G, Colors::GRASS.B);
		glRecti(GRASS_COORD.x1, GRASS_COORD.y1, GRASS_COORD.x2, GRASS_COORD.y2);
	}
}

void init (void) {
	glClearColor(Colors::SKY.R, Colors::SKY.G, Colors::SKY.B, 0.0); // precizeaza culoarea de fond a ferestrei de vizualizare
    glMatrixMode(GL_PROJECTION);  // se precizeaza este vorba de o reprezentare 2D, realizata prin proiectie ortogonala
	gluOrtho2D(0.0, 1200.0, 0.0, 900.0); // sunt indicate coordonatele extreme ale ferestrei de vizualizare
}
void desen (void) {

	Draw::grass();
	Draw::person(Coord(100, 200, 0, 0), Colors::YELLOW, Colors::PURPLE);
	Draw::person(Coord(300, 200, 0, 0), Colors::BLUE, Colors::DARK_BLUE);
	Draw::person(Coord(500, 200, 0, 0), Colors::LIGHT_BLUE_ONE, Colors::DARK_BLUE);
	Draw::person(Coord(700, 200, 0, 0), Colors::YELLOW, Colors::PURPLE);
	Draw::person(Coord(900, 200, 0, 0), Colors::LIGHT_BLUE_TWO, Colors::DARK_BLUE);
	glFlush();
	

}
int main (int argc, char** argv) {

	glutInit (&argc, argv); // initializare GLUT
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // se utilizeaza un singur buffer | modul de colorare RedGreenBlue (= default)
	glutInitWindowPosition (100, 100); // pozitia initiala a ferestrei de vizualizare (in coordonate ecran)
	glutInitWindowSize(1200, 900); // dimensiunile ferestrei 
	glutCreateWindow("Where's Wally: Social Distancing Edition"); // creeaza fereastra, indicand numele ferestrei de vizualizare - apare in partea superioara

	init(); // executa procedura de initializare
	glClear(GL_COLOR_BUFFER_BIT); // reprezentare si colorare fereastra de vizualizare
	glutDisplayFunc(desen); // procedura desen este invocata ori de cate ori este nevoie
	glutMainLoop(); // ultima instructiune a programului, asteapta (eventuale) noi date de intrare
    
	return 0;
}