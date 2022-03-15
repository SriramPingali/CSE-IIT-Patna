 
 module tb_updown ();
 reg [3: 0] IN; 
 reg Up, Down, Load;
 reg CLK=0;
 wire [3: 0] 	OUT;
 
 
 updown DUT (OUT, Up, Down, Load, IN, CLK);
 
 always #10 CLK = ~CLK;
 
 
 initial
 begin
 
 IN=4'b1111;Load=1'b1;Up=1'b0; Down=1'b0;
 #15  Load=1'b0;Up=1'b1; Down=1'b0;
 #100  Load=1'b0;Up=1'b0; Down=1'b1;
 
 end
 endmodule
 
 
