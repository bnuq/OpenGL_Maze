/*
    Cube �ϳ��� �׷��� �� ��ǥ��, Modeling Coordinate System �� ��ġ�� ��Ÿ���� Ŭ����
    �� Ŭ������ �̿��Ͽ� �� ť���� ��ǥ�� ��ȯ�� �ʿ��� ��ǥ���� ����Ѵ�.
*/
class CubeCoor {
public:
    CubeCoor(float x, float y, float z) : x(x), y(y), z(z) {}
    
    // ���� ��ǥ�� ����, �� ��ǥ���� ��ġ ���� �����ϴ� �޼���
    bool Move(bool direction, float length, float value);

    void Set(float x, float y, float z) {
        this->x = x; this->y = y; this->z = z;
    }

    float getX() { return x; }
    float getY() { return y; }
    float getZ() { return z; }
private:
    // ���� ��ǥ�迡�� Cube �� ��ǥ���� ���� ��ǥ
    float x, y, z;
};
