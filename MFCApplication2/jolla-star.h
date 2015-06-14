#define STAR_SPEED 1		// �׸��� �ӵ� (���� : ms)
#define STAR_MOVEMENT 0.03	// �ѹ� �����϶� ���� �̵��ϴ� �Ÿ� (���� : 0~1 ����)
#define STAR_RECTANGLE_SIZE 0.3				// �簢�� ��ü�� ũ�� (���� : 0~1 ����)

#define STAR_SCALE 2.25		// ��ũ��
#define STAR_LOOP 2			// ���� Ƚ��

#define STAR_SIZE STAR_RECTANGLE_SIZE / (2 * 5)	// �簢�� ���ο� �׷��� �� ũ�� ���

struct comma {
	float x;
	float y;
	float z;
};

static struct comma _rec_1;
static struct comma _rec_2;
static struct comma _rec_3;
static struct comma _rec_4;
static struct comma _rec_5;
static struct comma _rec_6;
static struct comma _rec_7;
static struct comma _rec_8;
static struct comma _rec_9;
static struct comma _rec_10;

void draw_star(int i) {
	// ������� �����ϱ� ���� �簢�� ��ü ������ ���� (�⺻���� ����ȭ ��)
	float a = STAR_MOVEMENT / 14;

	if (i) { // Rectangle to Star
		_rec_2.x += a * 2;
		_rec_2.y -= a;

		_rec_4.x += a;
		_rec_4.y += a * 2;

		_rec_6.y += a * 2;

		_rec_8.x -= a;
		_rec_8.y += a * 2;

		_rec_10.x -= a * 2;
		_rec_10.y -= a;
	} else { // Star to Rectangle
		_rec_2.x -= a * 2;
		_rec_2.y += a;

		_rec_4.x -= a;
		_rec_4.y -= a * 2;

		_rec_6.y -= a * 2;

		_rec_8.x += a;
		_rec_8.y -= a * 2;

		_rec_10.x += a * 2;
		_rec_10.y += a;

	}
}