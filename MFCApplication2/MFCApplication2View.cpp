
// MFCApplication2View.cpp : CMFCApplication2View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "MFCApplication2.h"
#endif

#include "MFCApplication2Doc.h"
#include "MFCApplication2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication2View

IMPLEMENT_DYNCREATE(CMFCApplication2View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication2View, CView)

ON_WM_CREATE()
ON_WM_DESTROY()
ON_WM_SIZE()

ON_COMMAND(IDC_ERASE_OBJECT, &CMFCApplication2View::OnEraseObject)
ON_COMMAND(IDC_DRAW_OBJECT, &CMFCApplication2View::OnDrawObject)
ON_COMMAND(IDC_BUTTON_TEAPOT, &CMFCApplication2View::OnButtonTeapot)
ON_COMMAND(IDC_BUTTON_SPHERE, &CMFCApplication2View::OnButtonSphere)
ON_COMMAND(IDC_BUTTON_SOLID_SPHERE, &CMFCApplication2View::OnButtonSolidSphere)
ON_COMMAND(IDC_BUTTON_CUBE, &CMFCApplication2View::OnButtonCube)
ON_COMMAND(IDC_BUTTON_COLOR_CUBE, &CMFCApplication2View::OnButtonColorCube)
ON_COMMAND(IDC_BUTTON_CONE, &CMFCApplication2View::OnButtonCone)
ON_COMMAND(IDC_BUTTON_CYLINDER, &CMFCApplication2View::OnButtonCylinder)
ON_COMMAND(IDC_JOLLA_DRAWING, &CMFCApplication2View::OnButtonjollaDrawing)

ON_COMMAND(IDC_BUTTON_mi10, &CMFCApplication2View::OnButtonmi10)
ON_COMMAND(IDC_BUTTON_pl10, &CMFCApplication2View::OnButtonpl10)
ON_COMMAND(IDC_BUTTON_ZOOM_OUT, &CMFCApplication2View::OnButtonZoomOut)
ON_COMMAND(IDC_BUTTON_ZOOM_IN, &CMFCApplication2View::OnButtonZoomIn)

ON_COMMAND(IDC_BUTTON_LEFT, &CMFCApplication2View::OnButtonLeft)
ON_COMMAND(IDC_BUTTON_RIGHT, &CMFCApplication2View::OnButtonRight)
ON_COMMAND(IDC_BUTTON_UP, &CMFCApplication2View::OnButtonUp)
ON_COMMAND(IDC_BUTTON_DOWN, &CMFCApplication2View::OnButtonDown)
ON_COMMAND(IDC_BUTTON_NEAR, &CMFCApplication2View::OnButtonNear)
ON_COMMAND(IDC_BUTTON_FAR, &CMFCApplication2View::OnButtonFar)

ON_COMMAND(IDC_BUTTON_HOME, &CMFCApplication2View::OnButtonHome)

ON_COMMAND(IDC_SLICES_PLUS_1, &CMFCApplication2View::OnButtonSlicesP1)
ON_COMMAND(IDC_SLICES_MINUS_1, &CMFCApplication2View::OnButtonSlicesM1)
ON_COMMAND(IDC_SLICES_PLUS_10, &CMFCApplication2View::OnButtonSlicesP10)
ON_COMMAND(IDC_SLICES_MINUS_10, &CMFCApplication2View::OnButtonSlicesM10)

ON_COMMAND(IDC_ROTATE_X, &CMFCApplication2View::OnRotateX)
ON_COMMAND(IDC_ROTATE_Y, &CMFCApplication2View::OnRotateY)
ON_COMMAND(IDC_ROTATE_Z, &CMFCApplication2View::OnRotateZ)
END_MESSAGE_MAP()

// CMFCApplication2View 생성/소멸
// MFC 화면이 보여질떄 m_iDrawType 를 0으로 초기화 하여 선을 그리지 않음
// starDraw_count (별모양을 그리는 선의 갯수) 역시 0으로 초기화 한다.
CMFCApplication2View::CMFCApplication2View()
{
	m_iDrawType = 0;
	starDraw_count = 0;
	m_myWidth = 0;
	m_myHeight = 0;
	rotate_d = 30;
	zoom = 0.5;
	trans_x = 0.0;
	trans_y = 0.0;
	trans_z = 0.0;
	slices = 40;
	slices_desk_leg = 8;

	is_rotate_x = 0;
	is_rotate_y = 0;
	is_rotate_z = 0;

	m_viewX = 10;
	m_viewY = 10;
	m_viewZ = 10;

	m_cntX = 0;
	m_cntY = 0;
	m_cntZ = 0;

	distC = 2.0;
	m_cameraTheta = 0.0f;
	m_cameraPhi = 0.0f;
	aspect = 0.0;

	fov = 60.0f;
	nearZ = 0.2 * distC;
	farZ = distC + 200.0f;

	m_lightPos[0] = 2000;
	m_lightPos[1] = 400;
	m_lightPos[2] = 1000;
	m_lightPos[3] = 1;		// 점광원임을 나타냄

	m_floorFlag = 0;
}

// CMFCApplication2View 그리기
void CMFCApplication2View::OnDraw(CDC* /*pDC*/)
{
	CMFCApplication2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// OpenGL 그리기 함수
	RenderScene();
}


// CMFCApplication2View 진단
#ifdef _DEBUG
void CMFCApplication2View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication2Doc* CMFCApplication2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication2Doc)));
	return (CMFCApplication2Doc*)m_pDocument;
}
#endif //_DEBUG


// 프로그램이 실행될떄 처리된다.
int CMFCApplication2View::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	int nPixelFormat;	// Pixel format
	m_hdc = ::GetDC(m_hWnd);

	static PIXELFORMATDESCRIPTOR pfd{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,	// Version
		// 스크린 상의 창에 그림 | 버퍼가 OpenGL 제공 | 더블 버퍼로 애니메이션이 가능
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		// Red Green Blue Alpha 모드 사용
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,	// alpha buffer
		0, 0, 0, 0, 0, 0, 0,	// accumulation buffer
		32,	// depth buffer bit
		0,	// stencil buffer
		0,	// assistance buffer
		PFD_MAIN_PLANE,	// Main layer type
		0,
		0, 0, 0
	};

	nPixelFormat = ChoosePixelFormat(m_hdc, &pfd);	// 정의한 포맷에 가장 가까운 픽셀 포맷 선택
	SetPixelFormat(m_hdc, nPixelFormat, &pfd);	// 가장 가까운 픽셀 포맷으로 DC를 정의
	m_hglrc = wglCreateContext(m_hdc);	// 렌더링 컨텍스트를 얻어옴.
	wglMakeCurrent(m_hdc, m_hglrc);		// RC와 DC를 연결
	
	m_viewX = m_cntX + distC * sin(PI * m_cameraTheta / 180)
		* cos(PI * m_cameraPhi / 180);
	m_viewY = m_cntZ + distC * sin(PI * m_cameraPhi / 180);
	m_viewZ = m_cntY + distC * cos(PI * m_cameraTheta / 180)
		* cos(PI * m_cameraPhi / 180);
	myglViewSetup();

	return 0;
}

// 프로그램을 마칠때 처리된다.
void CMFCApplication2View::OnDestroy()
{
	CView::OnDestroy();

	wglMakeCurrent(m_hdc, NULL);	// RC를 Current 에서 제외시킨다.
	wglDeleteContext(m_hglrc);		// RC 핸들객체를 시스템에 반환한다.
}

// 그리기 유형
enum DRAW_TYPE {
	ERASE,
	LINES,
	TEAPOT,
	SPHERE,
	SOLID_SPHERE,
	CUBE,
	COLOR_CUBE,
	CONE,
	CYLINDER,
	JOLLA_DRAWING
};

// OpenGL 그리기 함수
void CMFCApplication2View::RenderScene(void)
{
	static float matColor[] = { 1.0f, 0.2f, 1.2f, 1.0f };
	static float matSpecular [] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glClearColor(255, 255, 255, 0);	// 흰색 배경
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 버퍼 초기화
	glLoadIdentity();				// 좌표 초기화

	myglSetLight();
	myglViewSetup();

	//drawFloor(FLOOR_GRID);
	drawFloor(FLOOR_CHECK);

	glPushMatrix();

	glTranslatef(trans_x, trans_y, trans_z);

	// 회전
	if (is_rotate_x && !is_rotate_y && !is_rotate_z)		// x
		glRotatef(rotate_d, 1.0, 0.0, 0.0);
	else if (!is_rotate_x && is_rotate_y && !is_rotate_z)	// y
		glRotatef(rotate_d, 0.0, 1.0, 0.0);
	else if (!is_rotate_x && !is_rotate_y && is_rotate_z)	// z
		glRotatef(rotate_d, 0.0, 0.0, 1.0);
	else if (is_rotate_x && is_rotate_y && !is_rotate_z)	// x, y
		glRotatef(rotate_d, 1.0, 1.0, 0.0);
	else if (is_rotate_x && !is_rotate_y && is_rotate_z)	// x, z
		glRotatef(rotate_d, 1.0, 0.0, 1.0);
	else if (!is_rotate_x && is_rotate_y && is_rotate_z)	// y, z
		glRotatef(rotate_d, 0.0, 1.0, 1.0);
	else if (is_rotate_x && is_rotate_y && is_rotate_z)		// x, y, z
		glRotatef(rotate_d, 1.0, 1.0, 1.0);

	glScalef(zoom, zoom, zoom);			// 줌인 / 줌아웃
	glLineWidth(1.0);					// 라인 두께: 1
	switch (m_iDrawType) {
		case ERASE:
			break;
		case LINES:
			glLineWidth(2.0);		// 라인 두께: 2
			switch (starDraw_count) {
				case 0:	// 최초 클릭시, 선 1개 그림
					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(-1, 0.5);
					glVertex2f(1, 0.5);
					glEnd();

					starDraw_count++;
					break;

				case 1:	// 두번째 클릭시, 선 2개 그림
					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(-1, 0.5);
					glVertex2f(1, 0.5);
					glEnd();

					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(-1, 0.5);
					glVertex2f(0.7, -1);
					glEnd();

					starDraw_count++;
					break;

				case 2:	// 세번째 클릭시, 선 3개 그림
					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(-1, 0.5);
					glVertex2f(1, 0.5);
					glEnd();

					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(-1, 0.5);
					glVertex2f(0.7, -1);
					glEnd();

					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(0.7, -1);
					glVertex2f(0, 1);
					glEnd();

					starDraw_count++;
					break;

				case 3:	// 네번째 클릭시, 선 4개 그림
					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(-1, 0.5);
					glVertex2f(1, 0.5);
					glEnd();

					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(-1, 0.5);
					glVertex2f(0.7, -1);
					glEnd();

					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(0.7, -1);
					glVertex2f(0, 1);
					glEnd();

					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(-0.7, -1);
					glVertex2f(0, 1);
					glEnd();

					starDraw_count++;
					break;

				case 4:	// 다섯번째 클릭시, 선 5개 그림 (별모양 완성)
					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(-1, 0.5);
					glVertex2f(1, 0.5);
					glEnd();

					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(-1, 0.5);
					glVertex2f(0.7, -1);
					glEnd();

					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(0.7, -1);
					glVertex2f(0, 1);
					glEnd();

					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(-0.7, -1);
					glVertex2f(0, 1);

					glEnd();
					glColor3f(0.0f, 1.0f, 0.0f);
					glBegin(GL_LINES);
					glVertex2f(1, 0.5);
					glVertex2f(-0.7, -1);
					glEnd();
					starDraw_count++;
					break;

				default:	// 다섯번째 클릭 초과시 0으로 초기화
					starDraw_count = 0;
					break;
			}
			break;
		case TEAPOT:		// 차주전자
			glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
			glMaterialf(GL_FRONT, GL_SHININESS, 550.0f);
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matColor);

			glutSolidTeapot(0.5);
			break;
		case SPHERE:		// 구
			glColor3f(0.0f, 0.5f, 1.0f);
			myglWireSphere(slices);
			break;
		case SOLID_SPHERE:	// 솔리드 구
			glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
			glMaterialf(GL_FRONT, GL_SHININESS, 550.0f);
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matColor);

			myglSolidSphere(slices);
			break;
		case CUBE:			// 육방체
			glColor3f(0.5f, 0.5f, 1.0f);
			MyglWireCube();
			break;
		case COLOR_CUBE:	// 칼라육방체
			glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
			glMaterialf(GL_FRONT, GL_SHININESS, 550.0f);
			glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, matColor);

			MyglColorCube();
			break;
		case CONE:			// 원뿔
			glColor3f(0.8f, 0.5f, 0.0f);
			myglCone(slices);
			break;
		case CYLINDER:		// 원기둥
			glColor3f(0.7f, 0.5f, 0.3f);
			myglCylinder(slices);
			break;
		case JOLLA_DRAWING:	// 정물화
			jolla_Drawing();
			break;
		default:
			break;
	}

	glPopMatrix();

	glDisable(GL_LIGHTING);  //광원 main switch on
	//glDisable(GL_DEPTH_TEST); //Depth Buffer를 유효화한다
	glDisable(GL_NORMALIZE);

	// 다른 버튼 클릭시 선그리기 모양 남아있는 현상 수정
	if (m_iDrawType != LINES)
		starDraw_count = 0;

	SwapBuffers(m_hdc);
}

// 뷰 사이즈 지정
#define VIEW_SIZE 1.0f

// 윈도 창 사이즈가 바뀔시 호출된다.
void CMFCApplication2View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// 윈도 창 사이즈가 바뀔시 OpenGL 그리기 사이즈를 조정함
	float aspect;
	m_myWidth = cx;
	m_myHeight = cy;
	aspect = cx / (double)cy;
	glViewport(0, 0, m_myWidth, m_myHeight);
	if (cx > cy)
		glOrtho(-VIEW_SIZE * cx / cy, VIEW_SIZE * cx / cy, -VIEW_SIZE, VIEW_SIZE, 10.0, -10.0);
	else
		glOrtho(-VIEW_SIZE, VIEW_SIZE, VIEW_SIZE*cx / cy, VIEW_SIZE*cx / cy, 10.0, -10.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, aspect, nearZ, farZ);
	myglViewSetup();

	// 윈도 창 사이즈 바뀔시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;
}

// Erase 버튼 클릭시 호출된다.
void CMFCApplication2View::OnEraseObject()
{
	m_iDrawType = ERASE;
	starDraw_count = 0;	// 그려진 선의 갯수를 0으로 초기화
	InvalidateRect(NULL, FALSE);
}

// Draw Lines 버튼 클릭시 호출된다.
void CMFCApplication2View::OnDrawObject()
{
	m_iDrawType = LINES;
	InvalidateRect(NULL, FALSE);
}

// Teapot 버튼 클릭시 호출된다.
void CMFCApplication2View::OnButtonTeapot()
{
	m_iDrawType = TEAPOT;
	InvalidateRect(NULL, FALSE);
}

// Sphere 버튼 클릭시 호출된다.
void CMFCApplication2View::OnButtonSphere()
{
	m_iDrawType = SPHERE;
	InvalidateRect(NULL, FALSE);
}

// Solid Sphere 버튼 클릭시 호출된다.
void CMFCApplication2View::OnButtonSolidSphere()
{
	m_iDrawType = SOLID_SPHERE;
	InvalidateRect(NULL, FALSE);
}

// 육방체 각 점들의 좌표
GLfloat node[8][3] = {
	{ 0.5, 0.5, 0.5},	// 0
	{-0.5, 0.5, 0.5},	// 1
	{-0.5, -0.5, 0.5},	// 2
	{0.5, -0.5, 0.5},	// 3
	{0.5, 0.5, -0.5},	// 4
	{-0.5, 0.5, -0.5},	// 5
	{-0.5, -0.5, -0.5},	// 6
	{0.5, -0.5, -0.5}	// 7
};

// 위의 좌표 함수를 가져다가 면을 그리는 함수
void CMFCApplication2View::quad(int dot1, int dot2, int dot3, int dot4) {
	glBegin(GL_LINE_LOOP);
		glVertex3fv(node[dot1]);
		glVertex3fv(node[dot2]);
		glVertex3fv(node[dot3]);
		glVertex3fv(node[dot4]);
	glEnd();
}

// 위의 좌표 함수를 가져다가 무지개면을 그리는 함수
void CMFCApplication2View::color_quad(int dot1, int dot2, int dot3, int dot4) {
	glBegin(GL_POLYGON);
		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3fv(node[dot1]);
		glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3fv(node[dot2]);
		glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3fv(node[dot3]);
		glColor3f(1.0f, 1.0f, 0.0f);
		glVertex3fv(node[dot4]);
	glEnd();
}

// 육방체 선으로 그리는 함수
void CMFCApplication2View::MyglWireCube()
{
	quad(0, 1, 2, 3);
	quad(7, 4, 0, 3);
	quad(4, 5, 1, 0);
	quad(6, 5, 1, 2);
	quad(7, 6, 2, 3);
	quad(4, 5, 6, 7);
}

// 육방체 색깔 채워서 그리는 함수
void CMFCApplication2View::MyglColorCube()
{
	color_quad(0, 1, 2, 3);
	color_quad(7, 4, 0, 3);
	color_quad(4, 5, 1, 0);
	color_quad(6, 5, 1, 2);
	color_quad(7, 6, 2, 3);
	color_quad(4, 5, 6, 7);
}

void CMFCApplication2View::MyglSolidCube()
{
	color_quad(0, 1, 2, 3);
	color_quad(7, 4, 0, 3);
	color_quad(4, 5, 1, 0);
	color_quad(6, 5, 1, 2);
	color_quad(7, 6, 2, 3);
	color_quad(4, 5, 6, 7);
}

// Cube 버튼 클릭시 호출된다.
void CMFCApplication2View::OnButtonCube()
{
	m_iDrawType = CUBE;
	InvalidateRect(NULL, FALSE);
}

// ColorCube 버튼 클릭시 호출된다.
void CMFCApplication2View::OnButtonColorCube()
{
	m_iDrawType = COLOR_CUBE;
	InvalidateRect(NULL, FALSE);
}

// Cone 버튼 클릭시 호출된다.
void CMFCApplication2View::OnButtonCone()
{
	m_iDrawType = CONE;
	InvalidateRect(NULL, FALSE);
}

// Cylinder 버튼 클릭시 호출된다.
void CMFCApplication2View::OnButtonCylinder() {
	m_iDrawType = CYLINDER;
	InvalidateRect(NULL, FALSE);
}

// jolla-Drawing 버튼 클릭시 호출된다.
void CMFCApplication2View::OnButtonjollaDrawing() {
	m_iDrawType = JOLLA_DRAWING;
	InvalidateRect(NULL, FALSE);
}

// 원뿔 그리기 함수
void CMFCApplication2View::myglCone(short Nxy) {
	// 직경 1,  높이 1의 원기둥 (중심이 원점)
	float **p = NULL;		// 정점좌표
	double theta0, theta;
	short i, ii;

	// 슬라이스 갯수 메모리 동적 할당
	p = (float **)malloc(sizeof(float *) * Nxy * 2);
	for (i = 0; i < Nxy * 2; i++)
		p[i] = (float *)malloc(sizeof(float) * 3);

	theta0 = PI / (double)Nxy;

	for (i = 0; i < Nxy; i++) {
		theta = theta0 + 2.0 * theta0 * (double)i;

		p[i][0] = 0;		// 윗변의 x 성분
		p[i][1] = 0.3;		// y 성분
		p[i][2] = 0;		// z 성분 (높이)
		p[i + Nxy][0] = (float)(0.3 * sin(theta));	// 아랫변의 x 성분
		p[i + Nxy][1] = -0.5f;						// y 성분
		p[i + Nxy][2] = (float)(0.3 * cos(theta));	// z 성분
	}

	// 윗변
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < Nxy; i++)
		glVertex3fv(p[i]);
	glEnd();

	// 아랫변
	glBegin(GL_LINE_LOOP);
	for (i = 2 * Nxy - 1; i >= Nxy; i--)
		glVertex3fv(p[i]);
	glEnd();

	// 측면
	for (i = 0; i < Nxy; i++) {
		ii = i + 1;
		if (ii == Nxy)
			ii = 0;
		glBegin(GL_LINE_LOOP);
		glVertex3fv(p[i]);
		glVertex3fv(p[i + Nxy]);
		glVertex3fv(p[ii + Nxy]);
		glVertex3fv(p[ii]);
		glEnd();
	}

	// 슬라이스 갯수 할당 메모리 반납
	for (i = 0; i < Nxy * 2; i++)
		free(p[i]);
	free(p);
}

// 원기둥 그리기 함수
void CMFCApplication2View::myglCylinder(short Nxy) {
	// 직경 1,  높이 1의 원기둥 (중심이 원점)
	float **p = NULL;		// 정점좌표
	double theta0, theta;
	short i, ii;

	// 슬라이스 갯수 메모리 동적 할당
	p = (float **)malloc(sizeof(float *) * Nxy * 2);
	for (i = 0; i < Nxy * 2; i++)
		p[i] = (float *)malloc(sizeof(float) * 3);

	theta0 = PI / (double)Nxy;

	for (i = 0; i < Nxy; i++) {
		theta = theta0 + 2.0 * theta0 * (double)i;

		p[i][0] = (float)(0.5 * sin(theta));		// 윗변의 x 성분
		p[i][1] = 0.5f;								// y 성분
		p[i][2] = (float)(0.5 * cos(theta));		// z 성분 (높이)
		p[i + Nxy][0] = (float)(0.5 * sin(theta));	// 아랫변의 x 성분
		p[i + Nxy][1] = -0.5f;						// y 성분
		p[i + Nxy][2] = (float)(0.5 * cos(theta));	// z 성분
	}

	// 윗변
	glBegin(GL_LINE_LOOP);
		for (i = 0; i < Nxy; i++)
			glVertex3fv(p[i]);
	glEnd();

	// 아랫변
	glBegin(GL_LINE_LOOP);
		for (i = 2 * Nxy - 1; i >= Nxy; i--)
			glVertex3fv(p[i]);
	glEnd();

	// 측면
	for (i = 0; i < Nxy; i++) {
		ii = i + 1;
		if (ii == Nxy)
			ii = 0;
		glBegin(GL_LINE_LOOP);
			glVertex3fv(p[i]);
			glVertex3fv(p[i + Nxy]);
			glVertex3fv(p[ii + Nxy]);
			glVertex3fv(p[ii]);
		glEnd();
	}

	// 슬라이스 갯수 할당 메모리 반납
	for (i = 0; i < Nxy * 2; i++)
		free(p[i]);
	free(p);
}

// 단위 직경의 구체
void CMFCApplication2View::myglWireSphere(short Nzx) {
	short i, j, Ny;
	short **np = NULL;	// i, j 점의 정점번호 (i: z-x 단면 분할점, j: y 축 방향 분할점)
	double phai;		// z-x 평면에 대한 편각 (위도)
	double theta;		// z-y 평면에 대한 편각 (경도)
	float **p = NULL;	// 각 점의 좌표
	float p1[3], p2[3], p3[3], p4[3];

	Ny = Nzx;

	// 슬라이스 갯수 메모리 동적 할당
	np = (short **)malloc(sizeof(short *) * (Ny + 1));
	for (i = 0; i < (Ny + 1); i++)
		np[i] = (short *)malloc(sizeof(short) * (Nzx + 1));

	// 각점의 좌표 갯수 메모리 동적 할당
	p = (float **)malloc(sizeof(float *) * ((Nzx + 1) + (Nzx + 1) * (Ny + 1)));
	for (i = 0; i < (Nzx + 1) + (Nzx + 1) * (Ny + 1); i++)
		p[i] = (float *)malloc(sizeof(float) * 3);

	// 정점 번호, 면번호
	for (i = 0; i <= Nzx; i++)
		for (j = 0; j <= Ny; j++)
			np[i][j] = i + (Nzx + 1) * j;
	
	// 좌표
	for (i = 0; i <= Nzx; i++) {
		theta = 2.0 * PI * (double)i / (double)Nzx;
		for (j = 0; j <= Ny; j++) {
			phai = PI / 2.0 - PI * (double)j / (double)Ny;
			p[np[i][j]][0] = (float)(0.5 * cos(phai) * sin(theta));
			p[np[i][j]][1] = (float)(0.5 * sin(phai));
			p[np[i][j]][2] = (float)(0.5 * cos(phai) * cos(theta));
		}
	}

	// 정점면을 정의
	for (i = 0; i < Nzx; i++) {
		for (j = 0; j < Ny - 1; j++) {
			p1[0] = p[np[i][j]][0];		// x 좌표
			p1[1] = p[np[i][j]][1];		// y 좌표
			p1[2] = p[np[i][j]][2];		// z 좌표
			p2[0] = p[np[i][j + 1]][0];
			p2[1] = p[np[i][j + 1]][1];
			p2[2] = p[np[i][j + 1]][2];
			p3[0] = p[np[i + 1][j + 1]][0];
			p3[1] = p[np[i + 1][j + 1]][1];
			p3[2] = p[np[i + 1][j + 1]][2];
			p4[0] = p[np[i + 1][j]][0];
			p4[1] = p[np[i + 1][j]][1];
			p4[2] = p[np[i + 1][j]][2];

			glBegin(GL_LINE_LOOP);
				glVertex3fv(p1);
				glVertex3fv(p2);
				glVertex3fv(p3);
				glVertex3fv(p4);
			glEnd();
		}
	}

	// 변
	j = Ny - 1;
	for (i = 0; i < Nzx; i++) {
		// 면의 정범
		p1[0] = p[np[i][j]][0];
		p1[1] = p[np[i][j]][1];
		p1[2] = p[np[i][j]][2];
		p2[0] = p[np[i][j + 1]][0];
		p2[1] = p[np[i][j + 1]][1];
		p2[2] = p[np[i][j + 1]][2];
		p3[0] = p[np[i + 1][j]][0];
		p3[1] = p[np[i + 1][j]][1];
		p3[2] = p[np[i + 1][j]][2];

		glBegin(GL_LINE_LOOP);
			glVertex3fv(p1);
			glVertex3fv(p2);
			glVertex3fv(p3);
		glEnd();
	}

	// 슬라이스 갯수 할당 메모리 반납
	for (i = 0; i < (Ny + 1); i++)
		free(np[i]);
	free(np);
	
	// 각점의 좌표 갯수 할당 메모리 반납
	for (i = 0; i < (Nzx + 1) + (Nzx + 1) * (Ny + 1); i++)
		free(p[i]);
	free(p);
}

void CMFCApplication2View::myglSolidSphere(short Nzx) {
	short i, j, Ny;
	short **np = NULL;	// i, j 점의 정점번호 (i: z-x 단면 분할점, j: y 축 방향 분할점)
	double phai; //z-x평면에 대한 편각(위도)
	double theta; //z-y평면에 대한 편각(경도)
	float **p = NULL;	// 각 점의 좌표
	float p1[3], p2[3], p3[3], p4[3];

	Ny = Nzx;

	// 슬라이스 갯수 메모리 동적 할당
	np = (short **)malloc(sizeof(short *) * (Ny + 1));
	for (i = 0; i < (Ny + 1); i++)
		np[i] = (short *)malloc(sizeof(short) * (Nzx + 1));

	// 각점의 좌표 갯수 메모리 동적 할당
	p = (float **)malloc(sizeof(float *) * ((Nzx + 1) + (Nzx + 1) * (Ny + 1)));
	for (i = 0; i < (Nzx + 1) + (Nzx + 1) * (Ny + 1); i++)
		p[i] = (float *)malloc(sizeof(float) * 3);

	//정점번호 면번호 
	for (i = 0; i <= Nzx; i++)
		for (j = 0; j <= Ny; j++){
			np[i][j] = i + (Nzx + 1)*j;
		}

	//좌표
	for (i = 0; i <= Nzx; i++)
	{
		theta = 2.0*3.14*(double)i / (double)Nzx;
		for (j = 0; j <= Ny; j++)
		{
			phai = 3.14 / 2.0 - 3.14*(double)j / (double)Ny;
			p[np[i][j]][0] = (float)(0.5*cos(phai)*cos(theta));  //x좌A표¡Í
			p[np[i][j]][1] = (float)(0.5*cos(phai)*sin(theta));            //y좌A표¡Í
			p[np[i][j]][2] = (float)(0.5*sin(phai));  //z좌A표¡Í
		}
	}

	//정점열을 정의
	for (i = 0; i<Nzx; i++)
		for (j = 0; j<Ny - 1; j++)
		{ //면의 정점                                           
			p1[0] = p[np[i][j]][0]; //x좌A표¡Í
			p1[1] = p[np[i][j]][1]; //y좌A표¡Í
			p1[2] = p[np[i][j]][2]; //z좌A표¡Í
			p2[0] = p[np[i][j + 1]][0];
			p2[1] = p[np[i][j + 1]][1];
			p2[2] = p[np[i][j + 1]][2];
			p3[0] = p[np[i + 1][j + 1]][0];
			p3[1] = p[np[i + 1][j + 1]][1];
			p3[2] = p[np[i + 1][j + 1]][2];
			p4[0] = p[np[i + 1][j]][0];
			p4[1] = p[np[i + 1][j]][1];
			p4[2] = p[np[i + 1][j]][2];
			glBegin(GL_QUADS);
			glNormal3fv(p1);
			glVertex3fv(p1);
			glNormal3fv(p2);
			glVertex3fv(p2);
			glNormal3fv(p3);
			glVertex3fv(p3);
			glNormal3fv(p4);
			glVertex3fv(p4);
			glEnd();
		}

	//변
	j = Ny - 1;
	for (i = 0; i<Nzx; i++)
	{ //면의 정점
		p1[0] = p[np[i][j]][0];
		p1[1] = p[np[i][j]][1];
		p1[2] = p[np[i][j]][2];
		p2[0] = p[np[i][j + 1]][0];
		p2[1] = p[np[i][j + 1]][1];
		p2[2] = p[np[i][j + 1]][2];
		p3[0] = p[np[i + 1][j]][0];
		p3[1] = p[np[i + 1][j]][1];
		p3[2] = p[np[i + 1][j]][2];
		glBegin(GL_TRIANGLES);
		glNormal3fv(p1);
		glVertex3fv(p1);
		glNormal3fv(p2);
		glVertex3fv(p2);
		glNormal3fv(p3);
		glVertex3fv(p3);
		glEnd();
	}

	// 슬라이스 갯수 할당 메모리 반납
	for (i = 0; i < (Ny + 1); i++)
		free(np[i]);
	free(np);

	// 각점의 좌표 갯수 할당 메모리 반납
	for (i = 0; i < (Nzx + 1) + (Nzx + 1) * (Ny + 1); i++)
		free(p[i]);
	free(p);
}

// 꽃병 그리리 함수
void CMFCApplication2View::myglVase(short Nxy) {
	// 직경 1,  높이 1의 원기둥 (중심이 원점)
	float **vase_up;		// 윗 부분
	float **vase_mid;		// 꽃 병 오므려 지는 부분
	float **vase_down;		// 아랫 부분
	double theta0, theta;
	short i, ii;

	// 슬라이스 갯수 메모리 동적 할당
	vase_up = (float **)malloc(sizeof(float *) * Nxy * 2);
	for (i = 0; i < Nxy * 2; i++)
		vase_up[i] = (float *)malloc(sizeof(float) * 3);

	vase_mid = (float **)malloc(sizeof(float *) * Nxy * 2);
	for (i = 0; i < Nxy * 2; i++)
		vase_mid[i] = (float *)malloc(sizeof(float) * 3);

	vase_down = (float **)malloc(sizeof(float *) * Nxy * 2);
	for (i = 0; i < Nxy * 2; i++)
		vase_down[i] = (float *)malloc(sizeof(float) * 3);

	theta0 = PI / (double)Nxy;

	////////////////////////////////////////////////////////////////////
	// 윗부분
	for (i = 0; i < Nxy; i++) {
		theta = theta0 + 2.0 * theta0 * (double)i;

		vase_up[i][0] = (float)(0.2 * sin(theta));		// 윗변의 x 성분
		vase_up[i][1] = 0.6f;							// y 성분
		vase_up[i][2] = (float)(0.2 * cos(theta));		// z 성분 (높이)
		vase_up[i + Nxy][0] = (float)(0.1 * sin(theta));	// 아랫변의 x 성분
		vase_up[i + Nxy][1] = 0.3f;						// y 성분
		vase_up[i + Nxy][2] = (float)(0.1 * cos(theta));	// z 성분
	}

	// 윗변
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < Nxy; i++)
		glVertex3fv(vase_up[i]);
	glEnd();

	// 아랫변
	glBegin(GL_LINE_LOOP);
	for (i = 2 * Nxy - 1; i >= Nxy; i--)
		glVertex3fv(vase_up[i]);
	glEnd();

	// 측면
	for (i = 0; i < Nxy; i++) {
		ii = i + 1;
		if (ii == Nxy)
			ii = 0;
		glBegin(GL_LINE_LOOP);
		glVertex3fv(vase_up[i]);
		glVertex3fv(vase_up[i + Nxy]);
		glVertex3fv(vase_up[ii + Nxy]);
		glVertex3fv(vase_up[ii]);
		glEnd();
	}
	////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////
	// 중간 부분
	for (i = 0; i < Nxy; i++) {
		theta = theta0 + 2.0 * theta0 * (double)i;

		vase_mid[i][0] = (float)(0.1 * sin(theta));		// 윗변의 x 성분
		vase_mid[i][1] = 0.3f;								// y 성분
		vase_mid[i][2] = (float)(0.1 * cos(theta));		// z 성분 (높이)
		vase_mid[i + Nxy][0] = (float)(0.1 * sin(theta));	// 아랫변의 x 성분
		vase_mid[i + Nxy][1] = 0.1f;						// y 성분
		vase_mid[i + Nxy][2] = (float)(0.1 * cos(theta));	// z 성분
	}

	// 윗변
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < Nxy; i++)
		glVertex3fv(vase_mid[i]);
	glEnd();

	// 아랫변
	glBegin(GL_LINE_LOOP);
	for (i = 2 * Nxy - 1; i >= Nxy; i--)
		glVertex3fv(vase_mid[i]);
	glEnd();

	// 측면
	for (i = 0; i < Nxy; i++) {
		ii = i + 1;
		if (ii == Nxy)
			ii = 0;
		glBegin(GL_LINE_LOOP);
		glVertex3fv(vase_mid[i]);
		glVertex3fv(vase_mid[i + Nxy]);
		glVertex3fv(vase_mid[ii + Nxy]);
		glVertex3fv(vase_mid[ii]);
		glEnd();
	}
	////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////
	// 아랫 부분
	for (i = 0; i < Nxy; i++) {
		theta = theta0 + 2.0 * theta0 * (double)i;

		vase_down[i][0] = (float)(0.1 * sin(theta));		// 윗변의 x 성분
		vase_down[i][1] = 0.1f;								// y 성분
		vase_down[i][2] = (float)(0.1 * cos(theta));		// z 성분 (높이)
		vase_down[i + Nxy][0] = (float)(0.4 * sin(theta));	// 아랫변의 x 성분
		vase_down[i + Nxy][1] = -0.5f;						// y 성분
		vase_down[i + Nxy][2] = (float)(0.4 * cos(theta));	// z 성분
	}

	// 윗변
	glBegin(GL_LINE_LOOP);
	for (i = 0; i < Nxy; i++)
		glVertex3fv(vase_down[i]);
	glEnd();

	// 아랫변
	glBegin(GL_LINE_LOOP);
	for (i = 2 * Nxy - 1; i >= Nxy; i--)
		glVertex3fv(vase_down[i]);
	glEnd();

	// 측면
	for (i = 0; i < Nxy; i++) {
		ii = i + 1;
		if (ii == Nxy)
			ii = 0;
		glBegin(GL_LINE_LOOP);
		glVertex3fv(vase_down[i]);
		glVertex3fv(vase_down[i + Nxy]);
		glVertex3fv(vase_down[ii + Nxy]);
		glVertex3fv(vase_down[ii]);
		glEnd();
	}
	////////////////////////////////////////////////////////////////////


	// 슬라이스 갯수 할당 메모리 반납
	for (i = 0; i < Nxy * 2; i++)
		free(vase_up[i]);
	free(vase_up);

	for (i = 0; i < Nxy * 2; i++)
		free(vase_mid[i]);
	free(vase_mid);

	for (i = 0; i < Nxy * 2; i++)
		free(vase_down[i]);
	free(vase_down);
}

// 책상 다리 그리기 함수
void CMFCApplication2View::myglDesk_Leg(void) {
	glPushMatrix();
	glScalef(0.3, 0.5, 0.3);
	myglCylinder(slices_desk_leg);
	glPopMatrix();
}

// 책상 그리기 함수
void CMFCApplication2View::myglDesk(void) {
	// 책상 윗면
	glPushMatrix();
	glTranslatef(-0.15f, -0.65f, 0.0f);
	glScalef(3.0, 0.3, 1.7);
	MyglWireCube();
	glPopMatrix();

	// 책상 다리 1 (좌상)
	glPushMatrix();
	glTranslatef(-1.3f, -1.05f, -0.5f);
	myglDesk_Leg();
	glPopMatrix();

	// 책상 다리 2 (좌하)
	glPushMatrix();
	glTranslatef(-1.3f, -1.05f, 0.5f);
	myglDesk_Leg();
	glPopMatrix();

	// 책상 다리 3 (우상)
	glPushMatrix();
	glTranslatef(1.0f, -1.05f, -0.5f);
	myglDesk_Leg();
	glPopMatrix();

	// 책상 다리 4 (우하)
	glPushMatrix();
	glTranslatef(1.0f, -1.05f, 0.5f);
	myglDesk_Leg();
	glPopMatrix();
}

// 정물화 그리기 함수
void CMFCApplication2View::jolla_Drawing(void) {
	// 꽃병 그리기
	glColor3f(0.5f, 0.7f, 0.0f);
	glPushMatrix();
	glTranslatef(-1.0f, 0.0f, 0.0f);
	myglVase(slices);	
	glPopMatrix();

	// 공 그리기
	glColor3f(0.0f, 0.7f, 0.7f);
	glPushMatrix();
	glTranslatef(1.0f, -0.35f, 0.0f);
	glScalef(0.3, 0.3, 0.3);
	myglWireSphere(slices);
	glPopMatrix();

	// 책상 그리기
	glColor3f(1.0f, 0.6f, 0.0f);
	glPushMatrix();
	myglDesk();
	glPopMatrix();
}

// 광원 설정 함수
void CMFCApplication2View::myglSetLight(void) {
	static float lightDiffuse0[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //확산광 데이터
	static float lightSpecular0[] = { 1.0f, 1.0f, 1.0f, 1.0f };//경면광 데이터
	static float lightAmbient0[] = { 0.4f, 0.4f, 0.4f, 1.0f }; //환경광 데이터
	static float lightPos[4];  //광원 위치

	//광원(VB측에서 설정)
	lightPos[0] = m_lightPos[11];//x좌표
	lightPos[1] = m_lightPos[12];//y좌표
	lightPos[2] = m_lightPos[13];//z좌표
	lightPos[3] = m_lightPos[14]; //종류

	//광원의 정의
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightDiffuse0);  //확산광
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);//경면광
	glLightfv(GL_LIGHT0, GL_AMBIENT, lightAmbient0);  //환경광      
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);//광원위치

	glEnable(GL_LIGHTING);  //광원 main switch on
	glEnable(GL_LIGHT0);    //Light0 switch on
	glClear(GL_DEPTH_BUFFER_BIT); //Depth Buffer를 클리어
	glEnable(GL_DEPTH_TEST); //Depth Buffer를 유효화한다
	glShadeModel(GL_SMOOTH);  //Smooth Shading
	glEnable(GL_NORMALIZE);
}

// 바닥 그리기 함수
void CMFCApplication2View::drawFloor(short m_floorFlag) {
	double a;
	short i, j, nx, nz;
	float widG, widFloorX, widFloorZ, x0, x1, z0, z1;
	static float floorSpecular[] = { 0.1f, 0.1f, 0.1f, 1.0f };	// 경면광
	static float floorDiffuse1[] = { 0.8f, 0.5f, 0.3f, 1.0f };	// 바닥 - 확산광
	static float floorDiffuse2[] = { 0.3f, 0.5f, 0.8f, 1.0f };	// 바닥 - 확산광
	static float gridDiffuse[] = { 0.2f, 0.2f, 0.2f, 1.0f };	// grid - 확산관

	float floor_height = -2.0f;

	widG = 5.0f;	// Check 모양(Grid)의 폭
	widFloorX = 1200.0f;
	widFloorZ = 1200.0f;
	nx = (short)(widFloorX / widG);
	nz = (short)(widFloorZ / widG);

	glPushAttrib(GL_LIGHTING_BIT | GL_CURRENT_BIT);
	glMaterialfv(GL_FRONT, GL_SPECULAR, floorSpecular);
	glMaterialf(GL_FRONT, GL_SHININESS, 100.0f);

	switch (m_floorFlag) {
		case FLOOR_CHECK:
			for (j = 0; j < nz; j++) {
				for (i = 0; i < nx; i++) {
					a = (double)(i + j);
					if (fmod(a, 2.0) == 0.0)
						glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, floorDiffuse1);
					else
						glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, floorDiffuse2);
					glPushMatrix();
					x0 = -widFloorX / 2.0f + (float)i * widG + widG / 2.0f;
					z0 = -widFloorZ / 2.0f + (float)j * widG + widG / 2.0f;
					glTranslatef(x0, floor_height, z0);
					glBegin(GL_QUADS);
					glNormal3f(0.0f, 1.0f, 0.0f);
					glVertex3f(widG / 2.0f, 0.0f, widG / 2.0f);
					glVertex3f(widG / 2.0f, 0.0f, -widG / 2.0f);
					glVertex3f(-widG / 2.0f, 0.0f, -widG / 2.0f);
					glVertex3f(-widG / 2.0f, 0.0f, widG / 2.0f);
					glEnd();
					glPopMatrix();
				}
			}
			break;

		case FLOOR_GRID:
			glMaterialfv(GL_FRONT, GL_AMBIENT, gridDiffuse);

			// x축에 평행한 선
			x0 = widFloorX / 2.0f;
			x1 = -widFloorX / 2.0f;

			for (j = 0; j <= nz; j++) {
				z1 = -widFloorZ / 2.0f + (float)j * widG;
				glBegin(GL_LINES);
				glVertex3f(x0, floor_height, z1);
				glVertex3f(x1, floor_height, z1);
				glEnd();
			}

			// z축에 평행한 선
			z0 = widFloorZ / 2.0f;
			z1 = -widFloorZ / 2.0f;

			for (j = 0; j <= nx; j++) {
				x1 = -widFloorZ / 2.0f + (float)j * widG;
				glBegin(GL_LINES);
				glVertex3f(x1, floor_height, z0);
				glVertex3f(x1, floor_height, z1);
				glEnd();
			}
			break;

		default:
			break;
	}
	glPopAttrib();
}

// -10 버튼 클릭시 호출된다.
void CMFCApplication2View::OnButtonmi10()
{
	// 방향이 반대로 이므로 부호를 반대로 함
	rotate_d += 10;

	// 360 도 배수로 회전 되면 본래 좌표로 돌아오므로 0으로 초기화
	if (rotate_d % 360 == 0)
		rotate_d = 0;

	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

// +10 버튼 클릭시 호출된다.
void CMFCApplication2View::OnButtonpl10()
{
	// 방향이 반대로 이므로 부호를 반대로 함
	rotate_d -= 10;

	// 360 도 배수로 회전 되면 본래 좌표로 돌아오므로 0으로 초기화
	if (rotate_d % 360 == 0)
		rotate_d = 0;

	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

// Zoom In 버튼 클릭시 호출된다.
void CMFCApplication2View::OnButtonZoomIn()
{
	zoom += 0.05;

	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

// Zoom Out 버튼 클릭시 호출된다.
void CMFCApplication2View::OnButtonZoomOut()
{
	// zoom 이 더 작아 지면서 x, y, z 가 반전되는 현상 방지
	if (zoom < 0.1)
		goto redraw;

	zoom -= 0.05;

redraw:
	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

void CMFCApplication2View::OnButtonLeft() {
	trans_x -= 0.1;

	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

void CMFCApplication2View::OnButtonRight() {
	trans_x += 0.1;

	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

void CMFCApplication2View::OnButtonUp() {
	trans_y += 0.1;

	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

void CMFCApplication2View::OnButtonDown() {
	trans_y -= 0.1;

	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

void CMFCApplication2View::OnButtonNear() {
	trans_z += 0.1;

	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

void CMFCApplication2View::OnButtonFar() {
	trans_z -= 0.1;

	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

// Home 버튼 클릭시 호출된다.
void CMFCApplication2View::OnButtonHome()
{
	rotate_d = 30;
	zoom = 0.5;
	trans_x = 0.0;
	trans_y = 0.0;
	trans_z = 0.0;
	slices = 40;
	slices_desk_leg = 8;

	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

// Slices +1 버튼 클릭시 호출된다. (슬라이스 갯수 1 증가)
void CMFCApplication2View::OnButtonSlicesP1() {
	slices += 1;
	slices_desk_leg += 1;

	// 오버 플로우 방지
	if (slices > 150)
		slices--;
	if (slices_desk_leg > 150)
		slices_desk_leg--;

	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

// Slices -1 버튼 클릭시 호출된다. (슬라이스 갯수 1 감소)
void CMFCApplication2View::OnButtonSlicesM1() {
	slices -= 1;
	slices_desk_leg -= 1;

	// 슬라이스 갯수가 0 이하로 되는 경우 방지
	if (slices < 1)
		slices++;
	if (slices_desk_leg < 1)
		slices_desk_leg++;

	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

// Slices +10 버튼 클릭시 호출된다. (슬라이스 갯수 10 증가)
void CMFCApplication2View::OnButtonSlicesP10() {
	slices += 10;
	slices_desk_leg += 10;

	// 오버 플로우 방지
	if (slices > 150)
		slices -= 10;
	if (slices_desk_leg > 150)
		slices_desk_leg -= 10;

	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

// Slices -10 버튼 클릭시 호출된다. (슬라이스 갯수 10 감소)
void CMFCApplication2View::OnButtonSlicesM10() {
	slices -= 10;
	slices_desk_leg -= 10;

	// 슬라이스 갯수가 0 이하로 되는 경우 방지
	if (slices < 1)
		slices += 10;
	if (slices_desk_leg < 1)
		slices_desk_leg += 10;

	// 버튼 클릭시 별을 그린 선의 갯수가 증가하는 버그 수정
	if (starDraw_count)
		starDraw_count--;

	InvalidateRect(NULL, FALSE);
}

void CMFCApplication2View::OnRotateX()
{
	if (is_rotate_x)
		is_rotate_x = 0;
	else
		is_rotate_x = 1;
}

void CMFCApplication2View::OnRotateY()
{
	if (is_rotate_y)
		is_rotate_y = 0;
	else
		is_rotate_y = 1;
}

void CMFCApplication2View::OnRotateZ()
{
	if (is_rotate_z)
		is_rotate_z = 0;
	else
		is_rotate_z = 1;
}

void CMFCApplication2View::myglViewSetup() {
	float up[3];
	float norm;
	int i;

	// 시점과 보는 자세
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// View Vector 의 계산
	up[0] = -(m_cntX - m_viewX) * (m_cntY - m_viewY);
	up[1] = (m_cntX - m_viewX) * (m_cntX - m_viewX)
		+ (m_cntZ - m_viewZ) * (m_cntZ - m_viewZ);
	up[2] = -(m_cntZ - m_viewZ) * (m_cntY - m_viewY);
	norm = sqrt(up[0] * up[0] + up[1] * up[1] + up[2] * up[2]);
	for (i = 0; i < 3; i++)
		up[i] /= norm;

	// 시점과 보는 자세
	gluLookAt(m_viewX, m_viewY, m_viewZ, m_cntX, m_cntY, m_cntZ, 0, 1, 0);
}