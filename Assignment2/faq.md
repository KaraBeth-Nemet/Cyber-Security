### Simplified DES FAQ sheet
- The reading order is **most** significant bit to **least** significant bit.
  - For example the input key (i) is indexed as shown in (ii)   
  - (i)  1101101101 
  - (ii) 0123456789  

- The number of bits that you should be looking at is not the number of bits that 
  are in the storage value (uint8, uint16, etc) that you'll need to use.
  - Therefore when you store a 10 bit value in a 16 bit you need to be very 
    rigorous about how you're indexing and interpreting it.

  - It would be easy to either 
    - A.) treat your key (iii) as being indexed as in (iv)
          at which point your p10 will be off.
      - (iii) 0b 0000 0011 0110 1101
      - (iv)     0123 4567 89AB CDEF 
    - B.) align your key to the left like in (v) which effectively multiplies 
          p10 by 64(2^6) and moves the alignment for all of the later bit wise 
          operations off of the lower 8 bits
    - (v) 0b 1101 1011 0100 0000

  - Something similar can happen with your 4 bit nibbles later when you'll be 
    working with bytes.

- In short the indexing is never based on the shape of the container, and 
  always based on the theoretical number of bits you're working with.
  - 10 bits in a uint16 would be indexed as 
  - 0b0000 0011 1111 1111
  -   xxxx xx01 2345 6789
  - 4 bits in a nibble would either be indexed as depending on whether you're
    working with the high or low nibble
  - 0b0000 1111
  -   xxxx 0123
  - OR
  - 0b1111 0000 
  -   0123 xxxx 
