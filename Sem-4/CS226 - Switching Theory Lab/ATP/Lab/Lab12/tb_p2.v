// Testbench for Fuel Level Indicator Model
module tb_p2();
	
	// Initialize Variables
	reg [2:0] b;
	wire L;
	
	// Instantiate Model
	p2 UUT (L,b);
	
	// Iterate through all 3 bit numbers
	initial begin
		b = 3'b000;
		#10;
		b = 3'b001;
		#10;
		b = 3'b010;
		#10;
		b = 3'b011;
		#10;
		b = 3'b100;
		#10;
		b = 3'b101;
		#10;
		b = 3'b110;
		#10;
		b = 3'b111;
	end
	
	// Display output in each case
	initial begin
        $monitor("b=%b %b %b, L=%b, time=%t\n", b[2], b[1], b[0], L, $time);
    end
endmodule