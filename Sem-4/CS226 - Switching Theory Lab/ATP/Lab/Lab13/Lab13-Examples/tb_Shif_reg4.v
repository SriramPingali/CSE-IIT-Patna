module Shif_reg4 (Data_out, Data_In, clock, reset); 
output Data_out; 
input Data_In, clock, reset; 
reg [3: 0] Data_reg; 
assign Data_out = Data_reg[0]; 
always @ (posedge clock) 
begin 
if (reset == 1'b0) Data_reg <= 4'b0; 
else Data_reg <= {Data_In, Data_reg[3:1]}; 
end 
endmodule 


module tb_Shif_reg4(); 
reg Data_In,reset;
reg clock =0;
wire Data_out;
Shif_reg4 DUT(Data_out, Data_In, clock, reset);

always #5 clock=~clock;      // clock declaration
initial
begin
Data_In=1'b0; reset=0; // apply serial input
#10 reset=1'b1;
#10 Data_In=1'b1; 
#9 Data_In=1'b0; 
#9 Data_In=1'b1; 
end
endmodule




















