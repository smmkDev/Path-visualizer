#ifndef NODE_H
#define NODE_H

class Node
{
	public:

		int pos_x, pos_y;
		int pred_x, pred_y;
		Node* next;

		Node()
		{
			next = NULL;
		}

		Node(int x, int y, int x1, int y1)
		{
			pos_x = x; pos_y = y;
			pred_x = x1; pred_y = y1;
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

#endif // NODE_H
