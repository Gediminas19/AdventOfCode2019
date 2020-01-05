#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct segment {
  int x1;
  int y1;
  int x2;
  int y2;
} segment;


int main()
{
  char line1[1000];
  char line2[1000];
  scanf("%s\n%s\n", line1, line2);
  char *data1 = line1;
  char *data2 = line2;

  int len1 = 1;
  for (int j = 0; j < strlen(data1); j++) if (data1[j] ==',') len1++;
  int len2 = 1;
  for (int j = 0; j < strlen(data2); j++) if (data2[j] ==',') len2++;

  segment *hor1;
  segment *ver1;
  segment *hor2;
  segment *ver2;
  if (len1 % 2 == 0) {
    hor1 = calloc(len1/2, sizeof(segment));
    ver1 = calloc(len1/2, sizeof(segment));
  } else {
    if (data1[0] == 'R' || data1[0] == 'L') {
      hor1 = calloc(len1/2 + 1, sizeof(segment));
      ver1 = calloc(len1/2, sizeof(segment));
    } else {
      ver1 = calloc(len1/2 + 1, sizeof(segment));
      hor1 = calloc(len1/2, sizeof(segment));
    }
  }
  if (len2 % 2 == 0) {
    hor2 = calloc(len2/2, sizeof(segment));
    ver2 = calloc(len2/2, sizeof(segment));
  } else {
    if (data1[0] == 'R' || data1[0] == 'L') {
      hor2 = calloc(len2/2 + 1, sizeof(segment));
      ver2 = calloc(len2/2, sizeof(segment));
    } else {
      ver2 = calloc(len2/2 + 1, sizeof(segment));
      hor2 = calloc(len2/2, sizeof(segment));
    }
  }

  char dir;
  int dist;
  int h1 = 0, v1 = 0, h2 = 0, v2 = 0;
  int x = 0, y = 0;
  while(sscanf(data1, "%c%d,", &dir, &dist) == 1) {
    switch (dir) {
      case 'R':
        hor1[h1].x1 = x;
        hor1[h1].y1 = y;
        x += dist;
        hor1[h1].x2 = x;
        hor1[h1].y2 = y;
        h1 += 1;
        break;
      case 'L':
        hor1[h1].x1 = x;
        hor1[h1].y1 = y;
        x -= dist;
        hor1[h1].x2 = x;
        hor1[h1].y2 = y;
        h1 += 1;
        break;
      case 'U':
        ver1[v1].x1 = x;
        ver1[v1].y1 = y;
        y += dist;
        ver1[v1].x2 = x;
        ver1[v1].y2 = y;
        v1 += 1;
        break;
      case 'D':
        ver1[v1].x1 = x;
        ver1[v1].y1 = y;
        y -= dist;
        ver1[v1].x2 = x;
        ver1[v1].y2 = y;
        v1 += 1;
        break;
    }
  }

  x = 0;
  y = 0;
  while(sscanf(data2, "%c%d,", &dir, &dist) == 1) {
    switch (dir) {
      case 'R':
        hor2[h2].x1 = x;
        hor2[h2].y1 = y;
        x += dist;
        hor2[h2].x2 = x;
        hor2[h2].y2 = y;
        h2 += 1;
        break;
      case 'L':
        hor2[h2].x1 = x;
        hor2[h2].y1 = y;
        x -= dist;
        hor2[h2].x2 = x;
        hor2[h2].y2 = y;
        h2 += 1;
        break;
      case 'U':
        ver2[v2].x1 = x;
        ver2[v2].y1 = y;
        y += dist;
        ver2[v2].x2 = x;
        ver2[v2].y2 = y;
        v2 += 1;
        break;
      case 'D':
        ver2[v2].x1 = x;
        ver2[v2].y1 = y;
        y -= dist;
        ver2[v2].x2 = x;
        ver2[v2].y2 = y;
        v2 += 1;
        break;
    }
  }



  return 0;
}