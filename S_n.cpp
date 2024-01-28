#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <memory>
#include <vector>
#include <execution>

#define INT_CAST(x) static_cast<int>(x)
#define SHORT_CAST(x) static_cast<SHORT>(x)
HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

class Game
{
    class Info
    {
        friend class Snake;
    public:
        SHORT x, y;
        char data;

        Info(SHORT x, SHORT y, char data = '0') : x(x), y(y), data(data) {}
    };

    class Node
    {
        friend class Snake;

    public:
        //SHORT x, y;
        //char data;
        Info info;
        std::shared_ptr<Node> next;
        std::shared_ptr<Node> prev;

        Node(const Info& info) : info(info), next(nullptr), prev(nullptr) {}
    };

    friend class Snake;
    static const SHORT WIDTH = 20;
    static const SHORT HEIGHT = 20;
    //std::shared_ptr<Node> fruit;
    //std::shared_ptr<Node> fast;
    Info fruit;
    Info fast;
    int time = 500;
    float radius = 1;
    bool warFog = false;
    bool isDisplayOld = false;

    std::vector<std::vector<char>> matrix{HEIGHT + 2, std::vector<char>(WIDTH + 2, ' ')};
    std::vector<std::vector<char>> fog_Matrix;

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
            matrix[i][WIDTH + 1] = '#';
        }

        matrix[HEIGHT][0] = '#';
        matrix[HEIGHT][WIDTH + 1] = '#';

        for (SHORT i = 0; i < WIDTH + 2; i++)
        {
            matrix[HEIGHT + 1][i] = '#';
        }
    }

    void newFruit()
    {
        
        for (int i = 0; i <= (WIDTH * HEIGHT) * 20; i++)
        {
            fruit.x = SHORT_CAST(rand() % (WIDTH) + 1);
            fruit.y = SHORT_CAST(rand() % (HEIGHT) + 1);

            if (matrix[fruit.y][fruit.x] == ' ')
            {
                if (matrix[fruit.y][fruit.x] == '=')
                    std::cout << "\a";
                matrix[fruit.y][fruit.x] = fruit.data;
                return;
                //
                // for (size_t ii = 0; ii < matrix.size(); ii++)
                // {
                //     for (size_t j = 0; j < matrix[ii].size(); j++)
                //     {
                //         std::cout << matrix[ii][j];
                //     }
                //     std::cout << "\n";
                // }
            }
        }
        // std::cout << "\n" << i << "\n";
        throw std::logic_error("newFruit error");
    }

    void newFast()
    {
        for (int i = 0; i <= (WIDTH * HEIGHT) * 20; i++)
        {
            fast.x = SHORT_CAST(rand() % (WIDTH) + 1);
            fast.y = SHORT_CAST(rand() % (HEIGHT) + 1);

            if (matrix[fast.y][fast.x] == ' ')
            {
                matrix[fast.y][fast.x] = fast.data;
                return;
            }
        }
        throw std::logic_error("newFast error");
    }

    Info getFruit()
    {
        return fruit;
    }

public:
    Game() : fruit(0, 0,'&'), fast(0, 0,'F')
    {
        newFruit();
        newFast();
        drawFieldMtrx();
    }

    void WarFog(bool isFog)
    {
        warFog = isFog;
    }

    void displayOld(bool is_DisplayOld)
    {
        isDisplayOld = is_DisplayOld;
    }
    class Snake
    {
        friend class Game;

    public:
        std::shared_ptr<Node> head;
        std::shared_ptr<Node> tail;
        SHORT length;
        Game &game;
        int Pressed_Key = INT_CAST(Key::w);

        Snake(Game &game) : game(game), head(std::make_shared<Node>(Info(SHORT_CAST(WIDTH / 2), SHORT_CAST(HEIGHT / 2), '|')))
        {
            game.matrix[head->info.y][head->info.x] = head->info.data;
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

            SHORT tempX = head->info.x + x;
            SHORT tempY = head->info.y + y;

            Info old_tail = tail->info;

            if (tempX == game.fruit.x && tempY == game.fruit.y)
            {
                fruit_Eat();
                game.matrix[tail->info.y][tail->info.x] = ';';
                game.matrix[head->info.y][head->info.x] = '=';
                return;
            }
            if (tempX == game.fast.x && tempY == game.fast.y)
            {
                game.matrix[game.fast.y][game.fast.x] = ' ';
                fastEat();
            }

            moveTailToBeginning();

            checkRule(x, y);

            head->info.x += x;
            head->info.y += y;

            game.matrix[old_tail.y][old_tail.x] = ' ';
            
            game.matrix[game.fast.y][game.fast.x] = game.fast.data;
            game.matrix[tail->info.y][tail->info.x] = ';';
            game.matrix[head->info.y][head->info.x] = '=';
            game.matrix[game.fruit.y][game.fruit.x] = game.fruit.data;
        }

        void fruit_Eat()
        {
            length++;
            // fruit.data = static_cast<char>(length+48);
            std::shared_ptr<Node> fruit_node = std::make_shared<Node>(game.fruit);
            fruit_node->info.data = '|';
            head->prev = fruit_node;
            fruit_node->next = head;
            head = fruit_node;

            game.newFruit();
        }

        void fastEat()
        {
            game.time -= 25;
            game.radius += 0.500; // ceil(((game.radius + (float)1.5)/(float)1.4)*(float)1000)/(float)1000;
            if (game.radius > HEIGHT / 2)
                game.radius = HEIGHT / 2;
            game.newFast();
        }

        void moveTailToBeginning()
        {
            if (head == nullptr || tail == nullptr)
                return;

            if (head == tail)
                return;

            tail->info.x = head->info.x;
            tail->info.y = head->info.y;
            tail->info.data = head->info.data;
            tail->next = head;
            head->prev = tail;
            tail->prev->next = nullptr;
            tail = tail->prev;
            //
            head = head->prev;
            head->prev = nullptr;
        }

        void displayForward()
        {
            system("cls");
            if (game.isDisplayOld)
            {
                game.drawField();
                setCursorPosition(game.fruit.x, game.fruit.y);
                std::cout << game.fruit.data;
                setCursorPosition(game.fast.x, game.fast.y);
                std::cout << game.fast.data;
                std::shared_ptr<Node> current = head;
                setCursorPosition(0, 0);
                std::cout << game.radius;

                while (current)
                {
                    setCursorPosition(current->info.x, current->info.y);
                    if (current == head)
                        std::cout << '=';
                    else if (current != tail)
                    {
                        if ((Pressed_Key == INT_CAST(Key::a)) || (Pressed_Key == INT_CAST(Key::d)))
                            std::cout << '-';
                        else
                            std::cout << current->info.data;
                    }
                    else
                        std::cout << ';';
                    current = current->next;
                }
                setCursorPosition(head->info.x, head->info.y);
            }
            else
            { // setCursorPosition(0, game.HEIGHT + 3);
                if (!game.warFog)
                {
                    for (size_t i = 0; i < game.matrix.size(); i++)
                    {
                        for (size_t j = 0; j < game.matrix[i].size(); j++)
                        {
                            std::cout << game.matrix[i][j] << " ";
                        }
                        std::cout << "\n";
                    }
                }
                else
                {
                    WarFog();
                    for (size_t i = 0; i < game.fog_Matrix.size(); i++)
                    {
                        for (size_t j = 0; j < game.fog_Matrix[i].size(); j++)
                        {   
                            //map[' '] = Color::WHITE;
                            //setColor(map[' ']);
                            if (game.fog_Matrix[i][j] == ' ' ||
                                game.fog_Matrix[i][j] == ';' ||
                                game.fog_Matrix[i][j] == '=' ||
                                game.fog_Matrix[i][j] == 'F' ||
                                game.fog_Matrix[i][j] == '&')
                            {
                                std::cout << game.fog_Matrix[i][j] << " ";
                            }
                            else
                                std::cout << game.fog_Matrix[i][j] << "~";
                            
                        }
                        std::cout << "\n";
                    }
                }
            }
            Sleep(game.time);
        }

        void display()
        {
            std::shared_ptr<Node> current = head;
            system("cls");

            game.drawField();
            setCursorPosition(game.fruit.x, game.fruit.y);
            std::cout << "0";

            while (current)
            {
                setCursorPosition(current->info.x, current->info.y);
                std::cout << current->info.data;
                current = current->next;
            }

            setCursorPosition(0, HEIGHT + 2);
        }

        void checkRule(SHORT x, SHORT y)
        {
            if (game.matrix[head->info.y + y][head->info.x + x] != ' ')
            {
                std::cout << "\a";
                setCursorPosition(30, 30);
                system("pause");
                system("pause");
                exit(-9);
            }
        }

        std::vector<std::vector<char>> WarFog()
        {
            game.fog_Matrix = game.matrix;
            drawCircle(game.fog_Matrix, head->info.y, head->info.x, INT_CAST(game.radius), ':');

            // for (size_t i = 0; i < game.fog_Matrix.size(); ++i)
            // {
            //     for (size_t j = 0; j < game.fog_Matrix[i].size(); ++j)
            //     {
            //         std::cout << game.fog_Matrix[i][j];
            //     }
            //     std::cout << "\n";
            // }

            for (size_t i = 0; i < game.fog_Matrix.size(); ++i)
            {
                for (size_t j = 0; j < game.fog_Matrix[i].size(); ++j)
                {
                    if (game.fog_Matrix[i][j] == ':')
                    {
                        game.fog_Matrix[i][j] = game.matrix[i][j];
                    }
                    else
                    {
                        game.fog_Matrix[i][j] = '~';
                    }
                }
            }

            return game.fog_Matrix;
        }

        void drawCircle(std::vector<std::vector<char>> &matrix, int centerX, int centerY, int radius, char symbol)
        {
            int x = radius;
            int y = 0;
            int err = 0;

            while (x >= y)
            {
                {
                    int i, lim;
                    if (centerX - x < 0)
                        i = 0;
                    else
                        i = centerX - x;

                    if (centerX + x >= INT_CAST(matrix.size()) - 1)
                        lim = matrix.size() - 1;
                    else
                        lim = centerX + x;

                    int limY, Lim_Y;
                    if (centerY + y >= INT_CAST(matrix.size()) - 1)
                        limY = matrix.size() - 1;
                    else
                        limY = centerY + y;

                    if (centerY - y < 0)
                        Lim_Y = 0;
                    else
                        Lim_Y = centerY - y;

                    for (; i <= lim; ++i)
                    {
                        matrix[i][limY] = symbol;
                        matrix[i][Lim_Y] = symbol;
                    }
                }
                {
                    int i, lim;
                    if (centerX - y < 0)
                        i = 0;
                    else
                        i = centerX - y;

                    if (centerX + y >= INT_CAST(matrix.size()) - 1)
                        lim = matrix.size() - 1;
                    else
                        lim = centerX + y;

                    int limY, Lim_Y;
                    if (centerY + x >= INT_CAST(matrix.size()) - 1)
                        limY = matrix.size() - 1;
                    else
                        limY = centerY + x;

                    if (centerY - x < 0)
                        Lim_Y = 0;
                    else
                        Lim_Y = centerY - x;

                    for (; i <= lim; ++i)
                    {
                        matrix[i][limY] = symbol;
                        matrix[i][Lim_Y] = symbol;
                    }
                }

                if (err <= 0)
                {
                    y += 1;
                    err += 2 * y + 1;
                }

                if (err > 0)
                {
                    x -= 1;
                    err -= 2 * x + 1;
                }
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

        game.displayOld(true);
        snake.displayForward();
        snake.keyScan();
        snake.move();
        game.WarFog(false);
    }

    return 0;
}
