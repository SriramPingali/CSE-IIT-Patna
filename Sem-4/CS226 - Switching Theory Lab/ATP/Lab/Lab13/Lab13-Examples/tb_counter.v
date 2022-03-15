  
  module tb_counter();
   
 reg CLK=0;
 reg reset;
 wire [3: 0] C_OUT;
 
 
 counter  DUT (C_OUT,CLK,reset);
 
 always #5 CLK = ~CLK;
 
 
 initial
 begin
 
 reset=1'b1; 
 #20reset=1'b0;
  
 end
 endmodule
 
 
