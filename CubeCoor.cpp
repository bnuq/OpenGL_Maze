#include "CubeCoor.h"
#include <cmath>
/* 
    ���� ��ǥ�� ����, �� ��ǥ���� ��ġ ���� �����ϴ� �޼���
    ��ġ �� ���ſ� �����ߴٸ� true �� �����ϰ� ���̻� ������ ���ٸ� false �� �����Ѵ�.

    bool direction
        : ť�� ��ǥ�谡 �̵��� ����
        : true => x �� ���� �̵�
        : false => y �� ���� �̵�

    float length
        : �̵��ϰ��� �ϴ� �Ÿ�

    int value
        : �ѹ��� �̵��ϴ� ���� �̵���.

*/
bool CubeCoor::Move(bool direction, float length, float value) {
    /* 
        direction ���� �������� �����ϰ��� �ϴ� ����� �����Ѵ�.
        dirVal �� �����ϰ��� �ϴ� ��ġ ���� ���۷���
        direction == true => x �� ���� ��ġ ����
        direction == false => y �� ���� ��ġ ����
    */
    float& dirVal = (direction) ? this->x : this->y;

    /*
        ���� �̵��� �ѷ��� static ������ ����Ѵ�.
    */
    static float totalMove = 0;

    
    /*
        finish ������ �̿���, �̵��� ������ �� �ƴ����� Ȯ���Ѵ�
        if totalMove > length => �̵��� ������.
    */
    bool finish = true;
    if( totalMove > length ) finish = false;
    

    if(finish) { // ���� �̵��� ������ �ʾҴٸ�,
        /*
            value/10 �� ��ŭ �̵��� �ϰ� �ȴ�.
            �̵��ϴ� ���� dirVal �� ���Ͽ� ��ġ ���� �����Ѵ�.
            ���� �̵��ϴ� ���� ���밪�� totalMove �� ���Ͽ� �̵��� �ѷ��� üũ�Ѵ�.
            ��ġ �� ���ſ� ���������Ƿ�, true �� �����Ѵ�.
        */
        dirVal += value;
        totalMove += abs( value );

        return true;
    }
    else { // ���̻� �̵��� �� ���ٸ�,
        /*
            �־��� ���������� �̵��� �������Ƿ� totalMove �� ���� �ʱ�ȭ ��Ų��.
            dirVal ���� float Ÿ��������, ��Ȯ�� �� ���� ��ġ�ϵ��� �ϱ� ���� �ݿø����� �־� �����Ѵ�.
            ��ġ ���� ���̻� ���ŵ��� �����Ƿ� false �� �����Ѵ�.
        */
        totalMove = 0;
        dirVal = static_cast<float>(floor(dirVal+0.5f));
        return false;
    }
}
