/*
    Cube 하나가 그려질 모델 좌표계, Modeling Coordinate System 의 위치를 나타내는 클래스
    이 클래스를 이용하여 각 큐브의 좌표계 변환에 필요한 좌표값을 계산한다.
*/
class CubeCoor {
public:
    CubeCoor(float x, float y, float z) : x(x), y(y), z(z) {}
    
    // 전역 좌표계 기준, 모델 좌표계의 위치 값을 갱신하는 메서드
    bool Move(bool direction, float length, float value);

    void Set(float x, float y, float z) {
        this->x = x; this->y = y; this->z = z;
    }

    float getX() { return x; }
    float getY() { return y; }
    float getZ() { return z; }
private:
    // 전역 좌표계에서 Cube 모델 좌표계의 원점 좌표
    float x, y, z;
};
