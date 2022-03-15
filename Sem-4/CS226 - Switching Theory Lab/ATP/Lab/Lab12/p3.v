/* Simulation to demonstrate whether
 * functions f,g,h are equal or not */
module p3(f,g,h,a,b,c);
    
    // Variable Declaration
    input a,b,c;
    output f,g,h;

    // Boolean Functions
    assign f = (a & ~b) | (~b & ~c) | (a & c);
    assign g = (~b | c) & (a | b | ~c);
    assign h = (~b & ~c) | (b & c) | (a & c);
endmodule