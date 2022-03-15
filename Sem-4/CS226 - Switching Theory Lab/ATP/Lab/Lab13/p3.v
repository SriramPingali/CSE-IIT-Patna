// Module for given 16-bit ALU
module p3(op, zero_flag, opmode, ip1, ip2);
    // Mode Declaration
    input [15:0] ip1,ip2;   // ALU Inputs
    input [2:0] opmode;     // ALU Operation Mode
    output reg [15:0] op;   // ALU Output
    output zero_flag;       // ALU Zero Flag

    always @(opmode or ip1 or ip2)
        case(opmode)
            3'b000: op = ip1 + ip2;
            3'b001: op = ip1 - ip2;
            3'b010: op = ip1 ^ ip2;
            3'b011: op = ip1 && ip2;
            3'b100: op = ip1 || ip2;
            3'b101: op = ip1 + 16'b0000_0001;
            3'b110: op = ip1 << 1;
            3'b111: op = ip1 >> 1;
            default: op = 3'bx;
        endcase

    assign zero_flag = (op == 16'b0);
endmodule