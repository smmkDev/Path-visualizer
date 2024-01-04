#include <iostream>
#include<windows.h>
#include<vector>
#include<math.h>
#include<node.h>
#include<queue.h>
#include<stack.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

#define MAX_WIDTH 585
#define MAX_HEIGHT 595
using namespace std;

class Cell
{
    SDL_Surface *img;
    int start_x;
    int finish_x;
    int start_y;
    int finish_y;
    bool is_wall;
    bool is_visited;
    bool is_start;
    bool is_target;

    public:

        Cell(string s="grid.png", int st_x=0, int fin_x=0, int st_y=0, int fin_y=0)
        {
            img = IMG_Load(s.c_str());
            start_x = st_x;
            finish_x = fin_x;
            start_y = st_y;
            finish_y = fin_y;
            is_wall = is_visited = is_start = is_target = false;

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

        void set_is_visited(bool val)
        {
            is_visited = val;
        }

        void set_is_start(bool val)
        {
            is_start = val;
        }

        void set_is_target(bool val)
        {
            is_target = val;
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

        bool get_is_visited()
        {
            return is_visited;
        }

        bool get_is_start()
        {
            return is_start;
        }

        bool get_is_target()
        {
            return is_target;
        }

        SDL_Surface *get_surface()
        {
            return img;
        }

//        ~Cell()
//        {
//            SDL_FreeSurface(img);
//        }
};

class BFS
{
    Queue *neighbours;
    int start[2];
    int target[2];
    bool is_found;
    vector<Node*> visited;

    public:

        BFS():start{0, 0}, target{11, 11}
        {
            is_found = false;
            neighbours = new Queue;
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

        void bfs(SDL_Renderer *renderer, SDL_Window *window, int cols, int rows, Cell **grid)
        {
            Node *current_node = NULL;
            while(!is_found)
            {
                current_node = neighbours->dequeue();
                if(current_node == NULL)
                {
                    SDL_Rect rect;
                    rect.x = 70; rect.y = 250;
                    rect.w = 500; rect.h = 50;
                    SDL_Surface *surface = IMG_Load("no_path.png");
                    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_RenderCopy(renderer, texture, NULL, &rect);
                    SDL_RenderPresent(renderer);
                    SDL_FreeSurface(surface);
                    SDL_DestroyTexture(texture);
                    return;
                }
                visited.push_back(current_node);
                if(is_target(current_node->get_x(), current_node->get_y()))
                {
                    is_found = true;
                    break;
                }
                add_neighbours(current_node->get_x(), current_node->get_y(), cols, rows, grid);
            }
            display_traversal(renderer, window, grid);
            display_visited(renderer, window, grid);
            delete current_node;
        }

        void add_neighbours(int i, int j, int cols, int rows, Cell **grid)
        {
            if(i < rows && j + 1 < cols)
            {
                if(!grid[i][j + 1].get_is_visited() && !grid[i][j + 1].get_is_wall())
                {
                    neighbours->enqueue(i, j + 1, i, j);
                    grid[i][j + 1].set_is_visited(true);
                }
            }
            if(i < rows && j - 1 >= 0)
            {
                 if(!grid[i][j - 1].get_is_visited() && !grid[i][j - 1].get_is_wall())
                {

                    neighbours->enqueue(i, j - 1, i, j);
                    grid[i][j - 1].set_is_visited(true);
                }
            }
            if(i - 1 >= 0 && j < cols)
            {
                if(!grid[i - 1][j].get_is_visited() && !grid[i - 1][j].get_is_wall())
                {
                    neighbours->enqueue(i - 1, j, i, j);
                    grid[i - 1][j].set_is_visited(true);
                }
            }
            if(i + 1 < rows && j < cols)
            {
                if(!grid[i + 1][j].get_is_visited() && !grid[i + 1][j].get_is_wall())
                {
                    neighbours->enqueue(i + 1, j, i, j);
                    grid[i + 1][j].set_is_visited(true);
                }
            }
        }

        bool is_target(int x, int y)
        {
            return x == target[0] && y == target[1];
        }

        bool is_start(int x, int y)
        {
            return x == start[0] && y == start[1];
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

        void display_traversal(SDL_Renderer *renderer, SDL_Window *window, Cell **grid)
        {
            SDL_Rect rect;
            SDL_Texture *texture;
            for(int i=1; i<visited.size(); i++)
            {
                if(!is_start(visited[i]->get_x(), visited[i]->get_y()) && !is_target(visited[i]->get_x(), visited[i]->get_y()))
                {
                    grid[visited[i]->get_x()][visited[i]->get_y()].set_surface("bfs.png");
                    rect.x = grid[visited[i]->get_x()][visited[i]->get_y()].get_start_x();
                    rect.y = grid[visited[i]->get_x()][visited[i]->get_y()].get_start_y();
                    rect.w = rect.h = 20;
                    texture = SDL_CreateTextureFromSurface(renderer,  grid[visited[i]->get_x()][visited[i]->get_y()].get_surface());
                    SDL_RenderCopy(renderer, texture, NULL, &rect);
                    SDL_RenderPresent(renderer);
                    Sleep(20);
                }

            }
            SDL_DestroyTexture(texture);
        }

       void display_visited(SDL_Renderer *renderer, SDL_Window *window, Cell **grid)
       {
           SDL_Rect rect;
           SDL_Texture *texture;
           Node* itr = visited[visited.size() - 1];
           while(itr->get_x() != start[0] || itr->get_y() != start[1])
           {
                itr = get_node(itr->get_pred_x(), itr->get_pred_y());
                grid[itr->get_x()][itr->get_y()].set_surface("path.png");
                rect.x = grid[itr->get_x()][itr->get_y()].get_start_x();
                rect.y = grid[itr->get_x()][itr->get_y()].get_start_y();
                rect.w = rect.h = 20;
                texture = SDL_CreateTextureFromSurface(renderer,  grid[itr->get_x()][itr->get_y()].get_surface());
                SDL_RenderCopy(renderer, texture, NULL, &rect);
                SDL_RenderPresent(renderer);
                if(is_start(itr->get_pred_x(), itr->get_pred_y()))
                {
                    break;
                }
                Sleep(100);
           }
           SDL_DestroyTexture(texture);
       }

       ~BFS()
       {
            delete neighbours;
       }
};

class DFS
{
    Stack *neighbours;
    int start[2];
    int target[2];
    bool is_found;
    vector<Node*> visited;

    public:

        DFS():start{0, 0}, target{11, 11}
        {
            is_found = false;
            neighbours = new Stack;
        }

        void set_start(int x, int y)
        {
            start[0] = x;
            start[1] = y;
            neighbours->push(x, y, -1, -1);
        }

        void set_target(int x, int y)
        {
            target[0] = x;
            target[1] = y;
        }

        void dfs(SDL_Renderer *renderer, SDL_Window *window, int cols, int rows, Cell **grid)
        {
            Node *current_node = NULL;
            while(!is_found)
            {
                current_node = neighbours->pop();
                if(current_node == NULL)
                {
                    SDL_Rect rect;
                    rect.x = 70; rect.y = 250;
                    rect.w = 500; rect.h = 50;
                    SDL_Surface *surface = IMG_Load("no_path.png");
                    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                    SDL_RenderCopy(renderer, texture, NULL, &rect);
                    SDL_RenderPresent(renderer);
                    SDL_FreeSurface(surface);
                    SDL_DestroyTexture(texture);
                    return;
                }
                visited.push_back(current_node);
                if(is_target(current_node->get_x(), current_node->get_y()))
                {
                    is_found = true;
                    break;
                }
                add_neighbours(current_node->get_x(), current_node->get_y(), cols, rows, grid);
            }
            display_traversal(renderer, window, grid);
            delete current_node;
        }

        void add_neighbours(int i, int j, int cols, int rows, Cell **grid)
        {
            if(i - 1 >= 0 && j < cols && !grid[i - 1][j].get_is_wall() && !grid[i - 1][j].get_is_visited())
            {
                neighbours->push(i - 1, j, i, j);
                grid[i - 1][j].set_is_visited(true);
            }
            if(i + 1 < rows && j < cols && !grid[i + 1][j].get_is_wall() && !grid[i + 1][j].get_is_visited())
            {
                neighbours->push(i + 1, j, i, j);
                grid[i + 1][j].set_is_visited(true);
            }
            if(i < rows && j - 1 >= 0 && !grid[i][j - 1].get_is_wall() && !grid[i][j - 1].get_is_visited())
            {
                neighbours->push(i, j - 1, i, j);
                grid[i][j - 1].set_is_visited(true);
            }
            if(i < rows && j + 1 < cols && !grid[i][j + 1].get_is_wall() && !grid[i][j + 1].get_is_visited())
            {
                neighbours->push(i, j + 1, i, j);
                grid[i][j + 1].set_is_visited(true);
            }
        }

        bool is_target(int x, int y)
        {
            return x == target[0] && y == target[1];
        }

        bool is_start(int x, int y)
        {
            return x == start[0] && y == start[1];
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

        void display_traversal(SDL_Renderer *renderer, SDL_Window *window, Cell **grid)
        {
            SDL_Rect rect;
            SDL_Texture *texture;
            for(int i=1; i<visited.size(); i++)
            {
                if(!is_start(visited[i]->get_x(), visited[i]->get_y()) && !is_target(visited[i]->get_x(), visited[i]->get_y()))
                {
                    grid[visited[i]->get_x()][visited[i]->get_y()].set_surface("dfs.png");
                    rect.x = grid[visited[i]->get_x()][visited[i]->get_y()].get_start_x();
                    rect.y = grid[visited[i]->get_x()][visited[i]->get_y()].get_start_y();
                    rect.w = rect.h = 20;
                    texture = SDL_CreateTextureFromSurface(renderer,  grid[visited[i]->get_x()][visited[i]->get_y()].get_surface());
                    SDL_RenderCopy(renderer, texture, NULL, &rect);
                    SDL_RenderPresent(renderer);
                    Sleep(20);
                }

            }
        }

        ~DFS()
        {
            delete neighbours;
        }
};


class Visualizer
{
    BFS *bfs;
    DFS *dfs;
    Cell **grid;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    bool start_is_set;
    bool target_is_set;
    int rows;
    int cols;

    public:

        Visualizer(int r, int c)
        {
            dfs = new DFS;
            bfs = new BFS;
            rows = r;
            cols = c;
            window = SDL_CreateWindow("Path Visualizer",
                                      SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      MAX_WIDTH,
                                      MAX_HEIGHT,
                                      0
                                     );
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            start_is_set = target_is_set = false;
            grid = new Cell*[r];
            for(int i=0; i<r; i++)
            {
                grid[i] = new Cell[c];
            }
        }

        void create_grid()
        {
            SDL_Rect cells = {60, 49, 20, 20};
            for(int i=0; i<rows; i++)
            {
                for(int j=0; j<cols; j++)
                {
                    grid[i][j].set_start_x(cells.x);
                    grid[i][j].set_start_y(cells.y);
                    texture = SDL_CreateTextureFromSurface(renderer, grid[i][j].get_surface());
                    SDL_RenderCopy(renderer, texture, NULL, &cells);
                    SDL_RenderPresent(renderer);
                    cells.x += 22;
                    grid[i][j].set_finish_x(cells.x);
                    grid[i][j].set_finish_y(cells.y + 22);
                }
                cells.x = 60;
                cells.y += 22;
            }
        }

        Cell get_cell(int x, int y, int flag=0)
        {
            for(int i=0; i<rows; i++)
            {
                for(int j=0; j<cols; j++)
                {
                    if(x >= grid[i][j].get_start_x() && x<= grid[i][j].get_finish_x() &&
                       y >= grid[i][j].get_start_y() && y<= grid[i][j].get_finish_y())
                    {
                        if(flag == 1)
                        {
                            dfs->set_start(i, j);
                            bfs->set_start(i, j);
                            grid[i][j].set_is_start(true);
                        }
                        else if(flag == 2)
                        {
                            dfs->set_target(i, j);
                            bfs->set_target(i, j);
                            grid[i][j].set_is_start(true);
                        }
                        else
                        {
                            grid[i][j].set_is_wall(true);
                        }
                        return grid[i][j];
                    }
                }
            }
        }

        void on_click(SDL_MouseButtonEvent &button, bool flag=0)
        {
            Cell clicked_cell;
            SDL_Rect rect;
            if(button.x < 60 || button.x > 585)
            {
                return;
            }
            if(button.y < 49 || button.y > 595)
            {
                return;
            }
            if(button.button == SDL_BUTTON_LEFT && !start_is_set)
            {
                if(!clicked_cell.get_is_target())
                {
                    clicked_cell = get_cell(button.x, button.y, 1);
                    clicked_cell.set_surface("start.png");
                    rect.x = clicked_cell.get_start_x();
                    rect.y = clicked_cell.get_start_y();
                    rect.w = rect.h = 20;
                    texture = SDL_CreateTextureFromSurface(renderer, clicked_cell.get_surface());
                    SDL_RenderCopy(renderer, texture, NULL, &rect);
                    SDL_RenderPresent(renderer);
                    start_is_set = true;
                }
            }
            else if(button.button == SDL_BUTTON_LEFT && start_is_set)
            {

                clicked_cell = get_cell(button.x, button.y);
                if(!clicked_cell.get_is_start() && !clicked_cell.get_is_target())
                {
                    clicked_cell.set_surface("walls.png");
                    rect.x = clicked_cell.get_start_x();
                    rect.y = clicked_cell.get_start_y();
                    rect.w = rect.h = 20;
                    texture = SDL_CreateTextureFromSurface(renderer, clicked_cell.get_surface());
                    SDL_RenderCopy(renderer, texture, NULL, &rect);
                    SDL_RenderPresent(renderer);
                }
            }
            else if(button.button == SDL_BUTTON_RIGHT && !target_is_set)
            {
                if(!clicked_cell.get_is_start())
                {
                    clicked_cell = get_cell(button.x, button.y, 2);
                    clicked_cell.set_surface("end.png");
                    rect.x = clicked_cell.get_start_x();
                    rect.y = clicked_cell.get_start_y();
                    rect.w = rect.h = 20;
                    texture = SDL_CreateTextureFromSurface(renderer, clicked_cell.get_surface());
                    SDL_RenderCopy(renderer, texture, NULL, &rect);
                    SDL_RenderPresent(renderer);
                    target_is_set = true;
                }
            }

        }

        void make_walls(SDL_MouseButtonEvent &button)
        {
            Cell clicked_cell;
            SDL_Rect rect;
            if(button.x < 60 || button.x > 585)
            {
                return;
            }
            if(button.y < 49 || button.y > 595)
            {
                return;
            }
            if(button.button == SDL_BUTTON_LEFT && start_is_set)
            {
                clicked_cell = get_cell(button.x, button.y);
                if(!clicked_cell.get_is_start() && !clicked_cell.get_is_target())
                {
                    clicked_cell.set_surface("walls.png");
                    rect.x = clicked_cell.get_start_x();
                    rect.y = clicked_cell.get_start_y();
                    rect.w = rect.h = 20;
                    texture = SDL_CreateTextureFromSurface(renderer, clicked_cell.get_surface());
                    SDL_RenderCopy(renderer, texture, NULL, &rect);
                    SDL_RenderPresent(renderer);
                }
            }
        }

        void render_menu()
        {
            SDL_Surface *surface = IMG_Load("top.png");
            SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_Rect rect = {0, 0, 600, 600};
            SDL_RenderCopy(renderer, texture, NULL, &rect);
            SDL_RenderPresent(renderer);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }

        void reset()
        {
            start_is_set = target_is_set = false;
            for(int i=0; i<rows; i++)
            {
                delete[] grid[i];
            }
            delete[] grid;
            delete bfs;
            delete dfs;
            bfs = new BFS;
            dfs = new DFS;
            grid = new Cell*[rows];
            for(int i=0; i<rows; i++)
            {
                grid[i] = new Cell[cols];
            }
        }

        void visualize()
        {
            SDL_Init(SDL_INIT_VIDEO);
            SDL_Event event;
            render_menu();
            create_grid();
            while(true)
            {
                while(SDL_PollEvent(&event))
                {
                    if(event.type == SDL_QUIT)
                    {
                        SDL_Quit();
                    }
                    else if(event.type == SDL_KEYDOWN)
                    {
                        if(event.key.keysym.sym == SDLK_b && start_is_set && target_is_set)
                        {
                            bfs->bfs(renderer, window, cols, rows, grid);
                        }
                        else if (event.key.keysym.sym == SDLK_d && start_is_set && target_is_set)
                        {
                            dfs->dfs(renderer, window, cols, rows, grid);
                        }
                        else if (event.key.keysym.sym == SDLK_r)
                        {
                            reset();
                            render_menu();
                            create_grid();
                        }
                    }
                    else if(event.type == SDL_MOUSEBUTTONUP)
                    {
                        on_click(event.button);
                    }
                    else if(event.type == SDL_MOUSEMOTION && start_is_set)
                    {
                        make_walls(event.button);
                    }
                }
            }
       }

       ~Visualizer()
       {
           delete bfs;
           delete dfs;
           for(int i=0; i<rows; i++)
           {
            delete grid[i];
           }
           delete[] grid;
           SDL_DestroyRenderer(renderer);
           SDL_DestroyTexture(texture);
           SDL_DestroyWindow(window);
       }

};

int main(int argc, char *argv[])
{
    Visualizer v(25, 24);
    v.visualize();
    return 0;
}
