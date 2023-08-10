#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <vector>

using namespace std;
#define N (int)3 //--

using i64 = long long int; //--
#define INT_MAX 2147483647

struct Node
{
    Node *parent;  // stores node's [arents]
    int mtx[N][N]; // to store the matrix and its values
    bool discovered = false;
    bool visited = false;
    int x, y;
    int cost;  // total cost required
    int dis;   // stores distance
    int moved; // stores the moved number
    int depth;

    // compare the two nodes
    bool operator==(Node rhs)
    {
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                if (this->mtx[i][j] != rhs.mtx[i][j])
                    return false;
        }
        return true;
    }
};

void print_mtx(int mtx[N][N])
{ // prints a matrix of n*n
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            cout << mtx[i][j];
        cout << endl;
    }
}

Node *new_node(int mtx[N][N], int x, int y, int new_x, int new_y, int moved,
               int dis, Node *parent)
{
    Node *node = new Node;
    node->parent = parent; // setting the pointer to root
    memcpy(node->mtx, mtx,
           sizeof node
               ->mtx); // copies data from this node to parent node
    swap(node->mtx[x][y],
         node->mtx[new_x][new_y]); // Move the tile by swapping the position
    node->cost = INT_MAX - 1;
    node->moved =
        moved;
    node->depth = 0;
    node->dis =
        dis;
    node->x = new_x;
    node->y = new_y;

    return node;
}

int row[] = {1, 0, -1, 0}; // bottom, left, top, right
int col[] = {0, -1, 0, 1}; // bottom, left, top, right

int cal_cost(int start[N][N], int final[N][N])
{ // calculates the cost by calculating misplaced tiles
    int count = 0;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            if (start[i][j] && start[i][j] != final[i][j])
            {
                count++;
            }
        }
    }
    return count;
}

int check(int x, int y) { return (x >= 0 && x < N && y >= 0 && y < N); } // check if the co-oridnates are correct

void print_path(Node *root)
{ // prints the entire path from root till the destination node
    if (root == NULL)
        return;
    print_path(root->parent);
    print_mtx(root->mtx);

    cout << endl;
}

bool cmp(Node *lhs, Node *rhs) // comparing two node pointers
{
    return (*lhs == *rhs);
}

struct comp
{
    bool operator()(const Node *lhs, const Node *rhs) const
    {
        return (lhs->cost + lhs->dis) > (rhs->cost + rhs->dis);
    }
};

bool node_cmp(const Node *lhs, const Node *rhs) // helps the min_el algo
{
    return (lhs->dis) < (rhs->dis);
}

void dijkstra(int start[N][N], int x, int y, int final[N][N])
{
    set<Node *, node_cmp> s; // to store the created vertices of the graph

    // below we are creating a root node with the starting state
    Node *r1 = new_node(start, x, y, x, y, 0, 0, NULL);
    r1->cost = cal_cost(start, final);

    s.insert(r1);

    while (!s.empty())
    {
        // here we are trying to get the vertex with the most minimum dist
        Node *min = *min_element(s.begin(), s.end(), node_cmp);

        // to avoid further relaxation of the vertex, we erase it from explored
        // and set discovered to true
        explored.erase(min);
        min->discovered = true;

        if (min->cost == 0)
        {
            print_path(min); // printing path from root -> destination
            cout << "Cost: " << min->dis << endl;
            return;
        }

        // visit every child, or every possible next step from the puzzle's current state
        for (int i = 0; i < 4; i++)
        {
            if (check(min->x + row[i], min->y + col[i])) // checking move
            {
                // ch = child
                Node *ch =
                    new_node(min->mtx, min->x, min->y, min->x + row[i], min->y + col[i],
                             min->mtx[min->x + row[i]][min->y + col[i]], 0, min);

                ch->cost = cal_cost(ch->mtx, final);
                ch->dis = min->dis + ch->moved;

                // relaxation of the distance
                auto it = explored.find(ch);
                if (it != explored.end())
                { // checks if node is already in the set
                    if ((*it)->dis > ch->dis)
                    {
                        (*it)->dis = ch->dis;
                        explored.erase(it);
                        explored.insert(ch);
                    }
                }
                else
                { // if node is not in the set, add it
                    explored.insert(ch);
                }
            }
        }
    }
}

// end of new dijstra

// BFS

// below function is to help solve the 8 Puzzle with the help of BFS (x & y =
// blank tile co-ordinates which are in their starting state)
void BFS(int start[N][N], int x, int y, int final[N][N])
{
    vector<Node *> abc; // to store vertices

    Node *r1 = new_node(start, x, y, x, y, 0, 0,
                        NULL); // creating a root node which has the start
                               // state
    r1->visited = true;

    abc.push_back(r1);

    while (!abc.empty())
    {
        // vector acting as a queue here:
        Node *min = abc.front();
        for (auto d : abc)
        { // taking the first undiscovered vertex from front
            if (d->discovered == false)
            {
                min = d;
                break;
            }
        }
        // to delete the vertex form the queue, we mark discovered as true now
        // (not
        // really removed because used to print solution)
        min->discovered = true;

        if (min->cost == 0)
        {
            // printing path from root->destination
            print_path(min);
            cout << "Cost: " << min->dis << endl;
            return;
        }

        // visiting every child, or every possible next step from current state
        for (int i = 0; i < 4; i++)
        {
            if (check(min->x + row[i], min->y + col[i]))
            {
                Node *ch = new_node(min->mtx, min->x, min->y, min->x + row[i],
                                    min->y + col[i], 0, 0, min);
                ch->cost = cal_cost(ch->mtx, final);
                ch->dis = min->dis + 1; // adding cost

                unsigned int j = 0;
                while (j < abc.size())
                {
                    if (*ch == *abc[j] && abc[j]->discovered != true)
                        break;
                    j++;
                }

                if (j != abc.size()) // dis is updated if child is already found in the
                                     // queue
                {
                    abc[j]->dis = ch->dis;
                }
                else // child is added to queue if not already present
                {
                    abc.push_back(ch);
                    ch->visited = true;
                }
            }
        }
    }
}

// // DFS

// new DFS code
void DFS(int start[N][N], int x, int y, int final[N][N], int depth_limit)
{
    priority_queue<Node *, vector<Node *>, comp> pq;
    Node *root = new_node(start, x, y, x, y, 0, 0, NULL);
    root->cost = cal_cost(start, final);
    root->depth = 0;
    pq.push(root);

    while (!pq.empty())
    {
        Node *min = pq.top();
        pq.pop();

        if (min->cost == 0)
        {
            print_path(min);
            return;
        }

        if (min->depth >= depth_limit)
        {
            continue;
        }

        for (int i = 0; i < 4; i++)
        {
            if (min->x + row[i] >= 0 && min->x + row[i] < N && min->y + col[i] >= 0 &&
                min->y + col[i] < N)
            {
                Node *child = new_node(min->mtx, min->x, min->y, min->x + row[i],
                                       min->y + col[i], 0, 0, min);
                child->depth = min->depth + 1;
                pq.push(child);
            }
        }

        print_path(root);
    }
    cout << "cost is : " << root->cost << endl;
}

// end of dfs new

// // below is the helper function used in DFS which is used to solve the 8
// Puzzle void helper_DFS(Node *min, int final[N][N], Node *answer) {
//   if (min->cost == 0) {
//     if (answer->dis > min->dis) {
//       answer->parent = min->parent;
//       answer->dis = min->dis;
//       memcpy(answer->mtx, min->mtx, sizeof answer->mtx);
//     }
//   }
//   else {
//     // visiting every child
//     for (int i = 0; i < 4; i++) {
//       if (check(min->x + row[i], min->y + col[i])) {
//         Node *ch = new_node(min->mtx, min->x, min->y, min->x + row[i],
//                             min->y + col[i], 0, 0, min);
//         ch->cost = cal_cost(ch->mtx, final);
//         ch->dis = min->dis + 1;

//         if (ch->dis < 15) {
//           helper_DFS(ch, final, answer);
//         }
//       }
//     }
//   }
// }

// // below function is to help solve the 8 Puzzle with the help of DFS (x & y =
// // blank tile co-ordinates which are in their starting state)
// void DFS(int start[N][N], int x, int y, int final[N][N]) {

//   Node *r1 = new_node(start, x, y, x, y, 0, 0, NULL);
//   Node *t = new_node(start, x, y, x, y, 0, INT_MAX, NULL);

//   r1->cost = cal_cost(start, final);
//   helper_DFS(r1, final, t);
//   print_path(t);
//   cout << "Cost: " << t->dis << endl;

// }

// below is the driver code:
int main()
{
    int start[N][N] = {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}};

    // initial arrangement (empty space = 0)

    // filling start with input file
    ifstream inp;
    inp.open("input_file1.txt");
    if (inp)
    {
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                inp >> start[i][j];
            }
        }
    }
    inp.close();

    // final solvable arrangement (empty space = 0)\
  //ruk ja
    int final[N][N] = {{1, 2, 3}, {8, 0, 4}, {7, 6, 5}};

    // blank tile co-ordinates in initial arrangement:

    int x = 1, y = 1;
    cout << "------8 Puzzle Solver-----" << endl
         << endl;

    cout << "Solution using Dijkstra's Algortihm" << endl;
    cout << "Dijkstra  = 1, BFS = 2, DFS = 3, Please select input, 4 to quit" << endl;

    int input = 0;
    while (input != 4)
    {

        cout << "which algo to run : " << endl;

        cin >> input;

        if (input == 1)
        {
            cout << "Solution using Dijkstra: " << endl;
            dijkstra(start, x, y, final);
        }

        if (input == 2)
        {
            cout << endl
                 << "Solution using BFS" << endl;
            BFS(start, x, y, final);
        }

        if (input == 3)
        {

            cout << "Solution using DFS" << endl;
            int max_depth = 10;
            DFS(start, 1, 1, final, max_depth);
        }
    }
    // DFS(start, x, y, final);

    cout << "thanks" << endl;
    return 0;
}