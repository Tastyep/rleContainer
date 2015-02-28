#ifndef _RLECONTAINER_H_
#define _RLECONTAINER_H_

#include <vector>
#include <iostream>

template <typename T>
struct	rleData
{
  T		data;
  unsigned int	count;

  rleData(const T &d, unsigned c) :
    data(d), count(c)
  {
  }
};

enum class blockPlacement { Align, Fragment };

template <typename T>
class rleContainer
{
public:
  rleContainer();
  ~rleContainer() = default;

  void		push_back(const T &val);
  std::size_t	memory_used() const;
  T		&operator[](unsigned int idx);

private:
  bool	tryMerge(std::vector<T> &vec, const T &val);

private:
  std::vector<rleData<T> >	_aligned;
  std::vector<std::vector<T> >	_fragmented;
  std::vector<unsigned int>	_links;
  blockPlacement		_last;
};

template <typename T>
rleContainer<T>::rleContainer() :
  _last(blockPlacement::Fragment)
{
}

template <typename T>
void	rleContainer<T>::push_back(const T &val)
{
  if (_last == blockPlacement::Fragment)
    {
      if (!_fragmented.empty())
	{
	  std::vector<T>	&vec = _fragmented.back();

	  if (!vec.empty() && vec.back() == val)
	    {
	      if (tryMerge(vec, val) == false)
		vec.push_back(val);
	      else
		_last = blockPlacement::Align;
	    }
	  else
	    vec.push_back(val);
	}
      else // means it's the first call
	{
	  _fragmented.push_back({val});
	  _links.push_back(_aligned.size());
	}
    }
  else
    {
      if (_aligned.back().data == val)
	++_aligned.back().count;
      else
	{
	  _fragmented.push_back({val});
	  _links.push_back(_aligned.size());
	  _last = blockPlacement::Fragment;
	}
    }
}

template <typename T>
bool	rleContainer<T>::tryMerge(std::vector<T> &vec, const T &val)
{
  unsigned int	counter = 1;
  unsigned int	gain = 0;

  for (auto rit = vec.rbegin() + 1; rit != vec.rend(); ++rit)
    {
      if (*rit == val)
	++counter;
      else
	break;
    }

  if (counter == vec.size()) // means it will be emptyed
    gain += sizeof(unsigned int);
  if ((counter + 1) * sizeof(T) > sizeof(rleData<T>) - gain)
    _aligned.push_back(rleData<T>(val, counter + 1));
  else
    return false;
  if (gain)
    {
      _fragmented.pop_back();
      _links.pop_back();
    }
  else
    {
      for (unsigned int remover = 0; remover < counter; ++remover)
	vec.pop_back();
    }
  return true;
}

template <typename T>
T	&rleContainer<T>::operator[](unsigned int idx)
{
  unsigned int	counter = 0;
  unsigned int	linkIdx = 0;
  unsigned int	alignIdx = 0;
  unsigned int	nextlink = -1;

  if (!_links.empty())
    nextlink = _links[linkIdx];
  while (counter <= idx)
    {
      if (alignIdx == nextlink)
	{
	  std::vector<T>	&vec = _fragmented[linkIdx++];
	  int			pos = idx - counter;

	  if (pos < vec.size())
	    return (vec[pos]);
	  counter += vec.size();
	  if (linkIdx < _links.size())
	    nextlink = _links[linkIdx];
	  else
	    nextlink = -1;
	}
      else
	{
	  int	pos = idx - counter;

	  if (pos < _aligned[alignIdx].count)
	    return _aligned[alignIdx].data;
	  counter += _aligned[alignIdx].count;
	  ++alignIdx;
	}
    }
}

template <typename T>
std::size_t	rleContainer<T>::memory_used() const
{
  std::size_t	fragmentSize = 0;

  for (auto vec : _fragmented)
    fragmentSize += vec.size() * sizeof(T);
  return _aligned.size() * sizeof(rleData<T>) + fragmentSize + _links.size() * sizeof(unsigned int);
}

#endif /* _RLECONTAINER_H_ */
