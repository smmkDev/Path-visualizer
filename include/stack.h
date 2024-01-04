#ifndef STACK_
#define STACK_
#include <node.h>


class Stack
{
    private:

        Node *top;
        Node *bottom;

    public:

        Stack()
        {
            top = NULL;
            bottom = NULL;
        }

        Node *peek()
        {
            return top;
        }

        void push(int pos_x, int pos_y, int pred_x, int pred_y)
        {
            Node *new_node = new Node(pos_x, pos_y, pred_x, pred_y);
            if(top == NULL)
            {
                top = new_node;
                bottom = top;
            }
            else
            {
              new_node->next = top;
              top = new_node;
            }
        }

        Node* pop()
        {
            if(top != NULL)
            {
                Node *temp = top;
                top = top->next;
                return temp;
            }
            return top;
        }

};

#endif // STACK_
