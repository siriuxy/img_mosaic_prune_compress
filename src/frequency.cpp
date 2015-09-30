/**
 * @file frequency.cpp
 * Implementation of the frequency storage class.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */

#include "frequency.h"

frequency::frequency(int freq) : character_{'\0'}, count_{freq}
{
    // nothing
}

frequency::frequency(char c, int freq) : character_{c}, count_{freq}
{
    // nothing
}

char frequency::character() const
{
    return character_;
}

int frequency::count() const
{
    return count_;
}

bool operator<(const frequency& lhs, const frequency& rhs)
{
    return lhs.count() < rhs.count();
}
