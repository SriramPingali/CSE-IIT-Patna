// Module for 32x32 Register File
module Register_File (Data_Out_1, Data_Out_2, Data_in, Read_Addr_1, Read_Addr_2, Write_Addr, Write_Enable, Clock);
    // Mode Declaration
    input [31:0] Data_in;                             // Data to be written
    input [4:0] Read_Addr_1, Read_Addr_2, Write_Addr; // Addresses to Read and Write
    input Write_Enable;                               // Enable for Write Mode
    input Clock;                                      // Clock
    output [31:0] Data_Out_1, Data_Out_2;             // Data to be read from Read_Addr_1 & Read_Addr_2

    reg [31:0] Reg_File [31:0];                       // Memory declaration for 32 32-bit Words

    assign Data_Out_1 = Reg_File[Read_Addr_1];
    assign Data_Out_2 = Reg_File[Read_Addr_2];

    always @ (posedge Clock) begin
        if (Write_Enable) begin
            Reg_File[Write_Addr] = Data_in;
        end
    end
endmodule

// Module for ALU
module alu (op,zero_flag,ip0,ip1,opcode);
    // Mode Declaration
    output zero_flag;       // Zero Flag
    output [31:0] op;       // ALU Output
    input [1:0] opcode;     // ALU Mode of Operation
    input [31:0] ip0,ip1;   // ALU Inputs
    
    //    opcode :     00             01              10             11
    // Operation :  Addition      Subtraction     Bitwise AND    Bitwise OR
    assign op = opcode[0] ? (opcode[1] ? (ip0&ip1) : (ip0|ip1)) : (opcode[1] ? (ip0-ip1) : (ip0+ip1));
    assign zero_flag = (op == 32'b0);
endmodule

// Module for 2:1 Multiplexer
module mux2_1 (op,ip0,ip1,Mux_cntrl);
    output [31:0] op;       // MUX Output
    input Mux_cntrl;        // MUX Control Line
    input [31:0] ip0,ip1;   // MUX Inputs

    // Mux_cntrl :    0            1
    //    Output : Input 0      Input 1
    assign op = Mux_cntrl ? ip1 : ip0;
endmodule

// Module for given Architecture
module p7 (zero_flag, result, Data_in, Read_Addr_1, Read_Addr_2, Write_Addr, Write_Enable, Mux_cntrl, opcode, Clock);
    // Mode Declaration
    output zero_flag;
    output [31:0] result;
    input [31: 0] Data_in;
    input [4: 0] Read_Addr_1, Read_Addr_2, Write_Addr;
    input Mux_cntrl, Write_Enable, Clock;
    input [1:0] opcode;

    wire [31:0] mux_OP, Data_Out_1, Data_Out_2, result;

//  Import Modules for all blocks
/*    Module      | Name of  |         Parameters    
 *                | Instance |                         
------------------------------------------------------------*/
    mux2_1            MUX      (mux_OP,Data_in,result,Mux_cntrl);
    Register_File   regFile    (Data_Out_1, Data_Out_2, mux_OP, Read_Addr_1, Read_Addr_2, Write_Addr, Write_Enable, Clock);
    alu               ALU      (result,zero_flag,Data_Out_1,Data_Out_2,opcode);
endmodule