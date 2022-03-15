
module tb_Universal_Shift_Reg(); 

reg [3: 0] Data_In; 
reg MSB_In, LSB_In,s1, s0, reset; 
reg clk=0; 
wire [3: 0] Data_Out; 
wire MSB_out,LSB_out;

 Universal_Shift_Reg  DUT (Data_Out,MSB_out,LSB_out,Data_In,MSB_In, LSB_In, s1, s0, clk, reset); 
 
 always #5 clk = ~clk;
 
 initial
 begin
 
Data_In=4'b0000; MSB_In=1'b0; LSB_In=1'b0;s1=1'b0; s0=1'b0; reset=1'b1;  // reset machine
#10Data_In=4'b1111; MSB_In=1'b1; LSB_In=1'b1;s1=1'b1; s0=1'b1; reset=1'b0;  //parallel load Data_In to Data_out
#10Data_In=4'b0100; MSB_In=1'b1; LSB_In=1'b1;s1=1'b1; s0=1'b1; reset=1'b0;  //parallel load Data_In to Data_out
#10Data_In=4'b1100; MSB_In=1'b1; LSB_In=1'b1;s1=1'b0; s0=1'b0; reset=1'b0;  // Serial shift from MSB
#10Data_In=4'b0011; MSB_In=1'b1; LSB_In=1'b1;s1=1'b0; s0=1'b1; reset=1'b1;  // Serial shift from MSB
#10Data_In=4'b1100; MSB_In=1'b1; LSB_In=1'b1;s1=1'b1; s0=1'b0; reset=1'b0;  // Serial shift from LSB 
#10Data_In=4'b0011; MSB_In=1'b1; LSB_In=1'b1;s1=1'b1; s0=1'b0; reset=1'b0;  // Serial shift from LSB 
#10Data_In=4'b1100; MSB_In=1'b1; LSB_In=1'b1;s1=1'b1; s0=1'b0; reset=1'b0;  // // Hold 
#10Data_In=4'b0011; MSB_In=1'b1; LSB_In=1'b1;s1=1'b1; s0=1'b0; reset=1'b0;  // // Hold 

end 
endmodule

