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
 l -> int  means int type is 64 bit wide  
 floats are automatically promoted to doubles when provided as an argument  
 The float length specifier was removed for better compatibility with the gcc printf function  
  
Compile Options:  
 FLOAT_MAN_MAX -> 10 ^ number of sig figs to print float to  

# TODO
Need to do more testing of the printf function  
There is a bug in printf.c line number 729 where num isn't incremented for the outputting of the '?' char  
