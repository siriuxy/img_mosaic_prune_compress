/**
 * @file point.tcc
 * Implementation of the point class.
 *
 * @author Matt Sachtler
 * @date Spring 2009
 */

#include "point.h"

template <int Dim>
bool point<Dim>::enable_mines = false;

template <int Dim>
point<Dim>::point()
    : am_mine_{false}
{
    for (int i = 0; i < Dim; ++i)
        vals_[i] = 0;
}

template <int Dim>
point<Dim>::point(double arr[Dim])
    : am_mine_{false}
{
    for (int i = 0; i < Dim; ++i)
        vals_[i] = arr[i];
}

template <int Dim>
point<Dim>::point(double arr[Dim], bool mine)
    : am_mine_{mine}
{
    for (int i = 0; i < Dim; ++i)
        vals_[i] = arr[i];
}

template <int Dim>
template <class... Ts>
point<Dim>::point(Ts... args)
    : vals_{static_cast<double>(args)...}, am_mine_{false}
{
    static_assert(sizeof...(Ts) == Dim,
                  "must specify all elements of the point");
}

template <int Dim>
double point<Dim>::operator[](int index) const
{
    if (enable_mines && am_mine_)
        std::cout << "Hit mine " << *this << std::endl;

    if (index >= Dim)
        throw std::out_of_range{"point index out of range"};

    return vals_[index];
}

template <int Dim>
double& point<Dim>::operator[](int index)
{

    if (enable_mines && am_mine_)
        std::cout << "Hit mine " << *this << std::endl;

    if (index >= Dim)
        throw std::out_of_range{"point index out of range"};

    return vals_[index];
}

template <int Dim>
bool point<Dim>::is_mine() const
{
    return am_mine_;
}

template <int Dim>
std::ostream& operator<<(std::ostream& out, const point<Dim>& p)
{
    out << (p.is_mine() ? '{' : '(');

    for (int i = 0; i < Dim - 1; ++i)
        out << p[i] << ", ";
    out << p[Dim - 1];

    out << (p.is_mine() ? '}' : ')');
    return out;
}

template <int Dim>
bool operator==(const point<Dim>& lhs, const point<Dim>& rhs)
{
    for (int i = 0; i < Dim; ++i)
    {
        if (lhs[i] != rhs[i])
            return false;
    }
    return true;
}

template <int Dim>
bool operator!=(const point<Dim>& lhs, const point<Dim>& rhs)
{
    return !(lhs == rhs);
}

template <int Dim>
bool operator<(const point<Dim>& lhs, const point<Dim>& rhs)
{
    for (int i = 0; i < Dim; ++i)
    {
        if (lhs[i] != rhs[i])
            return lhs[i] < rhs[i];
    }
    return false;
}

template <int Dim>
bool operator>(const point<Dim>& lhs, const point<Dim>& rhs)
{
    return rhs < lhs;
}

template <int Dim>
bool operator<=(const point<Dim>& lhs, const point<Dim>& rhs)
{
    return lhs < rhs || lhs == rhs;
}

template <int Dim>
bool operator>=(const point<Dim>& lhs, const point<Dim>& rhs)
{
    return lhs > rhs || lhs == rhs;
}
