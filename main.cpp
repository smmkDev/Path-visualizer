#include <iostream>
#include<windows.h>
#include<vector>
#include<math.h>
#include<SDL.h>
#include<SDL_image.h>
#define MAX_WIDTH 500
#define MAX_HEIGHT 500
using namespace std;

class Cell
{
    SDL_Surface *img;
    int start_x;
    int finish_x;
    int start_y;
    int finish_y;
    float cost;
    bool is_wall;

    public:

        Cell(string s="square2.png", int st_x=0, int fin_x=0, int st_y=0, int fin_y=0, float c=0)
        {
            img = IMG_Load(s.c_str());
            start_x = st_x;
            finish_x = fin_x;
            start_y = st_y;
            finish_y = fin_y;
            cost = c;
            is_wall = false;
        }

        void set_cost(float c)
        {
            cost = c;
        }

        void set_start_x(int st_x)
        {
            start_x = st_x;
        }

        void set_start_y(int st_y)
        {
            start_y = st_y;
        }

        void set_finish_x(int fin_x)
        {
            finish_x = fin_x;
        }

        void set_finish_y(int fin_y)
        {
            finish_y = fin_y;
        }

        void set_surface(string s)
        {
            img = IMG_Load(s.c_str());
        }

        void set_is_wall(bool val)
        {
            is_wall = val;
        }

        bool get_is_wall()
        {
            return is_wall;
        }

        int get_start_x()
        {
            return start_x;
        }

        int get_start_y()
        {
            return start_y;
        }

        int get_finish_x()
        {
            return finish_x;
        }

        int get_finish_y()
        {
            return finish_y;
        }

        float get_cost()
        {
            return cost;
        }

        SDL_Surface *get_surface()
        {
            return img;
        }
};

class Node
{
	public:

		int pos_x, pos_y;
		int pred_x, pred_y;
		float cost;
		Node* next;

		Node()
		{
			next = NULL;
		}

		Node(int x, int y, int x1, int y1, float c=0)
		{
			pos_x = x; pos_y = y;
			pred_x = x1; pred_y = y1;
			cost = c;
			next = NULL;
		}

		int get_x()
		{
		    return pos_x;
		}

		int get_y()
		{
		    return pos_y;
		}

		int get_pred_x()
		{
		    return pred_x;
		}

		int get_pred_y()
		{
		    return pred_y;
		}
};

class Queue
{
    Node *front;
    Node *rear;
    int size;
    int capacity;

    public:

        Queue(int s=0)
        {
            size = s;
            capacity = 0;
            front = NULL;
            rear = NULL;
        }

        bool is_empty()
        {
            return front == NULL;
        }

        bool is_full()
        {
            return capacity == size;
        }

        void enqueue(int pos_x, int pos_y, int pred_x, int pred_y)
        {
            if(!is_full())
            {
                Node *new_node = new Node(pos_x, pos_y, pred_x, pred_y);
                if(front == NULL)
                {
                    front = rear = new_node;
                }
                else
                {
                    rear->next = new_node;
                    rear = rear->next;
                }
                capacity++;
            }
        }

        Node *peek()
        {
            return front;
        }

        Node *get_rear()
        {
            return rear;
        }

        Node *dequeue()
        {
            if(!is_empty())
            {
                Node *temp = front;
                front = front -> next;
                capacity--;
                return temp;
            }
            return NULL;
        }

        int get_size()
        {
            return size;
        }

        int get_capacity()
        {
            return capacity;
        }

        bool find(int x, int y)
        {
            Node *itr = front;
            while(itr != NULL)
            {
                if(itr->get_x() == x && itr->get_y() == y)
                {
                    return true;
                }
                itr = itr->next;
            }
            return false;
        }

        void display()
        {
            Node *itr = front;
            while(itr != NULL)
            {
                cout<< "("<< itr->pos_x << "," << itr->pos_y << ")" << "";
                itr = itr->next;
            }
        }

        Node* get_min(Cell **grid)
        {
            Node *min_1 = dequeue();
            Node *min_2 = dequeue();
            while(!is_empty())
            {
                if(grid[min_2->pos_x][min_2->pos_y].get_cost() < grid[min_1->pos_x][min_1->pos_y].get_cost() &&
                   !grid[min_2->pos_x][min_2->pos_y].get_is_wall())
                {
                    min_1 = min_2;
                }
                min_2 = dequeue();
            }
            return min_1;
        }
};

class PathFinder
{
    Queue *neighbours;
    int start[2];
    int target[2];
    int rows;
    int cols;
    vector<Node*> visited;

    public:

        Cell **grid;

        PathFinder()
        {
            grid = NULL;
            neighbours = NULL;
        }

        PathFinder(int r, int c):start{0, 0}, target{11, 11}
        {
            grid = new Cell*[r];
            rows = r;
            cols = c;
            for(int i=0; i<rows; i++)
            {
                grid[i] = new Cell[c];
            }
            neighbours = new Queue(12);
        }

        void set_start(int x, int y)
        {
            start[0] = x;
            start[1] = y;
            neighbours->enqueue(x, y, -1, -1);
        }

        void set_target(int x, int y)
        {
            target[0] = x;
            target[1] = y;
        }

        void calc_cost()
        {
            for(int i=0; i<rows; i++)
            {
                for(int j=0; j<cols; j++)
                {
                    grid[i][j].set_cost(sqrt((pow(target[0] - i, 2) + (pow(target[1] - j, 2)))));
                }
            }
        }

        void bfs(SDL_Surface *window_surface, SDL_Window *window)
        {
            calc_cost();
            Node *current_node = NULL;
            do
            {
                current_node = neighbours->get_min(grid);
                if(!is_visited(current_node->get_x(), current_node->get_y()))
                {
                    visited.push_back(current_node);
                }
                if(is_target(current_node))
                {
                    break;
                }
                add_neighbours(current_node->get_x(), current_node->get_y(), window_surface, window);
                Sleep(1000);
            }
            while(current_node != NULL);
            display_visited(window_surface, window);
        }

        bool in_range(int num, int range)
        {
            return num < range;
        }

        void add_neighbours(int i, int j, SDL_Surface *window_surface, SDL_Window *window)
        {
            SDL_Rect rect;
            if(i + 1 < rows && j < cols)
            {
                neighbours->enqueue(i + 1, j, i, j);
                grid[i + 1][j].set_surface("square3.png");
                rect.x = grid[i + 1][j].get_start_x();
                rect.y = grid[i + 1][j].get_start_y();
                SDL_BlitSurface(grid[i + 1][j].get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
            }
            if(i + 1 < rows && j + 1 < cols)
            {
                neighbours->enqueue(i + 1, j + 1, i, j);
                grid[i + 1][j + 1].set_surface("square3.png");
                rect.x = grid[i + 1][j + 1].get_start_x();
                rect.y = grid[i + 1][j + 1].get_start_y();
                SDL_BlitSurface(grid[i + 1][j + 1].get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
            }
            if(i + 1 < rows && j - 1 > 0)
            {
                neighbours->enqueue(i + 1, j - 1, i, j);
                grid[i + 1][j - 1].set_surface("square3.png");
                rect.x = grid[i + 1][j - 1].get_start_x();
                rect.y = grid[i + 1][j - 1].get_start_y();
                SDL_BlitSurface(grid[i + 1][j - 1].get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
            }
            if(i - 1 > 0 && j - 1 > 0)
            {
                neighbours->enqueue(i - 1, j - 1, i, j);
                grid[i - 1][j - 1].set_surface("square3.png");
                rect.x = grid[i - 1][j - 1].get_start_x();
                rect.y = grid[i - 1][j - 1].get_start_y();
                SDL_BlitSurface(grid[i - 1][j - 1].get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
            }
            if(i - 1 > 0 && j + 1 < cols)
            {
                neighbours->enqueue(i - 1, j + 1, i, j);
                grid[i - 1][j + 1].set_surface("square3.png");
                rect.x = grid[i - 1][j + 1].get_start_x();
                rect.y = grid[i - 1][j + 1].get_start_y();
                SDL_BlitSurface(grid[i + 1][j].get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
            }
            if(i < rows && j + 1 < cols)
            {
                neighbours->enqueue(i, j + 1, i, j);
                grid[i][j + 1].set_surface("square3.png");
                rect.x = grid[i][j + 1].get_start_x();
                rect.y = grid[i][j + 1].get_start_y();
                SDL_BlitSurface(grid[i][j + 1].get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
            }
            if(i - 1 > 0 && j < cols)
            {
                neighbours->enqueue(i - 1, j, i, j);
                grid[i - 1][j].set_surface("square3.png");
                rect.x = grid[i - 1][j].get_start_x();
                rect.y = grid[i - 1][j].get_start_y();
                SDL_BlitSurface(grid[i - 1][j].get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
            }
            if(i < rows && j - 1 > 0)
            {
                neighbours->enqueue(i, j - 1, i, j);
                grid[i][j - 1].set_surface("square3.png");
                rect.x = grid[i][j - 1].get_start_x();
                rect.y = grid[i][j - 1].get_start_y();
                SDL_BlitSurface(grid[i][j - 1].get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
            }
        }

        bool is_target(Node *v1)
        {
            return v1->get_x() == target[0] && v1->get_y() == target[1];
        }

        bool is_visited(int x, int y)
        {
            for(int i=0; i<visited.size(); i++)
            {
                if(visited[i]->get_x() == x && visited[i]->get_y() == y)
                {
                    return true;
                }
            }
            return false;
        }

       void display_visited(SDL_Surface *window_surface, SDL_Window *window)
       {
           SDL_Rect rect;
           for(int i=0; i<visited.size(); i++)
           {
                grid[visited[i]->get_x()][visited[i]->get_y()].set_surface("square6.png");
                rect.x = grid[visited[i]->get_x()][visited[i]->get_y()].get_start_x();
                rect.y = grid[visited[i]->get_x()][visited[i]->get_y()].get_start_y();
                SDL_BlitSurface(grid[visited[i]->get_x()][visited[i]->get_y()].get_surface(), NULL, window_surface,&rect);
                SDL_UpdateWindowSurface(window);
                Sleep(1000);
           }
       }

       int get_rows()
       {
           return rows;
       }

       int get_cols()
       {
           return cols;
       }
};

class Visualizer
{
    PathFinder *pv;
    SDL_Window *window;
    SDL_Surface *window_surface;
    bool start_is_set;
    bool target_is_set;

    public:

        Visualizer(int rows, int cols)
        {
            pv = new PathFinder(rows, cols);
            window = SDL_CreateWindow("Path Visualizer",
                                      100,
                                      100,
                                      MAX_WIDTH,
                                      MAX_HEIGHT,
                                      0
                                     );
            window_surface = SDL_GetWindowSurface(window);
            start_is_set = target_is_set = false;
        }

        void create_grid()
        {
            SDL_Surface *img = IMG_Load("square2.png");
            SDL_Rect cells;
            cells.x = cells.y = 0;
            for(int i=0; i<pv->get_rows(); i++)
            {
                for(int j=0; j<pv->get_cols(); j++)
                {
                    pv->grid[i][j].set_start_x(cells.x);
                    pv->grid[i][j].set_start_y(cells.y);
                    SDL_BlitSurface(pv->grid[i][j].get_surface(), NULL, window_surface, &cells);
                    SDL_UpdateWindowSurface(window);
                    cells.x += 22;
                    pv->grid[i][j].set_finish_x(cells.x);
                    pv->grid[i][j].set_finish_y(cells.y + 22);
                }
                cells.x = 0;
                cells.y += 22;
            }
        }

        Cell *get_cell(int x, int y, int flag=0)
        {
            for(int i=0; i<pv->get_rows(); i++)
            {
                for(int j=0; j<pv->get_cols(); j++)
                {
                    if(x >= pv->grid[i][j].get_start_x() && x<= pv->grid[i][j].get_finish_x() &&
                       y >= pv->grid[i][j].get_start_y() && y<= pv->grid[i][j].get_finish_y())
                    {
                        if(flag == 1)
                        {
                            pv->set_start(i, j);
                        }
                        else if(flag == 2)
                        {
                            pv->set_target(i, j);
                        }
                        return &pv->grid[i][j];
                    }
                }
            }
            return NULL;
        }

        void on_click(SDL_MouseButtonEvent &button)
        {
            Cell *clicked_cell;
            SDL_Rect rect;
            if(button.button == SDL_BUTTON_LEFT && !start_is_set)
            {
                clicked_cell = get_cell(button.x, button.y, 1);
                clicked_cell->set_surface("square5.png");
                rect.x = clicked_cell->get_start_x();
                rect.y = clicked_cell->get_start_y();
                SDL_BlitSurface(clicked_cell->get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
                start_is_set = true;
            }
            else if(button.button == SDL_BUTTON_LEFT && start_is_set)
            {
                clicked_cell = get_cell(button.x, button.y);
                clicked_cell->set_surface("square7.png");
                rect.x = clicked_cell->get_start_x();
                rect.y = clicked_cell->get_start_y();
                SDL_BlitSurface(clicked_cell->get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
                clicked_cell->set_is_wall(true);
            }
            else if(button.button == SDL_BUTTON_RIGHT)
            {
                clicked_cell = get_cell(button.x, button.y, 2);
                clicked_cell->set_surface("square4.png");
                rect.x = clicked_cell->get_start_x();
                rect.y = clicked_cell->get_start_y();
                SDL_BlitSurface(clicked_cell->get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
                target_is_set = true;
            }

        }

        void visualize()
        {
            SDL_Init(SDL_INIT_VIDEO);
            SDL_Event event;
            create_grid();
            while(true)
            {
                while(SDL_PollEvent(&event))
                {
                    if(event.type == SDL_QUIT)
                    {
                        SDL_Quit();
                    }
                    else if(event.type == SDL_MOUSEBUTTONDOWN)
                    {
                        on_click(event.button);
                    }
                    else if (event.type == SDL_KEYDOWN && start_is_set && target_is_set)
                    {
                        if(event.key.keysym.sym == 13)
                        {
                            pv->bfs(window_surface, window);
                        }
                    }
                }
            }
        }

};

int main(int argc, char *argv[])
{
    Visualizer v(12, 12);
    v.visualize();
    return 0;
}
