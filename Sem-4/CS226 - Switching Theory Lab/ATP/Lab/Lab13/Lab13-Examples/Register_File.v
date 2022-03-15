
module Register_File (Data_Out_1, Data_Out_2, Data_in, Read_Addr_1, Read_Addr_2, Write_Addr, Write_Enable, Clock); 
output [31: 0] Data_Out_1, Data_Out_2; 
input [31: 0] Data_in; 
input [4: 0] Read_Addr_1, Read_Addr_2, Write_Addr; 
input Write_Enable, Clock; 
reg [31: 0] Reg_File [31: 0]; //32bit x32 word memory declaration 
assign Data_Out_1 = Reg_File[Read_Addr_1]; 
assign Data_Out_2 = Reg_File[Read_Addr_2]; 
always @ (posedge Clock) begin 
if (Write_Enable) 
Reg_File[Write_Addr] <= Data_in; 
end 
endmodule 


