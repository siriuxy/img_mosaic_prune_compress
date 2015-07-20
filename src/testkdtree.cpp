#include <iostream>
#include <sstream>
#include "coloredout.h"
#include "kdtree.h"
#include "point.h"

using namespace std;

// Global helper data and functions

bool is_terminal = false;
bool is_colored = false;
colored_out::enable_t color_scheme = colored_out::DISABLE;

void output_header(string name, string desc)
{
    const string bar(79, '~');
    cout << bar << endl;
    colored_out::output_bold(name, cout, color_scheme);
    cout << " - " << desc << endl;
    cout << bar << endl;
}

template <typename T>
inline std::string to_string(const T& value)
{
    std::ostringstream ss;
    ss << value;
    return ss.str();
}

/******************************************************************************
 * Test Cases
 *****************************************************************************/

void test_smaller_in_dimension()
{
    output_header("test_smaller_in_dimension()",
                  "(constructor must not crash on empty vector)");
    vector<point<3>> pts;
    kd_tree<3> tree(pts);
    point<3> a(1, 2, 3);
    point<3> b(3, 2, 1);
    cout << "smaller_in_dimension(a, b, 0) = "
         << tree.smaller_in_dimension(a, b, 0) << endl; // should print true (1)
    cout << "smaller_in_dimension(a, b, 2) = "
         << tree.smaller_in_dimension(a, b, 2)
         << endl; // should print false (0)
    cout << "smaller_in_dimension(a, b, 1) = "
         << tree.smaller_in_dimension(a, b, 1)
         << endl; // based on operator<, this should be true (1)
    cout << endl;
}

void test_should_replace()
{
    output_header("test_should_replace()",
                  "(constructor must not crash on empty vector)");
    vector<point<3>> pts;
    kd_tree<3> tree(pts);

    point<3> target(1, 3, 5);
    point<3> curr_best1(1, 3, 2);
    point<3> possible_best1(2, 4, 4);
    point<3> curr_best2(1, 3, 6);
    point<3> possible_best2(2, 4, 4);
    point<3> curr_best3(0, 2, 4);
    point<3> possible_best3(2, 4, 6);

    cout << "should_replace(target, curr_best1, possible_best1) = "
         << tree.should_replace(target, curr_best1, possible_best1)
         << endl; // should print true (1)
    cout << "should_replace(target, curr_best2, possible_best2) = "
         << tree.should_replace(target, curr_best2, possible_best2)
         << endl; // should print false (0)
    cout << "should_replace(target, curr_best3, possible_best3) = "
         << tree.should_replace(target, curr_best3, possible_best3)
         << endl; // based on operator<, this should be false (0)
    cout << endl;
}

/**
 * Test the kd_tree constructor
 */
template <int K>
void test_linear_ctor(int size)
{
    output_header("test_linear_ctor<" + to_string(K) + ">(" + to_string(size) +
                      ")",
                  "constructs a " + to_string(K) +
                      "D tree of points 0 through " + to_string(size - 1));

    vector<point<K>> points;
    points.reserve(size);
    for (int i = 0; i < size; i++)
    {
        point<K> p;
        for (int j = 0; j < K; j++)
            p[j] = i;
        points.push_back(p);
    }

    kd_tree<K> tree(points);

    cout << endl << "~  ";
    colored_out::output_bold("Tree Diagram", cout, color_scheme);
    cout << "  ~    ~    ~    ~    ~    ~    ~";
    cout << endl;
    tree.print(cout, color_scheme);
    cout << endl;
}

/**
 * Test the kd_tree Nearest Neighbor Search
 */
template <int K>
void test_linear_nns(int size)
{
    output_header("test_linear_nns<" + to_string(K) + ">(" + to_string(size) +
                      ")",
                  "find_nearest_neighbor on a " + to_string(K) +
                      "D tree of elements 0 to " + to_string(size - 1));

    vector<point<K>> points;
    points.reserve(size);
    for (int i = 0; i < size; i++)
    {
        point<K> p;
        for (int j = 0; j < K; j++)
            p[j] = i;
        points.push_back(p);
    }

    kd_tree<K> tree(points);

    for (size_t i = 0; i < points.size(); i++)
    {
        cout << "find_nearest_neighbor(" << points[i] << ") = ";
        cout << tree.find_nearest_neighbor(points[i]) << endl;
    }

    cout << endl;
}

/*****************************************
*                    8                   *
*     X              7     X             *
*                    6                   *
*                    5                   *
*                    4                   *
*                    3                   *
*                    2             X     *
*       O            1                   *
* 9876543210987 X 32101234567890123456789*
*       X            1                   *
*                    2                   *
*                    3             X     *
*                    4                   *
*                    5                   *
*                    6                   *
*                    7                   *
*                    8                   *
*****************************************/
// Looking for O, Xs are points
void test_deceptive_nn_one_level()
{
    output_header("test_deceptive_nn_one_level()",
                  "tests for nearest neighbors across the dividing line");

    double coords[6][2] = {
        {-15, 7}, {6, 7}, {-13, -1}, {-5, 0}, {14, -3}, {14, 2}};
    double targetCoords[2] = {-13, 1};
    double expectedCoords[2] = {-13, -1};

    vector<point<2>> points;
    for (int i = 0; i < 6; ++i)
        points.push_back(point<2>(coords[i]));
    point<2> target(targetCoords);
    point<2> expected(expectedCoords);

    kd_tree<2> tree(points);
    tree.print(cout, color_scheme, 3);
    cout << "  ~    ~    ~    ~    ~    ~    ~" << endl;

    cout << "find_nearest_neighbor(" << target
         << ") result   = " << tree.find_nearest_neighbor(target) << endl;
    cout << "find_nearest_neighbor(" << target << ") expected = " << expected
         << endl;

    cout << endl;
}

/***********************
*    X      8          *
*           7          *
*       X   6          *
*     S  X             *
*      N    X          *
*                      *
*           2          *
*           1          *
* 098765432101 X 567890*
*           1          *
*           2          *
*           3     M    *
*           4          *
*           5   X      *
*           6      M   *
*           7 M        *
***********************/
// M: mine
// S: search target
// N: nearest neighbor
// X: non-mine elements
/**
 * returns true if nearest neighbor search of the kdree created by the
 * parameter vector of elements returns an element that is equal to the
 * expected element and false otherwise. The mine trigger is used to enable
 * all the mines. If the kdtree atempts to search subtrees of the kdtree
 * when it doesn't need to, the mine will "explode" resulting in a segmentation
 * fault.
 */
void test_mines()
{
    output_header("test_mines()",
                  "tests that you don't look at nodes you don't need to");

    //                              NN
    double coords[10][2] = {{-8, 7},
                            {-6, 4},
                            {-5, 6},
                            {-3, 5},
                            {0, 7},
                            {2, -7},
                            {3, 0},
                            {5, -4},
                            {6, -3},
                            {7, -6}};
    //                     M        M       M        M        M
    bool isMine[10] = {false, false, false, false, false,
                       true,  true,  true,  true,  true};
    double targetCoords[2] = {-7, 5};
    double expectedCoords[2] = {-6, 4};

    vector<point<2>> points;
    for (int i = 0; i < 10; ++i)
        points.push_back(point<2>(coords[i], isMine[i]));
    point<2> target(targetCoords);
    point<2> expected(expectedCoords);

    kd_tree<2> tree(points);
    tree.print(cout, color_scheme);
    cout << "  ~    ~    ~    ~    ~    ~    ~" << endl;

    // Turn on land mines to make sure we don't step on them
    point<2>::enable_mines = true;
    cout << "find_nearest_neighbor(" << target
         << ") result   = " << tree.find_nearest_neighbor(target) << endl;
    cout << "find_nearest_neighbor(" << target << ") expected = " << expected
         << endl;
    point<2>::enable_mines = false;

    cout << endl;
}

void test_deceptive_mines()
{
    output_header("test_deceptive_mines()",
                  "tests fence jumping with land mines");

    double coords[20][2] = {{84, 44}, // mine
                            {74, 0},  // mine
                            {54, 62}, // mine
                            {59, 0},  // mine
                            {34, 15}, // mine
                            {42, 63},
                            {96, 56}, // mine
                            {44, 79},
                            {44, 43},
                            {28, 10}, // mine
                            {60, 30}, // mine
                            {88, 72}, // mine
                            {75, 68}, // mine
                            {43, 65},
                            {48, 0},  // mine
                            {14, 15}, // mine
                            {49, 83},
                            {51, 35},
                            {95, 50},  // mine
                            {82, 20}}; // mine
    bool isMine[20] = {1, 1, 1, 1, 1, 0, 1, 0, 0, 1,
                       1, 1, 1, 0, 1, 1, 0, 0, 1, 1};
    double targetCoords[2] = {45, 59};
    double expectedCoords[2] = {42, 63};

    vector<point<2>> points;
    for (int i = 0; i < 20; ++i)
        points.push_back(point<2>(coords[i], isMine[i]));
    point<2> target(targetCoords);
    point<2> expected(expectedCoords);

    kd_tree<2> tree(points);
    tree.print(cout, color_scheme);
    cout << "  ~    ~    ~    ~    ~    ~    ~" << endl;

    // Turn on land mines to make sure we don't step on them
    point<2>::enable_mines = true;
    cout << "find_nearest_neighbor(" << target
         << ") result   = " << tree.find_nearest_neighbor(target) << endl;
    cout << "find_nearest_neighbor(" << target << ") expected = " << expected
         << endl;
    point<2>::enable_mines = false;

    cout << endl;
}

void test_tie_breaking()
{
    output_header("test_tie_breaking()",
                  "tests operator< used to break ties (building & searching)");

    double coords[14][3] = {{0, 0, 100},   // mine
                            {0, 100, 100}, // mine
                            {0, 50, 50},   // mine
                            {0, 0, 50},    // mine
                            {100, 0, 100}, // mine
                            {100, 100, 0},
                            {0, 0, 0},
                            {0, 50, 0},  // mine
                            {50, 0, 0},  // mine
                            {50, 0, 50}, // mine
                            {100, 0, 0}, // mine
                            {50, 50, 0},
                            {0, 100, 0}, // mine
                            {50, 50, 50}};
    bool isMine[14] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    double targetCoords[3] = {50, 100, 0};
    double expectedCoords[3] = {0, 100, 0};
    double targetCoords2[3] = {14, 17, 20};
    double expectedCoords2[3] = {0, 0, 0};

    vector<point<3>> points;
    for (int i = 0; i < 14; ++i)
        points.push_back(point<3>(coords[i], isMine[i]));
    point<3> target(targetCoords);
    point<3> expected(expectedCoords);
    point<3> target2(targetCoords2);
    point<3> expected2(expectedCoords2);

    kd_tree<3> tree(points);
    tree.print(cout, color_scheme);
    cout << "  ~    ~    ~    ~    ~    ~    ~" << endl;

    // Turn on land mines to make sure we don't step on them
    point<3>::enable_mines = true;
    cout << "Test finds operator< smallest on other side:" << endl;
    cout << "find_nearest_neighbor(" << target
         << ") result   = " << tree.find_nearest_neighbor(target) << endl;
    cout << "find_nearest_neighbor(" << target << ") expected = " << expected
         << endl << endl;

    cout << "Test condition for going to other side:" << endl;
    cout << "find_nearest_neighbor(" << target2
         << ") result   = " << tree.find_nearest_neighbor(target2) << endl;
    cout << "find_nearest_neighbor(" << target2 << ") expected = " << expected2
         << endl;
    point<3>::enable_mines = false;

    cout << endl;
}

void test_left_recurse()
{
    output_header("test_left_recurse()",
                  "tests that left recursion doesn't include the root node");

    double coords[10][3] = {{5, 3, 1},
                            {3, 1, 10},
                            {2, 0, 8},
                            {4, 3, 0},
                            {0, 2, 9},
                            {10, 10, 10},
                            {11, 11, 11},
                            {12, 12, 12},
                            {13, 13, 13},
                            {14, 14, 14}};
    bool isMine[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    double targetCoords[3] = {1, 1, 9};
    double expectedCoords[3] = {0, 2, 9};

    vector<point<3>> points;
    for (int i = 0; i < 10; ++i)
        points.push_back(point<3>(coords[i], isMine[i]));
    point<3> target(targetCoords);
    point<3> expected(expectedCoords);

    kd_tree<3> tree(points);
    tree.print(cout, color_scheme);
    cout << "  ~    ~    ~    ~    ~    ~    ~" << endl;

    // Turn on land mines to make sure we don't step on them
    point<3>::enable_mines = true;
    cout << "find_nearest_neighbor(" << target
         << ") result   = " << tree.find_nearest_neighbor(target) << endl;
    cout << "find_nearest_neighbor(" << target << ") expected = " << expected
         << endl;
    point<3>::enable_mines = false;
    cout << endl;
}

int main(int argc, char** argv)
{
    // set global bools for colored output
    color_scheme = colored_out::DISABLE;
    is_terminal = isatty(STDOUT_FILENO);
    is_colored = (argc > 1 && tolower(argv[1][0]) == 'c') && is_terminal;
    if (is_terminal)
        color_scheme = colored_out::ENABLE;

    // colored output magic
    colored_out coloroutput;
    if (is_colored)
    {
        coloroutput.set_expected_file("soln_testkdtree.out");
        int status = coloroutput.start();
        if (status < 0)
            return 0;
    }

    cout << std::boolalpha; // set true and false to print true/false instead of
                            // 1/0

    // test cases
    test_smaller_in_dimension();
    test_should_replace();
    test_linear_ctor<1>(10);
    test_linear_nns<1>(10);
    test_linear_ctor<3>(31);
    test_linear_nns<3>(31);
    test_deceptive_nn_one_level();
    test_mines();
    test_deceptive_mines();
    test_tie_breaking();
    test_left_recurse();
}

