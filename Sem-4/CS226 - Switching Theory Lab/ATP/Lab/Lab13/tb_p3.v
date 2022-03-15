// Testbench for p3.v
module tb_p3();
    // Mode Declaration
    reg [2:0] opmode;
    reg [15:0] ip1,ip2;
    wire zero_flag;
    wire [15:0] op;
    
    p3 UUT (op, zero_flag, opmode, ip1, ip2);   // Instantiate p3

    // For ip1 and ip2, obtain outputs for each mode
    initial begin
        ip1 = 16'b0010_0001; ip2 = 16'b1001_0001; opmode = 3'b000; #10;
        ip1 = 16'b1001_0001; ip2 = 16'b0010_0001; opmode = 3'b001; #10;
        ip1 = 16'b0101_0101; ip2 = 16'b1111_1111; opmode = 3'b010; #10;
        ip1 = 16'b0011_0011; ip2 = 16'b0101_0101; opmode = 3'b011; #10;
        ip1 = 16'b0011_0011; ip2 = 16'b0101_0101; opmode = 3'b100; #10;
        ip1 = 16'b0011_1111; ip2 = 16'bx;         opmode = 3'b101; #10;
        ip1 = 16'b0011_1111; ip2 = 16'bx;         opmode = 3'b110; #10;
        ip1 = 16'b0011_1111; ip2 = 16'bx;         opmode = 3'b111;
        #10;
    end

    // Display all parameters
    initial begin
        $monitor("opmode = %b\nip1 = %b\nip2 = %b\nop  = %b\nzero_flag  = %b\n----------",opmode,ip1,ip2,op,zero_flag);
    end
endmodule