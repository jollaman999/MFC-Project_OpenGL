
// MFCApplication2View.h : CMFCApplication2View Ŭ������ �������̽�
//

// OpenGL GL Utils ������� ��Ŭ���
#include <glut.h>

#pragma once

#define PI 3.14159265

enum FLOOR_SWITCH {
	FLOOR_GRID,
	FLOOR_CHECK
};

class CMFCApplication2View : public CView
{
protected: // serialization������ ��������ϴ�.
	CMFCApplication2View();
	DECLARE_DYNCREATE(CMFCApplication2View)

public:
	CMFCApplication2Doc* GetDocument() const;
	virtual void OnDraw(CDC* pDC);  // �� �並 �׸��� ���� �����ǵǾ����ϴ�.

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// ������ �޽��� �� �Լ�
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

	int m_iDrawType;		// ���� ��ư�� ���� 1, 0�� ���� ����, ȭ�鿡 ���� �׸��� ���� �Ǵ�
	int starDraw_count;		// ������� �׸��� ���� ���� ī��Ʈ
	int rotate_d;			// ȸ�� ����
	float zoom;				// ���� / �ܾƿ�
	float trans_x, trans_y, trans_z;
	int slices;				// �����̽� ����
	int slices_desk_leg;	// å�� �ٸ� �����̽� ����
	void RenderScene();		// GL ���� �׸��� �Լ�

	// ȸ���� Ȱ��ȭ(1) / ��Ȱ��ȭ(0)
	int is_rotate_x;
	int is_rotate_y;
	int is_rotate_z;

	void myglViewSetup();

	// ī�޶� ��ġ
	float m_viewX;
	float m_viewY;
	float m_viewZ;

	// ī�޶� �ֽ���
	float m_cntX;
	float m_cntY;
	float m_cntZ;

	float distC;			// ī�޶󿡼� ������ ������ �Ÿ�
	float m_cameraTheta;	// ī�޶� ������
	float m_cameraPhi;		// ī�޶� �簢
	float aspect;

	float fov;		// ȭ�� (field of view)
	float nearZ;	// ���ܸ��� ��ġ
	float farZ;		// �Ĵܸ��� ��ġ

	float m_lightPos[4];	// ������ ��ġ �� ����

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
	void MyglWireCube();			// ����ü �׸��� �Լ�
	void MyglColorCube();			// ������ ���ü �׸��� �Լ�
	void MyglSolidCube();			// �ָ��� ���ü �׸��� �Լ�
	void myglCone(short Nxy);		// ���� �׸��� �Լ�
	void myglCylinder(short Nxy);	// ����� �׸��� �Լ�
	void myglWireSphere(short Nzx);	// ��ü �׸��� �Լ�
	void myglSolidSphere(short Nzx);	// ��ü �׸��� �Լ�
	void myglVase(short Nxy);		// �ɺ� �׸��� �Լ�
	void myglDesk_Leg();			// å�� �ٸ� �׸��� �Լ�
	void myglDesk();				// å�� �׸��� �Լ�
	void jolla_Drawing();			// ����ȭ �׸��� �Լ�

	void myglSetLight();			// ���� ���� �Լ�

	void drawFloor(short m_floorFlag);	// �ٴ� �׸��� �Լ�

	afx_msg void OnButtonmi10();	// �·� 10�� ȸ��
	afx_msg void OnButtonpl10();	// ��� 10�� ȸ��
	afx_msg void OnButtonZoomIn();	// Ȯ��
	afx_msg void OnButtonZoomOut();	// ���
	afx_msg void OnButtonHome();	// ���� ��ǥ�� ����

	afx_msg void OnButtonLeft();	// x--
	afx_msg void OnButtonRight();	// x++
	afx_msg void OnButtonUp();		// y++
	afx_msg void OnButtonDown();	// y--
	afx_msg void OnButtonNear();	// z++
	afx_msg void OnButtonFar();		// z--

	afx_msg void OnButtonSlicesP1();	// �����̽� ���� +1
	afx_msg void OnButtonSlicesM1();	// �����̽� ���� -1
	afx_msg void OnButtonSlicesP10();	// �����̽� ���� +10
	afx_msg void OnButtonSlicesM10();	// �����̽� ���� -10

	afx_msg void OnRotateX();
	afx_msg void OnRotateY();
	afx_msg void OnRotateZ();
};

#ifndef _DEBUG  // MFCApplication2View.cpp�� ����� ����
inline CMFCApplication2Doc* CMFCApplication2View::GetDocument() const
   { return reinterpret_cast<CMFCApplication2Doc*>(m_pDocument); }
#endif

