module decoder_assign(decoder_out,enable,binary_in);
input [3:0] binary_in; //  4 bit binary input
input  enable;   //  Enable for the decoder
output [15:0] decoder_out;  //  16-bit  out
assign decoder_out = (enable) ? (1 << binary_in) : 16'b0 ;
//if enable =1, decoder_out will be  2** binary_n, otherwise 0 
endmodule
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 