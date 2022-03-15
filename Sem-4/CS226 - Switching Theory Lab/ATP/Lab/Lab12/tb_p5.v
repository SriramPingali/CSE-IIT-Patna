// Testbench for module p5
module tb_p5();
    reg m2,m1,m0;   // motion sensors
    wire A;         // Alarm

    p5 UUT (A,m2,m1,m0);

    // Loop through all combinations of m2,m1,m0
    initial begin
        m2 = 0;
        m1 = 0;
        m0 = 0;

        while ((m0 & m1 & m2) != 1) begin
            #10;
            m2 = m2^(m1 & m0);
            m1 = m1^m0;
            m0 = m0^1;
        end
    end

    // Display output for various cases
    initial begin
        $monitor("m2=%b m1=%b m0=%b A=%b",m2,m1,m0,A);
    end
endmodule
