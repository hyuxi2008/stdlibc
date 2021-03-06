/*
** list.c for list in /home/davy/Rendu/stdlibc/stdlibc
**
** Made by davy tran
** Login   <tran_y@epitech.net>
**
** Started on  Wed Feb  4 19:21:25 2015 davy tran
** Last update Wed Sep 30 16:35:11 2015 davy
*/

#include <string.h>
#include "list.h"

static int const	g_default_val = 0;

static void constructor(list *this)
{
  if (this != NULL)
    memset(this, 0, sizeof(*this));
  if ((this->content = calloc(sizeof(*(this->content)), 1)) == NULL)
    exit(EXIT_FAILURE);
  this->size = 0;
}

static void destructor(list *this)
{
  if (this == NULL)
    return ;
  g_list.clear(this);
  if (this->content != NULL)
    free(this->content);
  this->content = NULL;
}

static iterator begin(list *this)
{
  iterator      it;

  for (it = this->content; it != NULL && it->rewind != NULL; previous(it))
    ;
  return (it);
}

static iterator end(list *this)
{
  iterator      it;

  it = this->content;
  while (it != NULL && it->forward != NULL)
    next(it);
  return (it);
}

static bool empty(list *this)
{
  return (!this || !this->content || !this->content->value);
}

static size_type size(list *this)
{
  size_type		   n;

  n = 0;
  for (iterator it = g_list.begin(this); it != NULL; next(it), ++n)
    ;
  return (n);
}

static size_type  max_size(list *this)
{
  return ((size_type)this);
}

static void *front(list *this)
{
  iterator  it;

  it = begin(this);
  return (it != NULL ? it->value : NULL);
}

static void *back(list *this)
{
  iterator  it;

  it = end(this);
  return (it != NULL ? it->value : NULL);
}

static void assign(list *this, iterator first, iterator last)
{
  g_list.clear(this);
  for (iterator it = first; it != NULL && it != last; next(it))
    g_list.push_back(this, it->value);
}

static void push_front(list *this, const value_type val)
{
  iterator  it;

  if (g_list.begin(this)->value == NULL)
  {
    g_list.begin(this)->value = val;
    return ;
  }
  if ((it = calloc(sizeof(*it), 1)) == NULL)
    exit(EXIT_FAILURE);
  it->value = val;
  it->rewind = NULL;
  it->forward = NULL;
  if ((this->content = g_list.begin(this)) != NULL)
  {
    this->content->rewind = it;
    it->forward = this->content;
    this->content = it;
  }
}

static void pop_front(list *this)
{
  if ((this->content = begin(this)) != NULL && this->content->forward != NULL)
  {
    this->content = this->content->forward;
    free(this->content->rewind);
    this->content->rewind = NULL;
  }
  else if (this->content != NULL && this->content->rewind == NULL)
    this->content->value = NULL;
}

static void push_back(list *this, const value_type val)
{
  iterator  it;
  iterator  end;

  if ((end = g_list.end(this)) != NULL && end->value == NULL)
  {
    end->value = val;
    return ;
  }
  if ((it = calloc(sizeof(*it), 1)) == NULL)
    exit(EXIT_FAILURE);
  it->value = val;
  if (end != NULL)
  {
    end->forward = it;
    it->rewind = end;
  }
  else
    this->content = it;
}

static void pop_back(list *this)
{
  iterator  it;

  if ((it = g_list.end(this)) != NULL && it->rewind != NULL)
  {
    it->rewind->forward = NULL;
    free(it);
  }
  else if (it != NULL && it->rewind == NULL)
    it->value = NULL;
}

static iterator insert(list *this, iterator position, const value_type val)
{
  iterator      elem;
  iterator      it;

  if ((elem = calloc(sizeof(*elem), 1)) == NULL)
    exit(EXIT_FAILURE);
  elem->value = val;
  elem->rewind = NULL;
  elem->forward = NULL;
  for (it = g_list.begin(this);
       it != NULL && it->forward != NULL && it != position; next(it));
  elem->forward = it->forward;
  elem->rewind = it;
  if (it->forward != NULL)
    it->forward->rewind = elem;
  it->forward = elem;
  return (NULL);
}

static iterator erase(list *this, iterator position)
{
  iterator      it;
  iterator      tmp;

  for (it = g_list.begin(this); it != NULL && it != position; next(it))
    ;
  if (it != position)
    return (NULL);
  tmp = it->forward;
  if (it->rewind != NULL)
    it->rewind->forward = it->forward;
  if (it->forward != NULL)
    it->forward->rewind = it->rewind;
  free(it);
  return (tmp);
}

static void swap(list *this, list *x)
{
  SWAP_PTR(this->content, x->content);
  SWAP_NBR(this->size, x->size);
}

static void resize(list *this, size_type n)
{
  size_type i;

  if ((i = g_list.size(this)) == n)
    return ;
  else if (i < n)
  {
    for (; i < n; ++i)
      g_list.push_back(this, (void *)(&g_default_val));
  }
  else
  {
    for (; i > n; --i)
      g_list.pop_back(this);
  }
}

static void clear(list *this)
{
  iterator  it;
  iterator  forward;

  if (g_list.size(this) == 0)
    return ;
  if (g_list.size(this) == 1)
    return ((void)(this->content->value = NULL));
  if ((it = g_list.begin(this))->forward != NULL)
  {
    it->value = NULL;
    next(it);
  }
  while (it != NULL)
  {
    forward = it->forward;
    free(it);
    it = forward;
  }
}

static void splice(list *this, iterator position, list *x)
{
  iterator  it;
  iterator  tmp;

  if (x == NULL || x->content == NULL)
    return ;
  for (it = g_list.begin(this);
       it != NULL && it != position && it->forward != position; next(it));
  if (it == NULL)
    return ;
  tmp = it->forward;
  it->forward = x->content;
  x->content->rewind = it;
  it = g_list.end(x);
  if (tmp != NULL)
    tmp->rewind = it;
  if (it != NULL)
    it->forward = tmp;
  x->content = NULL;
}

static void remove(list *this, const value_type val)
{
  iterator  it;

  for (it = g_list.begin(this); it != NULL && it->value != val; next(it))
    ;
  if (it != NULL)
    g_list.erase(this, it);
}

static void remove_if(list *this, Predicate pred)
{
  for (iterator it = g_list.begin(this); it != NULL; next(it))
    if (pred(it->value))
      g_list.erase(this, it);
}

static void unique(list *this, BinaryPredicate binary_pred)
{
  for (iterator it = g_list.begin(this); it != NULL; next(it))
    for (iterator tmp = it->forward; tmp != NULL; )
      tmp = binary_pred(it->value, tmp->value) ? erase(this, tmp) : tmp->forward;
}

static void merge(list *this, list *x, Compare comp)
{
  splice(this, begin(this), x);
  g_list.sort(this, comp);
}

static void sort(list *this, Compare comp)
{
  for (iterator it = g_list.begin(this); it != NULL; next(it))
    for (iterator tmp = it->forward; tmp != NULL; next(tmp))
      if (comp(it->value, tmp->value))
        SWAP_PTR(it->value, tmp->value);
}

static void reverse(list *this)
{
  iterator  it1;
  iterator  it2;

  it1 = begin(this);
  it2 = end(this);
  while (it1 != NULL && it2 != NULL)
  {
    SWAP_PTR(it1->value, it2->value);
    if ((next(it1)) == it2 || (previous(it2)) == it1)
      return ;
  }
}

struct s_list_class	g_list = {
  &constructor,
  &destructor,
  &begin,
  &end,
  &empty,
  &size,
  &max_size,
  &front,
  &back,
  &assign,
  &push_front,
  &pop_front,
  &push_back,
  &pop_back,
  &insert,
  &erase,
  &swap,
  &resize,
  &clear,
  &splice,
  &remove,
  &remove_if,
  &unique,
  &merge,
  &sort,
  &reverse
};
