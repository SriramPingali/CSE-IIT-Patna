// Design for Fuel Level Detector Model
module p2(L,b);
	
	output L;
	/* Indicator Light --> Glows when there is low fuel 
	 *					i.e., when level is less than 3 */
	
	input [2:0] b;	// Fuel Level (3-bit binary)
	wire w;
	
	/* Final Expression = ~b2.~b1 + ~b2.~b0
						= ~b2.(~b1 + ~b0)
						= ~b2.~(b1.b0)
						= ~(b2 + b1.b0)
						= nor(b2, b1.b0)
	*/
	and and_0(w, b[1], b[0]);	// w = b1.b0
	nor nor_0(L, b[2], w);	// L = nor(b2, b1.b0)

endmodule