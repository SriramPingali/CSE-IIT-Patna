// Testbench for p7 module in p7.v
module tb_p7 ();
    // Mode Declarations
    // All terms explained in p7.v
    wire zero_flag;
    wire [31:0] result;
    reg [31:0] Data_in;
    reg [4:0] Read_Addr_1, Read_Addr_2, Write_Addr;
    reg Write_Enable, Mux_cntrl, Clock = 0;
    reg [1:0] opcode;

    always #1 Clock = ~Clock;

    // Instantiate p7
    p7 DUT_p7 (zero_flag, result, Data_in, Read_Addr_1, Read_Addr_2, Write_Addr, Write_Enable, Mux_cntrl, opcode, Clock);

    wire [31:0] sum, mux_OP;
    assign sum = DUT_p7.regFile.Reg_File[31];   // Value Stored at register 31 (Sum of input Data)
    assign mux_OP = DUT_p7.mux_OP;              // Output of 2:1 Multiplexer
    assign Data_Out_1 = DUT_p7.Data_Out_1;      // Data Output 1 from Register File
    assign Data_Out_2 = DUT_p7.Data_Out_2;      // Data Output 2 from Register File

    integer i;
    initial begin
        // Write Data to Register File
        Mux_cntrl = 0;
        Write_Enable = 1;
        for(i = 0; i < 25; i = i+1) begin
            #2;
            Write_Addr = i;
            Data_in = $urandom%(64);
        end

        // Initialize register 31 to zero to avoid garbage values
        #2;
        Write_Addr = 31;
        Data_in = 31'b0;

        // Calculate Sum of data in registers 0 to 24
        #2;
        Data_in = 31'bx;
        Mux_cntrl = 1;
        Read_Addr_1 = 31;
        Read_Addr_2 = 31;
        opcode = 0;

        for (i = 0; i < 25; i = i+1) begin
            #2 Read_Addr_1 = i;
        end
    end

    // Display all Parameters
    initial begin
        $monitor("time=%g\tMux_cntrl=%b\tData_in=%d\tmux_OP=%d\tRead_Addr_1=%d\tRead_Addr_2=%d\tWrite_Enable=%b\tWrite_Addr=%d\tData_Out_1=%d\tData_Out_2=%d\topcode=%b\tSum=%d",$time,Mux_cntrl,Data_in,mux_OP,Read_Addr_1,Read_Addr_2,Write_Enable,Write_Addr,Data_Out_1,Data_Out_2,opcode,sum);
    end
endmodule