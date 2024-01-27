#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <memory>
#include <vector>

#define INT_CAST(x) static_cast<int>(x)
#define SHORT_CAST(x) static_cast<SHORT>(x)
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

class Game
{
    class Node
    {
    public:
        SHORT x, y;
        char data;
        std::shared_ptr<Node> next;
        std::shared_ptr<Node> prev;

        Node(SHORT x, SHORT y, char data = '0') : x(x), y(y), data(data), next(nullptr), prev(nullptr) {}
    };
    
    friend class Snake;
    static const SHORT WIDTH = 10;
    static const SHORT HEIGHT = 10;
    std::shared_ptr<Node> fruit;
    std::vector<std::vector<char>> matrix{HEIGHT + 2, std::vector<char>(WIDTH + 2,' ')};

    enum class Key
    {
        w = 'w',
        a = 'a',
        d = 'd',
        s = 's',
        ESC = 27
    };
    enum class Direction
    {
        UP = Key::w,
        DOWN = Key::s,
        LEFT = Key::a,
        RIGHT = Key::d
    };
    void drawField()
    {
        for (SHORT i = 0; i < WIDTH + 2; i++)
        {
            std::cout << "#";
        }
        std::cout << "\n";

        for (SHORT i = 0; i < HEIGHT; i++)
        {
            std::cout << '#' << std::setw(WIDTH + 1) << std::setfill(' ') << '#' << "\n";
        }

        for (SHORT i = 0; i < WIDTH + 2; i++)
        {
            std::cout << "#";
        }
        std::cout << "\n";
    }

    void drawFieldMtrx()
    {
        for (SHORT i = 0; i < WIDTH + 2; i++)
        {
            matrix[0][i] = '#';
        }
        std::cout << "\n";

        for (SHORT i = 0; i < HEIGHT; i++)
        {
            matrix[i][0] = '#';
            matrix[i][WIDTH+1] = '#';
        }

        matrix[HEIGHT][0] = '#';
        matrix[HEIGHT][WIDTH+1] = '#';

        for (SHORT i = 0; i < WIDTH + 2; i++)
        {
            matrix[HEIGHT+1][i] = '#';
        }
    }

    void newFruit()
    {
        SHORT x = rand() % (WIDTH - 3) + 2;
        SHORT y = rand() % (HEIGHT - 3) + 2;
        fruit = std::make_shared<Node>(SHORT_CAST(x), SHORT_CAST(y), '&');
    }
    
    std::shared_ptr<Node> getFruit()
    {
        return fruit;
    }


    public:
        Game() 
        {
            newFruit();
            drawFieldMtrx();
        }

    class Snake
    {
        friend class Game;
    public:
        std::shared_ptr<Node> head;
        std::shared_ptr<Node> tail;
        SHORT length;
        Game& game;
        int Pressed_Key = INT_CAST(Key::w);

        Snake(Game& game) : game(game), head(std::make_shared<Node>(SHORT_CAST(WIDTH / 2), SHORT_CAST(HEIGHT / 2), '|'))
        {
          game.matrix[head->y][head->x] = head->data;   
          tail = head;  
        }

        void keyScan()
        {
            if (_kbhit())
            {
                int tempKey = _getch();
                if (tempKey == INT_CAST(Key::a) || tempKey == INT_CAST(Key::d) || tempKey == INT_CAST(Key::w) || tempKey == INT_CAST(Key::s))
                    Pressed_Key = tempKey;
                else
                    return;
            }
            else
                return;
        }
        //TODO del this
        void printList()
        {
            std::shared_ptr<Node> currentNode = head;
            while (currentNode)
            {
                std::cout << currentNode->data << " ";
                currentNode = currentNode->next;
            }
            std::cout << "\n";
        }

        void move()
        {
            SHORT x = 0, y = 0;
            if (Pressed_Key == INT_CAST(Direction::UP))
                y = -1;
            else if (Pressed_Key == INT_CAST(Direction::DOWN))
                y = 1;
            else if (Pressed_Key == INT_CAST(Direction::LEFT))
                x = -1;
            else if (Pressed_Key == INT_CAST(Direction::RIGHT))
                x = 1;

            SHORT tempX = head->x + x;
            SHORT tempY = head->y + y;
            
            game.matrix[tail->y][tail->x] = ' ';

            if (tempX ==  game.fruit->x && tempY ==  game.fruit->y)
            {
                Fruit_eat();
                game.matrix[tail->y][tail->x] = ';';
                game.matrix[head->y][head->x] = '=';
                return;
            }

            moveTailToBeginning();
            checkRule(x,y);
            head->x += x;
            head->y += y;

            game.matrix[tail->y][tail->x] = ';';
            game.matrix[head->y][head->x] = '=';
            game.matrix[game.fruit->y][game.fruit->x] = game.fruit->data;
        }

        void Fruit_eat()
        {
            length++;
            // fruit->data = static_cast<char>(length+48);
            game.fruit->data = '|';
            head->prev =  game.fruit;
            game.fruit->next = head;
            head = game.fruit;

            game.newFruit();
        }

        void moveTailToBeginning()
        {
            if (head == nullptr || tail == nullptr)
                return;

            if (head == tail)
                return;

            tail->x = head->x;
            tail->y = head->y;
            tail->data = head->data;
            tail->next = head;
            head->prev = tail;
            tail->prev->next = nullptr;
            tail = tail->prev;
            //
            head = head->prev;
            head->prev = nullptr;
        }
        //TODO move to Game
        void displayForward()
        {
            system("cls");
            game.drawField();
            setCursorPosition( game.fruit->x,  game.fruit->y);
            std::cout <<  game.fruit->data;

            std::shared_ptr<Node> current = head;

            while (current)
            {
                setCursorPosition(current->x, current->y);
                if (current == head)
                    std::cout << '=';
                else if (current != tail)
                {
                    if ((Pressed_Key == INT_CAST(Key::a)) || (Pressed_Key == INT_CAST(Key::d)))
                        std::cout << '-';
                    else
                        std::cout << current->data;
                }
                else
                    std::cout << ';';
                current = current->next;
            }
            setCursorPosition(head->x, head->y);

            setCursorPosition(0, game.HEIGHT+3);
            for (size_t i = 0; i < game.matrix.size(); i++)
            {
                for (size_t j = 0; j < game.matrix[i].size(); j++)
                {
                    std::cout << game.matrix[i][j];
                    
                }
                std::cout << "\n";
            }

            Sleep(300);
        }
        //TODO move to Game
        void display()
        {
            std::shared_ptr<Node> current = head;
            system("cls");

            game.drawField();
            setCursorPosition( game.fruit->x,  game.fruit->y);
            std::cout << "0";

            while (current)
            {
                setCursorPosition(current->x, current->y);
                std::cout << current->data;
                current = current->next;
            }

            setCursorPosition(0, HEIGHT + 2);
        }

        void checkRule(SHORT x, SHORT y)
        {
            if(game.matrix[head->y+y][head->x+x] != ' ')
            {
                std::cout << "\a";
                exit(-9);
            }
        }

    private:
        void setCursorPosition(SHORT x, SHORT y)
        {
            COORD coord;
            coord.X = x;
            coord.Y = y;
            SetConsoleCursorPosition(console, coord);
        }
    };
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    Game game;
    Game::Snake snake(game);

    while (true)
    {
        srand(static_cast<unsigned>(time(nullptr)));

        //TODO WarFog add
        snake.displayForward();
        snake.keyScan();
        snake.move();
    }

    return 0;
}
