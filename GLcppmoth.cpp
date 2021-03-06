// ----------------------------------------------------------------------------
// GLcppmoth.cpp : a skeleton class for quick building of simple opengl 
//				applications to run with the legacy x window in a console mode,
//				see	GLcppmoth.h for the detailed decription about embedded
//				features
//
// Creation : Jan. 11th 2011
//
// Copyright(C) 2011-2012 Haipeng Cai
//
// ----------------------------------------------------------------------------
#include "GLcppmoth.h"
#include "glrand.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
/* all the global data structures to be used throughout this namespace*/

unsigned int g_keys[] = { 'z', 'Z', 'j', 'k', 'l', 'h', 'i', ',',
						'a', 's', 'd', 'f', 'w', 'x'}; 

///////////////////////////////////////////////////////////////////////////////
/* following static members offered as public utilities */
int CGLApp::getRootWindowSize(int *w, int *h)
{
	Display* pdsp = NULL;
	Window wid = 0;
	XWindowAttributes xwAttr;

	pdsp = XOpenDisplay( NULL );
	if ( !pdsp ) {
		fprintf(stderr, "Failed to open default display.\n");
		return -1;
	}

	wid = DefaultRootWindow( pdsp );
	if ( 0 > wid ) {
		fprintf(stderr, "Failed to obtain the root windows Id "
						"of the default screen of given display.\n");
		return -2;
	}
	
	Status ret = XGetWindowAttributes( pdsp, wid, &xwAttr );
	if ( 0 != ret ) {
		fprintf(stderr, "Failed to obtain X window Attribute.\n");
		return -3;
	}
	*w = xwAttr.width;
	*h = xwAttr.height;

	XCloseDisplay( pdsp );
	return 0;
}

int CGLApp::getScreenSize(int *w, int*h)
{

	Display* pdsp = NULL;
	Screen* pscr = NULL;

	pdsp = XOpenDisplay( NULL );
	if ( !pdsp ) {
		fprintf(stderr, "Failed to open default display.\n");
		return -1;
	}

   	pscr = DefaultScreenOfDisplay( pdsp );
	if ( !pscr ) {
		fprintf(stderr, "Failed to obtain the default screen of given display.\n");
		return -2;
	}

	*w = pscr->width;
	*h = pscr->height;

	XCloseDisplay( pdsp );
	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// this the outermost shell to cover the model application described above
CGLApp* CGLApp::m_pInstance = NULL;
CGLApp::CGLApp() : CApplication(),
	// these defaults are not really the defaults we want, here they simple
	// indicate the status that they are not yet set
	m_wTitle("GL program untitled"),
	m_x(-1), m_y(-1),
	m_width(-1), m_height(-1),
	m_dispMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH),
	m_bAnimated(false),
	m_pressedbtn(-1),
	m_mx(0), m_my(0),
	m_bFullWindow(false),
	m_fvoy(60.0),
	m_near(1.0), m_far(5.0),
	m_eyeX(.0), m_eyeY(.0), m_eyeZ(2.0),
	m_centerX(.0), m_centerY(.0), m_centerZ(.0),
	m_upX(.0), m_upY(1.0), m_upZ(.0),
	m_dx(0),m_dy(0),m_dz(0),
	m_bGadgetEnabled(false)
{
	m_position[0] = .0, m_position[1] = .0, 
	m_position[2] = m_eyeZ, m_position[3] = .0;
	m_color[0] = .0, m_color[1] = .8, m_color[2] = .0;
	setVerinfo("GLcpp skeleton-noarg");
}

CGLApp::CGLApp(int argc, char** argv) : CApplication(argc, argv),
	m_wTitle("GL program untitled"),
	m_x(-1), m_y(-1),
	m_width(-1), m_height(-1),
	m_dispMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH),
	m_bAnimated(false),
	m_pressedbtn(-1),
	m_mx(0), m_my(0),
	m_bFullWindow(false),
	m_fvoy(60.0),
	m_near(1.0), m_far(5.0),
	m_eyeX(.0), m_eyeY(.0), m_eyeZ(2.0),
	m_centerX(.0), m_centerY(.0), m_centerZ(.0),
	m_upX(.0), m_upY(1.0), m_upZ(.0),
	m_dx(0),m_dy(0),m_dz(0),
	m_bGadgetEnabled(false)
{
	m_position[0] = .0, m_position[1] = .0, 
	m_position[2] = m_eyeZ, m_position[3] = .0;
	m_color[0] = .0, m_color[1] = .8, m_color[2] = .0;

	addOption('d', false, "default-gui", "run the legacy openGL GUI");
	setVerinfo("GLcpp skeleton");
}

CGLApp::~CGLApp() 
{
	for (size_t i = 0; i < m_gadgets.size(); ++i) {
		delete m_gadgets[i];
		m_gadgets[i] = NULL;
	}
}

void CGLApp::setGlutInfo(const char* wTitle,
						  int x, int y,
						  int width, int height,
						  int dispMode)
{
	m_wTitle = wTitle;
	m_x = x, m_y = y;
	m_width = width, m_height = height;
	m_dispMode = dispMode;
	if (m_dispMode & GLUT_DOUBLE) {
		m_cout << "double buffering enabled.\n";
	}
}

void CGLApp::setPrjInfo(GLfloat fvoy, GLfloat near, GLfloat far)
{
	m_fvoy = fvoy;
	m_near = near;
	m_far = far;
}

void CGLApp::setViewInfo(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
		GLdouble centerX, GLdouble centerY, GLdouble centerZ,
		GLdouble upX, GLdouble upY, GLdouble upZ)
{
	m_eyeX = eyeX, m_eyeY = eyeY, m_eyeZ = eyeZ;
	m_centerX = centerX, m_centerY = centerY, m_centerZ = centerZ;
	m_upX = upX, m_upY = upY, m_upZ = upZ;
}

int CGLApp::setFullWindow()
{
	m_bFullWindow = true;
	return 0;
}

void CGLApp::setObjOffset(GLdouble dx, GLdouble dy, GLdouble dz)
{
	m_dx = dx, m_dy = dy, m_dz = dz;
}

void CGLApp::setClearColor(GLclampf red, GLclampf green, 
						GLclampf blue, GLclampf alpha)
{
	glClearColor(red, green, blue, alpha);
}

void CGLApp::useCurrentColor()
{
	glColor3fv( m_color );
}

void CGLApp::trigger()
{
	if ( m_dispMode & GLUT_DOUBLE  ) {
		glutSwapBuffers();
	}
	else {
		glFlush();
	}
}

int CGLApp::mainstay() 
{
	// if the windows position and size are both left untouched...
	// we will use the defaults for them all, referencing to the display
	// parameter from X window server listening at 5992
	if ( -4 == m_x + m_y + m_width + m_height ) {
		_setDefaultWindow();
	}

	glutInitWindowPosition ( m_x, m_y );
	glutInitWindowSize ( m_width, m_height );
	glutInit (&m_argc, m_argv);
	glutInitDisplayMode	( m_dispMode );
	glutCreateWindow ( m_wTitle.c_str() );

	glInit();

	m_pInstance = this;

	m_cout << "at the CGLApp level.\n";
	m_cout << "in CGLApp m_pInstance: " << m_pInstance << " \n";

	glutMouseFunc(_mouseResponse);
	glutMotionFunc(_mouseMotionResponse);
	glutPassiveMotionFunc(_mousePassiveMotionResponse);
	//glutMouseWheelFunc(_mouseWheelRollResponse);
	glutKeyboardFunc(_keyResponse);
	glutSpecialFunc(_specialResponse);
	glutReshapeFunc(_onReshape);
	glutDisplayFunc(_display);
	glutMainLoop() ;

	return 1;
}

void CGLApp::show_version()
{
	CApplication::show_version();
	glutInit (&m_argc, m_argv);
	glutCreateWindow ( m_wTitle.c_str() );
	fprintf(stdout, "GL: %s (vendor)\n"
					"    %s (renderer)\n"
					"    %s (version)\n",
					glGetString(GL_VENDOR),
					glGetString(GL_RENDERER),
					glGetString(GL_VERSION)
			);
}

int CGLApp::handleOptions(int optv) 
{
	switch ( optv ) {
		case 'd':
			return 0;
		default:
			return CApplication::handleOptions(optv);
	};
	return 1;
}

void CGLApp::draw()
{
}

void CGLApp::drawGadgets()
{
	for (size_t i = 0; i < m_gadgets.size(); ++i) {
		m_gadgets[i]->display();
	}
}

int CGLApp::addGadget(CGLGadget* pgadget)
{
	m_gadgets.push_back( pgadget );
	return static_cast<int> (m_gadgets.size());
}

int CGLApp::removeGadget(int index)
{
	int cursz = static_cast<int> (m_gadgets.size());
	if ( index < 0 || index >= cursz ) {
		return -1;
	}

	m_gadgets.erase( m_gadgets.begin() + index );
	return cursz - 1;
}

void CGLApp::glInit( void )
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glDisable    ( GL_LIGHTING   );
	glDisable    ( GL_LIGHT0     );
	glEnable    ( GL_DEPTH_TEST );
	glShadeModel (GL_FLAT);
}

void CGLApp::onReshape ( GLsizei w, GLsizei h )
{
	m_width = w, m_height = h;
	glViewport	( 0, 0, w, h ) ;
	glMatrixMode  ( GL_PROJECTION ) ;
	glLoadIdentity( ) ;
	gluPerspective( m_fvoy, (GLfloat) w/(GLfloat) h, m_near, m_far ) ;
	m_cout << "in GLAPP: fvoy= " << m_fvoy << " far= " 
		<< m_far << " near= " << m_near << "\n";
	glMatrixMode  ( GL_MODELVIEW ) ;
	glLoadIdentity();
	gluLookAt(m_eyeX, m_eyeY, m_eyeZ,
			m_centerX, m_centerY, m_centerZ,
			m_upX, m_upY, m_upZ);
}

void CGLApp::mouseResponse(int button, int state, int x, int y)
{
	if ( GLUT_DOWN == state ) {
		m_pressedbtn = button;
		m_mx = x, m_my = y;
	}
	else { // when button has been bounced up again
		m_pressedbtn = -1;
	}

	switch (button) {
      case GLUT_LEFT_BUTTON:
         if (state == GLUT_DOWN) {
			m_cout << "rotating by mouse.\n";
         }
		 else {
			m_cout << "rotating finished.\n";
		 }
         break;
      case GLUT_MIDDLE_BUTTON:
		 // lighting switch on/off and let position follow the mouse
         if (state == GLUT_DOWN) {
			if (glIsEnabled( GL_LIGHTING)) {
				glDisable ( GL_LIGHTING) ;
				glDisable ( GL_LIGHT0 );
				m_cout << "Lighting off.\n";
			}
			else {
				glEnable( GL_LIGHTING );
				glEnable( GL_LIGHT0 );
				GLdouble a,b,c;
				_wincoord2objcoord(x,y,0,&a,&b,&c,true);
				m_position[0]=a, m_position[1]=b;
				/*
				m_position[0] = x * 1.0 / m_width ;
				m_position[1] = y * 1.0 / m_height * (m_width*1.0/m_height);
				*/
				glLightfv( GL_LIGHT0, GL_POSITION, m_position ) ;
				m_cout << "Lighting on, start positioning light source...\n";
			}
         }
		 else if (glIsEnabled( GL_LIGHTING)) {
			 m_cout << "Light positioning finished, updated to (" <<
				 m_position[0] << "," << m_position[1] << "," <<
				 m_position[2] << ")\n";
		 }
         break;
      case GLUT_RIGHT_BUTTON:
         if (state == GLUT_DOWN) {
			m_cout << 
				( GLUT_ACTIVE_SHIFT == glutGetModifiers() ?"Scaling":"Moving") 
				<< " by mouse.\n";
         }
		 else {
			m_cout << 
				( GLUT_ACTIVE_SHIFT == glutGetModifiers() ?"Scaling":"Moving") 
				<< " finished.\n";
		 }
         break;
      default:
		 return;
	} 
	glutPostRedisplay();
}

void CGLApp::keyResponse(unsigned char key, int x, int y)
{
	switch (key) {
	case 27:
		cleanup();
		cout << "quit.\n";
		exit(0);
	case '9':
		{
			// switch the shading model
			GLint smode = GL_FLAT;
			glGetIntegerv(GL_SHADE_MODEL,&smode);
			if ( GL_FLAT == smode ) {
				glShadeModel(GL_SMOOTH);
			}
			else {
				glShadeModel(GL_FLAT);
			}
			m_cout << "Shading mode changed to : " << 
				(GL_FLAT == smode?"GL_SMOOTH":"GL_FLAT") << ".\n";
		}
		break;
	case '0':
		{
			m_bAnimated = ! m_bAnimated;
			if (m_bAnimated) {
				srand((unsigned int)time(NULL));
				glutIdleFunc(_onIdle);
			}
			else {
				glutIdleFunc(NULL);
			}
			m_cout << "Animation " << (m_bAnimated?"enabled":"disabled") << "\n";
		}
		break;
	// increase in the red channel by step 0.1
	case 'a':
	case 'A':
		{
			m_color[ 0 ] += 0.1;
			if ( m_color[ 0 ] > 1.0 ) {
				m_color[ 0 ] = 0.0;
			}
		}
		m_cout << "color : (" << m_color[0] << "," << 
			m_color[1] << "," << m_color[2] << ").\n";
		break;
	// decrease in the red channel by step 0.1
	case 'd':
	case 'D':
		{
			m_color[ 0 ] -= 0.1;
			if ( m_color[ 0 ] < 0.0 ) {
				m_color[ 0 ] = 1.0;
			}
		}
		m_cout << "color : (" << m_color[0] << "," << 
			m_color[1] << "," << m_color[2] << ").\n";
		break;
	// increase in the green channel by step 0.1
	case 'w':
	case 'W':
		{
			m_color[ 1 ] += 0.1;
			if ( m_color[ 1 ] > 1.0 ) {
				m_color[ 1 ] = 0.0;
			}
		}
		m_cout << "color : (" << m_color[0] << "," << 
			m_color[1] << "," << m_color[2] << ").\n";
		break;
	// decrease in the green channel by step 0.1
	case 'x':
	case 'X':
		{
			m_color[ 1 ] -= 0.1;
			if ( m_color[ 1 ] < 0.0 ) {
				m_color[ 1 ] = 1.0;
			}
		}
		m_cout << "color : (" << m_color[0] << "," << 
			m_color[1] << "," << m_color[2] << ").\n";
		break;
	// increase in the blue channel by step 0.1
	case 's':
	case 'S':
		{
			m_color[ 2 ] += 0.1;
			if ( m_color[ 2 ] > 1.0 ) {
				m_color[ 2 ] = 0.0;
			}
		}
		m_cout << "color : (" << m_color[0] << "," << 
			m_color[1] << "," << m_color[2] << ").\n";
		break;
	// decrease in the blue channel by step 0.1
	case 'f':
	case 'F':
		{
			m_color[ 2 ] -= 0.1;
			if ( m_color[ 2 ] < 0.0 ) {
				m_color[ 2 ] = 1.0;
			}
		}
		m_cout << "color : (" << m_color[0] << "," << 
			m_color[1] << "," << m_color[2] << ").\n";
		break;
	/* ALL FOLLOWING MODEL VIEWING CONTROLS ARE VULNERABLE TO ANY MODEL OR
	 * VIEWING TRANSFORMATION SINCE IT DOES NOT INVOLVE THE NECESSARY
	 * TRANSFORMATION FOR THE AXES OR LOCAL COORDINATE SYSTEM.
	 *
	 * THIS IS ESPECIALLY LEFT SO HOWEVER TO PROVIDE A UNIQUE FEATURE SINCE
	 * THE INTUITIVE OR ORDINARILY DESIRABLE FUNCTIONALITIES HAVE ALL
	 * ALREADY BEEN PROVIDED THROUGH MOUSE INTERACTIONS
	 */
	// rotate clockwise around X axis
	case 'j':
	case 'J':
		glRotatef(-1.0f, 1.0, 0.0, 0.0);
		m_cout << "rotate around X clockwise.\n";
		break;
	// rotate anticlockwise around X axis
	case 'l':
	case 'L':
		glRotatef(1.0f, 1.0, 0.0, 0.0);
		m_cout << "rotate around X anticlockwise.\n";
		break;
	// rotate clockwise around Y axis
	case 'i':
	case 'I':
		glRotatef(-1.0f, 0.0, 1.0, 0.0);
		m_cout << "rotate around Y clockwise.\n";
		break;
	// rotate anticlockwise around Y axis
	case ',':
	case '<':
		glRotatef(1.0f, 0.0, 1.0, 0.0);
		m_cout << "rotate around Y anticlockwise.\n";
		break;
	// rotate clockwise around Z axis
	case 'h':
	case 'H':
		glRotatef(-1.0f, 0.0, 0.0, 1.0);
		m_cout << "rotate around Z clockwise.\n";
		break;
	// rotate anticlockwise around Z axis
	case 'k':
	case 'K':
		glRotatef(1.0f, 0.0, 0.0, 1.0);
		m_cout << "rotate around Z anticlockwise.\n";
		break;
	case 'z':
		glTranslatef(0.0, 0.0, 0.1);
		m_cout << "Move backwards.\n";
		break;
	case 'Z':
		glTranslatef(0.0, 0.0, -0.1);
		m_cout << "Move forwards.\n";
		break;
	case 32: // white space
		{
			onReshape(m_width, m_height);
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			m_color[0] = .0, m_color[1] = .8, m_color[2] = .0;
			if (glIsEnabled(GL_LIGHTING) && glIsEnabled(GL_LIGHT0) ) {
				m_position[0] = .0, m_position[1] = .0, 
				m_position[2] = m_eyeZ, m_position[3] = .0;
				glLightfv( GL_LIGHT0, GL_POSITION, m_position ) ;
			}
			m_cout << "reset all.\n";
		}
		break;
	case '=': // meant for '+'
	case '-':
		{
			static GLdouble eyeZ = m_eyeZ;
		    eyeZ += ('='==key?-1:1);
			m_cout << "Zoom " << ('+'==key?"in":"out") <<
				" eye at (" << m_eyeX << "," <<
				m_eyeY << "," << eyeZ << ")\n";

			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluLookAt(m_eyeX, m_eyeY, eyeZ,
					m_centerX, m_centerY, m_centerZ,
					m_upX, m_upY, m_upZ);
		}
		break;
	default:
		return;
	}
	glutPostRedisplay();
}

void CGLApp::specialResponse(int key, int x, int y)
{
	switch (key) {
	case GLUT_KEY_UP:
		glTranslatef(0.0, 0.1, 0.0);
		m_cout << "Move upwards.\n";
		break;
	case GLUT_KEY_DOWN:
		glTranslatef(0.0, -0.1, 0.0);
		m_cout << "Move downwards.\n";
		break;
	case GLUT_KEY_LEFT:
		glTranslatef(-0.1, 0.0, 0.0);
		m_cout << "Move leftwards.\n";
		break;
	case GLUT_KEY_RIGHT:
		glTranslatef(0.1, 0.0, 0.0);
		m_cout << "Move rightwards.\n";
		break;
	case GLUT_KEY_F6:
		m_bGadgetEnabled = !m_bGadgetEnabled;
		m_cout << "gadgets " << 
			(m_bGadgetEnabled?"Enabled":"Disabled") << "\n";
		break;
	default:
		return;
	}
	glutPostRedisplay();
}

void CGLApp::mouseMotionResponse(int x, int y)
{
	GLdouble dx = x - m_mx, dy = y - m_my, dz = 0.0;
	GLdouble a, b, c, d=0;
	GLdouble mvmat[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmat);

	switch (m_pressedbtn) {
		case GLUT_LEFT_BUTTON:
			{
				GLdouble fangle = -1.0;
				/*
				if ( ( ABS(dx) > ABS(dy) ? dx : -dy ) < 0 ) {
					fangle *= -1;
				}
				
				glRotatef(fangle, -dy, -dx, dz);
				*/
				a = -dy, b = -dx, c = dz, d=0;
				// when ctrl combined, rotate always around the z-axis
				if ( GLUT_ACTIVE_CTRL == glutGetModifiers() ) {
					a = b = 0;
					c = 1;
					fangle *= (dx > 0 || dy < 0)?-1:1;
				}
				transpoint(mvmat, a, b, c, d);
				glRotatef(fangle, a, b, c);
			}
			break;
		case GLUT_RIGHT_BUTTON:
			{
				if ( GLUT_ACTIVE_SHIFT == glutGetModifiers() ) {
					dx = dx*.5 / m_width + 1.0;
					dy = dy*.5 * (m_width*1.0/m_height) / m_height + 1.0;
					glScalef(dx, dy, min(dx, dy));
				}
				else {
					/*
					GLdouble objdx, objdy, objdz;
					GLdouble pobjdx, pobjdy, pobjdz;
					_wincoord2objcoord(m_mx,m_my,0, &pobjdx, &pobjdy, &pobjdz, false);
					_wincoord2objcoord(x, y, 0, &objdx, &objdy, &objdz, false);
					glTranslatef(objdx-pobjdx, objdy-pobjdy, objdz-pobjdz);
					GLdouble f = 1/abs(tan(m_fvoy/2)*(abs(m_eyeZ)+1)/m_height)
									* (m_far-m_near)/m_height;
					a = (objdx - pobjdx)*f, b = (objdy - pobjdy)*f, c = (objdz - pobjdz)*f;
					glTranslatef((objdx-pobjdx)*f, (objdy-pobjdy)*f, objdz-pobjdz);
					*/
					//glTranslatef(dx/f , -dy/f, dz);
					//glTranslatef(dx , -dy, dz);
					GLdouble f = 1/
						((abs(m_eyeZ)>1e-6?abs(m_eyeZ):1)*tan(m_fvoy*M_PI/180/2)/m_height);
					a = dx/f, b = -dy/f, c = dz/f;

					// when ctrl combined, translate always along the z-axis
					if ( GLUT_ACTIVE_CTRL == glutGetModifiers() ) {
						a = b = 0;
						c = 1;
						c *= (dx > 0 || dy > 0)?1:-1;
					}
					transpoint(mvmat, a, b, c, d);
					glTranslatef(a, b, c);
				}
			} 
			break;
		case GLUT_MIDDLE_BUTTON:
			{
				/*
				m_position[0] = x * 1.0 / m_width ;
				m_position[1] = y * 1.0 / m_height * (m_width*1.0/m_height);
				*/
				// just change the Z coordinate of the lighting position
				GLdouble a, b, c;
				_wincoord2objcoord(x,y,0,&a,&b,&c,true);
				if ( GLUT_ACTIVE_SHIFT == glutGetModifiers() ) {
					m_position[2] = (fabs(dx)>fabs(dy)?a/fabs(m_dx):b/fabs(m_dy))*m_dz;
				}
				m_position[0]=a, m_position[1]=b;
				glLightfv( GL_LIGHT0, GL_POSITION, m_position ) ;
			}
			break;
		default:
			return;
	}

	m_mx = x, m_my = y;
	glutPostRedisplay();
}

void CGLApp::mousePassiveMotionResponse(int x, int y)
{
}

void CGLApp::mouseWheelRollResponse(int wheel, int direction, int x, int y)
{
	m_cout << "mouse wheel rolling...\n";
	if (direction > 0) {
		keyResponse('-', x, y);
	}
	else {
		keyResponse('=', x, y);
	}
}

void CGLApp::display ( void )
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	draw();
	if ( m_bGadgetEnabled ) {
		drawGadgets();
	}
	trigger();
}

void CGLApp::onIdle ( void )
{
	// simulate key strokes randomly
	int idx = rand() % ARRAY_SIZE(g_keys);
	usleep(200000);
	this->keyResponse(g_keys[idx], 0, 0); 
}

void CGLApp::cleanup()
{
	this->~CGLApp();
}

int CGLApp::_setDefaultWindow()
{
	int ret = getScreenSize(&m_width, &m_height);
	if ( 0 != ret ) {
		m_cout << "failed in the invocation of getScreenSize\n";
		return ret;
	}

	if ( !m_bFullWindow ) {
		m_width /= 2;
		m_height /= 2;
		m_x = m_width / 2;
		m_y = m_height / 2;
	}
	else {
		m_y = m_x = 0;
	}

	return 0;
}

int CGLApp::_wincoord2objcoord(
		int winx, int winy, int winz,
		GLdouble *objx, GLdouble *objy, GLdouble *objz,
		bool notrans)
{
	GLint viewport[4];
	GLdouble mvmat[16], prjmat[16];

	glGetIntegerv(GL_VIEWPORT, viewport);
	if (notrans) {// using identity matrices 
		for (int i=0;i<4;++i) {
			for (int j=0;j<4;++j) {
				mvmat[ i*4+j ] = (i==j?1:0);
				prjmat[ i*4+j ] = (i==j?1:0);
			}
		}
	}
	else {
		glGetDoublev(GL_MODELVIEW_MATRIX, mvmat);
		glGetDoublev(GL_PROJECTION_MATRIX, prjmat);
	}

	winy = viewport[3] - (GLint)winy -1;

	if ( GL_TRUE != gluUnProject((GLdouble)winx, (GLdouble)winy, (GLdouble)winz,
								mvmat, prjmat, viewport,
								objx, objy, objz)) {
		m_cout << "_wincoord2objcoord failed with (" << winx << "," 
					<< winy << "," << winz << ")\n";
		return -1; // conversion failed, mostly due to the inrevertible matrix
	}

	if (notrans) {
		// scale the obj coordinate from the default (-1,1) grand-fixed system to
		// the object coordinate system
		*objx *= fabs(m_dx);
		*objy *= fabs(m_dy);
		*objz *= fabs(m_dz);
	}

	return 0;
}

void CGLApp::_mouseResponse(int button, int state, int x, int y)
{
	m_pInstance->mouseResponse(button, state, x, y);
}

void CGLApp::_onReshape( GLsizei w, GLsizei h )
{
	m_pInstance->onReshape(w, h);
}

void CGLApp::_keyResponse(unsigned char key, int x, int y)
{
	m_pInstance->keyResponse(key, x, y);
}

void CGLApp::_specialResponse(int key, int x, int y)
{
	m_pInstance->specialResponse(key, x, y);
}

void CGLApp::_mouseMotionResponse(int x, int y)
{
	m_pInstance->mouseMotionResponse(x, y);
}

void CGLApp::_mousePassiveMotionResponse(int x, int y)
{
	m_pInstance->mousePassiveMotionResponse(x, y);
}

void CGLApp::_mouseWheelRollResponse(int wheel, int direction, int x, int y)
{
	m_pInstance->mouseWheelRollResponse(wheel, direction, x, y);
}

void CGLApp::_display( void )
{
	m_pInstance->display();
}

void CGLApp::_onIdle( void )
{
	m_pInstance->onIdle();
}

/*set ts=4 tw=80 sts=4 sw=4*/

