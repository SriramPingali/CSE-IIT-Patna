module Seven_Seg_Display (Display, BCD); 
output [6: 0] Display; 
input [3: 0] BCD; 
reg [6: 0] Display; 
                // abc_detg 
parameter BLANK =7'b111_1111; 	 
parameter ZERO = 7'b000_0001; 	// h01
parameter ONE =  7'b100_1111; 	// h4f
parameter TWO =  7'b001_0010; 	// h12
parameter THREE =7'b000_0110; 	// h06
parameter FOUR = 7'b100_1100;   // h4c
parameter FIVE = 7'b010_0100;   // h24
parameter SIX =  7'b010_0000; 	// h20
parameter SEVEN= 7'b000_1111; 	// hOf
parameter EIGHT =7'b000_0000; 	// hOO
parameter NINE = 7'b000_0100; 	// h04


always @ (BCD) 
case (BCD) 
0: Display = ZERO;
1: Display = ONE; 
2: Display = TWO; 
3: Display = THREE; 
4: Display = FOUR; 
5: Display = FIVE; 
6: Display = SIX; 
7: Display = SEVEN; 
8: Display = EIGHT; 
9: Display = NINE; 
default:Display = BLANK; 
endcase 
endmodule 


































 
















