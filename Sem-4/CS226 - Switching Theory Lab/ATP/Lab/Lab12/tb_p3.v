// Testbench for module p3
module tb_p3();
    
    // Variable Declaration
    reg a,b,c;
    wire f,g,h;

    // Instantiate module p3
    p3 UUT (f,g,h,a,b,c);

    // Loop through all possible triplets of a,b,c
    initial begin
        a = 0;
        b = 0;
        c = 0;

        while ((a & b & c) != 1) begin
            #10;
            a = a^(b&c);
            b = b^c;
            c = c^1;
        end
    end

    // Print Truth Table for f,g,h
    initial begin
        $monitor("a=%b b=%b c=%b f=%b g=%b h=%b",a,b,c,f,g,h);
    end
endmodule