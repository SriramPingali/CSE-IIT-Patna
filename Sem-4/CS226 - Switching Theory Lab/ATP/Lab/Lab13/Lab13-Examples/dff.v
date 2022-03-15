
module dff (q,d,clk,reset); 
input d, clk, reset ; 

output q;
reg q;
 always @ ( posedge clk)
 if (~reset) 
   q <= 1'b0;
 else  
   q <= d;
  endmodule 


