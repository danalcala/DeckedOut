//Daniel Alcala - CSE 4200 - Final Project
#include <GL/glut.h>
#include <time.h>
#include <unistd.h> //Change to Windows.h on Windows
#include <vector>
#include <iostream>
#include <stdlib.h>

using namespace std;
vector<int> offset{ 25,25 };

int ts = 50;
int score = 0;
bool firstClick = true;
int firstClickX;
int firstClickY;
int window;
int seconds;

struct piece 
{
    int kind;
}grid[8][8];

void drawShapes(void);
void drawGrid(void);

//initialization
void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 0.0);	//get white background color
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();			//replace current matrix with identity matrix
    gluOrtho2D(0.0, 400.0, 0.0, 400.0);
}

//Swaps the pieces inputed into the function
void swap(int x1, int y1, int x2, int y2)
{
    if ((abs(x1 - x2) == 1 && y1 == y2) || (x1 == x2 && abs(y1 - y2) == 1))
    {
        int tempKind = grid[x1][y1].kind;
        grid[x1][y1].kind = grid[x2][y2].kind;
        grid[x2][y2].kind = tempKind;
    }
    else
    {
        cout << "Illegal Move! You may only move the card by one space over!" << endl;
    }
}

//Loops through and finds all marked and collapses them
void collapse()
{
    for (int y = 8 - 2; y >= 0; y--)
    {
        for (int x = 0; x < 8; x++)
        {
            if (grid[x][y].kind == -1)
            {
                for (int k = y + 1; k < 8; k++)
                {
                    if (grid[x][k].kind != -1)
                    {
                        grid[x][y].kind = grid[x][k].kind;
                        grid[x][k].kind = -1;
                        break;
                    }
                }
            }
        }
    }
}

//Refills any marked slots as new cards
void refill(void)
{
    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            if (grid[x][y].kind == -1)
            {
                grid[x][y].kind = rand() % 7;
            }
        }
    }
}

//Checks for matches and marks them for removal
void match(void)
{
    bool matchedGrid[8][8] = { false };

    //Horizontal check
    for(int y = 0; y < 8; ++y)
        for (int x = 0; x < (8 - 2); ++x)
        {
            int currentType = grid[x][y].kind;
            if (currentType == -1)
                continue;

            if (grid[x + 1][y].kind == currentType && grid[x + 2][y].kind == currentType)
            {
                matchedGrid[x][y] = matchedGrid[x + 1][y] = matchedGrid[x + 2][y] = true;
                score++;
            }

        }
    //Vertical check
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < (8 - 2); ++y)
        {
            int currentType = grid[x][y].kind;
            if (currentType == -1)
                continue;

            if (grid[x][y+1].kind == currentType && grid[x][y+2].kind == currentType)
            {
                matchedGrid[x][y] = matchedGrid[x][y + 1] = matchedGrid[x][y + 2] = true;
                score++;

            }
        }
    
    //Mark for removal
    for(int y = 0; y < 8; ++y)
        for (int x = 0; x < 8; ++x)
        {
            if (matchedGrid[x][y])
            {
                grid[x][y].kind = -1;
            }
        }
    return;
}

//Generates the entire grid randomly
void randomGrid(void)
{
    srand(time(0));

    for (int y = 0; y < 8; ++y)
    {
        for (int x = 0; x < 8; ++x)
        {
            int randomType = rand() % 7;
            grid[x][y].kind = randomType;
        }
    }
    return;
}

//Gets first click, stores it, then second click and swaps them 
void mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        int mousex = x / ts;
        int mousey = (400 - y) / ts;

        if (mousex >= 0 && mousex < 8 && mousey >= 0 && mousey < 8)
        {
            if (firstClick)
            {
                firstClickX = mousex;
                firstClickY = mousey;
                firstClick = false;
            }
            else
            {
                swap(firstClickX, firstClickY, mousex, mousey);
                firstClick = true;
            }
        }
    }
}

//Keyboard function to quit
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        glutDestroyWindow(window);
        exit(0);
    }
}

//Output console text, score, and timer
void consoleText(void)
{
    system("clear"); //Change "clear" to "cls" when on Windows
    cout << "-------------------------------------------" << endl;
    cout << "|                                         |" << endl;
    cout << "|               Decked Out                |" << endl;
    cout << "|           By: Daniel Alcala             |" << endl;
    cout << "|                                         |" << endl;
    cout << "-------------------------------------------" << endl;
    cout << "| How to Play: Click on a card to select  |" << endl;
    cout << "| it, then press on another card that is  |" << endl;
    cout << "| next to it horizontically or vertically |" << endl;
    cout << "| to swap it! Match as many as you can    |" << endl;
    cout << "|  within the time limit of 60 seconds!   |" << endl;
    cout << "|     (PRESS 'ESC' TO QUIT THE GAME!)     |" << endl;
    cout << "-------------------------------------------" << endl;
    cout << "        Score: " << score << "          Timer: " << (seconds/1000) << endl;
    cout << "-------------------------------------------" << endl;
}

//Draw the gameboard background
void drawGrid(void)
{
    bool isLight = true;
    for (int x = 0; x <= 7; x++)
        for (int y = 0; y <= 7; y++)
        {
            if (isLight == true)
            {
                glColor3f(0.64, 0.64, 0.64);
                isLight = false;
            }
            else if (isLight == false)
            {
                glColor3f(0.33, 0.33, 0.33);
                isLight = true;
            }

            int gridx = x * ts;
            int gridy = y * ts;
            glRecti(gridx, gridy, gridx + ts, gridy + ts);
        }
    return;
}

//Draws the shapes for all the pieces.
void drawShapes(void)
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
        {
            int gridx = (i * ts) + 25;
            int gridy = (j * ts) + 25;

            switch (grid[i][j].kind) {
            case 0:
                glColor3f(1.0, 0.0, 0.0);//Red Triangle with Star
                glBegin(GL_TRIANGLES);
                    glVertex2i( gridx - 20, gridy - 20 );
                    glVertex2i( gridx, gridy + 20);
                    glVertex2i( gridx + 20, gridy - 20);
                glEnd();
                glColor3f(0.0, 0.0, 0.0);
                glLineWidth(1.0);
                glBegin(GL_LINE_LOOP);
                gridy -= 8;
                    glVertex2i(gridx, gridy + 10);
                    glVertex2i(gridx + 2, gridy + 4);
                    glVertex2i(gridx + 10, gridy + 4);

                    glVertex2i(gridx + 4, gridy);
                    glVertex2i(gridx + 6, gridy - 6);
                    glVertex2i(gridx, gridy - 2);

                    glVertex2i(gridx - 6, gridy - 6);
                    glVertex2i(gridx - 4, gridy);
                    glVertex2i(gridx - 10, gridy + 4);

                    glVertex2i(gridx - 2, gridy + 4);
                glEnd();
                break;
            case 1:
                glColor3f(0.0, 1.0, 0.0);//Green Hexagon with White Hexagon
                glBegin(GL_POLYGON);
                    glVertex2i(gridx, gridy - 20);
                    glVertex2i(gridx - 20, gridy - 10);
                    glVertex2i(gridx - 20, gridy + 10);
                    glVertex2i(gridx, gridy + 20);
                    glVertex2i(gridx + 20, gridy + 10);
                    glVertex2i(gridx + 20, gridy - 10);
                glEnd();
                glColor3f(1.0, 1.0, 1.0);
                glBegin(GL_POLYGON);
                    glVertex2i(gridx, gridy - 10);
                    glVertex2i(gridx - 10, gridy - 5);
                    glVertex2i(gridx - 10, gridy + 5);
                    glVertex2i(gridx, gridy + 10);
                    glVertex2i(gridx + 10, gridy + 5);
                    glVertex2i(gridx + 10, gridy - 5);
                glEnd();
                break;
            case 2:
                glColor3f(0.0, 0.0, 1.0);//Blue Square with Square in Center
                glRecti(gridx - 20, gridy - 20, gridx + 20, gridy + 20);
                glColor3f(0.5, 0.0, 0.9);
                glBegin(GL_POLYGON);
                    glVertex2i(gridx, gridy + 20);
                    glVertex2i(gridx + 20, gridy);
                    glVertex2i(gridx, gridy - 20);
                    glVertex2i(gridx - 20, gridy);
                glEnd();
                break;
            case 3:
                glColor3f(1.0, 1.0, 0.0);//Yellow Jewel Shape
                glLineWidth(1.0);
                glBegin(GL_POLYGON);
                    glVertex2i(gridx - 20, gridy + 10);
                    glVertex2i(gridx - 15, gridy + 15);
                    glVertex2i(gridx + 15, gridy + 15);
                    glVertex2i(gridx + 20, gridy + 10);
                    glVertex2i(gridx + 20, gridy - 10);
                    glVertex2i(gridx + 15, gridy - 15);
                    glVertex2i(gridx - 15, gridy - 15);
                    glVertex2i(gridx - 20, gridy - 10);
                glEnd();
                glColor3f(0.0, 0.0, 0.0);
                glBegin(GL_LINES);
                    glVertex2i(gridx - 15, gridy + 15);
                    glVertex2i(gridx - 15, gridy - 15);
                glEnd();
                glBegin(GL_LINES);
                    glVertex2i(gridx - 20, gridy + 10);
                    glVertex2i(gridx + 20, gridy + 10);
                glEnd();
                glBegin(GL_LINES);
                    glVertex2i(gridx - 20, gridy - 10);
                    glVertex2i(gridx + 20, gridy - 10);
                glEnd();
                glBegin(GL_LINES);
                    glVertex2i(gridx + 15, gridy + 15);
                    glVertex2i(gridx + 15, gridy - 15);
                glEnd();
                break;
            case 4:
                glColor3f(1.0, 0.0, 1.0);//Pink X with Square
                glBegin(GL_POLYGON);
                    glVertex2i(gridx - 20, gridy + 20);
                    glVertex2i(gridx - 15, gridy + 20);
                    glVertex2i(gridx + 20, gridy - 20);
                    glVertex2i(gridx + 15, gridy - 20);
                glEnd();
                glBegin(GL_POLYGON);
                    glVertex2i(gridx + 20, gridy + 20);
                    glVertex2i(gridx + 15, gridy + 20);
                    glVertex2i(gridx - 20, gridy - 20);
                    glVertex2i(gridx - 15, gridy - 20);
                glEnd();
                glColor3f(0.6, 0.0, 0.3);
                glRecti(gridx - 10, gridy - 10, gridx + 10, gridy + 10);
                break;
            case 5:
                glColor3f(0.0, 1.0, 1.0);//Cyan Square
                glLineWidth(2.0);
                glBegin(GL_LINE_LOOP);
                    glVertex2i(gridx - 15, gridy + 15);
                    glVertex2i(gridx + 15, gridy + 15);
                    glVertex2i(gridx + 15, gridy - 15);
                    glVertex2i(gridx - 15, gridy - 15);
                glEnd();
                glLineWidth(3.0);
                glBegin(GL_LINE_LOOP);
                    glVertex2i(gridx, gridy + 5);
                    glVertex2i(gridx + 5, gridy);
                    glVertex2i(gridx, gridy - 5);
                    glVertex2i(gridx - 5, gridy);
                glEnd();
                break;
            case 6:
                glColor3f(1.0, 1.0, 1.0);//White with Lines
                glBegin(GL_POLYGON);
                    glVertex2i(gridx, gridy + 20);
                    glVertex2i(gridx + 20, gridy);
                    glVertex2i(gridx, gridy - 20);
                    glVertex2i(gridx - 20, gridy);
                glEnd();
                glColor3f(0.0, 0.0, 0.0);
                glLineWidth(1.0);
                glBegin(GL_LINE_LOOP);
                    glVertex2i(gridx - 20, gridy);
                    glVertex2i(gridx - 15, gridy + 5);
                    glVertex2i(gridx - 10, gridy - 10);
                    glVertex2i(gridx - 5, gridy + 15);
                    glVertex2i(gridx, gridy - 20);
                    glVertex2i(gridx + 5, gridy + 15);
                    glVertex2i(gridx + 10, gridy - 10);
                    glVertex2i(gridx + 15, gridy + 5);
                    glVertex2i(gridx + 20, gridy);
                glEnd();
                break;
            }
        }
    consoleText();
    glFlush();
}

//Idle function to loop gameplay
void idle(void)
{
    //1) Increase the amount of time passes and check if game timer is over
    //2) Check match, then update score (score is updated in match func)
    //3) Remove marked cards (done within match function)
    //4) Collapse (collapse all marked cards in each column)
    //5) Generate new cards (use the refill function to generate new cards for all empty spots)
    //6) Check for cascading (run matches() again)
    //7) Output score and draw grid/cards

    seconds++;
    collapse();
    refill();
    match();

    if (seconds >= 61000)
        glutDestroyWindow(window);
}

//Display function to initialize and output screen
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);	//Clear screen
    match();
    collapse();
    refill();
    drawGrid();
    drawShapes();
    glFlush();
    glutPostRedisplay();            //Needed for Linux (Do not need it for Windows)
}

//Main function
int main(int argc, char** argv)
{
    randomGrid();
    glutInit(&argc, argv);	                        //initialize toolkit
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);	//set display mode: single bufferring, RGBA model
    glutInitWindowSize(400, 400);		            //set window size on screen
    glutInitWindowPosition(100, 150); 	            //set window position on screen
    window = glutCreateWindow("Decked Out");		//open screen window
    init();
    glutDisplayFunc(display);		                //points to display function
    glutMouseFunc(mouse);                           //mouse function
    glutKeyboardFunc(keyboard);                     //display function
    glutIdleFunc(idle);                             //idle function
    glutMainLoop();			                        //go into perpetual loop
    return 0;
}