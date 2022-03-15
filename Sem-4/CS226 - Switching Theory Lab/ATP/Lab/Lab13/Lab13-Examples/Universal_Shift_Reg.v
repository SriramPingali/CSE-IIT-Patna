module Universal_Shift_Reg (Data_Out,MSB_out,LSB_out,Data_In,MSB_In, LSB_In, s1, s0, clk, reset); 
output [3: 0] Data_Out; 
input [3: 0] Data_In; 
output MSB_out,LSB_out;
input MSB_In, LSB_In; 
input s1, s0, clk, reset; 
reg [3: 0] Data_Out; 
assign MSB_out=Data_Out[3];
assign LSB_out=Data_Out[0];
always @ (posedge clk) 
begin 
if (reset) Data_Out <= 0; 
else 
case ({s1, s0}) 
0: Data_Out <= Data_Out; 		 // Hold 
1: Data_Out <= {MSB_In, Data_In[3:1]};  // Serial shift from MSB 
2: Data_Out <= {Data_In[2:0], LSB_In}; // Serial shift from LSB 
3: Data_Out <= Data_In; 		 // Parallel Load 
endcase 
end 
endmodule 

