module tb_p8();
	reg s3,s2,s1,s0;                        //Select Lines
    reg d7,d6,d5,d4,d3,d2,d1,d0;            // Inputs 0-7
    reg d15,d14,d13,d12,d11,d10,d9,d8;      // Inputs 8-15
    wire op;                                // Output

	// Instantiate 16:1 MUX
	p8 UUT(op,s3,s2,s1,s0,d15,d14,d13,d12,d11,d10,d9,d8,d7,d6,d5,d4,d3,d2,d1,d0);

	initial begin
		// Initialize Inputs
		d0 = 0;
		d1 = 0;
		d2 = 0;
		d3 = 0;
		d4 = 0;
		d5 = 0;
		d6 = 0;
		d7 = 0;
		d8 = 0;
		d9 = 0;
		d10 = 0;
		d11 = 0;
		d12 = 0;
		d13 = 0;
		d14 = 0;
		d15 = 0;
		
        s0 = 0;
		s1 = 0;
		s2 = 0;
		s3 = 0;
        #10;
        d0 = 1;
        
        #10;
        s0 = 1;
		s1 = 0;
		s2 = 0;
		s3 = 0;
        #10;
        d1 = 1;
        
        #10;
        s0 = 0;
		s1 = 1;
		s2 = 0;
		s3 = 0;
        #10;
        d2 = 1;
        
        #10;
        s0 = 1;
		s1 = 1;
		s2 = 0;
		s3 = 0;
        #10;
        d3 = 1;
        
        #10;
        s0 = 0;
		s1 = 0;
		s2 = 1;
		s3 = 0;
        #10;
        d4 = 1;
        
        #10;
        s0 = 1;
		s1 = 0;
		s2 = 1;
		s3 = 0;
        #10;
        d5 = 1;
        
        #10;
        s0 = 0;
		s1 = 1;
		s2 = 1;
		s3 = 0;
        #10;
        d6 = 1;
        
        #10;
        s0 = 1;
		s1 = 1;
		s2 = 1;
		s3 = 0;
        #10;
        d7 = 1;
        
        #10
        s0 = 0;
		s1 = 0;
		s2 = 0;
		s3 = 1;
        #10;
        d8 = 1;
        
        #10;
        s0 = 1;
		s1 = 0;
		s2 = 0;
		s3 = 1;
        #10;
        d9 = 1;
        
        #10;
        s0 = 0;
		s1 = 1;
		s2 = 0;
		s3 = 1;
        #10;
        d10 = 1;
        
        #10;
        s0 = 1;
		s1 = 1;
		s2 = 0;
		s3 = 1;
        #10;
        d11 = 1;
        
        #10;
        s0 = 0;
		s1 = 0;
		s2 = 1;
		s3 = 1;
        #10;
        d12 = 1;
        
        #10;
        s0 = 1;
		s1 = 0;
		s2 = 1;
		s3 = 1;
        #10;
        d13 = 1;
        
        #10;
        s0 = 0;
		s1 = 1;
		s2 = 1;
		s3 = 1;
        #10;
        d14 = 1;
        
        #10;
        s0 = 1;
		s1 = 1;
		s2 = 1;
		s3 = 1;
        #10;
        d15 = 1;
    end
	initial begin
        $monitor("s1=%b s0=%b\n\td0=%b d1=%b d2=%b d3=%b\n\td4=%b d5=%b d6=%b d7=%b\n\td8=%b d9=%b d10=%b d11=%b\n\td12=%b d13=%b d14=%b d15=%b\nop=%b",s1,s0,d0,d1,d2,d3,d4,d5,d6,d7,d8,d9,d10,d11,d12,d13,d14,d15,op);
    end
endmodule