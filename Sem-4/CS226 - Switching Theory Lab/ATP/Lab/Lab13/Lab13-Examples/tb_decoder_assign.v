module tb_decoder_assgn();
wire [15:0] decoder_out;
reg enable;
reg [3:0] binary_in;
decoder_assign uut(decoder_out,enable,binary_in);
initial
begin
binary_in =4'b0;
enable=1'b0;
#10;
binary_in =4'b1;
enable=1'b1;
#10;   
binary_in =4'b0001; enable=1'b0;  #10;
binary_in =4'b0011; enable=1'b1;  #10;
binary_in =4'b0000; enable=1'b1;  #10;
binary_in =4'b0101; enable=1'b0;  #10;   
binary_in =4'b0110; enable=1'b1;  #10;
binary_in =4'b0111; enable=1'b1;  #10;
binary_in =4'b1001; enable=1'b0;  #10;   
binary_in =4'b1100; enable=1'b1;  #10;
binary_in =4'b1111; enable=1'b1;  #10;

end
endmodule
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 