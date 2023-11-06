#ifndef CHESS_H
#define CHESS_H

typedef struct Point {
  unsigned char x;
  unsigned char y;
  // algebraic notation
  // (a1, b1, c1, d1, e1, f1, g1, h1, a2, b2, ..., h8)
  char algebraic[3];
  // chess piece value
  // (q, r, b, n, p, k, Q, R, B, N, P, K) or 0 if empty
  // case sensitive
  char value;
} Point;

// 8x8 matrix of points
typedef struct Matrix {
  // 8x8 = 64 points
  Point points[64];
} Matrix;

void setAlgebraic(Matrix *matrix, int toPrint);
void pathfinder(Matrix *matrix, Point *point);

#endif