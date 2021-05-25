#include "CubeCoor.h"
#include <cmath>
/* 
    전역 좌표계 기준, 모델 좌표계의 위치 값을 갱신하는 메서드
    위치 값 갱신에 성공했다면 true 를 리턴하고 더이상 갱신이 없다면 false 를 리턴한다.

    bool direction
        : 큐브 좌표계가 이동할 방향
        : true => x 축 방향 이동
        : false => y 축 방향 이동

    float length
        : 이동하고자 하는 거리

    int value
        : 한번에 이동하는 값인 이동량.

*/
bool CubeCoor::Move(bool direction, float length, float value) {
    /* 
        direction 값을 기준으로 갱신하고자 하는 멤버를 선택한다.
        dirVal 은 갱신하고자 하는 위치 값의 레퍼런스
        direction == true => x 축 방향 위치 갱신
        direction == false => y 축 방향 위치 갱신
    */
    float& dirVal = (direction) ? this->x : this->y;

    /*
        현재 이동한 총량을 static 변수에 기록한다.
    */
    static float totalMove = 0;

    
    /*
        finish 변수를 이용해, 이동이 끝났는 지 아닌지를 확인한다
        if totalMove > length => 이동이 끝났다.
    */
    bool finish = true;
    if( totalMove > length ) finish = false;
    

    if(finish) { // 아직 이동이 끝나지 않았다면,
        /*
            value/10 값 만큼 이동을 하게 된다.
            이동하는 값을 dirVal 에 더하여 위치 값을 갱신한다.
            또한 이동하는 값의 절대값을 totalMove 에 더하여 이동한 총량을 체크한다.
            위치 값 갱신에 성공했으므로, true 를 리턴한다.
        */
        dirVal += value;
        totalMove += abs( value );

        return true;
    }
    else { // 더이상 이동할 수 없다면,
        /*
            주어진 방향으로의 이동이 끝났으므로 totalMove 의 값을 초기화 시킨다.
            dirVal 값은 float 타입이지만, 정확히 각 점에 위치하도록 하기 위해 반올림값을 넣어 보정한다.
            위치 값이 더이상 갱신되지 않으므로 false 를 리턴한다.
        */
        totalMove = 0;
        dirVal = static_cast<float>(floor(dirVal+0.5f));
        return false;
    }
}
