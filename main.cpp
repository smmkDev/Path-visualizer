#include <iostream>
#include<windows.h>
#include<vector>
#include<math.h>
#include<node.h>
#include<queue.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#define MAX_WIDTH 594
#define MAX_HEIGHT 594
using namespace std;

class Cell
{
    SDL_Surface *img;
    int start_x;
    int finish_x;
    int start_y;
    int finish_y;
    bool is_wall;

    public:

        Cell(string s="square2.png", int st_x=0, int fin_x=0, int st_y=0, int fin_y=0)
        {
            img = IMG_Load(s.c_str());
            start_x = st_x;
            finish_x = fin_x;
            start_y = st_y;
            finish_y = fin_y;
            is_wall = false;
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

        SDL_Surface *get_surface()
        {
            return img;
        }
};

class PathFinder
{
    Queue *neighbours;
    int start[2];
    int target[2];
    int rows;
    int cols;
    bool is_found;
    vector<Node*> visited;

    public:

        Cell **grid;

        PathFinder()
        {
            grid = NULL;
            neighbours = NULL;
            is_found = false;
        }

        PathFinder(int r, int c):start{0, 0}, target{11, 11}
        {
            grid = new Cell*[r];
            rows = r;
            cols = c;
            is_found = false;
            for(int i=0; i<rows; i++)
            {
                grid[i] = new Cell[c];
            }
            neighbours = new Queue(r*c);
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

        void bfs(SDL_Surface *window_surface, SDL_Window *window)
        {
            Node *current_node = NULL;
            do
            {
                current_node = neighbours->dequeue();
                if(!is_visited(current_node->get_x(), current_node->get_y()))
                {
                    visited.push_back(current_node);
                }
                add_neighbours(current_node->get_x(), current_node->get_y(), window_surface, window);
            }
            while(!is_found);
            display_visited(window_surface, window);
        }

        void add_neighbours(int i, int j, SDL_Surface *window_surface, SDL_Window *window)
        {
            SDL_Rect rect;
            if(i < rows && j + 1 < cols && !grid[i][j + 1].get_is_wall() && !is_visited(i, j + 1))
            {
                neighbours->enqueue(i, j + 1, i, j);
                grid[i][j + 1].set_surface("square3.png");
                rect.x = grid[i][j + 1].get_start_x();
                rect.y = grid[i][j + 1].get_start_y();
                SDL_BlitSurface(grid[i][j + 1].get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
            }
            if(i < rows && j - 1 >= 0 && !grid[i][j - 1].get_is_wall() && !is_visited(i, j - 1))
            {
                neighbours->enqueue(i, j - 1, i, j);
                grid[i][j - 1].set_surface("square3.png");
                rect.x = grid[i][j - 1].get_start_x();
                rect.y = grid[i][j - 1].get_start_y();
                SDL_BlitSurface(grid[i][j - 1].get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
            }
            if(i - 1 >= 0 && j < cols && !grid[i - 1][j].get_is_wall() && !is_visited(i - 1, j))
            {
                neighbours->enqueue(i - 1, j, i, j);
                grid[i - 1][j].set_surface("square3.png");
                rect.x = grid[i - 1][j].get_start_x();
                rect.y = grid[i - 1][j].get_start_y();
                SDL_BlitSurface(grid[i - 1][j].get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
            }
            if(i + 1 < rows && j < cols && !grid[i + 1][j].get_is_wall() && !is_visited(i + 1, j))
            {
                neighbours->enqueue(i + 1, j, i, j);
                grid[i + 1][j].set_surface("square3.png");
                rect.x = grid[i + 1][j].get_start_x();
                rect.y = grid[i + 1][j].get_start_y();
                SDL_BlitSurface(grid[i + 1][j].get_surface(), NULL, window_surface, &rect);
                SDL_UpdateWindowSurface(window);
            }
        }

        bool is_target(int x, int y)
        {
            return x == target[0] && y == target[1];
        }

        Node* get_node(int pre_x, int pre_y)
        {
            for(int i=0; i<int(visited.size()); i++)
            {
                if(visited[i]->get_x() == pre_x && visited[i]->get_y() == pre_y)
                {
                    return visited[i];
                }
            }
            return NULL;
        }

        bool is_visited(int x, int y)
        {
            if(is_target(x, y))
            {
                is_found = true;
                return true;
            }
            for(int i=0; i<int(visited.size()); i++)
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
           Node* itr = visited[visited.size() - 1];
           while(itr->get_x() != start[0] || itr->get_y() != start[1])
           {
                grid[itr->get_x()][itr->get_y()].set_surface("square6.png");
                rect.x = grid[itr->get_x()][itr->get_y()].get_start_x();
                rect.y = grid[itr->get_x()][itr->get_y()].get_start_y();
                SDL_BlitSurface(grid[itr->get_x()][itr->get_y()].get_surface(), NULL, window_surface,&rect);
                SDL_UpdateWindowSurface(window);
                itr = get_node(itr->get_pred_x(), itr->get_pred_y());
                Sleep(100);
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
                                      55,
                                      55,
                                      MAX_WIDTH,
                                      MAX_HEIGHT,
                                      0
                                     );
            window_surface = SDL_GetWindowSurface(window);
            start_is_set = target_is_set = false;
        }

        void create_grid()
        {
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
    Visualizer v(50, 50);
    v.visualize();
    return 0;
}
