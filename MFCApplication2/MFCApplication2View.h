
// MFCApplication2View.h : CMFCApplication2View 클래스의 인터페이스
//

// OpenGL GL Utils 헤더파일 인클루드
#include <glut.h>

#pragma once

#define PI 3.14159265

enum FLOOR_SWITCH {
	FLOOR_GRID,
	FLOOR_CHECK
};

class CMFCApplication2View : public CView
{
protected: // serialization에서만 만들어집니다.
	CMFCApplication2View();
	DECLARE_DYNCREATE(CMFCApplication2View)

public:
	CMFCApplication2Doc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	HDC m_hdc;
	HGLRC m_hglrc;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

	int m_myWidth;
	int m_myHeight;
	afx_msg void OnSize(UINT nType, int cx, int cy);

	int m_iDrawType;		// 누른 버튼에 따라 1, 0의 값을 저장, 화면에 선을 그릴지 여부 판단
	int starDraw_count;		// 별모양을 그리는 선의 갯수 카운트
	int rotate_d;			// 회전 각도
	float zoom;				// 줌인 / 줌아웃
	float trans_x, trans_y, trans_z;
	int slices;				// 슬라이스 갯수
	int slices_desk_leg;	// 책상 다리 슬라이스 갯수
	void RenderScene();		// GL 메인 그리기 함수

	// 회전축 활성화(1) / 비활성화(0)
	int is_rotate_x;
	int is_rotate_y;
	int is_rotate_z;

	void myglViewSetup();

	// 카메라 위치
	float m_viewX;
	float m_viewY;
	float m_viewZ;

	// 카메라 주시점
	float m_cntX;
	float m_cntY;
	float m_cntZ;

	float distC;			// 카메라에서 응시점 까지의 거리
	float m_cameraTheta;	// 카메라 방위각
	float m_cameraPhi;		// 카메라 양각
	float aspect;

	float fov;		// 화각 (field of view)
	float nearZ;	// 전단면의 위치
	float farZ;		// 후단면의 위치

	float m_lightPos[4];	// 광원의 위치 및 종류

	short m_floorFlag;

	afx_msg void OnEraseObject();
	afx_msg void OnDrawObject();
	afx_msg void OnButtonTeapot();
	afx_msg void OnButtonSphere();
	afx_msg void OnButtonSolidSphere();
	afx_msg void OnButtonCube();
	afx_msg void OnButtonColorCube();
	afx_msg void OnButtonCone();
	afx_msg void OnButtonCylinder();
	afx_msg void OnButtonjollaDrawing();

	void quad(int dot1, int dot2, int dot3, int dot4);
	void color_quad(int dot1, int dot2, int dot3, int dot4);
	void MyglWireCube();			// 육방체 그리기 함수
	void MyglColorCube();			// 무지개 욱방체 그리기 함수
	void MyglSolidCube();			// 솔리드 욱방체 그리기 함수
	void myglCone(short Nxy);		// 원뿔 그리기 함수
	void myglCylinder(short Nxy);	// 원기둥 그리기 함수
	void myglWireSphere(short Nzx);	// 구체 그리기 함수
	void myglSolidSphere(short Nzx);	// 구체 그리기 함수
	void myglVase(short Nxy);		// 꽃병 그리기 함수
	void myglDesk_Leg();			// 책상 다리 그리기 함수
	void myglDesk();				// 책상 그리기 함수
	void jolla_Drawing();			// 정물화 그리기 함수

	void myglSetLight();			// 광원 설정 함수

	void drawFloor(short m_floorFlag);	// 바닥 그리기 함수

	afx_msg void OnButtonmi10();	// 좌로 10도 회전
	afx_msg void OnButtonpl10();	// 우로 10도 회전
	afx_msg void OnButtonZoomIn();	// 확대
	afx_msg void OnButtonZoomOut();	// 축소
	afx_msg void OnButtonHome();	// 본래 좌표로 복구

	afx_msg void OnButtonLeft();	// x--
	afx_msg void OnButtonRight();	// x++
	afx_msg void OnButtonUp();		// y++
	afx_msg void OnButtonDown();	// y--
	afx_msg void OnButtonNear();	// z++
	afx_msg void OnButtonFar();		// z--

	afx_msg void OnButtonSlicesP1();	// 슬라이스 갯수 +1
	afx_msg void OnButtonSlicesM1();	// 슬라이스 갯수 -1
	afx_msg void OnButtonSlicesP10();	// 슬라이스 갯수 +10
	afx_msg void OnButtonSlicesM10();	// 슬라이스 갯수 -10

	afx_msg void OnRotateX();
	afx_msg void OnRotateY();
	afx_msg void OnRotateZ();
};

#ifndef _DEBUG  // MFCApplication2View.cpp의 디버그 버전
inline CMFCApplication2Doc* CMFCApplication2View::GetDocument() const
   { return reinterpret_cast<CMFCApplication2Doc*>(m_pDocument); }
#endif

