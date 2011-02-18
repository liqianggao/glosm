/*
 * Copyright (C) 2010-2011 Dmitry Marakasov
 *
 * This file is part of glosm.
 *
 * glosm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * glosm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with glosm.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "GlosmViewer.hh"

#if defined(USE_GLEW)
#	include <GL/glew.h>
#endif
#if defined(__APPLE__)
#	include <OpenGL/gl.h>
#	include <GLUT/glut.h>
#else
#	include <GL/gl.h>
#	include <GL/glut.h>
#endif

class GlosmViewerImpl : public GlosmViewer {
protected:
	virtual void WarpCursor(int x, int y) {
		glutWarpPointer(x, y);
	}

	virtual void Flip() {
		glutSwapBuffers();
	}
};

GlosmViewerImpl app;

void Display(void) {
	app.Render();
}

void Reshape(int w, int h) {
	app.Resize(w, h);
}

void Mouse(int x, int y) {
	app.MouseMove(x, y);
}

void SpecialDown(int key, int, int) {
	switch (key) {
	case GLUT_KEY_UP: app.KeyDown(GlosmViewer::UP); break;
	case GLUT_KEY_DOWN: app.KeyDown(GlosmViewer::DOWN); break;
	case GLUT_KEY_LEFT: app.KeyDown(GlosmViewer::LEFT); break;
	case GLUT_KEY_RIGHT: app.KeyDown(GlosmViewer::RIGHT); break;
	default: break;
	}
}

void SpecialUp(int key, int, int) {
	switch (key) {
	case GLUT_KEY_UP: app.KeyUp(GlosmViewer::UP); break;
	case GLUT_KEY_DOWN: app.KeyUp(GlosmViewer::DOWN); break;
	case GLUT_KEY_LEFT: app.KeyUp(GlosmViewer::LEFT); break;
	case GLUT_KEY_RIGHT: app.KeyUp(GlosmViewer::RIGHT); break;
	default: break;
	}
}

void KeyDown(unsigned char key, int, int) {
	app.KeyDown(key);
}

void KeyUp(unsigned char key, int, int) {
	app.KeyUp(key);
}

int real_main(int argc, char** argv) {
	glutInit(&argc, argv);

	app.Init(argc, argv);

	/* glut init */
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutCreateWindow("glosm viewer");

#if defined(USE_GLEW)
	GLenum err = glewInit();
	if (err != GLEW_OK)
		throw Exception() << "Cannot init glew: " << glewGetErrorString(err);
	const char *gl_requirements = "GL_VERSION_1_5";
	if (!glewIsSupported(gl_requirements))
		throw Exception() << "Minimal OpenGL requirements (" << gl_requirements << ") not met, unable to continue";
#endif

	glutIgnoreKeyRepeat(1);
	glutSetCursor(GLUT_CURSOR_NONE);

	glutDisplayFunc(Display);
	glutIdleFunc(Display);
	glutReshapeFunc(Reshape);
	glutPassiveMotionFunc(Mouse);
	glutKeyboardFunc(KeyDown);
	glutKeyboardUpFunc(KeyUp);
	glutSpecialFunc(SpecialDown);
	glutSpecialUpFunc(SpecialUp);

	app.InitGL();

	/* main loop */
	/* note that this never returns and objects created above
	 * are never properly destroyed; should dump GLUT ASAP */
	glutMainLoop();

	return 0;
}

int main(int argc, char** argv) {
	try {
		return real_main(argc, argv);
	} catch (std::exception &e) {
		fprintf(stderr, "Exception: %s\n", e.what());
	} catch (...) {
		fprintf(stderr, "Unknown exception\n");
	}

	return 1;
}
