/* Piyush Manjhi
   U10602103
   Description: Hexagon game. Playing against an AI, user will draw dashed lines between
   6 nodes. The first player to draw a triangle will lose.*/

#include <iostream>
#include <climits>
using namespace std;

//global declaration
const int N = 6;
char graph_adj_matrix[N][N]; // adj matrix of the graph
const int inf = INT_MAX;

//declaring a matrix of 6x6 for keeping track of edges
void initialize_matrix()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            graph_adj_matrix[i][j] = '-'; // "-" represents empty space of no edges
        }
    }
}

//this function prints graph after every step
void display_graph()
{   
    //reference
    cout << "Graph:" << endl;
    cout << "Nodes" << endl;
    // the nodes are from 0 - 5, these are hard coded below just for x-axis reference, it is same for y-axis, however not displayed
    cout << "0 " << "1 " << "2 " << "3 " << "4 " << "5 " << endl;
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            cout << graph_adj_matrix[i][j] << " ";
        }
        cout << endl;
    }
}

//this function helps AI to check for triangle before it turns it move in. If this function detects that currect edge
//selection can create a triangle; it will do everything to avoid it
bool evaluate_poss_triangle(int u, int v, char c)
{
    for (int i = 0; i < N; i++)
    {
        if (graph_adj_matrix[i][u] == c && graph_adj_matrix[i][v] == c )
        {      
            return true;
        }
        if (graph_adj_matrix[u][i] == c && graph_adj_matrix[v][i] == c )
        {
            return true;
        }
    } 
    return false;
}

//this is a copy of above function but with display of triangle if exists;
//this function is called after the step is taken by any player to display triangle found
bool check_triangle_display_when_found(int u, int v, char c)
{
    for (int i = 0; i < N; i++)
    {
        if (graph_adj_matrix[i][u] == c && graph_adj_matrix[i][v] == c)
        {
            //display triangle's edges
            cout << "Triangle found! " << endl;
            cout << "edge: " << u << "-" << v << ": " << c <<endl;
            cout << "edge: " << i << "-" << u << ": " << c << endl;
            cout << "edge: " << i << "-" << v << ": " << c << endl;
            return true;
        }
        if (graph_adj_matrix[u][i] == c && graph_adj_matrix[v][i] == c)
        {   // display triangle's edges
            cout << "Triangle found! " << endl;
            cout << "edge" << u << "-" << "v" << ": " << c <<endl;
            cout << "edge" << i << "-" << "u" << ": " << c << endl;
            cout << "edge" << i << "-" << "v" << ": " << c << endl;
            return true;
        }
    }
    return false;
}

/*this function uses minimax method and alpha beta turing to ensure that AI is minimizing it's chances of creating a triangle
by following certain pattern. I wrote the pattern by observing the game..the pattern trys to creat edges from minimal node to next node
if already taken then it'll just skip that edge. It'll always try to keep the best score as low as possible 
*/
int minimax(int depth, int alpha, int beta, bool maximizing_player)
{
    if (depth == 0 || evaluate_poss_triangle(0, 0, 'X'))
    {
        return 0;
    }
    
    //AI compares best_score -inf to current score and keep the highest one
    if (maximizing_player)
    {
        int best_score = -inf;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (graph_adj_matrix[i][j] == '-')
                {
                    graph_adj_matrix[i][j] = 's';
                    int score = minimax(depth - 1, alpha, beta, false);
                    graph_adj_matrix[i][j] = '-';
                    best_score = max(best_score, score);
                    alpha = max(alpha, score);
                    if (beta <= alpha)
                    {
                        break;
                    }
                }
            }
        }
        return best_score;
    }
    else
    {
        int best_score = inf;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                if (graph_adj_matrix[i][j] == '-')
                {
                    graph_adj_matrix[i][j] = 'd';
                    int score = minimax(depth - 1, alpha, beta, true);
                    graph_adj_matrix[i][j] = '-';
                    best_score = min(best_score, score);
                    beta = min(beta, score);
                    if (beta <= alpha)
                    {
                        break;
                    }
                }
            }
        }
        return best_score;
    }
}

/* the main function asks for initial player, switches turns among the player
takes input for both player and adds it to the adj matrix, it validates if the matrix is full
and finally displays output by calling print graph function
*/
int main()
{
    initialize_matrix();
    int turn = 0;
    int total_moves = 0;
    bool AI_first_move = true;

    // Prompt user to choose player who'll go first
    cout << "WELCOME TO HEXAGON GAME \nThis game is played against and AI (player 1) which follows certain pattern to maximize it's winning chances. Good Luck!" << endl;
    cout << "Player 1 is AI, You are Player 2, choose 2 to go first:" << endl;
    cin >> turn;

    // this while loop will iterate until the matrix if full or until any of the player loses
    //  N * (N-1) / 2 is maximum possible number of moves
    while (total_moves < N * (N - 1) / 2)
    { // maximum possible number of moves
        display_graph();

        //player 1's move
        if (turn == 1)
        {
            cout << "AI's turn:" << endl;
            int best_move_u = -1;
            int best_move_v = -1;
            int best_score = -inf;
            
                for (int i = 0; i < N; i++)
                {
                    for (int j = 0; j < N; j++) // start from i+1 to avoid checking duplicates
                    {
                        if(i == j) continue;
                        if (graph_adj_matrix[i][j] == '-') // check if the edge is empty
                        {
                            graph_adj_matrix[i][j] = 's'; // make solid line for AI's move
                            graph_adj_matrix[j][i] = 's';

                            // check if drawing this edge creates a triangle
                            bool creates_triangle = evaluate_poss_triangle(i, j, 's');

                            if (!creates_triangle) // if it doesn't create a triangle
                            {
                                // cout << "here " << endl; 
                                int score = minimax(3, -inf, inf, true);
                                graph_adj_matrix[i][j] = '-'; // undo the move
                                graph_adj_matrix[j][i] = '-';
                                if (score > best_score)
                                {
                                    best_score = score;
                                    best_move_u = i;
                                    best_move_v = j;
                                }
                            }
                            else // if it creates a triangle, undo the move and move on to the next edge
                            {
                                graph_adj_matrix[i][j] = '-';
                                graph_adj_matrix[j][i] = '-';
                            }
                        }
                        else continue;
                    }
                }
            // }
            graph_adj_matrix[best_move_u][best_move_v] = 's';
            graph_adj_matrix[best_move_v][best_move_u] = 's';
            if (check_triangle_display_when_found(best_move_u, best_move_v, 's'))
            {
                cout << "AI lost the game!" << endl;
                break;
            }
        }
        else //player two (user)'s move
        {
            cout << "Your turn (enter two vertex indices separated by a space):" << endl;
            int u, v;
            cin >> u >> v;
            while (graph_adj_matrix[u][v] != '-' || v == u){
                cout << "cannot overwrite edge, please try again" << endl;
                cin >> u >> v;
            }
                graph_adj_matrix[u][v] = 'd';
                graph_adj_matrix[v][u] = 'd';
            if (check_triangle_display_when_found(u, v, 'd'))
            {
                cout << "You lost the game!" << endl;
                break;
            }
        }
        turn = 3 - turn; // switch turns between 1 and 2
        total_moves++;
    }

    //if the matrix is full, it'll declare a draw
    if (total_moves == ( N * (N - 1) / 2))
        cout << "It's a draw" << endl;

    display_graph();
    cout << "Game over, thanks for playing" << endl;

    return 0;
}
