# printf

A basic version of the printf function found in the c standard library  
To use in your project, change put_char to call the function which puts a char to your framebuffer  
Ryu is used for printing floats  
For the format:  
 %s -> string  
 %c -> char byte  
 %d -> integer (decimal format)  
 %u -> integer (decimal format, unsigned)  
 %b -> integer (binary format)  
 %o -> integer (octal format)  
 %h -> integer (hex format)  
 %f -> float (decimal format)  
 %e -> float (scientific notation, base 10)  
 %% -> %  
   
length specifiers:  
 l -> int, float means int type is 64 bit wide and float type is 64 bit wide (double)  
   
Compile Options:  
 FLOAT_MAN_MAX -> 10 ^ number of sig figs to print float to  

# TODO
Need to do more testing of the printf function  
There are errors when printing floats in the general case  
Please check the code works for your specific range of floats before using
