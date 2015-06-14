#define STAR_SPEED 1		// 그리는 속도 (단위 : ms)
#define STAR_MOVEMENT 0.03	// 한번 움직일때 마다 이동하는 거리 (범위 : 0~1 사이)
#define STAR_RECTANGLE_SIZE 0.3				// 사각형 형체의 크기 (범위 : 0~1 사이)

#define STAR_SCALE 2.25		// 별크기
#define STAR_LOOP 2			// 루프 횟수

#define STAR_SIZE STAR_RECTANGLE_SIZE / (2 * 5)	// 사각형 내부에 그려질 별 크기 계산

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
	// 별모양을 유지하기 위해 사각형 형체 움직임 조정 (기본값에 최적화 됨)
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