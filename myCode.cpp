#include <GL/glut.h>

//#include <GL/gl.h>
//#include <GL/glu.h>

#include <stack>
#include <iostream>

#include "CubeCoor.h"

/*
    정사각형 모양의 2차원 int 배열을 바탕으로 맵을 그립니다.
    한변의 길이는 mapSize 라는 상수로 설정합니다.
    그리고자 하는 맵을 mapSize 에 맞게 입력합니다.
    0 은 지나갈 수 있는 점, 
    1 은 지나갈 수 없는 점을 의미합니다.
*/
const int mapSize = 7;
int givenMap[mapSize][mapSize] = {
    {1, 1, 0, 0, 1, 0, 0},
    {1, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 0, 0, 0, 0},
    {1, 0, 0, 0, 1, 0, 1},
    {0, 0, 1, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 0}
};



/* 
    주어진 배열 그대로 화면에 그려내기 위해서는, givenMap 을 시계방향으로 90도 회전시킨 행렬이 필요합니다.
    -90도 회전 변환한 배열을 담기 위한 2차원 배열 processedMap 을 선언합니다.
    그리고 주어진 2차원 배열을 -90도 회전 변환시켜주는 함수 rotateMinus90 를 정의합니다.
*/
int processedMap[mapSize][mapSize];
void rotateMinus90(int source[mapSize][mapSize], int dest[mapSize][mapSize]) {
    for(int j = 0; j < mapSize; j++) {
        for(int i = mapSize-1; i >= 0; i--) {
            dest[j][mapSize-1 - i] = source[i][j];
        }
    }
}



// 전역 변수 정의
/*
    on_going
        : 입력받은 작업은 진행하고 있음을 알리는 변수
        : on_going 이 true 인 동안은 키보드 입력을 무시한다.

    CubeCoor Player
        : 플레이어가 조종하는 Cube

    CubeCoor Goal
        : 골인 지점을 나타내는 Cube

    stack<int> commands
        : 골인 지점으로 가는 길 중 하나를 찾을 때, 움직이는 방향을 저장하는 스택

    stack<int> inverse
        : commands 에 쌓인 방향을 꺼내면, 실제 적용된 순서와 역순으로 나열되기 때문에
          commands 에서 꺼낸 명령어를 다시 역순으로 저장하는 스택

    bool showLine
        : checkTheAnswer 함수 실행 시, Player 가 움직인 흔적을 보이기 위해선
          Display Callback 에서 glClear 함수를 실행시키지 말아햐 한다.
          showLine 변수 값을 이용하여 이를 조절한다.
*/
bool on_going = false;
CubeCoor Player(0, 0, 0);
CubeCoor Goal(mapSize-1, mapSize-1, 0);
std::stack<int> commands;
std::stack<int> inverse;
bool showLine = false;



/*
    Display Callback
*/
void MyDisplay() {
    // Player 가 움직인 흔적을 남길지 여부에 따라 glClear 가 실행된다.
    if(!showLine) glClear(GL_COLOR_BUFFER_BIT);
    /*
        모델 뷰 변환으로 전환하고, 등차 행렬로 초기화 한다.
    */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /*
        전역 좌표계에 주어진 행렬을 바탕으로 맵을 그린다.
        주어진 배열을 -90도 회전한 processedMap 을 이용한다.

        2차원 배열의 각 요소들은 점으로 표현되며,
        값이 0 인 곳은 파란색으로 지나갈 수 있고
        값이 1 인 곳은 빨간색으로 지나갈 수 없다.
    */
    glPointSize(10);
    glBegin(GL_POINTS);
        for(int i = 0; i < mapSize; i++) {
            for(int j = 0; j < mapSize; j++) {
                if(processedMap[i][j] == 0) glColor3f(0.3, 0.3, 1.0);
                else glColor3f(1.0, 0.3, 0.3);
                glVertex2d(i, j);
            }
        }
    glEnd();

    /*
        Goal Cube 를 Player Cube 보다 먼저 그려서
        도착 시에 Player 가 가리지 않도록 한다.
    */
    glPushMatrix();
        glTranslatef(Goal.getX(), Goal.getY(), Goal.getZ());
        glColor3f(1.0, 1.0, 0.3);
        glutSolidCube(0.7);
    glPopMatrix();

    /*
        Player Cube 를 그리기 위해
        행렬 스택을 Push 하여 새로운 변환행렬을 만든다.
    */
    glPushMatrix();
        // Player cube 의 위치 값을 가져와, 이동 변환을 적용해 모델 좌표계를 이동시킨다.
        glTranslatef(Player.getX(), Player.getY(), Player.getZ());
        glColor3f(0.3, 1.0, 0.3);

        // 옮긴 모델 좌표계에 큐브를 그린다.
        glutSolidCube(0.4);
    glPopMatrix();


    glutSwapBuffers();
}



/*
    큐브 좌표계의 위치를 바꾸기 전에, 이동하려는 위치가 유효한 위치인지 확인하는 함수.
    큐브 좌표계가 Map 을 벗어나거나, 빨간색 칸으로 이동하는 경우엔 이동이 불가능하다.
    갈 수 없는 곳으로 가려 한다면, false 를 리턴한다.
*/
bool checkValidMove(int value) {
    // 큐브 좌표계의 현재 좌표
    int x = static_cast<int>(Player.getX());
    int y = static_cast<int>(Player.getY());
    
    // value = 입력된 방향키
    switch(value) {
        case GLUT_KEY_UP:
            if(y == mapSize-1 || processedMap[x][y+1] == 1) return false;
            else return true;

        case GLUT_KEY_DOWN:
            if(y == 0 || processedMap[x][y-1] == 1) return false;
            else return true;

        case GLUT_KEY_LEFT:
            if(x == 0 || processedMap[x-1][y] == 1) return false;
            else return true;

        case GLUT_KEY_RIGHT:
            if(x == mapSize-1 || processedMap[x+1][y] == 1) return false;
            else return true;

        default:
            return false;
    }
}



/*
    출발 지점에서 골인 지점까지 도달하는 경로 중 한가지 경로를 찾는 함수이다.
    그러한 경로가 존재하면 true 를 반환하고, 경로가 존재하지 않으면 false 를 리턴한다.
*/
bool findTheAnswer(std::stack<int>& st) {
    // processedMap 을 복사
    int checkTheRoute[mapSize][mapSize];
    for(int i = 0; i < mapSize; i++) {
        for(int j = 0; j < mapSize; j++)
            checkTheRoute[i][j] = processedMap[i][j];
    }

    /*
        경로 조사는 출발지점인 (0, 0) 에서 시작한다.
        cx, cy 는 현재 조사중인 좌표를 의미한다.    
    */
    int cx = 0, cy = 0;

    /*
        아직 지나가지 않은 점 = 0
        지나갈 수 없는 점 = 1
        지나온 길 = 2
        의 값을 부여한다.
    */
    checkTheRoute[cx][cy] = 2; // 출발 지점에서 경로를 시작하므로 2를 부여한다.


    /*
        골인 지점에 도착할 때까지, 현재 점의 상,하,좌,우 에 갈 수 있는 길이 있는 지 조사한다.
        골인 지점이 출발 지점의 우상향 방향에 있기 때문에,
        위로 가는 것과 오른쪽으로 가는 것에 우선순위를 두었다.
    */
    while(cx != mapSize-1 || cy != mapSize-1) {
        // 위로 갈 수 있다
        if(cy+1 < mapSize && checkTheRoute[cx][cy+1] == 0) {
            /*
                해당 방향으로 이동 시, 그러한 이동을 지시하는 화살표 키 값을 스택에 저장한다.
            */
            st.push(GLUT_KEY_UP);
            cy++;
            checkTheRoute[cx][cy] = 2;
        }

        // 오른쪽으로 갈 수 있다
        else if(cx+1 < mapSize && checkTheRoute[cx+1][cy] == 0) {
            st.push(GLUT_KEY_RIGHT);
            cx++;
            checkTheRoute[cx][cy] = 2; // 오른쪽으로 이동
        }

        // 왼쪽으로 갈 수 있다
        else if(cx-1 >= 0 && checkTheRoute[cx-1][cy] == 0) {
            st.push(GLUT_KEY_LEFT);
            cx--;
            checkTheRoute[cx][cy] = 2;
        }

        // 아래로 갈 수 있다
        else if(checkTheRoute[cx][cy-1] == 0) {
            st.push(GLUT_KEY_DOWN);
            cy--;
            checkTheRoute[cx][cy] = 2;
        }

        // 사방이 다 막혔다면
        else {
            // 현재 좌표는 경로가 될수 없으니, 막혔다는 의미에서 1을 배치
            checkTheRoute[cx][cy] = 1;

            // 스택을 확인하여 바로 이전에 이동한 방향을 확인한다.
            int action = st.top();
            st.pop();

            // action 값을 이용하여 이전 좌표로 되돌아 간다.
            switch(action) {
                case GLUT_KEY_UP:
                    cy--;
                    break;

                case GLUT_KEY_RIGHT:
                    cx--;
                    break;

                case GLUT_KEY_LEFT:
                    cx++;
                    break;

                case GLUT_KEY_DOWN:
                    cy++;
                    break;

            }
            /*
                모든 경로가 막혀 다시 처음 출발 지점으로 돌아온 경우.
                이동 명령어를 저장한 스택이 비어버렸다.
                경로가 없음을 뜻하는 false 를 반환한다.
            */
            if(st.empty()) return false;
        }
    }
    return true;
}



/*
    Timer Callback
    Player Cube 가 움직이는 것을 애니메이션으로 표현하기 위해 Timer 를 이용하였다.
*/
void CubeMoving(int value) {
    /*
        dir : Player 가 움직이려는 방향
            : x축 이동 => true
            : y축 이동 => false
        tip : 이동 시 위치 값을 더할 지 뺄 지를 결정
            : 이번 코드에서는 -0.1 or +0.1 만을 값으로 가진다.
    */
    bool dir;
    float tip = 0;

   
    switch(value) {
        case GLUT_KEY_UP:
            dir = false; tip = 0.1;
            break;

        case GLUT_KEY_DOWN:
            dir = false; tip = -0.1;
            break;

        case GLUT_KEY_LEFT:
            dir = true; tip = -0.1;
            break;

        case GLUT_KEY_RIGHT:
            dir = true; tip = 0.1;
            break;
    }


    // Player Cube 의 위치 값이 잘 갱신 되었다면
    if(Player.Move(dir, 1.0, tip)) {
        /*
            20ms 후 다시 Timer Callback 을 호출하여
            Player 가 움직이는 애니메이션을 표현한다.
        */
        glutPostRedisplay(); // 다음 이벤트 루프 때 Display Callback 을 요구
        glutTimerFunc(10, CubeMoving, value); 
    }
    // Player Cube 위치 값이 더이상 갱신이 되지 않는다면
    else {
        /*
            키 입력에 의한 이동이 모두 끝났으므로, on_going 값을 false 로 설정
            이제 다시 키 입력을 받을 수 있도록 한다.
        */
        on_going = false;
        return;
    }
}



/*
    findTheAnswer 함수를 통해 찾은 경로를 자동으로 그려주는 함수
    inverse 스택에서 화살표 키 값을 하나씩 읽어와서 큐브를 이동시킨다.
    Timer Callback 을 이용하여 일정 시간에 하나씩 자동으로 경로를 그리게 한다.
*/
void showTheAnswer(int value) {
    if(!inverse.empty()) {
        on_going = true;
        glutTimerFunc(10, CubeMoving, inverse.top());
        inverse.pop();

        on_going = true;
        glutTimerFunc(500, showTheAnswer, 1);
    }
}



/*
    화살표 키를 처리하는 콜백 함수
*/
void MySpecialKey (int key, int x, int y) {
    // on_going 값이 true 라면, 아직 애니메이션 처리중이므로 키 입력을 받지 않는다.
    if(!on_going) {
        switch (key) {
        // 화살표 키를 입력받았을 때
        case GLUT_KEY_LEFT:
        case GLUT_KEY_UP:
        case GLUT_KEY_RIGHT:
        case GLUT_KEY_DOWN:
            /*
                큐브 좌표계를 이동시키기 전에, Map 을 벗어나는 지 확인해야 한다.
                맵을 벗어나면 이동하지 않는다.
            */
            if(!checkValidMove(key)) {
                // 이동할 수 없는 상황이라면, 다시 키 입력을 받는 상태로 돌아간다.
                on_going = false;   
                return;
            }

            /*
                이동 가능하다면, on_going = true 로 설정하여 더이상 키를 입력받지 않는다.
                Timer Callback 을 호출해 애니메이션을 처리한다.
            */
            on_going = true;
            glutTimerFunc(10, CubeMoving, key);
            break;

        default:
            break;
        }
    }
}



/*
    일반 키보드 키 입력을 처리하는 콜백 함수
*/
void MyCharKey (unsigned char key, int x, int y) {
    if(!on_going) {
        switch(key) {
            /* 
                Player Cube 의 위치 리셋
                다시 원점으로 이동시킨다.
            */
            case 'r':
                showLine = false;
                Player.Set(0.0, 0.0, 0.0);
                glutPostRedisplay();
                break;

            /*
                출발 지점에서 골인 지점을 향하는 경로를 자동으로 출력한다.
            */
            case 's':
                // 경로가 없는 경우, 그냥 원점으로 돌아간다.
                if(!findTheAnswer(commands)) {
                    Player.Set(0.0, 0.0, 0.0);
                    glutPostRedisplay();
                    break;
                }
                else {
                    on_going = true;

                    /*
                        commands 스택에 쌓여 있는 화살표 키 값들을 
                        역순으로 inverse 스택에 다시 쌓아준다.
                    */
                    while(!commands.empty()) {
                       inverse.push(commands.top());
                        commands.pop();
                    }
                    
                    // 자동으로 그려주는 경로는 그 흔적이 남게 한다.
                    showLine = true;

                    // 출발 지점에서 부터 경로를 그리게 한다.
                    glClear(GL_COLOR_BUFFER_BIT);
                    Player.Set(0.0, 0.0, 0.0);
                    glutTimerFunc(100, showTheAnswer, 1);
                    break;
                }
            // 프로그램 종료
            case 'q':
                exit(0);
                break;

            default:
                break;
        }
    }
}



int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowSize(800, 800);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("OpenGL Maze");
    
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // 2차원 배열의 범위만은 뷰 포트에 표현했다.
    glOrtho(-1, mapSize, -1, mapSize, -1, 1);


    /*
        이벤트 루프에 들어가기 전, 주어진 2차원 배열을 
        -90도 회전하여 새로운 2차원 배열을 만들었다.
    */
    rotateMinus90(givenMap, processedMap);
    

    glutDisplayFunc(MyDisplay);
    
    // 키보드 콜백
    glutSpecialFunc(MySpecialKey);
    glutKeyboardFunc(MyCharKey);
    
    glutMainLoop();
    return 0;
}