#ifndef INSET_H
#define INSET_H
int up(int, char[16][16]);
int down(int, char[16][16]);
int point(int, char[16][16]);
#endif

#ifdef inst

inst(UP, up)
inst(DOWN, down)
inst(POINT, point)

#endif
