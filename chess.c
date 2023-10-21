/*
  Partial Chess
  Made by: Miten
  Date: 2023-21-10
*/

#include <stdio.h>
#include <string.h>

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

int main() {
  // read in an 8x8 matrix from standard input
  // store the location as x and y coordinates
  Matrix matrix;
  int x = 0, y = 0;
  char value;

  while (scanf("%c", &value) != EOF) {
    // if newline, increment y and reset x
    if (value == '\n') {
      x = 0;
      y++;
    } else {
      // store the value in the matrix if within range
      if (x < 8 && y < 8) {
        int index = x + y * 8;
        matrix.points[index].x = x;
        matrix.points[index].y = y;
        matrix.points[index].value = value;
      }
      x++;
    }
  }

  // set the algebraic notation for each point and print it
  setAlgebraic(&matrix, 1);

  // find the queen(s) [q]
  for (int i = 0; i < 64; i++) {
    if (matrix.points[i].value == 'q') {
      printf("\nFound a queen at %s\n", matrix.points[i].algebraic);

      // find and print the queen's possible moves
      pathfinder(&matrix, &matrix.points[i]);
    }
  }

  return 0;
}

void printMatrix(Matrix matrix) {
  // print the top border
  printf("Board: \n  +-+-+-+-+-+-+-+-+\n");

  // print the matrix using the algebraic notation and value (a8 = top left, h1
  // = bottom right)
  for (int i = 0; i < 64; i++) {
    // print the left border, value, and right border in a single call to printf
    if (i % 8 == 0) {
      printf("%d |%c|", 8 - i / 8, matrix.points[i].value);
    } else if (i % 8 == 7) {
      printf("%c|\n  +-+-+-+-+-+-+-+-+\n", matrix.points[i].value);
    } else {
      printf("%c|", matrix.points[i].value);
    }
  }

  // print the x coordinate
  printf("   a b c d e f g h\n\n");
}

void setAlgebraic(Matrix *matrix, int toPrint) {
  // set the algebraic notation for each point
  // based on the x and y coordinates
  // a8 = top left, h1 = bottom right

  for (int i = 0; i < 64; i++) {
    int x = matrix->points[i].x;
    int y = matrix->points[i].y;

    if (x < 0 || x > 7 || y < 0 || y > 7) {
      continue;
    }

    // set the algebraic notation for each point
    matrix->points[i].algebraic[0] = 'a' + x;
    matrix->points[i].algebraic[1] = '8' - y;
    matrix->points[i].algebraic[2] = '\0';
  }

  if (toPrint) {
    printMatrix(*matrix);
  }
}

void pathfinder(Matrix *matrix, Point *point) {
  char moves[64][10];
  char move_count = 0;

  // Define the 8 directions a queen can move in a 2D array
  int directions[8][2] = {{-1, 0},  {1, 0},  {0, -1}, {0, 1},
                          {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

  // Loop through each direction
  for (int d = 0; d < 8; d++) {
    for (int i = point->x + directions[d][0], j = point->y + directions[d][1];
         i >= 0 && i < 8 && j >= 0 && j < 8;
         i += directions[d][0], j += directions[d][1]) {

      // If the point is empty or an opponent's piece
      if (matrix->points[i + j * 8].value == ' ' ||
          (matrix->points[i + j * 8].value >= 'A' &&
           matrix->points[i + j * 8].value <= 'Z')) {
        strncpy(moves[move_count], matrix->points[i + j * 8].algebraic, 2);
        if (matrix->points[i + j * 8].value != ' ') {
          moves[move_count][2] = 'x';
          moves[move_count][3] = matrix->points[i + j * 8].value;
          moves[move_count][4] = '\0';
        } else {
          moves[move_count][2] = '\0';
        }
        move_count++;
      }

      // If the point is an opponent's king
      if (matrix->points[i + j * 8].value == 'K') {
        strcpy(moves[move_count], "checkmate");
        move_count++;
      }

      // If the point is own piece or an opponent's piece, stop
      if (matrix->points[i + j * 8].value != ' ') {
        break;
      }
    }
  }

  // print the moves, if none print "No moves for the queen."
  if (move_count == 0) {
    printf("No moves for the queen.\n");
  } else {
    // if found, print "checkmate"
    // else print the moves
    char *checkmate_move = NULL;
    for (int i = 0; i < move_count; i++) {
      if (strcmp(moves[i], "checkmate") == 0) {
        checkmate_move = moves[i];
        break;
      }
    }

    if (checkmate_move != NULL) {
      printf("%s\n", checkmate_move);
    } else {
      for (int i = 0; i < move_count; i++) {
        printf("%s\n", moves[i]);
      }
    }
  }
}