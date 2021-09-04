#include <GL/glut.h>

//#include <GL/gl.h>
//#include <GL/glu.h>

#include <stack>
#include <iostream>

#include "CubeCoor.h"

/*
    ���簢�� ����� 2���� int �迭�� �������� ���� �׸��ϴ�.
    �Ѻ��� ���̴� mapSize ��� ����� �����մϴ�.
    �׸����� �ϴ� ���� mapSize �� �°� �Է��մϴ�.
    0 �� ������ �� �ִ� ��, 
    1 �� ������ �� ���� ���� �ǹ��մϴ�.
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
    �־��� �迭 �״�� ȭ�鿡 �׷����� ���ؼ���, givenMap �� �ð�������� 90�� ȸ����Ų ����� �ʿ��մϴ�.
    -90�� ȸ�� ��ȯ�� �迭�� ��� ���� 2���� �迭 processedMap �� �����մϴ�.
    �׸��� �־��� 2���� �迭�� -90�� ȸ�� ��ȯ�����ִ� �Լ� rotateMinus90 �� �����մϴ�.
*/
int processedMap[mapSize][mapSize];
void rotateMinus90(int source[mapSize][mapSize], int dest[mapSize][mapSize]) {
    for(int j = 0; j < mapSize; j++) {
        for(int i = mapSize-1; i >= 0; i--) {
            dest[j][mapSize-1 - i] = source[i][j];
        }
    }
}



// ���� ���� ����
/*
    on_going
        : �Է¹��� �۾��� �����ϰ� ������ �˸��� ����
        : on_going �� true �� ������ Ű���� �Է��� �����Ѵ�.

    CubeCoor Player
        : �÷��̾ �����ϴ� Cube

    CubeCoor Goal
        : ���� ������ ��Ÿ���� Cube

    stack<int> commands
        : ���� �������� ���� �� �� �ϳ��� ã�� ��, �����̴� ������ �����ϴ� ����

    stack<int> inverse
        : commands �� ���� ������ ������, ���� ����� ������ �������� �����Ǳ� ������
          commands ���� ���� ��ɾ �ٽ� �������� �����ϴ� ����

    bool showLine
        : checkTheAnswer �Լ� ���� ��, Player �� ������ ������ ���̱� ���ؼ�
          Display Callback ���� glClear �Լ��� �����Ű�� ������ �Ѵ�.
          showLine ���� ���� �̿��Ͽ� �̸� �����Ѵ�.
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
    // Player �� ������ ������ ������ ���ο� ���� glClear �� ����ȴ�.
    if(!showLine) glClear(GL_COLOR_BUFFER_BIT);
    /*
        �� �� ��ȯ���� ��ȯ�ϰ�, ���� ��ķ� �ʱ�ȭ �Ѵ�.
    */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    /*
        ���� ��ǥ�迡 �־��� ����� �������� ���� �׸���.
        �־��� �迭�� -90�� ȸ���� processedMap �� �̿��Ѵ�.

        2���� �迭�� �� ��ҵ��� ������ ǥ���Ǹ�,
        ���� 0 �� ���� �Ķ������� ������ �� �ְ�
        ���� 1 �� ���� ���������� ������ �� ����.
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
        Goal Cube �� Player Cube ���� ���� �׷���
        ���� �ÿ� Player �� ������ �ʵ��� �Ѵ�.
    */
    glPushMatrix();
        glTranslatef(Goal.getX(), Goal.getY(), Goal.getZ());
        glColor3f(1.0, 1.0, 0.3);
        glutSolidCube(0.7);
    glPopMatrix();

    /*
        Player Cube �� �׸��� ����
        ��� ������ Push �Ͽ� ���ο� ��ȯ����� �����.
    */
    glPushMatrix();
        // Player cube �� ��ġ ���� ������, �̵� ��ȯ�� ������ �� ��ǥ�踦 �̵���Ų��.
        glTranslatef(Player.getX(), Player.getY(), Player.getZ());
        glColor3f(0.3, 1.0, 0.3);

        // �ű� �� ��ǥ�迡 ť�긦 �׸���.
        glutSolidCube(0.4);
    glPopMatrix();


    glutSwapBuffers();
}



/*
    ť�� ��ǥ���� ��ġ�� �ٲٱ� ����, �̵��Ϸ��� ��ġ�� ��ȿ�� ��ġ���� Ȯ���ϴ� �Լ�.
    ť�� ��ǥ�谡 Map �� ����ų�, ������ ĭ���� �̵��ϴ� ��쿣 �̵��� �Ұ����ϴ�.
    �� �� ���� ������ ���� �Ѵٸ�, false �� �����Ѵ�.
*/
bool checkValidMove(int value) {
    // ť�� ��ǥ���� ���� ��ǥ
    int x = static_cast<int>(Player.getX());
    int y = static_cast<int>(Player.getY());
    
    // value = �Էµ� ����Ű
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
    ��� �������� ���� �������� �����ϴ� ��� �� �Ѱ��� ��θ� ã�� �Լ��̴�.
    �׷��� ��ΰ� �����ϸ� true �� ��ȯ�ϰ�, ��ΰ� �������� ������ false �� �����Ѵ�.
*/
bool findTheAnswer(std::stack<int>& st) {
    // processedMap �� ����
    int checkTheRoute[mapSize][mapSize];
    for(int i = 0; i < mapSize; i++) {
        for(int j = 0; j < mapSize; j++)
            checkTheRoute[i][j] = processedMap[i][j];
    }

    /*
        ��� ����� ��������� (0, 0) ���� �����Ѵ�.
        cx, cy �� ���� �������� ��ǥ�� �ǹ��Ѵ�.    
    */
    int cx = 0, cy = 0;

    /*
        ���� �������� ���� �� = 0
        ������ �� ���� �� = 1
        ������ �� = 2
        �� ���� �ο��Ѵ�.
    */
    checkTheRoute[cx][cy] = 2; // ��� �������� ��θ� �����ϹǷ� 2�� �ο��Ѵ�.


    /*
        ���� ������ ������ ������, ���� ���� ��,��,��,�� �� �� �� �ִ� ���� �ִ� �� �����Ѵ�.
        ���� ������ ��� ������ ����� ���⿡ �ֱ� ������,
        ���� ���� �Ͱ� ���������� ���� �Ϳ� �켱������ �ξ���.
    */
    while(cx != mapSize-1 || cy != mapSize-1) {
        // ���� �� �� �ִ�
        if(cy+1 < mapSize && checkTheRoute[cx][cy+1] == 0) {
            /*
                �ش� �������� �̵� ��, �׷��� �̵��� �����ϴ� ȭ��ǥ Ű ���� ���ÿ� �����Ѵ�.
            */
            st.push(GLUT_KEY_UP);
            cy++;
            checkTheRoute[cx][cy] = 2;
        }

        // ���������� �� �� �ִ�
        else if(cx+1 < mapSize && checkTheRoute[cx+1][cy] == 0) {
            st.push(GLUT_KEY_RIGHT);
            cx++;
            checkTheRoute[cx][cy] = 2; // ���������� �̵�
        }

        // �������� �� �� �ִ�
        else if(cx-1 >= 0 && checkTheRoute[cx-1][cy] == 0) {
            st.push(GLUT_KEY_LEFT);
            cx--;
            checkTheRoute[cx][cy] = 2;
        }

        // �Ʒ��� �� �� �ִ�
        else if(checkTheRoute[cx][cy-1] == 0) {
            st.push(GLUT_KEY_DOWN);
            cy--;
            checkTheRoute[cx][cy] = 2;
        }

        // ����� �� �����ٸ�
        else {
            // ���� ��ǥ�� ��ΰ� �ɼ� ������, �����ٴ� �ǹ̿��� 1�� ��ġ
            checkTheRoute[cx][cy] = 1;

            // ������ Ȯ���Ͽ� �ٷ� ������ �̵��� ������ Ȯ���Ѵ�.
            int action = st.top();
            st.pop();

            // action ���� �̿��Ͽ� ���� ��ǥ�� �ǵ��� ����.
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
                ��� ��ΰ� ���� �ٽ� ó�� ��� �������� ���ƿ� ���.
                �̵� ��ɾ ������ ������ �����ȴ�.
                ��ΰ� ������ ���ϴ� false �� ��ȯ�Ѵ�.
            */
            if(st.empty()) return false;
        }
    }
    return true;
}



/*
    Timer Callback
    Player Cube �� �����̴� ���� �ִϸ��̼����� ǥ���ϱ� ���� Timer �� �̿��Ͽ���.
*/
void CubeMoving(int value) {
    /*
        dir : Player �� �����̷��� ����
            : x�� �̵� => true
            : y�� �̵� => false
        tip : �̵� �� ��ġ ���� ���� �� �� ���� ����
            : �̹� �ڵ忡���� -0.1 or +0.1 ���� ������ ������.
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


    // Player Cube �� ��ġ ���� �� ���� �Ǿ��ٸ�
    if(Player.Move(dir, 1.0, tip)) {
        /*
            20ms �� �ٽ� Timer Callback �� ȣ���Ͽ�
            Player �� �����̴� �ִϸ��̼��� ǥ���Ѵ�.
        */
        glutPostRedisplay(); // ���� �̺�Ʈ ���� �� Display Callback �� �䱸
        glutTimerFunc(10, CubeMoving, value); 
    }
    // Player Cube ��ġ ���� ���̻� ������ ���� �ʴ´ٸ�
    else {
        /*
            Ű �Է¿� ���� �̵��� ��� �������Ƿ�, on_going ���� false �� ����
            ���� �ٽ� Ű �Է��� ���� �� �ֵ��� �Ѵ�.
        */
        on_going = false;
        return;
    }
}



/*
    findTheAnswer �Լ��� ���� ã�� ��θ� �ڵ����� �׷��ִ� �Լ�
    inverse ���ÿ��� ȭ��ǥ Ű ���� �ϳ��� �о�ͼ� ť�긦 �̵���Ų��.
    Timer Callback �� �̿��Ͽ� ���� �ð��� �ϳ��� �ڵ����� ��θ� �׸��� �Ѵ�.
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
    ȭ��ǥ Ű�� ó���ϴ� �ݹ� �Լ�
*/
void MySpecialKey (int key, int x, int y) {
    // on_going ���� true ���, ���� �ִϸ��̼� ó�����̹Ƿ� Ű �Է��� ���� �ʴ´�.
    if(!on_going) {
        switch (key) {
        // ȭ��ǥ Ű�� �Է¹޾��� ��
        case GLUT_KEY_LEFT:
        case GLUT_KEY_UP:
        case GLUT_KEY_RIGHT:
        case GLUT_KEY_DOWN:
            /*
                ť�� ��ǥ�踦 �̵���Ű�� ����, Map �� ����� �� Ȯ���ؾ� �Ѵ�.
                ���� ����� �̵����� �ʴ´�.
            */
            if(!checkValidMove(key)) {
                // �̵��� �� ���� ��Ȳ�̶��, �ٽ� Ű �Է��� �޴� ���·� ���ư���.
                on_going = false;   
                return;
            }

            /*
                �̵� �����ϴٸ�, on_going = true �� �����Ͽ� ���̻� Ű�� �Է¹��� �ʴ´�.
                Timer Callback �� ȣ���� �ִϸ��̼��� ó���Ѵ�.
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
    �Ϲ� Ű���� Ű �Է��� ó���ϴ� �ݹ� �Լ�
*/
void MyCharKey (unsigned char key, int x, int y) {
    if(!on_going) {
        switch(key) {
            /* 
                Player Cube �� ��ġ ����
                �ٽ� �������� �̵���Ų��.
            */
            case 'r':
                showLine = false;
                Player.Set(0.0, 0.0, 0.0);
                glutPostRedisplay();
                break;

            /*
                ��� �������� ���� ������ ���ϴ� ��θ� �ڵ����� ����Ѵ�.
            */
            case 's':
                // ��ΰ� ���� ���, �׳� �������� ���ư���.
                if(!findTheAnswer(commands)) {
                    Player.Set(0.0, 0.0, 0.0);
                    glutPostRedisplay();
                    break;
                }
                else {
                    on_going = true;

                    /*
                        commands ���ÿ� �׿� �ִ� ȭ��ǥ Ű ������ 
                        �������� inverse ���ÿ� �ٽ� �׾��ش�.
                    */
                    while(!commands.empty()) {
                       inverse.push(commands.top());
                        commands.pop();
                    }
                    
                    // �ڵ����� �׷��ִ� ��δ� �� ������ ���� �Ѵ�.
                    showLine = true;

                    // ��� �������� ���� ��θ� �׸��� �Ѵ�.
                    glClear(GL_COLOR_BUFFER_BIT);
                    Player.Set(0.0, 0.0, 0.0);
                    glutTimerFunc(100, showTheAnswer, 1);
                    break;
                }
            // ���α׷� ����
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

    // 2���� �迭�� �������� �� ��Ʈ�� ǥ���ߴ�.
    glOrtho(-1, mapSize, -1, mapSize, -1, 1);


    /*
        �̺�Ʈ ������ ���� ��, �־��� 2���� �迭�� 
        -90�� ȸ���Ͽ� ���ο� 2���� �迭�� �������.
    */
    rotateMinus90(givenMap, processedMap);
    

    glutDisplayFunc(MyDisplay);
    
    // Ű���� �ݹ�
    glutSpecialFunc(MySpecialKey);
    glutKeyboardFunc(MyCharKey);
    
    glutMainLoop();
    return 0;
}