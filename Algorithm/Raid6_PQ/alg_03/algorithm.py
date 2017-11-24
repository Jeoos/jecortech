#!/usr/bin/python
# coding=UTF-8 
# key : value => w : primitive_polynomial

primitive_polynomial_dict = {4: 0b10011,                            # x**4  + x     + 1  
                             8: (1 << 8) + 0b11101,                 # x**8  + x**4  + x**3 + x**2 +1  
                             16: (1 << 16) + (1 << 12) + 0b1011,    # x**16 + x**12 + x**3 + x    + 1
                             32: (1 << 32) + (1 << 22) + 0b111,     # x**32 + x**22 + x**2 + x    + 1
                             64: (1 << 64) + 0b11011                # x**64 + x**4  + x**3 + x    + 1
                             }

def make_gf_dict(w):  
        gf_element_total_number = 1 << w  
        primitive_polynomial = primitive_polynomial_dict[w]  

        gfilog = [1]  # g(0) = 1  
        for i in xrange(1, gf_element_total_number - 1):  
                temp = gfilog[i - 1] << 1  # g(i) = g(i-1) * g  
                if temp & gf_element_total_number:  # if overflow, then mod primitive polynomial  
                        temp ^= primitive_polynomial  # mod primitive_polynomial in GF(2**w) == XOR  
                gfilog.append(temp)  

        assert (gfilog[gf_element_total_number - 2] << 1) ^ primitive_polynomial  
        gfilog.append(None)
        gflog = [None] * gf_element_total_number  
        for i in xrange(0, gf_element_total_number - 1):
                gflog[gfilog[i]] = i
        print "{:>8}\t{:>8}\t{:>8}".format("i", "gfilog[i]", "gflog[i]")

        for i in xrange(0, gf_element_total_number):
                print "{:>8}\t{:>8}\t{:>8}".format(i, gfilog[i], gflog[i])

if __name__ == "__main__":
        make_gf_dict(16)

