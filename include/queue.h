#ifndef QUEUE_H
#define QUEUE_H
#include<node.h>

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
                std::cout<< "("<< itr->pos_x << "," << itr->pos_y << ")" << "";
                itr = itr->next;
            }
        }
};


#endif // QUEUE_H
