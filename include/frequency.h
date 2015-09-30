/**
 * @file frequency.h
 * Definition of a character frequency class.
 *
 * @author Chase Geigle
 * @date Summer 2012
 */

#ifndef FREQUENCY_H_
#define FREQUENCY_H_

/**
 * frequency: a plain-old-data type representing a character and its
 * frequency. Used by the HuffmanTree as the data element of the TreeNodes.
 * If a node is an internal node, the character is the null character ('\0').
 *
 * @author Chase Geigle
 * @date Summer 2012
 */
class frequency
{
  public:
    /**
     * Constructs a new frequency object from a given character and its
     * associated frequency in a file.
     *
     * @param character The character this frequency represents.
     * @param count The number of times this character appears in
     * the file.
     */
    frequency(char character, int count);

    /**
     * Constructs a new frequency object from a given frequency count.
     * Frequencies created this way have the null character ('\0') as
     * their character data (these kinds of frequency objects are used
     * as the data of internal nodes in the HuffmanTree).
     *
     * @param count The number to be stored.
     */
    frequency(int count);

    /**
     * Getter for the character for this frequency object.
     *
     * @return The character for this frequency.
     */
    char character() const;

    /**
     * Getter for the count for this frequency object.
     *
     * @return The count member of this object.
     */
    int count() const;

  private:
    char character_;
    int count_;
};

/**
 * Comparison operator for frequency objects. Allows for sorting
 * with std::sort.
 *
 * @param lhs The frequency object.
 * @param rhs The frequency object to compare against.
 * @return Whether the current frequency (lhs) is less than the given
 * frequency (rhs).
 */
bool operator<(const frequency& lhs, const frequency& rhs);
#endif
