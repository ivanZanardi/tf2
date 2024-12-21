// Test vector operations

#include <stdlib.h>
#include <time.h>
#include "tf2.h"

using namespace std;


std::vector<std::vector<int>> rand_mat(std::vector<int> shape)
{
  std::srand(unsigned(std::time(nullptr)));
  std::vector<std::vector<int>> a(shape[0], std::vector<int>(shape[1]));
  for (int i = 0; i < shape[0]; i++) {
    for (int j = 0; j < shape[1]; j++) {
      a[i][j] = std::rand();
    }
  }
  return a;
}


int main()
{
  std::vector<int> shape = {10,3};

  // Generate and save a 2D random array
  auto a = rand_mat(shape);
  tf2::csv::write("./data.csv", a);

  // Test cases: all the output data
  // files should be the same as 'data.csv'
  // ----------------------------------
  // Test 1: read and write the matrix
  auto a1 = tf2::csv::read<int>("./data.csv");
  tf2::csv::write("./data.T1.csv", a1);

  // Test 2.a: flatten and write the matrix (row-major)
  auto v1 = tf2::ops::flatten(a, false);
  tf2::csv::write("./data.T2.a.csv", v1, shape, true);

  // Test 2.b: flatten and write the matrix (column-major)
  auto v2 = tf2::ops::flatten(a, true);
  tf2::csv::write("./data.T2.b.csv", v2, shape, false);

  // Test 3.a: flatten, reshape and write the matrix (row-major)
  auto r1 = tf2::ops::reshape(v1, shape, false);
  tf2::csv::write("./data.T3.a.csv", r1);

  // Test 3.b: flatten, reshape and write the matrix (column-major)
  auto r2 = tf2::ops::reshape(v2, shape, true);
  tf2::csv::write("./data.T3.b.csv", r2);

  return 0;
}
