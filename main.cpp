#include <GL/freeglut.h> // nu trebuie uitat freeglut.h (sau glut.h sau gl.h & glu.h)
//#include <bits/stdc++.h>
#include <iostream>
#include <cmath>
#define LEG_W 22
#define LEG_H 94
#define ARM_W 16
#define ARM_H 72
#define HEAD 37
#define LEG_SPACE 7
#define ARM_SPACE 8
#define HEAD_SPACE_H 6
#define HEAD_SPACE_W 7
#define MASK_H 15
#define MASK_W 29
#define Waldo_x 750
#define Waldo_y 200
#define WINDOW_H 800
#define WINDOW_W 1200

using namespace std;

const double TWO_PI = 6.2831853;
GLuint clouds_list;
float translate_clouds = 0.0;
float alpha_clouds = 0.5;
float rotate_wave_left = 0.0;
float alpha_wave_left = 0.1;
float rotate_wave_right = 0.0;
float alpha_wave_right = 0.1;
float alpha_cloud = 0.1;
float text_scale = 1.0f;
bool found = 0;
int lives = 3;


class scrPt {
	public:
		GLint x, y;
};

struct RGB_color {
	// the values for R, G and B go from 0 to 255
	double R, G, B;
	public:
		RGB_color(double R, double G, double B) {
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

struct Point {
	double x, y;
	public:
		Point(double x, double y) {
			this->x = x;
			this->y = y;
		}
};

namespace Colors {

	RGB_color FACE = RGB_color(253, 224, 194);
	RGB_color GRASS = RGB_color(202, 220, 92);
	RGB_color SKY = RGB_color(133, 206, 203);
	RGB_color PURPLE = RGB_color(170, 108, 171);
	RGB_color YELLOW = RGB_color(253, 242, 126);
	RGB_color DARK_BLUE = RGB_color(101, 117, 143);
	RGB_color BLUE = RGB_color(100, 141, 197);
	RGB_color LIGHT_BLUE_ONE = RGB_color(144, 179, 173);
	RGB_color LIGHT_BLUE_TWO = RGB_color(143, 211, 220);
	RGB_color RED = RGB_color(216, 35, 42);
	RGB_color WALDO_JEANS = RGB_color(81, 129, 193);
	RGB_color WHITE = RGB_color(255, 255, 255);
	RGB_color BLACK = RGB_color(0, 0, 0);

}

Coord GRASS_COORD = Coord(0, 0, 1200, 500);
Coord CLOUDS_COORD = Coord(0, 700, 0, 0);
namespace Draw {

	void person(Coord person, RGB_color shirt_color, RGB_color pants_color) {
		
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

		// drawing the mask
		glColor3f(Colors::LIGHT_BLUE_TWO.R, Colors::LIGHT_BLUE_TWO.G, Colors::LIGHT_BLUE_TWO.B);
		glRecti(person.x1 + ARM_W + ARM_SPACE + HEAD_SPACE_W + 4, person.y1 + LEG_H + ARM_H + HEAD_SPACE_H + 4, person.x1 + ARM_W + ARM_SPACE + HEAD_SPACE_W + MASK_W + 4, person.y1 + LEG_H + ARM_H + HEAD_SPACE_H + MASK_H + 4);

		glBegin(GL_LINES);
		
		glLineWidth(6);
		glColor3f(Colors::WHITE.R, Colors::WHITE.G, Colors::WHITE.B);
		glVertex2f(person.x1 + ARM_W + ARM_SPACE + HEAD_SPACE_W, person.y1 + LEG_H + ARM_H + HEAD_SPACE_H + 4);
		glVertex2f(person.x1 + ARM_W + ARM_SPACE + HEAD_SPACE_W + MASK_W + 8, person.y1 + LEG_H + ARM_H + HEAD_SPACE_H + 4);
		glVertex2f(person.x1 + ARM_W + ARM_SPACE + HEAD_SPACE_W, person.y1 + LEG_H + ARM_H + HEAD_SPACE_H + MASK_H + 4);
		glVertex2f(person.x1 + ARM_W + ARM_SPACE + HEAD_SPACE_W + MASK_W + 8, person.y1 + LEG_H + ARM_H + HEAD_SPACE_H + MASK_H + 4);
		
		glEnd();

	}
	void wave_left(Coord person, RGB_color shirt_color) {
		glColor3f(shirt_color.R, shirt_color.G, shirt_color.B);
		glPushMatrix();
		glTranslated(person.x1 + ARM_W, person.y1 + LEG_H + ARM_W, 0.0); // in cg
		glRotated(rotate_wave_left, 0.0, 0.0, 1.0); // rotatia
		glTranslated(-person.x1 - ARM_W, -person.y1 - LEG_H - ARM_W, 0.0); // translatare in origine
		glRecti(person.x1 - ARM_H/2, person.y1 + LEG_H, person.x1 + ARM_W, person.y1 + LEG_H + ARM_W); // left hand
		glPopMatrix();

		rotate_wave_left = rotate_wave_left + alpha_wave_left;
		if (rotate_wave_left < -70.0)
			alpha_wave_left = 0.1;
		else if (rotate_wave_left > 0.0)
			alpha_wave_left = -0.1;
	}
	void wave_right(Coord person, RGB_color shirt_color) {
		
		double x1 = person.x1 + ARM_W + ARM_SPACE * 2 + LEG_W * 2 + LEG_SPACE;
		double y1 = person.y1 + LEG_H;
		double x2 = person.x1 + ARM_W * 2 + ARM_SPACE * 2 + LEG_W * 2 + LEG_SPACE + ARM_H / 2;
		double y2 = person.y1 + LEG_H + ARM_W;
		
		glColor3f(shirt_color.R, shirt_color.G, shirt_color.B);
		glPushMatrix();
		glTranslated(x1, y1 + ARM_W, 0.0); // in cg
		glRotated(rotate_wave_right, 0.0, 0.0, 1.0); // rotatia
		glTranslated(-x1, -y1 - ARM_W, 0.0); // translatare in origine
		glRecti(x1, y1, x2, y2); // right hand
		glPopMatrix();

		rotate_wave_right = rotate_wave_right + alpha_wave_right;
		if (rotate_wave_right > 70)
			alpha_wave_right = -0.1;
		else if (rotate_wave_right < 0.0)
			alpha_wave_right = 0.1;
	}
	void grass() {
		glColor3f(Colors::GRASS.R, Colors::GRASS.G, Colors::GRASS.B);
		glRecti(GRASS_COORD.x1, GRASS_COORD.y1, GRASS_COORD.x2, GRASS_COORD.y2);
	}

	void print_text(Point P, std::string text, RGB_color color, int size) {
		// type = 1 -> HEADING, type = 2 -> SUBTITLE, type = 3 -> NORMAL
		glColor3f(color.R, color.G, color.B);
		for (int i = 0; i < text.size(); i++) {
			glRasterPos2i(P.x + i * size, P.y);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
		}
	}

	void print_increasing_text(Point P, std::string text, int size) {
		
		glColor3f(Colors::RED.R, Colors::RED.G, Colors::RED.B);
		
		glPushMatrix();
		
		glScalef(text_scale, text_scale, text_scale);
		text_scale += 0.1;

		for (int i = 0; i < text.size(); i++) {
			glRasterPos2i(P.x + i * size, P.y);
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i]);
		}

		glPopMatrix();
		

	}

	void cloud(Coord cloud) {
		glColor3f(Colors::BLACK.R, Colors::BLACK.G, Colors::BLACK.B);
		glRecti(cloud.x1 + 12, cloud.y1, cloud.x1 + 80 + 12, cloud.y1 + 4);
		glRecti(cloud.x1 + 8, cloud.y1 + 4, cloud.x1 + 12, cloud.y1 + 8);
		glRecti(cloud.x1 + 4, cloud.y1 + 8, cloud.x1 + 8, cloud.y1 + 12);
		glRecti(cloud.x1, cloud.y1 + 12, cloud.x1 + 4, cloud.y1 + 28);
		glRecti(cloud.x1 + 4, cloud.y1 + 28, cloud.x1 + 8, cloud.y1 + 32);
		glRecti(cloud.x1 + 8, cloud.y1 + 32, cloud.x1 + 20, cloud.y1 + 36);
		glRecti(cloud.x1 + 20, cloud.y1 + 36, cloud.x1 + 24, cloud.y1 + 44);
		glRecti(cloud.x1 + 24, cloud.y1 + 44, cloud.x1 + 36, cloud.y1 + 48);
		glRecti(cloud.x1 + 36, cloud.y1 + 48, cloud.x1 + 40, cloud.y1 + 52);
		glRecti(cloud.x1 + 40, cloud.y1 + 52, cloud.x1 + 44, cloud.y1 + 56);
		glRecti(cloud.x1 + 44, cloud.y1 + 56, cloud.x1 + 48, cloud.y1 + 60);
		glRecti(cloud.x1 + 48, cloud.y1 + 60, cloud.x1 + 64, cloud.y1 + 64);
		glRecti(cloud.x1 + 64, cloud.y1 + 56, cloud.x1 + 68, cloud.y1 + 60);
		glRecti(cloud.x1 + 68, cloud.y1 + 52, cloud.x1 + 72, cloud.y1 + 56);
		glRecti(cloud.x1 + 72, cloud.y1 + 48, cloud.x1 + 76, cloud.y1 + 52);
		glRecti(cloud.x1 + 76, cloud.y1 + 40, cloud.x1 + 80, cloud.y1 + 48);
		glRecti(cloud.x1 + 80, cloud.y1 + 36, cloud.x1 + 92, cloud.y1 + 40);
		glRecti(cloud.x1 + 92, cloud.y1 + 32, cloud.x1 + 96, cloud.y1 + 36);
		glRecti(cloud.x1 + 96, cloud.y1 + 28, cloud.x1 + 100, cloud.y1 + 32);
		glRecti(cloud.x1 + 100, cloud.y1 + 12, cloud.x1 + 104, cloud.y1 + 28);
		glRecti(cloud.x1 + 92, cloud.y1 + 4, cloud.x1 + 96, cloud.y1 + 8);
		glRecti(cloud.x1 + 96, cloud.y1 + 8, cloud.x1 + 100, cloud.y1 + 12);
		glColor3f(Colors::WHITE.R, Colors::WHITE.G, Colors::WHITE.B);
		glRecti(cloud.x1 + 4, cloud.y1 + 12, cloud.x1 + 8, cloud.y1 + 28);
		glRecti(cloud.x1 + 8, cloud.y1 + 8, cloud.x1 + 12, cloud.y1 + 32);
		glRecti(cloud.x1 + 12, cloud.y1 + 4, cloud.x1 + 92, cloud.y1 + 32);
		glRecti(cloud.x1 + 20, cloud.y1 + 32, cloud.x1 + 92, cloud.y1 + 36);
		glRecti(cloud.x1 + 24, cloud.y1 + 36, cloud.x1 + 80, cloud.y1 + 40);
		glRecti(cloud.x1 + 24, cloud.y1 + 40, cloud.x1 + 76, cloud.y1 + 44);
		glRecti(cloud.x1 + 36, cloud.y1 + 44, cloud.x1 + 76, cloud.y1 + 48);
		glRecti(cloud.x1 + 40, cloud.y1 + 48, cloud.x1 + 72, cloud.y1 + 52);
		glRecti(cloud.x1 + 44, cloud.y1 + 52, cloud.x1 + 68, cloud.y1 + 56);
		glRecti(cloud.x1 + 48, cloud.y1 + 56, cloud.x1 + 64, cloud.y1 + 60);
		glRecti(cloud.x1 + 92, cloud.y1 + 8, cloud.x1 + 96, cloud.y1 + 32);
		glRecti(cloud.x1 + 96, cloud.y1 + 12, cloud.x1 + 100, cloud.y1 + 28);
	}

	void move_cloud(Coord cloud, float speed) {
		glPushMatrix();
		glTranslatef(alpha_cloud, 0.0, 0.0);
		alpha_cloud += speed;
		std::cout << alpha_cloud << "\n";
		Draw::cloud(cloud);
		glPopMatrix();
		if (alpha_cloud > 1300) {
			alpha_cloud = -speed;
		} else if (alpha_cloud < -104) {
			alpha_cloud = speed;
		}
	}

	void stripes(Coord person) {
		glColor3f(Colors::WHITE.R, Colors::WHITE.G, Colors::WHITE.B);
		for(int i = 0; i < 3; ++i) {
			glRecti(person.x1 + ARM_W + ARM_SPACE, person.y1 + LEG_H + i * 24, person.x1 + ARM_W + ARM_SPACE + 2 * LEG_W + LEG_SPACE, person.y1 + LEG_H + 12 + i * 24);
		}
	}

	/*

	void create_circle(GLuint clouds_list, double x, double y) {
		scrPt hexVertex;
		GLdouble hexTheta;
		GLint k;
		clouds_list = glGenLists(1);
		glNewList(clouds_list, GL_COMPILE);
		glColor3f(Colors::WHITE.R, Colors::WHITE.G, Colors::WHITE.B);
		glBegin(GL_POLYGON);
		for (k = 0; k < 100; k++)
		{
			hexTheta = TWO_PI * k / 100;
			hexVertex.x = x + y * cos(hexTheta);
			hexVertex.y = x + y * sin(hexTheta);
			glVertex2i(hexVertex.x, hexVertex.y);
		}
		glEnd();
		glEndList();
	}


	void clouds() {
		glPushMatrix();
		glTranslated(200, CLOUDS_COORD.y1, 0.0);
		glCallList(clouds_list);
		glColor3f(Colors::WHITE.R, Colors::WHITE.G, Colors::WHITE.B);
		glPopMatrix();

		/*glPushMatrix();
		glTranslated(CLOUDS_COORD.x1 + 100 + translate_clouds, CLOUDS_COORD.y1, 0.0);
		glCallList(clouds_list[1]);
		glPopMatrix();

		glPushMatrix();
		glTranslated(CLOUDS_COORD.x1 + 200 + translate_clouds, CLOUDS_COORD.y1, 0.0);
		glCallList(clouds_list[2]);
		glPopMatrix();
		
		translate_clouds = translate_clouds + alpha_clouds;
		glutSwapBuffers();
		if (translate_clouds > 1200.0)
			translate_clouds = 0.0;
	}
	*/

}

void init (void) {
	glClearColor(Colors::SKY.R, Colors::SKY.G, Colors::SKY.B, 0.0); // precizeaza culoarea de fond a ferestrei de vizualizare
    glMatrixMode(GL_PROJECTION);  // se precizeaza este vorba de o reprezentare 2D, realizata prin proiectie ortogonala
	gluOrtho2D(0.0, 1200.0, 0.0, 900.0); // sunt indicate coordonatele extreme ale ferestrei de vizualizare


	//Draw::create_circle(clouds_list, CLOUDS_COORD.x1, CLOUDS_COORD.y1);
	//Draw::create_circle(clouds_list[1], CLOUDS_COORD.x1 + 100, CLOUDS_COORD.y1);
	//Draw::create_circle(clouds_list[2], CLOUDS_COORD.x1 + 200, CLOUDS_COORD.y1);
}

void draw() {

	Draw::grass();
	Draw::move_cloud(Coord(-104, 750, 0, 0), 0.2);
	Draw::print_text(Point(250, 800), "WHERE'S WALLY?", Colors::RED, 50);
	Draw::print_text(Point(250, 700), "Social Distancing Edition", Colors::BLACK, 30);
	Draw::person(Coord(150, 200, 0, 0), Colors::YELLOW, Colors::PURPLE);
	Draw::person(Coord(350, 200, 0, 0), Colors::BLUE, Colors::DARK_BLUE);
	Draw::person(Coord(550, 200, 0, 0), Colors::LIGHT_BLUE_ONE, Colors::DARK_BLUE);
	Draw::wave_left(Coord(550, 200, 0, 0), Colors::LIGHT_BLUE_ONE);
	Draw::person(Coord(Waldo_x, Waldo_y, 0, 0), Colors::RED, Colors::WALDO_JEANS); // Waldo  // Waldo 
	Draw::stripes(Coord(Waldo_x, Waldo_y, 0, 0));
	Draw::person(Coord(950, 200, 0, 0), Colors::LIGHT_BLUE_TWO, Colors::DARK_BLUE);
	Draw::wave_right(Coord(950, 200, 0, 0), Colors::LIGHT_BLUE_TWO);

}

void found_waldo() {

	glColor3f(Colors::RED.R, Colors::RED.G, Colors::RED.B);
	glBegin(GL_LINE_LOOP);
	glVertex2i(Waldo_x - 10, Waldo_y - 10);
	glVertex2i(Waldo_x + ARM_W * 2 + ARM_SPACE * 2 + LEG_W * 2 + LEG_SPACE + 10, Waldo_y - 10);
	glVertex2i(Waldo_x + ARM_W * 2 + ARM_SPACE * 2 + LEG_W * 2 + LEG_SPACE + 10, Waldo_y + LEG_H + ARM_H + HEAD_SPACE_H + HEAD + 10);
	glVertex2i(Waldo_x - 10, Waldo_y + LEG_H + ARM_H + HEAD_SPACE_H + HEAD + 10);
   	glEnd();
	Draw::print_text(Point(250, 650), "You found Waldo :)", Colors::RED, 30);
	Draw::print_text(Point(250, 600), "Right click to reset the game", Colors::BLACK, 20);

}

void desen (void) {

	glClear(GL_COLOR_BUFFER_BIT);
	if (found == 1) {
		draw();
		found_waldo();
	} else {
		draw();
	}

	glFlush();
	glutPostRedisplay();

}
/*
void ending(void) {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(Colors::RED.R, Colors::RED.G, Colors::RED.B);
	glRecti(Waldo_x, Waldo_y, Waldo_x + ARM_W * 2 + ARM_SPACE * 2 + LEG_W * 2 + LEG_SPACE, Waldo_y + LEG_H + ARM_H + HEAD_SPACE_H + HEAD); // highlight Waldo
	glutPostRedisplay();
}
*/

void mouse(int button, int state, int x, int y) {
	cout << x << " " <<  y << endl;
	cout << Waldo_x + ARM_W * 2 + ARM_SPACE * 2 + LEG_W * 2 + LEG_SPACE << " " << Waldo_y + LEG_H + ARM_H + HEAD_SPACE_H + HEAD << endl;
	bool condition = (x >= Waldo_x && x <= Waldo_x + ARM_W * 2 + ARM_SPACE * 2 + LEG_W * 2 + LEG_SPACE) && (WINDOW_H - y >= Waldo_y && y <= WINDOW_H - Waldo_y + LEG_H + ARM_H + HEAD_SPACE_H + HEAD);
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			if (condition) {
				found = 1;
				cout << "DA" << endl;
			}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN)
			found = 0;
		break;
	default:
		break;
	}
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
	glutMouseFunc(mouse);
	glutMainLoop(); // ultima instructiune a programului, asteapta (eventuale) noi date de intrare
    
	return 0;
}