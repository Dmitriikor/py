#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <conio.h>
#include <windows.h>
#include <memory>

#define INT_CAST(x) static_cast<int>(x)
#define SHORT_CAST(x) static_cast<SHORT>(x)

const SHORT WIDTH = 20;
const SHORT HEIGHT = 20;

enum class Key {w = 'w', a = 'a', d = 'd', s = 's', ESC = 27};
enum class Direction {UP = Key::w, DOWN = Key::s, LEFT = Key::a, RIGHT = Key::d};
class Node 
{
public:
    SHORT x, y;
    char data;
    std::shared_ptr<Node> next;
    std::shared_ptr<Node> prev;

    Node(SHORT x, SHORT y, char data = '0') : x(x), y(y), data(data), next(nullptr), prev(nullptr) {}
};

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

void drawField() 
{
    for (SHORT i = 0; i < WIDTH + 2; i++) 
    {
        std::cout << "#";
    }
    std::cout << "\n";

    for (SHORT i = 0; i < HEIGHT; i++) 
    {
        std::cout << '#' << std::setw(WIDTH+1) << std::setfill(' ') << '#' << "\n";
    }

    for (SHORT i = 0; i < WIDTH + 2; i++) 
    {
        std::cout << "#";
    }
    std::cout << "\n";
}

std::shared_ptr<Node> newFruit() 
{
    SHORT x = rand() % (WIDTH - 3) + 2;  
    SHORT y = rand() % (HEIGHT - 3) + 2;  
    return std::make_shared<Node>(SHORT_CAST(x), SHORT_CAST(y), '&');
}

class Snake 
{
public:
    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;
    SHORT length;
    bool is_new_pressed = false;
    int Pressed_Key = INT_CAST(Key::w);
    std::shared_ptr<Node> fruit;

    Snake() : head(new Node(SHORT_CAST(10), SHORT_CAST(10), '|')), length(1), fruit(newFruit()), tail(head) {}

    void keyScan() 
    {
         if (_kbhit()) 
         {
            int tempKey = _getch();
            if (tempKey == INT_CAST(Key::a) || tempKey == INT_CAST(Key::d)
               || tempKey == INT_CAST(Key::w) || tempKey == INT_CAST(Key::s)) 
            {
                Pressed_Key = tempKey;
                is_new_pressed = true;
            }
            else 
            {
                return;
            }
        }
        else 
        {
            return;
        }
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

    void move() 
    {
        SHORT x = 0, y = 0;
        if (Pressed_Key == INT_CAST(Direction::UP)) 
            y = -1;
        else if (Pressed_Key == INT_CAST(Direction::DOWN) )
            y = 1;
        else if (Pressed_Key == INT_CAST(Direction::LEFT) )
            x = -1;
        else if (Pressed_Key == INT_CAST(Direction::RIGHT)) 
            x = 1;

        SHORT tempX = head->x + x;
        SHORT tempY = head->y + y;

        if (tempX == fruit->x && tempY == fruit->y) 
        {
            Fruit();
            return;
        }

        moveTailToBeginning();

        head->x += x;
        head->y += y;
    }

    void Fruit() 
    {
        length++;
        //fruit->data = static_cast<char>(length+48);
        fruit->data = '|';
        head->prev = fruit;
        fruit->next = head;
        head = fruit;

        fruit = newFruit();
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

    void displayForward() 
    {
        system("cls");
        drawField();
        setCursorPosition(fruit->x, fruit->y);
        std::cout << fruit->data;

        std::shared_ptr<Node> current = head;
        
        while (current) 
        {
            setCursorPosition(current->x, current->y);
            if (current == head)
                std::cout << '=';
            else if (current!=tail)
            {
                if((Pressed_Key == INT_CAST(Key::a)) || (Pressed_Key == INT_CAST(Key::d)))
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

    void display() 
    {
        std::shared_ptr<Node> current = head;
        system("cls");

        drawField();
        setCursorPosition(fruit->x, fruit->y);
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

int main() 
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);


    Snake snake;
    while (true) 
    {
        srand(static_cast<unsigned>(time(nullptr)));
        snake.displayForward();
        snake.keyScan();
        snake.move();
    }

    return 0;
}
