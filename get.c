#include	"lemipc.h"

void		init(t_limit *limit)
{
  limit[0].beg = 0;
  limit[0].end = 6;
  limit[1].beg = 7;
  limit[1].end = 13;
  limit[2].beg = 14;
  limit[2].end = 20;
  limit[3].beg = 21;
  limit[3].end = 27;
  limit[4].beg = 28;
  limit[4].end = 34;
  limit[5].beg = 35;
  limit[5].end = 42;
  limit[6].beg = 43;
  limit[6].end = 49;
}

int		getX(int pos)
{
  t_limit	*limit;
  int		x;

  x = 0;
  limit = malloc(7 * sizeof(t_limit));
  init(limit);
  while (x < 7)
    {
      if (pos >= limit[x].beg && pos <= limit[x].end)
	break;
      x++;
    }
  return (x + 1);
}

int		getY(int pos)
{
  return (pos % 7);
}
