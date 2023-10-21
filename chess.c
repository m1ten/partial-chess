#include <stdio.h>

typedef struct {
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
typedef struct {
  // 8x8 = 64 points
  Point points[64];
} Matrix;

void setAlgebraic(Matrix *matrix, int toPrint);
void pathfinder(Matrix *matrix, Point *point);

int main() {
  // read in an 8x8 matrix from standard input
  // store the location as x and y coordinates
  Matrix matrix;
  int x, y = 0;
  char value;
  while (scanf("%c", &value) != EOF) {
    // if newline, increment y and reset x
    if (value == '\n') {
      x = 0;
      y++;
    } else {
      // store the value in the matrix if within range
      if (x < 8 && y < 8) {
        matrix.points[x + y * 8].x = x;
        matrix.points[x + y * 8].y = y;
        matrix.points[x + y * 8].value = value;
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

// print out the matrix
void printMatrix(Matrix matrix) {
  /* Format:
    +-+-+-+-+-+-+-+-+
  8 |R|N|B|Q|K|B|N|R|
    +-+-+-+-+-+-+-+-+
  7 |P|P|P|P|P|P|P|P|
    +-+-+-+-+-+-+-+-+
  6 | | | | | | | | |
    +-+-+-+-+-+-+-+-+
  5 | | | | | | | | |
    +-+-+-+-+-+-+-+-+
  4 | | | | | | | | |
    +-+-+-+-+-+-+-+-+
  3 | | | | | | | | |
    +-+-+-+-+-+-+-+-+
  2 |p|p|p|p|p|p|p|p|
    +-+-+-+-+-+-+-+-+
  1 |r|n|b|q|k|b|n|r|
    +-+-+-+-+-+-+-+-+
     a b c d e f g h
  */

  // print the top border
  printf("Board: \n  +-+-+-+-+-+-+-+-+\n");

  // print the matrix using the algebraic notation and value (a8 = top left, h1
  // = bottom right)
  for (int i = 0; i < 64; i++) {
    // print the left border
    if (i % 8 == 0) {
      printf("%d |", 8 - i / 8);
    }

    // print the value
    printf("%c|", matrix.points[i].value);

    // print the right border
    if (i % 8 == 7) {
      printf("\n");
      printf("  +-+-+-+-+-+-+-+-+\n");
    }
  }

  // print the x coordinate
  printf("   a b c d e f g h\n\n");
}

void setAlgebraic(Matrix *matrix, int toPrint) {
  // set the algebraic notation for each point
  // based on the x and y coordinates
  // a8 = top left, h1 = bottom right
  char col_values[8] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};

  for (int i = 0; i < 64; i++) {
    // set the algebraic notation for each point
    matrix->points[i].algebraic[0] = col_values[matrix->points[i].x];
    matrix->points[i].algebraic[1] = 8 - matrix->points[i].y + '0';
    matrix->points[i].algebraic[2] = '\0';
  }

  if (toPrint) {
    printMatrix(*matrix);
  }
}

void pathfinder(Matrix *matrix, Point *point) {
  // find all the possible moves for the queen until it hits a piece or the edge
  // of the board store them in array of algebraic notations if piece is
  // opponent's, store the alg notation + "x" + value of piece ('P', 'R', etc.)
  // if piece is opponent's king, store "checkmate"
  // if piece is own, ignore

  // find the queen's possible moves

  // largest string possible is "checkmate" (9 characters)
  char moves[64][10];
  char move_count = 0;

  // check the 8 directions
  // up
  for (int i = point->y - 1; i >= 0; i--) {
    // if the point is empty, add it to the list of moves
    if (matrix->points[point->x + i * 8].value == ' ') {
      moves[move_count][0] = matrix->points[point->x + i * 8].algebraic[0];
      moves[move_count][1] = matrix->points[point->x + i * 8].algebraic[1];
      moves[move_count][2] = '\0';
      move_count++;
    }
    // if the point is an opponent's piece, add it to the list of moves
    else if (matrix->points[point->x + i * 8].value >= 'A' &&
             matrix->points[point->x + i * 8].value <= 'Z') {
      moves[move_count][0] = matrix->points[point->x + i * 8].algebraic[0];
      moves[move_count][1] = matrix->points[point->x + i * 8].algebraic[1];
      moves[move_count][2] = 'x';
      moves[move_count][3] = matrix->points[point->x + i * 8].value;
      moves[move_count][4] = '\0';
      move_count++;
      break;
    }
    // if the point is an opponent's king, add it to the list of moves
    else if (matrix->points[point->x + i * 8].value == 'K') {
      moves[move_count][0] = 'c';
      moves[move_count][1] = 'h';
      moves[move_count][2] = 'e';
      moves[move_count][3] = 'c';
      moves[move_count][4] = 'k';
      moves[move_count][5] = 'm';
      moves[move_count][6] = 'a';
      moves[move_count][7] = 't';
      moves[move_count][8] = 'e';
      moves[move_count][9] = '\0';
      move_count++;
      break;
    }

    // if the point is own piece, stop
    else if (matrix->points[point->x + i * 8].value >= 'a' &&
             matrix->points[point->x + i * 8].value <= 'z') {
      break;
    }
  }

  // down
  for (int i = point->y + 1; i < 8; i++) {
    // if the point is empty, add it to the list of moves
    if (matrix->points[point->x + i * 8].value == ' ') {
      moves[move_count][0] = matrix->points[point->x + i * 8].algebraic[0];
      moves[move_count][1] = matrix->points[point->x + i * 8].algebraic[1];
      moves[move_count][2] = '\0';
      move_count++;
    }
    // if the point is an opponent's piece, add it to the list of moves
    else if (matrix->points[point->x + i * 8].value >= 'A' &&
             matrix->points[point->x + i * 8].value <= 'Z') {
      moves[move_count][0] = matrix->points[point->x + i * 8].algebraic[0];
      moves[move_count][1] = matrix->points[point->x + i * 8].algebraic[1];
      moves[move_count][2] = 'x';
      moves[move_count][3] = matrix->points[point->x + i * 8].value;
      moves[move_count][4] = '\0';
      move_count++;
      break;
    }
    // if the point is an opponent's king, add it to the list of moves
    else if (matrix->points[point->x + i * 8].value == 'K') {
      moves[move_count][0] = 'c';
      moves[move_count][1] = 'h';
      moves[move_count][2] = 'e';
      moves[move_count][3] = 'c';
      moves[move_count][4] = 'k';
      moves[move_count][5] = 'm';
      moves[move_count][6] = 'a';
      moves[move_count][7] = 't';
      moves[move_count][8] = 'e';
      moves[move_count][9] = '\0';
      move_count++;
      break;
    }

    // if the point is own piece, stop
    else if (matrix->points[point->x + i * 8].value >= 'a' &&
             matrix->points[point->x + i * 8].value <= 'z') {
      break;
    }
  }

  // left
  for (int i = point->x - 1; i >= 0; i--) {
    // if the point is empty, add it to the list of moves
    if (matrix->points[i + point->y * 8].value == ' ') {
      moves[move_count][0] = matrix->points[i + point->y * 8].algebraic[0];
      moves[move_count][1] = matrix->points[i + point->y * 8].algebraic[1];
      moves[move_count][2] = '\0';
      move_count++;
    }
    // if the point is an opponent's piece, add it to the list of moves
    else if (matrix->points[i + point->y * 8].value >= 'A' &&
             matrix->points[i + point->y * 8].value <= 'Z') {
      moves[move_count][0] = matrix->points[i + point->y * 8].algebraic[0];
      moves[move_count][1] = matrix->points[i + point->y * 8].algebraic[1];
      moves[move_count][2] = 'x';
      moves[move_count][3] = matrix->points[i + point->y * 8].value;
      moves[move_count][4] = '\0';
      move_count++;
      break;
    }
    // if the point is an opponent's king, add it to the list of moves
    else if (matrix->points[i + point->y * 8].value == 'K') {
      moves[move_count][0] = 'c';
      moves[move_count][1] = 'h';
      moves[move_count][2] = 'e';
      moves[move_count][3] = 'c';
      moves[move_count][4] = 'k';
      moves[move_count][5] = 'm';
      moves[move_count][6] = 'a';
      moves[move_count][7] = 't';
      moves[move_count][8] = 'e';
      moves[move_count][9] = '\0';
      move_count++;
      break;
    }

    // if the point is own piece, stop
    else if (matrix->points[i + point->y * 8].value >= 'a' &&
             matrix->points[i + point->y * 8].value <= 'z') {
      break;
    }
  }

  // right
  for (int i = point->x + 1; i < 8; i++) {
    // if the point is empty, add it to the list of moves
    if (matrix->points[i + point->y * 8].value == ' ') {
      moves[move_count][0] = matrix->points[i + point->y * 8].algebraic[0];
      moves[move_count][1] = matrix->points[i + point->y * 8].algebraic[1];
      moves[move_count][2] = '\0';
      move_count++;
    }
    // if the point is an opponent's piece, add it to the list of moves
    else if (matrix->points[i + point->y * 8].value >= 'A' &&
             matrix->points[i + point->y * 8].value <= 'Z') {
      moves[move_count][0] = matrix->points[i + point->y * 8].algebraic[0];
      moves[move_count][1] = matrix->points[i + point->y * 8].algebraic[1];
      moves[move_count][2] = 'x';
      moves[move_count][3] = matrix->points[i + point->y * 8].value;
      moves[move_count][4] = '\0';
      move_count++;
      break;
    }
    // if the point is an opponent's king, add it to the list of moves
    else if (matrix->points[i + point->y * 8].value == 'K') {
      moves[move_count][0] = 'c';
      moves[move_count][1] = 'h';
      moves[move_count][2] = 'e';
      moves[move_count][3] = 'c';
      moves[move_count][4] = 'k';
      moves[move_count][5] = 'm';
      moves[move_count][6] = 'a';
      moves[move_count][7] = 't';
      moves[move_count][8] = 'e';
      moves[move_count][9] = '\0';
      move_count++;
      break;
    }

    // if the point is own piece, stop
    else if (matrix->points[i + point->y * 8].value >= 'a' &&
             matrix->points[i + point->y * 8].value <= 'z') {
      break;
    }
  }

  // up-left
  for (int i = 1; point->x - i >= 0 && point->y - i >= 0; i++) {
    // if the point is empty, add it to the list of moves
    if (matrix->points[point->x - i + (point->y - i) * 8].value == ' ') {
      moves[move_count][0] =
          matrix->points[point->x - i + (point->y - i) * 8].algebraic[0];
      moves[move_count][1] =
          matrix->points[point->x - i + (point->y - i) * 8].algebraic[1];
      moves[move_count][2] = '\0';
      move_count++;
    }
    // if the point is an opponent's piece, add it to the list of moves
    else if (matrix->points[point->x - i + (point->y - i) * 8].value >= 'A' &&
             matrix->points[point->x - i + (point->y - i) * 8].value <= 'Z') {
      moves[move_count][0] =
          matrix->points[point->x - i + (point->y - i) * 8].algebraic[0];
      moves[move_count][1] =
          matrix->points[point->x - i + (point->y - i) * 8].algebraic[1];
      moves[move_count][2] = 'x';
      moves[move_count][3] =
          matrix->points[point->x - i + (point->y - i) * 8].value;
      moves[move_count][4] = '\0';
      move_count++;
      break;
    }
    // if the point is an opponent's king, add it to the list of moves
    else if (matrix->points[point->x - i + (point->y - i) * 8].value == 'K') {
      moves[move_count][0] = 'c';
      moves[move_count][1] = 'h';
      moves[move_count][2] = 'e';
      moves[move_count][3] = 'c';
      moves[move_count][4] = 'k';
      moves[move_count][5] = 'm';
      moves[move_count][6] = 'a';
      moves[move_count][7] = 't';
      moves[move_count][8] = 'e';
      moves[move_count][9] = '\0';
      move_count++;
      break;
    }

    // if the point is own piece, stop
    else if (matrix->points[point->x - i + (point->y - i) * 8].value >= 'a' &&
             matrix->points[point->x - i + (point->y - i) * 8].value <= 'z') {
      break;
    }
  }

  // up-right
  for (int i = 1; point->x + i < 8 && point->y - i >= 0; i++) {
    // if the point is empty, add it to the list of moves
    if (matrix->points[point->x + i + (point->y - i) * 8].value == ' ') {
      moves[move_count][0] =
          matrix->points[point->x + i + (point->y - i) * 8].algebraic[0];
      moves[move_count][1] =
          matrix->points[point->x + i + (point->y - i) * 8].algebraic[1];
      moves[move_count][2] = '\0';
      move_count++;
    }
    // if the point is an opponent's piece, add it to the list of moves
    else if (matrix->points[point->x + i + (point->y - i) * 8].value >= 'A' &&
             matrix->points[point->x + i + (point->y - i) * 8].value <= 'Z') {
      moves[move_count][0] =
          matrix->points[point->x + i + (point->y - i) * 8].algebraic[0];
      moves[move_count][1] =
          matrix->points[point->x + i + (point->y - i) * 8].algebraic[1];
      moves[move_count][2] = 'x';
      moves[move_count][3] =
          matrix->points[point->x + i + (point->y - i) * 8].value;
      moves[move_count][4] = '\0';
      move_count++;
      break;
    }
    // if the point is an opponent's king, add it to the list of moves
    else if (matrix->points[point->x + i + (point->y - i) * 8].value == 'K') {
      moves[move_count][0] = 'c';
      moves[move_count][1] = 'h';
      moves[move_count][2] = 'e';
      moves[move_count][3] = 'c';
      moves[move_count][4] = 'k';
      moves[move_count][5] = 'm';
      moves[move_count][6] = 'a';
      moves[move_count][7] = 't';
      moves[move_count][8] = 'e';
      moves[move_count][9] = '\0';
      move_count++;
      break;
    }

    // if the point is own piece, stop
    else if (matrix->points[point->x + i + (point->y - i) * 8].value >= 'a' &&
             matrix->points[point->x + i + (point->y - i) * 8].value <= 'z') {
      break;
    }
  }

  // down-left
  for (int i = 1; point->x - i >= 0 && point->y + i < 8; i++) {
    // if the point is empty, add it to the list of moves
    if (matrix->points[point->x - i + (point->y + i) * 8].value == ' ') {
      moves[move_count][0] =
          matrix->points[point->x - i + (point->y + i) * 8].algebraic[0];
      moves[move_count][1] =
          matrix->points[point->x - i + (point->y + i) * 8].algebraic[1];
      moves[move_count][2] = '\0';
      move_count++;
    }
    // if the point is an opponent's piece, add it to the list of moves
    else if (matrix->points[point->x - i + (point->y + i) * 8].value >= 'A' &&
             matrix->points[point->x - i + (point->y + i) * 8].value <= 'Z') {
      moves[move_count][0] =
          matrix->points[point->x - i + (point->y + i) * 8].algebraic[0];
      moves[move_count][1] =
          matrix->points[point->x - i + (point->y + i) * 8].algebraic[1];
      moves[move_count][2] = 'x';
      moves[move_count][3] =
          matrix->points[point->x - i + (point->y + i) * 8].value;
      moves[move_count][4] = '\0';
      move_count++;
      break;
    }
    // if the point is an opponent's king, add it to the list of moves
    else if (matrix->points[point->x - i + (point->y + i) * 8].value == 'K') {
      moves[move_count][0] = 'c';
      moves[move_count][1] = 'h';
      moves[move_count][2] = 'e';
      moves[move_count][3] = 'c';
      moves[move_count][4] = 'k';
      moves[move_count][5] = 'm';
      moves[move_count][6] = 'a';
      moves[move_count][7] = 't';
      moves[move_count][8] = 'e';
      moves[move_count][9] = '\0';
      move_count++;
      break;
    }

    // if the point is own piece, stop
    else if (matrix->points[point->x - i + (point->y + i) * 8].value >= 'a' &&
             matrix->points[point->x - i + (point->y + i) * 8].value <= 'z') {
      break;
    }
  }

  // down-right
  for (int i = 1; point->x + i < 8 && point->y + i < 8; i++) {
    // if the point is empty, add it to the list of moves
    if (matrix->points[point->x + i + (point->y + i) * 8].value == ' ') {
      moves[move_count][0] =
          matrix->points[point->x + i + (point->y + i) * 8].algebraic[0];
      moves[move_count][1] =
          matrix->points[point->x + i + (point->y + i) * 8].algebraic[1];
      moves[move_count][2] = '\0';
      move_count++;
    }
    // if the point is an opponent's piece, add it to the list of moves
    else if (matrix->points[point->x + i + (point->y + i) * 8].value >= 'A' &&
             matrix->points[point->x + i + (point->y + i) * 8].value <= 'Z') {
      moves[move_count][0] =
          matrix->points[point->x + i + (point->y + i) * 8].algebraic[0];
      moves[move_count][1] =
          matrix->points[point->x + i + (point->y + i) * 8].algebraic[1];
      moves[move_count][2] = 'x';
      moves[move_count][3] =
          matrix->points[point->x + i + (point->y + i) * 8].value;
      moves[move_count][4] = '\0';
      move_count++;
      break;
    }
    // if the point is an opponent's king, add it to the list of moves
    else if (matrix->points[point->x + i + (point->y + i) * 8].value == 'K') {
      moves[move_count][0] = 'c';
      moves[move_count][1] = 'h';
      moves[move_count][2] = 'e';
      moves[move_count][3] = 'c';
      moves[move_count][4] = 'k';
      moves[move_count][5] = 'm';
      moves[move_count][6] = 'a';
      moves[move_count][7] = 't';
      moves[move_count][8] = 'e';
      moves[move_count][9] = '\0';
      move_count++;
      break;
    }

    // if the point is own piece, stop
    else if (matrix->points[point->x + i + (point->y + i) * 8].value >= 'a' &&
             matrix->points[point->x + i + (point->y + i) * 8].value <= 'z') {
      break;
    }
  }

  // print the moves, if none print "No moves for the queen."
  if (move_count == 0) {
    printf("No moves for the queen.\n");
  } else {
    for (int i = 0; i < move_count; i++) {
      printf("%s\n", moves[i]);
    }
  }
}