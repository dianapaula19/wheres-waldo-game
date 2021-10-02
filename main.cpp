#include <GL/freeglut.h> 
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
float translate_clouds = 0.0;
float alpha_clouds = 0.5;
float rotate_wave_left = 0.0;
float alpha_wave_left = 0.1;
float rotate_wave_right = 0.0;
float alpha_wave_right = 0.1;
float alpha_cloud = 0.1;
float translate_person = 0.0;
float scale_person = 0.0;
float alpha_translate_person = 0.05;
float alpha_scale_person = 0.001;
float rotate_legs = 0.0;
float alpha_legs = 0.1;
float text_scale = 1.0f; 
float text_apha = 0.001;
bool found = 0;
int lives = 3;


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

	void move_person(Coord person, RGB_color shirt_color, RGB_color pants_color) {

		glColor3f(pants_color.R, pants_color.G, pants_color.B);

		translate_person += alpha_translate_person;
		scale_person += alpha_scale_person;
		rotate_legs += alpha_legs;

		if (translate_person > 0) {
			alpha_translate_person = -0.05;
		}
		else if (translate_person < -60) {
			alpha_translate_person = 0.05;
		}
		if (scale_person > 1) {
			alpha_scale_person = -0.001;
		}
		else if (scale_person < 0) {
			alpha_scale_person = 0.001;
		}
		if (rotate_legs > 25) {
			alpha_legs = -0.1;
		}
		else if (rotate_legs < -25) {
			alpha_legs = 0.1;
		}

		// drawing the left leg
		glPushMatrix();
		glTranslated(person.x1 + ARM_W + ARM_SPACE + LEG_W + translate_person, person.y1 + LEG_H + translate_person, 0);
		glRotated(rotate_legs, 0.0, 0.0, 1.0); // rotatia
		glScaled(scale_person, scale_person, 1.0);
		glTranslated(-(person.x1 + ARM_W + ARM_SPACE + LEG_W), -(person.y1 + LEG_H), 0);
		glRecti(person.x1 + ARM_W + ARM_SPACE, person.y1, person.x1 + ARM_W + ARM_SPACE + LEG_W, person.y1 + LEG_H);
		glPopMatrix();


		// drawing the right leg
		glPushMatrix();
		glTranslated(person.x1 + ARM_W + ARM_SPACE + LEG_W + LEG_SPACE + translate_person, person.y1 + LEG_H + translate_person, 0);
		glRotated(-rotate_legs, 0.0, 0.0, 1.0); // rotatia
		glScaled(scale_person, scale_person, 1.0);
		glTranslated(-(person.x1 + ARM_W + ARM_SPACE + LEG_W + LEG_SPACE), -(person.y1 + LEG_H), 0);
		glRecti(person.x1 + ARM_W + ARM_SPACE + LEG_W + LEG_SPACE, person.y1, person.x1 + ARM_W + ARM_SPACE + LEG_W * 2 + LEG_SPACE, person.y1 + LEG_H);
		glPopMatrix();

		//the rest of the body
		glPushMatrix();
		glTranslated(person.x1 + ARM_W + ARM_SPACE + LEG_W + LEG_SPACE/2 + translate_person, person.y1 + LEG_H + translate_person, 0);
		glScaled(scale_person, scale_person, 1.0);
		glTranslated(- (person.x1 + ARM_W + ARM_SPACE + LEG_W + LEG_SPACE / 2), -(person.y1 + LEG_H), 0);

		glColor3f(shirt_color.R, shirt_color.G, shirt_color.B);
		// drawing the upper body
		glRecti(person.x1 + ARM_W + ARM_SPACE, person.y1 + LEG_H, person.x1 + ARM_W + ARM_SPACE + LEG_W * 2 + LEG_SPACE, person.y1 + LEG_H + ARM_H);

		// drawing the arms
		glRecti(person.x1, person.y1 + LEG_H, person.x1 + ARM_W, person.y1 + LEG_H + ARM_H); // left arm
		glRecti(person.x1 + ARM_W + ARM_SPACE * 2 + LEG_W * 2 + LEG_SPACE, person.y1 + LEG_H, person.x1 + ARM_W * 2 + ARM_SPACE * 2 + LEG_W * 2 + LEG_SPACE, person.y1 + LEG_H + ARM_H); // right arm

		// drawing the head

		glColor3f(Colors::FACE.R, Colors::FACE.G, Colors::FACE.B);
		glRecti(person.x1 + ARM_W + ARM_SPACE + HEAD_SPACE_W, person.y1 + LEG_H + ARM_H + HEAD_SPACE_H, person.x1 + ARM_W + ARM_SPACE + HEAD_SPACE_W + HEAD, person.y1 + LEG_H + ARM_H + HEAD_SPACE_H + HEAD);

		if (alpha_scale_person > 0)
		{
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

		glPopMatrix();
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

	void print_increasing_text(Point P, std::string text, int size) 
	{
		if (text_scale <= 1)
			text_apha = 0.001;
		else if (text_scale >= 1.5)
			text_apha = -0.001;

		glColor3f(Colors::RED.R, Colors::RED.G, Colors::RED.B);
		glPushMatrix();
		text_scale += text_apha;
		glTranslated(P.x + 200, P.y, 0);
		glScalef(text_scale, text_scale, 1.0);
		glTranslated(-P.x - 200, - P.y, 0);

		for (int i = 0; i < text.size(); i++) {
			glRasterPos2i(P.x + i * (size), P.y);
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
	void heart(Point P)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glColor3f(Colors::RED.R, Colors::RED.G, Colors::RED.B);
		glBegin(GL_POLYGON);
		glVertex2s(P.x + 20, P.y);
		glVertex2s(P.x + 28, P.y + 12);
		glVertex2s(P.x + 32, P.y + 20);
		glVertex2s(P.x + 32, P.y + 24);
		glVertex2s(P.x + 28, P.y + 28);
		glVertex2s(P.x + 24, P.y + 28);
		glVertex2s(P.x + 20, P.y + 24);
		glVertex2s(P.x + 16, P.y + 28);
		glVertex2s(P.x + 12, P.y + 28);
		glVertex2s(P.x + 8, P.y + 24);
		glVertex2s(P.x + 8, P.y + 20);
		glVertex2s(P.x + 12, P.y + 12);
		glEnd();
	}
	void draw_hearts()
	{
		if (lives == 1)
			heart(Point(50, 520));
		else if (lives == 2)
		{
			heart(Point(50, 520));
			heart(Point(90, 520));
		}
		else if (lives == 3)
		{
			heart(Point(50, 520));
			heart(Point(90, 520));
			heart(Point(130, 520));
		}
	}
}

void init (void) {
	glClearColor(Colors::SKY.R, Colors::SKY.G, Colors::SKY.B, 0.0); // precizeaza culoarea de fond a ferestrei de vizualizare
    glMatrixMode(GL_PROJECTION);  // se precizeaza este vorba de o reprezentare 2D, realizata prin proiectie ortogonala
	gluOrtho2D(0.0, 1200.0, 0.0, 800.0); // sunt indicate coordonatele extreme ale ferestrei de vizualizare

}

void draw() {

	Draw::grass();
	Draw::move_cloud(Coord(-104, 620, 0, 0), 0.2);
	Draw::print_text(Point(250, 750), "WHERE'S WALLY?", Colors::RED, 50);
	Draw::print_text(Point(250, 700), "Social Distancing Edition", Colors::BLACK, 30);
	Draw::move_person(Coord(150, 200, 0, 0), Colors::YELLOW, Colors::PURPLE);
	Draw::person(Coord(350, 200, 0, 0), Colors::BLUE, Colors::DARK_BLUE);
	Draw::person(Coord(550, 200, 0, 0), Colors::LIGHT_BLUE_ONE, Colors::DARK_BLUE);
	Draw::wave_left(Coord(550, 200, 0, 0), Colors::LIGHT_BLUE_ONE);
	Draw::person(Coord(Waldo_x, Waldo_y, 0, 0), Colors::RED, Colors::WALDO_JEANS); // Waldo  // Waldo 
	Draw::stripes(Coord(Waldo_x, Waldo_y, 0, 0));
	Draw::person(Coord(950, 200, 0, 0), Colors::LIGHT_BLUE_TWO, Colors::DARK_BLUE);
	Draw::wave_right(Coord(950, 200, 0, 0), Colors::LIGHT_BLUE_TWO);
	Draw::draw_hearts();
}

void found_waldo() {

	glColor3f(Colors::RED.R, Colors::RED.G, Colors::RED.B);
	glBegin(GL_LINE_LOOP);
	glVertex2i(Waldo_x - 10, Waldo_y - 10);
	glVertex2i(Waldo_x + ARM_W * 2 + ARM_SPACE * 2 + LEG_W * 2 + LEG_SPACE + 10, Waldo_y - 10);
	glVertex2i(Waldo_x + ARM_W * 2 + ARM_SPACE * 2 + LEG_W * 2 + LEG_SPACE + 10, Waldo_y + LEG_H + ARM_H + HEAD_SPACE_H + HEAD + 10);
	glVertex2i(Waldo_x - 10, Waldo_y + LEG_H + ARM_H + HEAD_SPACE_H + HEAD + 10);
   	glEnd();
	Draw::print_increasing_text(Point(250, 600), "You found Waldo :)", 30);
	Draw::print_text(Point(250, 570), "Right click to reset the game", Colors::BLACK, 20);

}
void lost_game() {

	Draw::print_increasing_text(Point(250, 600), "You lost :(", 30);
	Draw::print_text(Point(250, 570), "Right click to reset the game", Colors::BLACK, 20);
}
void desen (void) {

	glClear(GL_COLOR_BUFFER_BIT);
	draw();
	if (lives <= 0)
		lost_game();
	else if (found == 1)
		found_waldo();

	glFlush();
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	bool condition = (x >= Waldo_x && x <= Waldo_x + ARM_W * 2 + ARM_SPACE * 2 + LEG_W * 2 + LEG_SPACE) && (WINDOW_H - y >= Waldo_y && y <= WINDOW_H - Waldo_y + LEG_H + ARM_H + HEAD_SPACE_H + HEAD);
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
			if (condition) {
				found = 1;
				cout << "DA" << endl;
				cout << x << " " << y << endl;
			}
			else
				lives--;
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN && (found == 1 || lives <= 0))
		{
			found = 0;
			lives = 3;
		}

		break;
	default:
		break;
	}
}

int main (int argc, char** argv) {

	glutInit (&argc, argv); // initializare GLUT
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // se utilizeaza un singur buffer | modul de colorare RedGreenBlue (= default)
	glutInitWindowPosition (100, 100); // pozitia initiala a ferestrei de vizualizare (in coordonate ecran)
	glutInitWindowSize(1200, 800); // dimensiunile ferestrei 
	glutCreateWindow("Where's Wally: Social Distancing Edition"); // creeaza fereastra, indicand numele ferestrei de vizualizare - apare in partea superioara

	init(); // executa procedura de initializare
	glClear(GL_COLOR_BUFFER_BIT); // reprezentare si colorare fereastra de vizualizare
	glutDisplayFunc(desen); // procedura desen este invocata ori de cate ori este nevoie
	glutMouseFunc(mouse);
	glutMainLoop(); // ultima instructiune a programului, asteapta (eventuale) noi date de intrare
    
	return 0;
}
