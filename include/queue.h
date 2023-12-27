#ifndef QUEUE_H
#define QUEUE_H
#include<node.h>

class Queue
{
    Node *front;
    Node *rear;

    public:

        Queue()
        {
            front = NULL;
            rear = NULL;
        }

        bool is_empty()
        {
            return front == NULL;
        }

        void enqueue(int pos_x, int pos_y, int pred_x, int pred_y)
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
                return temp;
            }
            return NULL;
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
};


#endif // QUEUE_H
