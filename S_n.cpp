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
    static const SHORT WIDTH = 20;
    static const SHORT HEIGHT = 20;
    std::shared_ptr<Node> fruit;
    std::vector<std::vector<char>> matrix{WIDTH + 2, std::vector<char>(HEIGHT + 2, '~')};

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
    static void drawField()
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
        }

    class Snake
    {
        friend class Game;
    public:
        std::shared_ptr<Node> head;
        std::shared_ptr<Node> tail;
        SHORT length;

        int Pressed_Key = INT_CAST(Key::w);

        Snake( ) : head(std::make_shared<Node>(SHORT_CAST(WIDTH / 2), SHORT_CAST(HEIGHT / 2), '|')), length(1), tail(head) {}

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

        void move(Game& game)
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
            
            if (tempX ==  game.fruit->x && tempY ==  game.fruit->y)
            {
                Fruit_eat(game);
                return;
            }

            moveTailToBeginning();

            head->x += x;
            head->y += y;
        }

        void Fruit_eat(Game& game)
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

        void displayForward(Game& game)
        {
            system("cls");
            drawField();
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

            Sleep(300);
        }

        void display(Game& game)
        {
            std::shared_ptr<Node> current = head;
            system("cls");

            drawField();
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
    Game::Snake snake;

    while (true)
    {
        srand(static_cast<unsigned>(time(nullptr)));
        snake.displayForward(game);
        snake.keyScan();
        snake.move(game);
    }

    return 0;
}
