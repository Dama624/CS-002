We explain what is happening by applying the naive single precision float
addition algorithm. We are adding 10^-8 to 1 (10^8 times). We first add
1e0 to 1e-8. Since 1e0 has greater exponent, we convert the numbers such that:
1e0 + 0.00000001e0. Unfortunately, adding the two numbers gives us now a
number with many significant digits: 1.00000001e0. In decimal, we need 8
significant digits (for 10^-8, not counting the initial 1.0). Converting to 
binary: log2(10^8) = approx. 26.5 significant digits, which is greater than
23, the number of bits allocated for the mantissa. Thus, adding 1e0 and 1e-8
as floats rounds to 1.0e0. If we repeat the process 10^8 times, we'll always
end up with 1.0e0 at every step.