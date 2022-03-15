// Testbench for p4.v
module tb_p4();
    // Mode Declaration
    reg z1,z2,z3,z4,z5,z6,z7,z8;
    wire [2:0] op;

    // Instantiate p4
    p4 UUT (op,z1,z2,z3,z4,z5,z6,z7,z8);

    // Testcase: Only one zone is intruded one time
    initial begin
        {z1,z2,z3,z4,z5,z6,z7,z8} = 8'b10000000;
        #10;
        {z1,z2,z3,z4,z5,z6,z7,z8} = 8'b01000000;
        #10;
        {z1,z2,z3,z4,z5,z6,z7,z8} = 8'b00100000;
        #10;
        {z1,z2,z3,z4,z5,z6,z7,z8} = 8'b00010000;
        #10;
        {z1,z2,z3,z4,z5,z6,z7,z8} = 8'b00001000;
        #10;
        {z1,z2,z3,z4,z5,z6,z7,z8} = 8'b00000100;
        #10;
        {z1,z2,z3,z4,z5,z6,z7,z8} = 8'b00000010;
        #10;
        {z1,z2,z3,z4,z5,z6,z7,z8} = 8'b00000001;
        #10;
    end
    
    // Display all parameters
    initial begin
        $monitor("z1=%b z2=%b z3=%b z4=%b z5=%b z6=%b z7=%b z8=%b op=%b",z1,z2,z3,z4,z5,z6,z7,z8,op);
    end
endmodule